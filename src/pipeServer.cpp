#include "pipeServer.h"
#include <iostream>

int pipeServer(LPCWSTR lpName, struct SPBasicSuite* sp, ProcessMessage processMessage)
{
    HANDLE hPipe = CreateNamedPipeW(
        lpName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        4096,
        4096,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateNamedPipe failed. Error: " << GetLastError() << std::endl;
        return 1;
    }

    BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

    if (!connected) {
        std::cerr << "ConnectNamedPipe failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    char buf[4096] = { 0 };
    DWORD rLen = 0;

    if (!ReadFile(hPipe, buf, sizeof(buf) - 1, &rLen, NULL) || rLen == 0) {
        std::cerr << "ReadFile failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    buf[rLen] = '\0';

    char resultBuffer[4096] = { 0 };
    processMessage(sp, buf, resultBuffer);

    DWORD wLen = 0;
    WriteFile(
        hPipe,
        resultBuffer,
        strlen(resultBuffer),
        &wLen,
        NULL);

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 0;
}
