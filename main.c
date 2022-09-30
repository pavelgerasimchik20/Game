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
#define clientAreaHor 1200
#define clientAreaVert 700
#define pixelMove 5 // this var will responce for speed of obj   ***TODO pick mode ???***
RECT clientArea; 

typedef struct SPoint { //bla bla
    float x, y;
}TPoint;

typedef struct SObject {
    TPoint pos;
    TPoint size;
    TPoint speed;
    COLORREF brush;
}TObject;

TObject player;

TPoint point(float x, float y);
void ObjectInit(TObject* obj, float x, float y, float width, float height);
void ObjectShow(TObject obj, HDC dc);
void WinInit(void);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CenterWindow(HWND);
void AddMenus(HWND);
void WinShow(HDC dc, int hor, int vert);
void WinMove(void);
void ObjectMove(TObject* obj);

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
        WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) | WS_VISIBLE,
        0, 0, clientAreaHor, clientAreaVert, NULL, NULL, hInstance, NULL);
    HDC dc = GetDC(hwnd);  // getting context of the window

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    WinInit();  

    Sleep(3000); // show welcome page
    while (1) {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) { // check the messages queue
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            WinMove();
            WinShow(dc,clientAreaHor,clientAreaVert);
            Sleep(1);
        }
        printf("msg.message: %d & msg.wParam: %d\n", msg.message, msg.wParam);  //that needs to spy on action codes of messages 
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    static HBITMAP hbtm;
    HDC hdc;
    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;

    if (msg == WM_SIZE) GetClientRect(hwnd, &clientArea);
    switch (msg) 
    {
    case WM_CREATE:
        AddMenus(hwnd);
        RegisterHotKey(hwnd, ID_HOTKEY_QUIT, MOD_ALT, 0x51);  // ALT + Q  to close the window
        RegisterHotKey(hwnd, ID_HOTKEY_NEW, MOD_ALT, 0x4E);  // ALT + N  to restart
        CenterWindow(hwnd);
        break;

    case WM_PAINT:
        hbtm = (HBITMAP)LoadImageW(NULL, L"D:\\Pictures\\welcome.bmp",
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hdc = BeginPaint(hwnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, hbtm);
        GetObject(hbtm, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight,
            hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_NEW:
            MessageBeep(MB_ICONINFORMATION);
            break;
        case IDM_QUIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            int ret = MessageBoxW(hwnd, L"Are you sure to quit?",
                L"Message", MB_OKCANCEL);
            if (ret == IDOK) {
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            }
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
        DeleteObject(hbtm);
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
    //AppendMenuW(hMenu, MF_STRING, IDM_FULL, L"&Full screen");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_QUIT, L"&Quit         (ALT + Q)");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Game menu");
    SetMenu(hwnd, hMenubar);
}

void WinShow(HDC dc, int hor, int vert) {

    HDC virtualDC = CreateCompatibleDC(dc); // virtual context
    HBITMAP virtualBITMAP = CreateCompatibleBitmap(dc, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
    SelectObject(virtualDC, virtualBITMAP); // so in a context virtualDC I will draw on the picture virtualBITMAP
        SelectObject(virtualDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(virtualDC, RGB(25, 0, 12));
        Rectangle(virtualDC, 0, 0, hor, vert);

        ObjectShow(player, virtualDC);
        /*SelectObject(virtualDC, GetStockObject(DC_PEN));
        SetDCBrushColor(virtualDC, RGB(100, 0, 0));
        for (int i = 0; i < masSize; i++)
        {
            Rectangle(virtualDC, mas[i].left, mas[i].top, mas[i].right, mas[i].bottom);
        }*/
        BitBlt(dc, 0, 0, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top, virtualDC, 0, 0, SRCCOPY);
        DeleteDC(virtualDC);
        DeleteObject(virtualBITMAP);
}
void PlayerControl() {
    static float playerSpeed = 4.0;
    player.speed.x = 0;
    player.speed.y = 0;
    if (GetKeyState('W') < 0) player.speed.y = -playerSpeed;
    if (GetKeyState('S') < 0) player.speed.y = playerSpeed;
    if (GetKeyState('A') < 0) player.speed.x = -playerSpeed;
    if (GetKeyState('D') < 0) player.speed.x = playerSpeed;
    if ((player.speed.x != 0) && (player.speed.y != 0))
        player.speed = point(player.speed.x * 0.7f, player.speed.y * 0.7f);
}

void WinMove(){
    PlayerControl();
    ObjectMove(&player);
}

TPoint point(float x, float y) {
    TPoint point;
    point.x = x;
    point.y = y;
    return point;
}

void ObjectInit(TObject* obj, float x, float y, float width, float height) {
    obj->pos = point(x, y);
    obj->speed = point(0, 0);
    obj->size = point(width, height);
    obj->brush = RGB(200, 150, 30);
}

void ObjectShow(TObject obj, HDC dc) {
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, obj.brush);
    Rectangle(dc, (int)(obj.pos.x), (int)(obj.pos.y),
        (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.y));
}

void WinInit() {
    ObjectInit(&player, 100, 100, 40, 40);
}

void ObjectMove(TObject* obj) {
    obj->pos.x += obj->speed.x;
    obj->pos.y += obj->speed.y;
}// bla bla 