#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED
#define clientAreaHor 1200
#define clientAreaVert 700

typedef struct SPoint {
    float x, y;
}TPoint;

typedef struct SObject {
    TPoint pos;
    TPoint size;
    TPoint speed;
    COLORREF brush;
    char oType;
    float range, vecSpeed;
    BOOL isDel;
}TObject, * PObject;

LRESULT CALLBACK WndProc(HWND ,UINT ,WPARAM ,LPARAM);
void ObjectInit(TObject* obj, float x, float y, float width, float height, char type);
void ObjectShow(TObject obj, HDC dc);
void WinInit(void);
void CenterWindow(HWND);
void AddMenus(HWND);
void WinShow(HDC dc, int hor, int vert);
void WinMove(void);
void ObjectMove(TObject* obj);
void LoadImageBtm(HWND hwnd, wchar_t path[]);
void ObjectSetDestPoint(TObject* obj, float xPos, float yPos, float vecSpeed);
void AddBullet(float xPos, float yPos, float x, float y);
BOOL ObjectCollision(TObject o1, TObject o2);
void DelObjects(void);
void GenerateEnemies(void);
void AddEnemy(float, float);
void ShowOnlyPlayer(HDC, int, int);

#endif
