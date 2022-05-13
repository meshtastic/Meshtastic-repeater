#include <CubeFS.h>

uint16_t OSFS::startOfEEPROM = 1;
uint16_t OSFS::endOfEEPROM = (CY_FLASH_SIZEOF_ROW * 3);

CubeFS CUBEFS;

void OSFS::readNBytes(uint16_t address, unsigned int num, byte* output) {
	for (uint16_t i = address; i < address + num; i++) {
		*output = EEPROM.read(i);
		output++;
	}
}

void OSFS::writeNBytes(uint16_t address, unsigned int num, const byte* input) {
	for (uint16_t i = address; i < address + num; i++) {
		EEPROM.write(i, *input);
		input++;
	}
}

bool CubeFS::begin() {
    OSFS::result r = OSFS::checkLibVersion();
    if (r == OSFS::result::UNFORMATTED) {
        OSFS::format();
    }
    return true;
}