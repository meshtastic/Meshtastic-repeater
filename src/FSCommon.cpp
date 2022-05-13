#include "configuration.h"
#include "FSCommon.h"
#include "assert.h"

#ifdef FSCom
void listDir(const char * dirname, uint8_t levels)
{
    // Cubecell has flat 8.3 filesystem. Interate through EEPROM
#ifdef OSFS_ID_STR
    OSFS::result r = OSFS::checkLibVersion();
    if (r != OSFS::result::NO_ERROR)
			return;
    OSFS::fileHeader workingHeader;
	uint16_t workingAddress = OSFS::startOfEEPROM + sizeof(OSFS::FSInfo);
    while (true) {
        OSFS::result r = readNBytesChk(workingAddress, sizeof(OSFS::fileHeader), &workingHeader);
        if (r != OSFS::result::NO_ERROR)
            return;
        if (!isDeletedFile(workingHeader)) {
            DEBUG_MSG("  %s (%i Bytes)\n", workingHeader.fileID, workingHeader.fileSize);
        }
        if (workingHeader.nextFile == 0) {
            return;
        }
        workingAddress = workingHeader.nextFile;
    }
#else
    File root = FSCom.open(dirname);
    if(!root){
        return;
    }
    if(!root.isDirectory()){
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory() && !String(file.name()).endsWith(".")) {
            if(levels){
                listDir(file.name(), levels -1);
            }
        } else {
            DEBUG_MSG("  %s (%i Bytes)\n", file.name(), file.size());
        }
        file.close();
        file = root.openNextFile();
    }
    file.close();
#endif
}

void fsInit()
{
    if (!FSBegin())
    {
        DEBUG_MSG("ERROR filesystem mount Failed. Formatting...\n");
        assert(0); // FIXME - report failure to phone
    }

    DEBUG_MSG("Filesystem files:\n");
    listDir("/", 10);
}
#endif
