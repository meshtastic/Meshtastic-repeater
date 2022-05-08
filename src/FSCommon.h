#pragma once

#include "configuration.h"

// Cross platform filesystem API

#ifdef PORTDUINO
// Portduino version
#include "PortduinoFS.h"
#define FSCom PortduinoFS
#define FSBegin() true
#define FILE_O_WRITE "w"
#define FILE_O_READ "r"
#elif defined(NRF52_SERIES)
// NRF52 version
#include "InternalFileSystem.h"
#define FSCom InternalFS
#define FSBegin() FSCom.begin()
using namespace Adafruit_LittleFS_Namespace;
#else
// ESP32 version
#include "LITTLEFS.h"
#define FSCom LITTLEFS
#define FSBegin() FSCom.begin(true)
#define FILE_O_WRITE "w"
#define FILE_O_READ "r"
#endif

void fsInit();