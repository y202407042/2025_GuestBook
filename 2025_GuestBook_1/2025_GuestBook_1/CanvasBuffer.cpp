#include "CanvasBuffer.h"

CanvasBuffer::CanvasBuffer()
    : memDC(NULL), bmp(NULL), oldBmp(NULL), w(0), h(0) {
}

void CanvasBuffer::resizeTo(HWND hwnd) {
    // 기존 버퍼 정리
    if (memDC) {
        SelectObject(memDC, oldBmp);
        DeleteObject(bmp);
        DeleteDC(memDC);
        memDC = NULL; bmp = NULL; oldBmp = NULL;
    }

    // 새 버퍼 만들기
    HDC hdc = GetDC(hwnd);
    memDC = CreateCompatibleDC(hdc);

    RECT rc;
    GetClientRect(hwnd, &rc);
    w = rc.right - rc.left;
    h = rc.bottom - rc.top;

    bmp = CreateCompatibleBitmap(hdc, w, h);
    oldBmp = (HBITMAP)SelectObject(memDC, bmp);

    ReleaseDC(hwnd, hdc);

    // 초기화
    PatBlt(memDC, 0, 0, w, h, WHITENESS);
}

void CanvasBuffer::destroy() {
    if (memDC) {
        SelectObject(memDC, oldBmp);
        DeleteObject(bmp);
        DeleteDC(memDC);
        memDC = NULL; bmp = NULL; oldBmp = NULL;
        w = h = 0;
    }
}
