#pragma once
#include <windows.h>

class CanvasBuffer {
public:
    HDC     memDC;
    HBITMAP bmp;
    HBITMAP oldBmp;
    int     w, h;

    CanvasBuffer();
    void resizeTo(HWND hwnd);
    void destroy();
};
