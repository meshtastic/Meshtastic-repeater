#include "mesh-pb-constants.h"
#include "DeviceStore.h"
#include "mesh/Channels.h"
#include "mesh/MeshRadio.h"
#include "error.h"

Config config;
ChannelFile channelFile;

DeviceStore deviceStore;

DeviceStore::DeviceStore() {}

void DeviceStore::init() {
    if (channelFile.channels_count != MAX_NUM_CHANNELS) {
        Serial.println("Setting default channel and radio preferences\n");
        channels.initDefaults();
    }
    initRegion();

    channels.onConfigChanged();
}

void DeviceStore::installDefaultConfig()
{
    memset(&config, 0, sizeof(config));
}

/// Record an error that should be reported via analytics
void recordCriticalError(CriticalErrorCode code, uint32_t address, const char *filename)
{
    // Print error to screen and serial port
    String lcd = String("Critical error ") + code + "!\n";
    //screen->print(lcd.c_str());
    if (filename)
        DEBUG_MSG("NOTE! Recording critical error %d at %s:%lx\n", code, filename, address);
    else
        DEBUG_MSG("NOTE! Recording critical error %d, address=%lx\n", code, address);

    // Record error to DB
    // myNodeInfo.error_code = code;
    // myNodeInfo.error_address = address;
    // myNodeInfo.error_count++;

    // Currently portuino is mostly used for simulation.  Make sue the user notices something really bad happend
#ifdef PORTDUINO
    DEBUG_MSG("A critical failure occurred, portduino is exiting...");
    exit(2);
#endif
}
