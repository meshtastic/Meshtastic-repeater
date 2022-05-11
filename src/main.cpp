#include "DeviceStore.h"
#include "Repeater.h"

Repeater *repeater = NULL; 

void setup()
{
    deviceStore.init();
    repeater = new Repeater();
    repeater->init();
}

void loop() 
{
}