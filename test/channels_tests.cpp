#include <Arduino.h>
#include <unity.h>
#include "mesh/Channels.h"

ChannelIndex DefaultChannelIndex = 0;

void setUp(void) 
{
    channels.initDefaults();
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
    // digitalWrite(13, HIGH);
    // delay(100);
    // digitalWrite(13, LOW);
    // delay(500);
}