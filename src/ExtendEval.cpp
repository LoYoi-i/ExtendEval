#include "ExtendEval.h"
#include <thread>

static int evalExtendScript(AEGP_SuiteHandler& suite, const char* code, const char** result)
{
    InitError();
    AEGP_MemorySuite1* MemorySuite = suite.MemorySuite1();
    AEGP_MemSize size = 0;
    AEGP_MemHandle outResult = NULL;
    AEGP_MemHandle outErrorString = NULL;
    AEGP_MemHandle* targetResult = nullptr;
    ListenError(suite.UtilitySuite5()->AEGP_ExecuteScript(0, code, false, &outResult, &outErrorString));

    if (outResult)
        targetResult = &outResult;
    else if (outErrorString)
        targetResult = &outErrorString;

    if (targetResult == nullptr)
        return GetError();

    ListenError(MemorySuite->AEGP_LockMemHandle(*targetResult, (void**)(result)));

    ListenError(MemorySuite->AEGP_UnlockMemHandle(*targetResult));
    ListenError(MemorySuite->AEGP_FreeMemHandle(*targetResult));
    return GetError();
}

static void processMessage(struct SPBasicSuite* sp, const char* message, char* result)
{
    InitError();
    AEGP_SuiteHandler suite(sp);
    const char* evalResult = nullptr;
    ListenError(evalExtendScript(suite, message, &evalResult));
    if (err == 0)
        sprintf(result, evalResult);
    else
        suite.UtilitySuite3()->AEGP_WriteToOSConsole("Error: Failed to evaluate script");
}

int master(
    struct SPBasicSuite* pica_basicP,
    A_long driver_major_versionL,
    A_long driver_minor_versionL,
    AEGP_PluginID aegp_plugin_id,
    AEGP_GlobalRefcon* global_refconP)
{
    globalPicaBasicPtr = pica_basicP;
    std::thread([]() {
        while (true) {
            std::this_thread::yield();
            int err = pipeServer(
                L"\\\\.\\pipe\\ExtendEval",
                globalPicaBasicPtr,
                processMessage);
            if (err == 1)
                ((AEGP_SuiteHandler)globalPicaBasicPtr).UtilitySuite3()->AEGP_WriteToOSConsole("Failed to start pipe server");
        }
    }).detach();
    return 0;
}
