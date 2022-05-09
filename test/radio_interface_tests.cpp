#include <Arduino.h>
#include <unity.h>
#include "mesh/SX1262Interface.h"

// RadioInterface is abstract so we'll declare an implementation
//RadioInterface *radioInterface = new SX1262Interface(-1, -1, -1, -1, SPI);

void setUp(void) 
{
    
}

void tearDown(void) 
{
}

void initDefaults_Should_SetMeshtasticChannelParams(void) 
{
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