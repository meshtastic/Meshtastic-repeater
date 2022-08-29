#pragma once

#include "mesh-pb-constants.h"


#define DEVICESTATE_CUR_VER 15
#define DEVICESTATE_MIN_VER DEVICESTATE_CUR_VER

// extern DeviceState devicestate;
extern LocalConfig config;
extern ChannelFile channelFile;

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
        void installDefaultDeviceState(), installDefaultChannels(), installDefaultConfig();
};

extern DeviceStore deviceStore;