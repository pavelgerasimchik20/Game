#pragma comment(linker, "\"/manifestdependency:type='Win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#define ID_HOTKEY_QUIT 1
#define ID_HOTKEY_NEW 2
#define IDM_NEW 1
#define IDM_FULL 2
#define IDM_QUIT 3
#define masSize 5
#define pixelMove 10 // this var will responce for speed of obj   ***TODO pick mode ???***
RECT mas[masSize];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CenterWindow(HWND);
void AddMenus(HWND);
void WinShow(HDC dc);
void InitCoordinates(void);
void ItemMove(void);

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
    HDC dc = GetDC(hwnd);  // getting context of the window

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    InitCoordinates();

    while (1) {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) { // check the mess queue
            if (msg.message == WM_QUIT)break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            ItemMove();
            WinShow(dc);
            Sleep(1);
        }
        //printf("msg.message: %d & msg.wParam: %d\n", msg.message, msg.wParam);  that needs to spy on action codes
    }
   
    return 0;
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
        return 0;

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

void WinShow(HDC dc) {
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(15, 0, 12));
    Rectangle(dc, 0, 0, 800, 600);
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCBrushColor(dc, RGB(200, 0, 0));
    for (int i = 0; i < masSize; i++)
    {
        Rectangle(dc, mas[i].left, mas[i].top, mas[i].right, mas[i].bottom);
    }
}

void InitCoordinates() {
    srand(13);
    for (int i = 0; i < masSize; i++)
    {
        mas[i].left = rand() % 800 - 50;
        mas[i].top = rand() % 400;
        mas[i].right = mas[i].left + 50;
        mas[i].bottom = mas[i].top + 50;
    }
}

void ItemMove(){
    for (int i = 0; i < masSize; i++)
    {
        mas[i].left += pixelMove;     
        if(mas[i].left > 750) mas[i].left = -50;
        mas[i].right = mas[i].left + 50;
        mas[i].bottom = mas[i].top + 50;
    }
}