#pragma once

#include "AEConfig.h"
#include "AEGP_SuiteHandler.h"
#include "AE_GeneralPlug.h"
#include "SPBasic.h"
#include "SoCClient.h"
#include "entry.h"

#ifdef _WIN32
#include "windows.h"
#define WIN32_LEAN_AND_MEAN
#define __export extern "C" DllExport
#define strdup _strdup
#elif defined __APPLE__
#include <dlfcn.h>
#define __export extern "C" DllExport
#endif

#define InitError() int err = 0;
#define GetError() err;
#define ListenError(f) \
    if (err == 0)      \
        err = f;

__export AEGP_PluginInitFuncPrototype master;
