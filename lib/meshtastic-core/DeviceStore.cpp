#include "mesh-pb-constants.h"
#include "DeviceStore.h"
#include "mesh/Channels.h"
#include "mesh/MeshRadio.h"

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
