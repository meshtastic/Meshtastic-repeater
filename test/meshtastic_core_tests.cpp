#include <Arduino.h>
#include "DeviceStore.h"
#include "mesh/Channels.h"
#include "mesh/MeshRadio.h"
#include "mesh/SX1262Interface.h"
#include <unity.h>

// typedef struct _Config_LoRaConfig { 
//     int32_t tx_power; 
//     Config_LoRaConfig_ModemPreset modem_preset; 
//     uint32_t bandwidth; 
//     uint32_t spread_factor; 
//     uint32_t coding_rate; 
//     float frequency_offset; 
//     Config_LoRaConfig_RegionCode region; 
// } Config_LoRaConfig;

ChannelIndex DefaultChannelIndex = 0;

void setUp(void) 
{
}

void tearDown(void) 
{
}

void Channels_initDefaults_ShouldSetMeshtasticChannelParams(void) 
{
    auto channelName = channels.getName(DefaultChannelIndex);
    TEST_ASSERT_EQUAL_STRING("LongFast", channelName);

    auto primaryName = channels.getPrimaryName();
    TEST_ASSERT_EQUAL_STRING("#LongFast-V", primaryName);
}

void RadioInterface_reconfigure_ShouldResetChannelSettings(void) 
{    
    // #ifdef IS_CUBECELL
    //     RadioInterface *radioInterface = new SX1262Interface(RADIOLIB_BUILTIN_MODULE, SPI);
    // #elif USE_SX1262
    //     radioInterface = new SX1262Interface(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI);
    // #endif
    // radioInterface->reconfigure();
    // Config_LoRaConfig &loraConfig = config.payloadVariant.lora;
}

void RadioInterface_init_ShouldSetDefaultLoRAChannel(void) 
{
    RadioInterface *radioInterface = NULL;
    #ifdef IS_CUBECELL
        radioInterface = new SX1262Interface(RADIOLIB_BUILTIN_MODULE, SPI);
    #elif USE_SX1262
        radioInterface = new SX1262Interface(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI);
    #endif
    radioInterface->init();
    Config_LoRaConfig &loraConfig = config.payloadVariant.lora;

    TEST_ASSERT_EQUAL_UINT32(loraConfig.bandwidth, 250);
    TEST_ASSERT_EQUAL_UINT32(loraConfig.coding_rate, 8);
    TEST_ASSERT_EQUAL_UINT32(loraConfig.spread_factor, 10);
}

void setup() 
{
    delay(5000);
    UNITY_BEGIN();
    #ifdef DEBUG_PORT
        consoleInit(); // Set serial baud rate and init our mesh console
    #endif
    SPI.begin();
    config.payloadVariant.lora.region = Config_LoRaConfig_RegionCode_US; 

    deviceStore.init();
    RUN_TEST(Channels_initDefaults_ShouldSetMeshtasticChannelParams);
    // RUN_TEST(RadioInterface_init_ShouldSetDefaultLoRAChannel);
    RUN_TEST(RadioInterface_init_ShouldSetDefaultLoRAChannel);
    UNITY_END();
}

void loop()
{
}