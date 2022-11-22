#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <math.h> 
#include "functions.h" 

#define ID_HOTKEY_QUIT 1
#define ID_HOTKEY_NEWGAME 2
#define IDM_NEW 1
#define IDM_QUIT 3
#define clientAreaHor 1200
#define clientAreaVert 700

TObject player;
PObject mas = NULL;
int masCounter = 0;
RECT clientArea;
HBITMAP hbtm;
wchar_t welcomeStr[] = L"D:\\Pictures\\welcome.bmp";
BOOL needNewGame = FALSE;

TPoint point(float x, float y) {
    TPoint point;
    point.x = x;
    point.y = y;
    return point;
}

PObject NewObject() { //function NewObject cteate element of array and return (last) element 
    masCounter++;
    mas = realloc(mas, sizeof(*mas) * masCounter);
    return mas + masCounter - 1;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    if (msg == WM_SIZE) GetClientRect(hwnd, &clientArea);
    else if (msg == WM_LBUTTONDOWN) {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        AddBullet(player.pos.x + player.size.x / 2,
            player.pos.y + player.size.y / 2,
            (float)xPos, (float)yPos);
    }

    switch (msg)
    {
    case WM_CREATE:
        AddMenus(hwnd);
        RegisterHotKey(hwnd, ID_HOTKEY_QUIT, MOD_ALT, 0x51);  // ALT + Q  to close the window
        RegisterHotKey(hwnd, ID_HOTKEY_NEWGAME, MOD_ALT, 0x4E);  // ALT + N  to restart
        CenterWindow(hwnd);
        break;

    case WM_PAINT:
        LoadImageBtm(hwnd, welcomeStr);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_NEW:
            needNewGame = TRUE;
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
        if ((wParam) == ID_HOTKEY_NEWGAME) {
            needNewGame = TRUE;
            MessageBeep(MB_ICONINFORMATION);
        }
        if ((wParam) == ID_HOTKEY_QUIT) {
            DestroyWindow(hwnd);
        }
        break;
    case WM_DESTROY:
        DeleteObject(hbtm);
        UnregisterHotKey(hwnd, ID_HOTKEY_QUIT);
        UnregisterHotKey(hwnd, ID_HOTKEY_NEWGAME);
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

    AppendMenuW(hMenu, MF_STRING, IDM_NEW, L"&New game      (ALT + N)");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_QUIT, L"&Quit      (ALT + Q) or ESC");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Game menu");
    SetMenu(hwnd, hMenubar);
}

void ShowOnlyPlayer(HDC dc, int hor, int vert) {

    HDC virtualDC = CreateCompatibleDC(dc); // virtual context
    HBITMAP virtualBITMAP = CreateCompatibleBitmap(dc, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
    SelectObject(virtualDC, virtualBITMAP); // so in a context virtualDC I will draw on the picture virtualBITMAP
    ObjectShow(player, virtualDC);

    BitBlt(dc, 0, 0, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top, virtualDC, 0, 0, SRCCOPY);
    DeleteDC(virtualDC);
    DeleteObject(virtualBITMAP);
}

void WinShow(HDC dc, int hor, int vert) {

    HDC virtualDC = CreateCompatibleDC(dc); // virtual context
    HBITMAP virtualBITMAP = CreateCompatibleBitmap(dc, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
    SelectObject(virtualDC, virtualBITMAP); // so in a context virtualDC I will draw on the picture virtualBITMAP
    /*SelectObject(virtualDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(virtualDC, RGB(25, 25, 25));
    Rectangle(virtualDC, 0, 0, hor, vert);*/
    ObjectShow(player, virtualDC);

    for (int i = 0; i < masCounter; i++)
    {
        ObjectShow(mas[i], virtualDC);
    }

    BitBlt(dc, 0, 0, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top, virtualDC, 0, 0, SRCCOPY);
    DeleteDC(virtualDC);
    DeleteObject(virtualBITMAP);
}
void PlayerControl() {
    static float playerSpeed = 10.0;
    player.speed.x = 0;
    player.speed.y = 0;
    if (GetKeyState('A') < 0) player.speed.x = -playerSpeed;
    if (GetKeyState('D') < 0) player.speed.x = playerSpeed;
    /*if (GetKeyState('W') < 0) player.speed.y = -playerSpeed;*/
   /* if (GetKeyState('S') < 0) player.speed.y = playerSpeed;*/
    if (player.pos.x < -20) player.pos.x = 0;
    if (player.pos.x > 1170) player.pos.x = 1150;
    //if (player.pos.y < -20) player.pos.y = 0;
    //if (player.pos.y > 620) player.pos.y = 600;
    if ((player.speed.x != 0) && (player.speed.y != 0))
        player.speed = point(player.speed.x * 0.7f, player.speed.y * 0.7f);
}
void GenerateEnemies() {

    int posX = (rand() % 2 == 0 ? -200 : 200);
    if (player.pos.x < 200) {
        posX = 150;
    }
    //int posY = 0;
    int k = rand() % 20;  // takt en cr
    if (k == 1) {
        AddEnemy((float)(player.pos.x + posX), (float)(player.pos.y - 700));
    }
    if (k == 2) {
        AddEnemy((float)(player.pos.x - posX), (float)(player.pos.y + 700));
    }
}
void WinMove() {

    if (needNewGame) WinInit();
    PlayerControl();
    ObjectMove(&player);
    for (int i = 0; i < masCounter; i++)
    {
        ObjectMove(mas + i);
    }
    GenerateEnemies();
    DelObjects();
}

void DelObjects() {
    int i = 0;
    while (i < masCounter)
    {
        if (mas[i].isDel) {
            masCounter--;
            mas[i] = mas[masCounter];
            // and allocate memory
            mas = realloc(mas, sizeof(*mas) * masCounter);
        }
        else {
            i++;
        }
    }
}


void ObjectInit(TObject* obj, float x, float y, float width, float height, char obType) {
    obj->pos = point(x, y);
    obj->speed = point(0, 0);
    obj->size = point(width, height);
    obj->brush = RGB(200, 150, 30);
    obj->oType = obType;
    obj->isDel = FALSE;
    if (obType == 'z') {
        obj->brush = RGB(110, 50, 20);
    }
    if (obType == 'b') {
        obj->brush = RGB(220, 0, 10);
    }
}

void ObjectShow(TObject obj, HDC dc) {
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(255, 0, 0));
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, obj.brush);

    if (obj.oType == 'z') {
        Rectangle(dc, (int)(obj.pos.x), (int)(obj.pos.y),
            (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.y));
    }
    else {
        Ellipse(dc, (int)(obj.pos.x), (int)(obj.pos.y),
            (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.y));
    }
}

void WinInit() {
    needNewGame = FALSE;
    masCounter = 0;
    mas = realloc(mas, 0); // cleaning mas in the begin of the game
    ObjectInit(&player, clientAreaHor / 2, clientAreaVert - 125, 40, 40, 'i');
}

void ObjectMove(TObject* obj) {
    if (obj->oType == 'z') {
        if (rand() % 50 == 1) {
            static float enemySpeed = 6.5;
            ObjectSetDestPoint(obj, player.pos.x, 700, enemySpeed);
        }

        for (int i = 0; i < masCounter; i++)
        {
            if (obj->pos.y > 600) {
                obj->isDel = TRUE;
            }
        }

        if (ObjectCollision(*obj, player)) {
            needNewGame = TRUE; // GG WP
        }
    }
    obj->pos.x += obj->speed.x;
    obj->pos.y += obj->speed.y;

    if (obj->oType == 'b') {
        obj->range -= obj->vecSpeed;
        if (obj->range < 0) obj->isDel = TRUE; // bullet will be deleted if range be exceeded
        for (int i = 0; i < masCounter; i++)
        {
            if ((mas[i].oType == 'z') && (ObjectCollision(*obj, mas[i]))) {
                mas[i].isDel = TRUE;
                obj->isDel = TRUE;
            }
        }
    }
}

void LoadImageBtm(HWND hwnd, wchar_t path[]) {

    HDC hdc;
    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;

    hbtm = LoadImageW(NULL, path,
        IMAGE_BITMAP, 1200, 700, LR_LOADFROMFILE);
    hdc = BeginPaint(hwnd, &ps);
    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, hbtm);
    GetObject(hbtm, sizeof(bitmap), &bitmap);
    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
        hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
    EndPaint(hwnd, &ps);
}

void ObjectSetDestPoint(TObject* obj, float xPos, float yPos, float vecSpeed) {

    TPoint xyLen = point(xPos - obj->pos.x, yPos - obj->pos.y); // found length between obj and destination point
    float dxy = (float)sqrt(xyLen.x * xyLen.x + xyLen.y * xyLen.y);  // directly path
    // and set speed to each axis
    obj->speed.x = xyLen.x / dxy * vecSpeed;
    obj->speed.y = xyLen.y / dxy * vecSpeed;
    obj->vecSpeed = vecSpeed;
}

void AddBullet(float xPos, float yPos, float x, float y) {
    PObject obj = NewObject();
    ObjectInit(obj, xPos, yPos, 10, 10, 'b');
    ObjectSetDestPoint(obj, x, y, 20);
    obj->range = 600;
}

BOOL ObjectCollision(TObject o1, TObject o2) {
    return ((o1.pos.x + o1.size.x) > o2.pos.x) && (o1.pos.x < (o2.pos.x + o2.size.x)) &&
        ((o1.pos.y + o1.size.y) > o2.pos.y) && (o1.pos.y < (o2.pos.y + o2.size.y));
}

void AddEnemy(float a, float b) { ObjectInit(NewObject(), a, b, 25, 25, 'z'); }