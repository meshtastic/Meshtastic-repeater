#include "mesh/MeshRadio.h"
#include <SPI.h>
#include "configuration.h"
#include "DeviceStore.h"
#include "Repeater.h"

Repeater *repeater = NULL; 

void setup()
{
    SPI.begin();
    #ifdef DEBUG_PORT
        consoleInit(); // Set serial baud rate and init our mesh console
    #endif
    deviceStore.init();
    config.payloadVariant.lora.region = Config_LoRaConfig_RegionCode_US; 
    repeater = new Repeater();
    repeater->init();
}

void loop() 
{
    delay(100);
    DEBUG_MSG("Waiting for a packet %i\n", millis());
}