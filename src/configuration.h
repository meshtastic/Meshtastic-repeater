#pragma once

#include <Arduino.h>
// -----------------------------------------------------------------------------
// Version
// -----------------------------------------------------------------------------

// If app version is not specified we assume we are not being invoked by the build script
// #ifndef APP_VERSION
// #error APP_VERSION must be set by the build environment
// #endif

// FIXME: This is still needed by the Bluetooth Stack and needs to be replaced by something better. Remnant of the old versioning system.
#ifndef HW_VERSION
#define HW_VERSION "1.0"
#endif

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

/// Convert a preprocessor name into a quoted string
#define xstr(s) str(s)
#define str(s) #s

/// Convert a preprocessor name into a quoted string and if that string is empty use "unset"
#define optstr(s) (xstr(s)[0] ? xstr(s) : "unset")

#include "variant.h"
#include "RF95Configuration.h"
#include "DebugConfiguration.h"
