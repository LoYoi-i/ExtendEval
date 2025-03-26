#include "ExtendEval.h"
#include <thread>

struct SPBasicSuite* globalPicaBasicPtr = nullptr;

const char* eval(const char* code, int* errCode)
{
    InitError();
    const char* result = nullptr;
    AEGP_SuiteHandler suite(globalPicaBasicPtr);
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
        *errCode = GetError();

    ListenError(MemorySuite->AEGP_LockMemHandle(*targetResult, (void**)(result)));

    ListenError(MemorySuite->AEGP_UnlockMemHandle(*targetResult));
    ListenError(MemorySuite->AEGP_FreeMemHandle(*targetResult));
    *errCode = GetError();
    return result;
}

typedef const uint8_t* (*CallbackFn)(const uint8_t* data, int len);

typedef const char*(_cdecl* Start_pipe_server)(CallbackFn fn);

const uint8_t* callback(const uint8_t* data, int length)
{
    int errCode = 0;
    const char* result = eval(reinterpret_cast<const char*>(data), &errCode);
    return reinterpret_cast<const uint8_t*>(result);
}

int master(
    struct SPBasicSuite* pica_basicP,
    A_long driver_major_versionL,
    A_long driver_minor_versionL,
    AEGP_PluginID aegp_plugin_id,
    AEGP_GlobalRefcon* plugin_refconP)
{
    globalPicaBasicPtr = pica_basicP;
    std::thread([]() {
        std::this_thread::yield();
        HMODULE dll = LoadLibraryA("./Plug-ins/pipe_server.dll");
        FARPROC fn = GetProcAddress(dll, "start_pipe_server");
        Start_pipe_server start_pipe_server = reinterpret_cast<Start_pipe_server>(fn);
        start_pipe_server(callback);
        FreeLibrary(dll);
    }).detach();
    return 0;
}
