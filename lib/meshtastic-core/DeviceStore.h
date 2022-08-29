#pragma once

#include "mesh-pb-constants.h"

/// 16 bytes of random PSK for our _public_ default channel that all devices power up on (AES128)
static const uint8_t defaultpsk[] = {0xd4, 0xf1, 0xbb, 0x3a, 0x20, 0x29, 0x07, 0x59,
                                     0xf0, 0xbc, 0xff, 0xab, 0xcf, 0x4e, 0x69, 0x01};


#define DEVICESTATE_CUR_VER 15
#define DEVICESTATE_MIN_VER DEVICESTATE_CUR_VER

extern ChannelFile channelFile; // Not used currently
extern LocalConfig config;
extern Channel channel;

class DeviceStore
{
    public:
        DeviceStore();

        /// Called from service after app start, to do init which can only be done after OS load
        void init();
        
    private:
        void loadFromDisk();
        void resetRadioConfig();
        /// Reinit device state from scratch (not loading from disk)
        void installDefaultDeviceState(), installDefaultChannel(), installDefaultConfig();
};

extern DeviceStore deviceStore;