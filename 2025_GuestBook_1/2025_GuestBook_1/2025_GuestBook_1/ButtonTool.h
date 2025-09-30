#pragma once
#include <windows.h>

class ButtonTool {
public:
    ButtonTool(HINSTANCE hInst, HWND prnt, int btnId, LPCWSTR btnText);

    void create(int x, int y, int width, int height);
    void move(int x, int y, int width, int height);
    HWND handle() const { return hWnd; }

private:
    HINSTANCE hInstance;
    HWND parent;
    HWND hWnd;
    int id;
    LPCWSTR text;
};
