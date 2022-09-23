#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR szCmdLine, int CmdShow) {

    printf("Hi1\n");
    MessageBoxW(NULL, szCmdLine, L"first Window", MB_YESNOCANCEL);
    printf("Hi2\n");
    return 0;
}

int main() {
    printf("Hi3\n");
    wWinMain(NULL,NULL,NULL,NULL);
    return 0;
}

