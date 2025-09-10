#pragma once
#include <windows.h>
#include <vector>
#include "ButtonTool.h"

#include "PenView.h"
#include "DrawPoints.h"
#include "PenReplay.h"
#include "ColorPicker.h"
#include "ColorManager.h"
#include "FileSave.h"
#include "FileLoad.h"

struct CanvasBuffer {
    HDC      memDC = nullptr;
    HBITMAP  bmp = nullptr;
    HBITMAP  oldBmp = nullptr;
    int      w = 0, h = 0;

    void destroy() {
        if (memDC) {
            if (oldBmp) { SelectObject(memDC, oldBmp); oldBmp = nullptr; }
            if (bmp) { DeleteObject(bmp); bmp = nullptr; }
            DeleteDC(memDC); memDC = nullptr;
        }
        w = h = 0;
    }

    // hwnd의 클라이언트 크기에 맞춰 버퍼 생성/재생성
    void resizeTo(HWND hwnd) {
        RECT rc; GetClientRect(hwnd, &rc);
        int nw = max(1, rc.right - rc.left);
        int nh = max(1, rc.bottom - rc.top);
        if (nw == w && nh == h && memDC && bmp) return; // 사이즈 동일 → 유지

        destroy();

        HDC wndDC = GetDC(hwnd);
        memDC = CreateCompatibleDC(wndDC);
        bmp = CreateCompatibleBitmap(wndDC, nw, nh);
        oldBmp = (HBITMAP)SelectObject(memDC, bmp);
        ReleaseDC(hwnd, wndDC);

        w = nw; h = nh;

        // 초기 배경 지우기(흰색)
        HBRUSH br = (HBRUSH)(COLOR_WINDOW + 1);
        RECT fill = { 0,0,w,h };
        FillRect(memDC, &fill, br);
    }

    // 배경 또는 전체 지우기
    void clear(COLORREF rgb = RGB(255, 255, 255)) {
        HBRUSH hb = CreateSolidBrush(rgb);
        RECT r = { 0,0,w,h };
        FillRect(memDC, &r, hb);
        DeleteObject(hb);
    }
};  

/// WindowTool 클래스: 메인 윈도우와 영역 분할 관리
class WindowTool {
public:
    WindowTool(HINSTANCE hInstance);
    bool createMainWindow(int width, int height);
    void showWindow(int nCmdShow);
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    static constexpr int TOP_HEADER_HEIGHT = 50;
    static constexpr int TOOLBAR_HEIGHT = 48;
    static constexpr int BUTTON_MARGIN = 8;
    static constexpr COLORREF HEADER_BG = RGB(144, 238, 144);

    HWND canvasArea = nullptr;     
    CanvasBuffer canvas;           // 더블버퍼

    HINSTANCE hInstance;
    HWND mainWindow{};
    HWND headerPanel{};
    HWND toolbarPanel{};

    std::vector<ButtonTool*> buttons;

    PenView penView;
    DrawPoints drawPoints;
    PenReplay penReplay;
    ColorPicker colorPicker;
    ColorManager colorMgr;
    FileSave fileSave;
    FileLoad fileLoad;

    bool isDrawing = false;
    POINT lastPt{ 0,0 };

    void createChildWindows(HWND parent);
    void resizeChildWindows(RECT clientRect);
    void layoutToolbarButtons(int width, int height);

    /// 캔버스 전용 윈도우 프로시저
    static LRESULT CALLBACK canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};