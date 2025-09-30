#define NOMINMAX
#include "ColorBoxPanel.h"
#include <Windowsx.h>   // GET_X_LPARAM, GET_Y_LPARAM
#include <cmath>
#include <algorithm>

#ifndef GWLP_USERDATA
#define GWLP_USERDATA (-21)
#endif

// ---- 내부 전용 상태 (헤더에 노출 안 함)
struct PanelState {
    ColorBox box;
    // HSV 상태
    double hue = 0.0, sat = 1.0, val = 1.0;
    // 두께 상태
    int thickMin = 1, thickMax = 30;
    int thickness = 10;

    // 레이아웃
    POINT  wheelCenter{ 0,0 };
    int    wheelRadius = 120;
    RECT   valueBar{ 0,0,0,0 };     // V(밝기) 바
    RECT   thickBar{ 0,0,0,0 };     // 두께 라운드 바
    RoundRECT thickRR{ 0,0,0,0, 10,10 };
    RECT   swatch{ 0,0,0,0 };       // 현재색 미리보기

    // 입력
    bool dragWheel = false, dragV = false, dragT = false;

    // 콜백
    std::function<void(COLORREF, int)> onChanged;
};

static bool PtInRectI(const RECT& r, int x, int y) {
    return x >= r.left && x < r.right && y >= r.top && y < r.bottom;
}
static int clampi(int v, int lo, int hi) { return (v < lo) ? lo : ((v > hi) ? hi : v); }

static void ComputeLayout(HWND h, PanelState* st) {
    RECT rc; GetClientRect(h, &rc);
    const int W = rc.right - rc.left, H = rc.bottom - rc.top;

    st->wheelRadius = std::min(W, H) / 3;
    st->wheelCenter = { W / 2, st->wheelRadius + 10 };

    const int pad = 10, barH = 18;
    st->valueBar = { pad, H - pad - barH, W - pad, H - pad };

    st->thickBar = { pad, st->valueBar.top - 10 - barH, W - pad, st->valueBar.top - 10 };
    st->thickRR = { st->thickBar.left, st->thickBar.top,
                     st->thickBar.right, st->thickBar.bottom, 10, 10 };

    st->swatch = { pad, st->thickBar.top - 10 - 35, pad + 100, st->thickBar.top - 10 };
}

static void DrawTrackWithKnob(HDC hdc, const RECT& rc, double t /*0..1*/, bool filled, COLORREF fillColor)
{
    // 트랙: 밝은 회색 라운드
    const int radius = (rc.bottom - rc.top) / 2;
    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, radius, radius);

    // 진행 영역 (선택): 옵션
    if (filled) {
        RECT prog = rc;
        prog.right = rc.left + int((rc.right - rc.left) * t);
        HBRUSH fill = CreateSolidBrush(fillColor);
        HBRUSH oldB = (HBRUSH)SelectObject(hdc, fill);
        RoundRect(hdc, prog.left, prog.top, prog.right, prog.bottom, radius, radius);
        SelectObject(hdc, oldB);
        DeleteObject(fill);
    }

    // 노브
    int cx = rc.left + int((rc.right - rc.left) * t);
    int cy = (rc.top + rc.bottom) / 2;
    const int r = radius; // 원형 노브
    HBRUSH knob = CreateSolidBrush(RGB(255, 255, 255));
    HPEN   kpen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
    HGDIOBJ oldB = SelectObject(hdc, knob);
    HGDIOBJ oldP = SelectObject(hdc, kpen);
    Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);
    SelectObject(hdc, oldP); SelectObject(hdc, oldB);
    DeleteObject(kpen); DeleteObject(knob);
}

static void PaintAll(HWND h, HDC hdc, PanelState* st) {
    // 1) 색상 휠(H,S)
    st->box.DrawColorWheel(hdc, st->wheelCenter.x, st->wheelCenter.y, st->wheelRadius);

    // 2) 밝기 바: 가로 그라데이션으로 교체
    st->box.DrawValueBarH(hdc, st->valueBar, st->hue, st->sat);

    // 3) 두께 바: 라운드 트랙 + 노브
    HBRUSH bg = CreateSolidBrush(RGB(235, 235, 235));
    HGDIOBJ oldB = SelectObject(hdc, bg);
    HPEN frame = CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
    HGDIOBJ oldP = SelectObject(hdc, frame);

    // 트랙 그리기(두께 바)
    const int radius = (st->thickBar.bottom - st->thickBar.top) / 2;
    RoundRect(hdc, st->thickBar.left, st->thickBar.top, st->thickBar.right, st->thickBar.bottom, radius, radius);

    SelectObject(hdc, oldP); DeleteObject(frame);
    SelectObject(hdc, oldB); DeleteObject(bg);

    // 진행 + 노브
    double tThick = double(st->thickness - st->thickMin) / double(st->thickMax - st->thickMin);
    DrawTrackWithKnob(hdc, st->thickBar, tThick, true, RGB(50, 140, 220));

    // 4) 현재색 스와치
    COLORREF cur = st->box.HSVtoRGB(st->hue, st->sat, st->val);
    HBRUSH b = CreateSolidBrush(cur);
    FillRect(hdc, &st->swatch, b);
    DeleteObject(b);

    // 스와치 테두리
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
    HPEN old = (HPEN)SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    Rectangle(hdc, st->swatch.left, st->swatch.top, st->swatch.right, st->swatch.bottom);
    SelectObject(hdc, old); DeleteObject(pen);
}

static void NotifyAndStore(HWND h, PanelState* st) {
    COLORREF c = st->box.HSVtoRGB(st->hue, st->sat, st->val);

    // ColorBox 정적 슬롯에도 반영 (선택 슬롯 기준)
    ColorBox::setColorNum(ColorBox::colorSelect, c);
    ColorBox::setThicknessNum(ColorBox::colorSelect, st->thickness);

    if (st->onChanged) st->onChanged(c, st->thickness);
    InvalidateRect(h, nullptr, FALSE);
}

// ----- public API -----
HWND ColorBoxPanel::Create(HWND parent, int x, int y, int w, int h) {
    static const wchar_t* kClass = L"ColorBoxPanelClass_v3";
    static bool reg = false;
    if (!reg) {
        WNDCLASS wc = {};
        wc.lpfnWndProc = ColorBoxPanel::WndProc;
        wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = kClass;
        RegisterClass(&wc);
        reg = true;
    }
    return CreateWindowEx(0, kClass, L"", WS_CHILD | WS_VISIBLE,
        x, y, w, h, parent, nullptr,
        (HINSTANCE)GetModuleHandle(nullptr), nullptr);
}

void ColorBoxPanel::SetOnChanged(HWND panel, std::function<void(COLORREF, int)> cb) {
    auto st = reinterpret_cast<PanelState*>(GetWindowLongPtr(panel, GWLP_USERDATA));
    if (st) st->onChanged = std::move(cb);
}

void ColorBoxPanel::SetThicknessRange(HWND panel, int minT, int maxT) {
    auto st = reinterpret_cast<PanelState*>(GetWindowLongPtr(panel, GWLP_USERDATA));
    if (!st) return;
    if (minT < 1) minT = 1;
    if (maxT <= minT) maxT = minT + 1;
    st->thickMin = minT; st->thickMax = maxT;
    st->thickness = clampi(st->thickness, st->thickMin, st->thickMax);
    InvalidateRect(panel, nullptr, FALSE);
}

// ----- window proc -----
LRESULT CALLBACK ColorBoxPanel::WndProc(HWND h, UINT msg, WPARAM w, LPARAM l) {
    auto st = reinterpret_cast<PanelState*>(GetWindowLongPtr(h, GWLP_USERDATA));
    switch (msg) {
    case WM_CREATE:
        st = new PanelState();
        SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)st);
        // 초기값: ColorBox 현재 슬롯에서 가져오기
        st->thickness = ColorBox::getThicknessNum(ColorBox::colorSelect);
        {
            COLORREF c = ColorBox::getColorNum(ColorBox::colorSelect);
            st->box.RGBtoHSV(c, st->hue, st->sat, st->val);
        }
        ComputeLayout(h, st);
        return 0;

    case WM_SIZE:
        if (st) { ComputeLayout(h, st); InvalidateRect(h, nullptr, FALSE); }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps; HDC dc = BeginPaint(h, &ps);
        if (st) PaintAll(h, dc, st);
        EndPaint(h, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        if (!st) return 0;
        const int x = GET_X_LPARAM(l), y = GET_Y_LPARAM(l);

        // 휠 히트
        {
            const int dx = x - st->wheelCenter.x, dy = y - st->wheelCenter.y;
            const double dist = std::sqrt(double(dx * dx + dy * dy));
            if (dist <= st->wheelRadius) {
                st->dragWheel = true; SetCapture(h);
                // ↓ 클릭 지점 즉시 계산 (y 부호 뒤집어 좌표계 일치)
                st->sat = std::max(0.0, std::min(1.0, dist / (double)st->wheelRadius));
                double hue = std::atan2(-(double)dy, (double)dx) * 180.0 / 3.141592653589793 + 180.0;
                if (hue < 0) hue += 360.0; if (hue >= 360.0) hue = std::fmod(hue, 360.0);
                st->hue = hue;
                NotifyAndStore(h, st);
                return 0;
            }
        }

        // 밝기 바 히트 (가로)
        if (PtInRectI(st->valueBar, x, y)) {
            st->dragV = true; SetCapture(h);
            double t = (x - st->valueBar.left) / double(st->valueBar.right - st->valueBar.left);
            t = std::max(0.0, std::min(1.0, t));
            st->val = 1.0 - t;           // 왼쪽 밝음, 오른쪽 어두움
            NotifyAndStore(h, st);
            return 0;
        }

        // 두께 바 히트
        if (PtInRectI(st->thickBar, x, y)) {
            st->dragT = true; SetCapture(h);
            double t = (x - st->thickBar.left) / double(st->thickBar.right - st->thickBar.left);
            t = std::max(0.0, std::min(1.0, t));
            const int span = st->thickMax - st->thickMin;
            st->thickness = st->thickMin + (int)std::round(t * span);
            NotifyAndStore(h, st);
            return 0;
        }
        return 0;
    }


    case WM_MOUSEMOVE: {
        if (!st) return 0;
        if (!(st->dragWheel || st->dragV || st->dragT)) return 0;
        const int x = GET_X_LPARAM(l), y = GET_Y_LPARAM(l);

        if (st->dragWheel) {
            const int dx = x - st->wheelCenter.x, dy = y - st->wheelCenter.y;
            const double dist = std::sqrt(double(dx * dx + dy * dy));
            st->sat = std::max(0.0, std::min(1.0, dist / (double)st->wheelRadius));
            // y 부호 반전 적용
            double hue = std::atan2(-(double)dy, (double)dx) * 180.0 / 3.14159265358979323846 + 180.0;
            if (hue < 0) hue += 360.0; if (hue >= 360.0) hue = std::fmod(hue, 360.0);
            st->hue = hue;
            NotifyAndStore(h, st);
        }
        else if (st->dragV) {
            double t = (x - st->valueBar.left) / double(st->valueBar.right - st->valueBar.left);
            t = std::max(0.0, std::min(1.0, t));
            st->val = 1.0 - t; // DrawValueBarH와 방향 통일
            NotifyAndStore(h, st);
        }
        else if (st->dragT) {
            double t = (x - st->thickBar.left) / double(st->thickBar.right - st->thickBar.left);
            t = std::max(0.0, std::min(1.0, t));
            const int span = st->thickMax - st->thickMin;
            st->thickness = st->thickMin + (int)std::round(t * span);
            st->thickness = clampi(st->thickness, st->thickMin, st->thickMax);
            NotifyAndStore(h, st);
        }
        return 0;
    }

    case WM_LBUTTONUP:
        if (st && (st->dragWheel || st->dragV || st->dragT)) {
            st->dragWheel = st->dragV = st->dragT = false;
            ReleaseCapture();
        }
        return 0;

    case WM_DESTROY:
        if (st) { delete st; SetWindowLongPtr(h, GWLP_USERDATA, 0); }
        return 0;
    }
    return DefWindowProc(h, msg, w, l);
}
