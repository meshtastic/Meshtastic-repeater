#include "RadioLibInterface.h"
#include "MeshTypes.h"
#include "DeviceStore.h"
// Repeater shouldn't need exclusive spi lock
// #include "SPILock.h"
#include "configuration.h"
#include "error.h"
#include "mesh-pb-constants.h"
#include <pb_decode.h>
#include <pb_encode.h>
#include <assert.h>


// FIXME, we default to 4MHz SPI, SPI mode 0, check if the datasheet says it can really do that
static SPISettings spiSettings(4000000, MSBFIRST, SPI_MODE0);

RadioLibInterface::RadioLibInterface(RADIOLIB_PIN_TYPE cs, RADIOLIB_PIN_TYPE irq, RADIOLIB_PIN_TYPE rst, RADIOLIB_PIN_TYPE busy,
                                     SPIClass &spi, PhysicalLayer *_iface)
    : module(cs, irq, rst, busy, spi, spiSettings), iface(_iface)
{
    instance = this;
}

#ifndef NO_ESP32
// ESP32 doesn't use that flag
#define YIELD_FROM_ISR(x) portYIELD_FROM_ISR()
#else
#define YIELD_FROM_ISR(x) portYIELD_FROM_ISR(x)
#endif


/** Our ISR code currently needs this to find our active instance
 */
RadioLibInterface *RadioLibInterface::instance;

/** Could we send right now (i.e. either not actively receving or transmitting)? */
bool RadioLibInterface::canSendImmediately()
{
    // We wait _if_ we are partially though receiving a packet (rather than just merely waiting for one).
    // To do otherwise would be doubly bad because not only would we drop the packet that was on the way in,
    // we almost certainly guarantee no one outside will like the packet we are sending.
    bool busyTx = sendingPacket != NULL;
    bool busyRx = isReceiving && isActivelyReceiving();

    if (busyTx || busyRx) {
        if (busyTx)
            DEBUG_MSG("Can not send yet, busyTx\n");
        // If we've been trying to send the same packet more than one minute and we haven't gotten a
        // TX IRQ from the radio, the radio is probably broken.
        if (busyTx && (millis() - lastTxStart > 60000)) {
            DEBUG_MSG("Hardware Failure! busyTx for more than 60s\n");
            RECORD_CRITICALERROR(CriticalErrorCode_TransmitFailed);
// #ifndef NO_ESP32
//             if (busyTx && (millis() - lastTxStart > 65000)) // After 5s more, reboot
//                 ESP.restart();
// #endif
        }
        if (busyRx)
            DEBUG_MSG("Can not send yet, busyRx\n");
        return false;
    } else
        return true;
}

/// Send a packet (possibly by enquing in a private fifo).  This routine will
/// later free() the packet to pool.  This routine is not allowed to stall because it is called from
/// bluetooth comms code.  If the txmit queue is empty it might return an error
ErrorCode RadioLibInterface::send(MeshPacket *p)
{
    if (config.payloadVariant.lora.region != Config_LoRaConfig_RegionCode_Unset) {
        if (disabled || config.payloadVariant.lora.tx_disabled) {
            DEBUG_MSG("send - lora_tx_disabled\n");
            //TODO: alternate send method
            //packetPool.release(p);
            return ERRNO_DISABLED;
        }

    } else {
        DEBUG_MSG("send - lora_tx_disabled because RegionCode_Unset\n");
        //TODO: alternate send method
        //packetPool.release(p);
        return ERRNO_DISABLED;
    }

    // Sometimes when testing it is useful to be able to never turn on the xmitter
    printPacket("enqueuing for send", p);

    DEBUG_MSG("txGood=%d,rxGood=%d,rxBad=%d\n", txGood, rxGood, rxBad);

    // TODO: alternate
    ErrorCode res = ERRNO_OK;//Queue.enqueue(p) ? ERRNO_OK : ERRNO_UNKNOWN;

    if (res != ERRNO_OK) { // we weren't able to queue it, so we must drop it to prevent leaks
        //TODO: alternate send method
        //packetPool.release(p);
        return res;
    }

    // set (random) transmit delay to let others reconfigure their radio,
    // to avoid collisions and implement timing-based flooding
    // DEBUG_MSG("Set random delay before transmitting.\n");
    setTransmitDelay();

    return res;
    //TODO: alternate send method
    //packetPool.release(p);
    return ERRNO_DISABLED;
}

/** radio helper thread callback.

We never immediately transmit after any operation (either rx or tx).  Instead we should start receiving and
wait a random delay of 100ms to 100ms+shortPacketMsec to make sure we are not stomping on someone else.  The 100ms delay at the
beginning ensures all possible listeners have had time to finish processing the previous packet and now have their radio in RX
state.  The up to 100ms+shortPacketMsec random delay gives a chance for all possible senders to have high odds of detecting that
someone else started transmitting first and then they will wait until that packet finishes.

NOTE: the large flood rebroadcast delay might still be needed even with this approach.  Because we might not be able to hear other
transmitters that we are potentially stomping on.  Requires further thought.

FIXME, the MIN_TX_WAIT_MSEC and MAX_TX_WAIT_MSEC values should be tuned via logic analyzer later.
*/


void RadioLibInterface::handleTransmitInterrupt()
{
    // DEBUG_MSG("handling lora TX interrupt\n");
    // This can be null if we forced the device to enter standby mode.  In that case
    // ignore the transmit interrupt
    if (sendingPacket)
        completeSending();
}

void RadioLibInterface::completeSending()
{
    // We are careful to clear sending packet before calling printPacket because
    // that can take a long time
    auto p = sendingPacket;
    sendingPacket = NULL;

    if (p) {
        txGood++;
        printPacket("Completed sending", p);

        // We are done sending that packet, release it
        // TODO: alternate
        //packetPool.release(p);
        // DEBUG_MSG("Done with send\n");
    }
}

void RadioLibInterface::handleReceiveInterrupt()
{
    uint32_t xmitMsec;
    assert(isReceiving);
    isReceiving = false;

    // read the number of actually received bytes
    size_t length = iface->getPacketLength();

    xmitMsec = getPacketTime(length);

    int state = iface->readData(radiobuf, length);
    if (state != RADIOLIB_ERR_NONE) {
        DEBUG_MSG("ignoring received packet due to error=%d\n", state);
        rxBad++;
    } else {
        // Skip the 4 headers that are at the beginning of the rxBuf
        int32_t payloadLen = length - sizeof(PacketHeader);
        const uint8_t *payload = radiobuf + sizeof(PacketHeader);

        // check for short packets
        if (payloadLen < 0) {
            DEBUG_MSG("ignoring received packet too short\n");
            rxBad++;
        } else {
            const PacketHeader *h = (PacketHeader *)radiobuf;

            rxGood++;

            // Note: we deliver _all_ packets to our router (i.e. our interface is intentionally promiscuous).
            // This allows the router and other apps on our node to sniff packets (usually routing) between other
            // nodes.
            MeshPacket *mp;
            memset(&mp, 0, sizeof(MeshPacket));

            mp->from = h->from;
            mp->to = h->to;
            mp->id = h->id;
            mp->channel = h->channel;
            assert(HOP_MAX <= PACKET_FLAGS_HOP_MASK); // If hopmax changes, carefully check this code
            mp->hop_limit = h->flags & PACKET_FLAGS_HOP_MASK;
            mp->want_ack = !!(h->flags & PACKET_FLAGS_WANT_ACK_MASK);

            addReceiveMetadata(mp);

            mp->which_payloadVariant = MeshPacket_encrypted_tag; // Mark that the payload is still encrypted at this point
            assert(((uint32_t)payloadLen) <= sizeof(mp->encrypted.bytes));
            memcpy(mp->encrypted.bytes, payload, payloadLen);
            mp->encrypted.size = payloadLen;

            printPacket("Lora RX", mp);

            deliverToReceiver(mp);
        }
    }
}

/** start an immediate transmit */
void RadioLibInterface::startSend(MeshPacket *txp)
{
    printPacket("Starting low level send", txp);
    if (disabled || config.payloadVariant.lora.tx_disabled) {
        DEBUG_MSG("startSend is dropping tx packet because we are disabled\n");
        // packetPool.release(txp);
    } else {
        setStandby(); // Cancel any already in process receives

        configHardwareForSend(); // must be after setStandby

        size_t numbytes = beginSending(txp);

        int res = iface->startTransmit(radiobuf, numbytes);
        if (res != RADIOLIB_ERR_NONE) {
            RECORD_CRITICALERROR(CriticalErrorCode_RadioSpiBug);

            // This send failed, but make sure to 'complete' it properly
            completeSending();
            startReceive(); // Restart receive mode (because startTransmit failed to put us in xmit mode)
        }

        // Must be done AFTER, starting transmit, because startTransmit clears (possibly stale) interrupt pending register bits
        enableInterrupt(isrTxLevel0);
    }
}
