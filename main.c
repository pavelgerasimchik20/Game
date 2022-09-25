#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#define ID_HOTKEY_QUIT 1
#define ID_HOTKEY_NEW 2
#define IDM_NEW 1
#define IDM_FULL 2
#define IDM_QUIT 3

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CenterWindow(HWND);
void AddMenus(HWND);

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

    ShowWindow(hwnd, SW_SHOWNORMAL);
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
        AddMenus(hwnd);
        RegisterHotKey(hwnd, ID_HOTKEY_QUIT, MOD_ALT, 0x51);  // ALT + Q  to close the window
        RegisterHotKey(hwnd, ID_HOTKEY_NEW, MOD_ALT, 0x4E);  // ALT + N  to restart
        CenterWindow(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_NEW:
            MessageBeep(MB_ICONINFORMATION);
            break;
        case IDM_FULL:
            ShowWindow(hwnd, SW_MAXIMIZE);
            break;
        case IDM_QUIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        break;

    case WM_HOTKEY:
        if ((wParam) == ID_HOTKEY_QUIT) {
            DestroyWindow(hwnd);
        }
        if ((wParam) == ID_HOTKEY_NEW) {
            MessageBeep(MB_ICONINFORMATION);
        }
        break;
    case WM_DESTROY:
        UnregisterHotKey(hwnd, ID_HOTKEY_QUIT);
        UnregisterHotKey(hwnd, ID_HOTKEY_NEW);
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

void AddMenus(HWND hwnd) {

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_NEW, L"&New          (ALT + N)");
    AppendMenuW(hMenu, MF_STRING, IDM_FULL, L"&Full screen");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_QUIT, L"&Quit         (ALT + Q)");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Game menu");
    SetMenu(hwnd, hMenubar);
}
