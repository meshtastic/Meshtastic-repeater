#include "mesh-pb-constants.h"
#include "configuration.h"
#include "DeviceStore.h"
#include "mesh/MeshRadio.h"
#include "error.h"

DeviceStore deviceStore;

LocalConfig config;
DeviceState devicestate;

Channel channel;

/** The current change # for radio settings.  Starts at 0 on boot and any time the radio settings
 * might have changed is incremented.  Allows others to detect they might now be on a new channel.
 */
uint32_t radioGeneration;

DeviceStore::DeviceStore() {}

void DeviceStore::init() {
    // DEBUG_MSG("Installing default DeviceState\n");
    // memset(&devicestate, 0, sizeof(DeviceState));

    installDefaultConfig();
    installDefaultChannel();
    resetRadioConfig();
}

void DeviceStore::installDefaultConfig()
{
    DEBUG_MSG("Installing default LocalConfig\n");
    memset(&config, 0, sizeof(LocalConfig));
    config.version = DEVICESTATE_CUR_VER;
    config.has_device = true;
    config.has_display = true;
    config.has_lora = true;
    config.has_position = true;
    config.has_power = true;
    config.has_wifi = true;
    config.has_bluetooth = true;

    config.lora.region = Config_LoRaConfig_RegionCode_US;
    config.lora.modem_preset = Config_LoRaConfig_ModemPreset_LongFast;
}

void DeviceStore::resetRadioConfig() 
{
    radioGeneration++;
    initRegion();
}

void DeviceStore::installDefaultChannel()
{
    memset(&channel, 0, sizeof(Channel));
    channel.index = 0;
    channel.role = Channel_Role_PRIMARY;
    channel.has_settings = true;
    strcpy(channel.settings.name, "LongF");
    // channel.settings.psk = defaultpsk;
    channel.settings.channel_num = 0;
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
