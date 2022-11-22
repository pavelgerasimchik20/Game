#pragma comment(linker, "\"/manifestdependency:type='Win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <math.h> 
#include "functions.h" 

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow) {

    MSG msg;
    HWND hwnd;
    WNDCLASSW wc; // reg

    wc.style = CS_HREDRAW | CS_VREDRAW; // vertical and horizontal redrawing
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszClassName = L"Game field";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DHILIGHT);
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursorW(NULL, IDC_CROSS); //crossline
    wc.hIcon = LoadIcon(NULL, IDI_ERROR);

    RegisterClassW(&wc);
    hwnd = CreateWindowW(wc.lpszClassName, L"Star Wars",
        WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) | WS_VISIBLE,
        0, 0, clientAreaHor, clientAreaVert, NULL, NULL, hInstance, NULL);

    HDC dc = GetDC(hwnd);  // getting context of the window

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    WinInit();

    ShowCursor(FALSE);
    Sleep(2500); // show welcome page
    ShowOnlyPlayer(dc, clientAreaHor, clientAreaVert); 
    Sleep(2500);
    ShowCursor(TRUE);
    SetCursorPos((clientAreaHor/2) + 190, (clientAreaVert/2)); //set cursor to middle of the game field
    while (1) {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) { // check the messages queue
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            WinMove();
            WinShow(dc, clientAreaHor, clientAreaVert);
            Sleep(1);
        }
        //printf("msg.message: %d & msg.wParam: %d\n", msg.message, msg.wParam);  //that needs to spy on action codes of messages 
    }
    return 0;
}