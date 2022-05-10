#include <Arduino.h>
#include <unity.h>
#include "mesh/Channels.h"
#include "mesh/RF95Interface.h"
#include "mesh/SX1262Interface.h"

// RadioInterface is abstract so we'll declare an implementation
RadioInterface *radioInterface = NULL;

ChannelIndex DefaultChannelIndex = 0;

#define USE_SX1262
#define SX126X_CS 0
#define SX126X_DIO1 0
#define SX126X_BUSY 0
#define SX126X_RESET 0
#define SX126X_TXEN 0
#define SX126X_RXEN 0
#define SX126X_E22 // DIO2 controlls an antenna switch and the TCXO voltage is controlled by DIO3

#define RF95_NSS 0
#define RF95_IRQ 0
#define RF95_RESET 0

void setUp(void) 
{
    channels.initDefaults();
    radioInterface = new RF95Interface(RF95_NSS, RF95_IRQ, RF95_RESET, SPI);
    radioInterface = new SX1262Interface(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI);
}

void tearDown(void) 
{
}

void initDefaults_Should_SetMeshtasticChannelParams(void) 
{
    auto channelName = channels.getName(DefaultChannelIndex);
    TEST_ASSERT_EQUAL_STRING("LongFast", channelName);

    auto primaryName = channels.getPrimaryName();
    TEST_ASSERT_EQUAL_STRING("#LongFast-V", primaryName);
}

void setup() 
{
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(initDefaults_Should_SetMeshtasticChannelParams);
    UNITY_END();
}

void loop() 
{
}