#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#define ID_HOTKEY 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CenterWindow(HWND);

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow) {

    MSG msg;
    HWND hwnd;
    WNDCLASSW wc;

    wc.style = CS_HREDRAW | CS_VREDRAW ; // vertical and horizontal redrawing
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszClassName = L"Game field";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DHILIGHT);
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_ERROR);

    RegisterClassW(&wc);
    hwnd = CreateWindowW(wc.lpszClassName, L"Catch me if you can",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, 800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {

        DispatchMessage(&msg);
    }
   
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    switch (msg) 
    {
    case WM_CREATE:
        RegisterHotKey(hwnd, ID_HOTKEY, MOD_ALT, 0x51);  // ALT + Q  to close the window
        CenterWindow(hwnd);
        break;

    case WM_HOTKEY:
        if ((wParam) == ID_HOTKEY) {
            CloseWindow(hwnd);
        }
        break;
    case WM_DESTROY:
        UnregisterHotKey(hwnd, ID_HOTKEY);
        PostQuitMessage(EXIT_SUCCESS);    
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void CenterWindow(HWND hwnd) {

    RECT rc = { 0 };

    GetWindowRect(hwnd, &rc);
    int win_w = rc.right - rc.left;
    int win_h = rc.bottom - rc.top;

    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w) / 2,
        (screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}

