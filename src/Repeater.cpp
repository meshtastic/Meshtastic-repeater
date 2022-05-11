#include "Repeater.h"
#include "mesh/Channels.h"
#include "DeviceStore.h"
#include "configuration.h"
#include "mesh-pb-constants.h"

#include "mesh/RF95Interface.h"
#include "mesh/SX1262Interface.h"

/**
 * Constructor
 *
 * Currently we only allow one interface, that may change in the future
 */
Repeater::Repeater() 
{
}

void Repeater::init() 
{
    #ifdef SX126X_ANT_SW
    // make analog PA vs not PA switch on SX126x eval board work properly
    pinMode(SX126X_ANT_SW, OUTPUT);
    digitalWrite(SX126X_ANT_SW, 1);
#endif

#if defined(RF95_IRQ)
    if (!radioInterface) {
        radioInterface = new RF95Interface(RF95_NSS, RF95_IRQ, RF95_RESET, SPI);
        if (!radioInterface->init()) {
            DEBUG_MSG("Warning: Failed to find RF95 radio\n");
            delete radioInterface;
            radioInterface = NULL;
        } else {
            DEBUG_MSG("RF95 Radio init succeeded, using RF95 radio\n");
        }
    }
#endif

#if defined(USE_SX1262)
    if (!radioInterface) {
        #ifdef IS_CUBECELL
            radioInterface = new SX1262Interface(RADIOLIB_BUILTIN_MODULE);
        #elif
            radioInterface = new SX1262Interface(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI);
        #endif
        if (!radioInterface->init()) {
            DEBUG_MSG("Warning: Failed to find SX1262 radio\n");
            delete radioInterface;
            radioInterface = NULL;
        } else {
            DEBUG_MSG("SX1262 Radio init succeeded, using SX1262 radio\n");
        }
    }
#endif

#ifdef USE_SIM_RADIO
    if (!radioInterface) {
        radioInterface = new SimRadio;
        if (!radioInterface->init()) {
            DEBUG_MSG("Warning: Failed to find simulated radio\n");
            delete radioInterface;
            radioInterface = NULL;
        } else {
            DEBUG_MSG("Using SIMULATED radio!\n");
        }
    }
#endif
}
