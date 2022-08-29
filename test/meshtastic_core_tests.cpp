#include <Arduino.h>
#include "DeviceStore.h"
#include "mesh/MeshRadio.h"
#include "mesh/SX1262Interface.h"
#include <unity.h>

ChannelIndex DefaultChannelIndex = 0;

void setUp(void) 
{
}

void tearDown(void) 
{
}

// void Channels_initDefaults_ShouldSetMeshtasticChannelParams(void) 
// {
//     auto channelName = channels.getName(DefaultChannelIndex);
//     TEST_ASSERT_EQUAL_STRING("LongFast", channelName);

//     auto primaryName = channels.getPrimaryName();
//     TEST_ASSERT_EQUAL_STRING("#LongFast-V", primaryName);
// }

// void RadioInterface_reconfigure_ShouldResetChannelSettings(void) 
// {    
// }

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
}

void setup() 
{
    delay(5000);
    SPI.begin();
    #ifdef DEBUG_PORT
        consoleInit(); // Set serial baud rate and init our mesh console
    #endif
    UNITY_BEGIN();
    deviceStore.init();
    config.payloadVariant.lora.region = Config_LoRaConfig_RegionCode_US; 
    initRegion();
    // RUN_TEST(Channels_initDefaults_ShouldSetMeshtasticChannelParams);
    RUN_TEST(RadioInterface_init_ShouldSetDefaultLoRAChannel);
    UNITY_END();
}

void loop()
{
}