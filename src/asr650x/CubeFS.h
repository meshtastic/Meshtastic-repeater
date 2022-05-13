#pragma once
#ifndef CubeFS_H
#define CubeFS_H

#include <OSFS.h>
#include <EEPROM.h>

extern uint16_t OSFS::startOfEEPROM;
extern uint16_t OSFS::endOfEEPROM;

#define FILE_READ       "r"
#define FILE_WRITE      "w"
#define FILE_APPEND     "a"

class CubeFS
{
public:
    bool begin();
};

extern CubeFS CUBEFS;

#endif //CubeFS_H