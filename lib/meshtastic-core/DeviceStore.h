#pragma once

#include "mesh-pb-constants.h"

extern Config config;
extern ChannelFile channelFile;

class DeviceStore
{
    public:
        DeviceStore();

        /// Called from service after app start, to do init which can only be done after OS load
        void init();
        
    private:
        /// Reinit device state from scratch (not loading from disk)
        void installDefaultDeviceState(), installDefaultChannels(), installDefaultConfig();
};

extern DeviceStore deviceStore;