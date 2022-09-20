#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <wchar.h>

int wmain(int argc, wchar_t** argv) {

    PDWORD cChars = NULL;
    HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("Hi its me");

    if (std == INVALID_HANDLE_VALUE) {
        wprintf(L"Cannot retrieve standard output handle\n (%d)",
            GetLastError());
    }

    if (argv[1]) {

        WriteConsoleW(std, argv[1], wcslen(argv[1]), cChars, NULL);
    }
    
    CloseHandle(std);

    return 0;
}