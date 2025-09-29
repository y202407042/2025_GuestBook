#define NOMINMAX
#include "ColorBox.h"
#include <vector>
#include <algorithm> // std::max, std::min

// ==== 정적 멤버 초기값 ====
int      ColorBox::colorSelect = 0;
COLORREF ColorBox::colorP[kSlots] = { RGB(0,0,0), RGB(255,0,0), RGB(0,255,0) };
int      ColorBox::thicknessP[kSlots] = { 10, 10, 10 };

// ==== 내부 유틸 (C++14 호환 clamp) ====
template <typename T>
static inline T my_clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi ? hi : v);
}
static inline int clampi(int v, int lo, int hi) { return my_clamp(v, lo, hi); }

// ==== 접근자 구현 ====
COLORREF ColorBox::getColorNum(int idx) {
    idx = clampi(idx, 0, kSlots - 1);
    return colorP[idx];
}
void ColorBox::setColorNum(int idx, COLORREF color) {
    idx = clampi(idx, 0, kSlots - 1);
    colorP[idx] = color;
}
int ColorBox::getThicknessNum(int idx) {
    idx = clampi(idx, 0, kSlots - 1);
    return thicknessP[idx];
}
void ColorBox::setThicknessNum(int idx, int thickness) {
    idx = clampi(idx, 0, kSlots - 1);
    thicknessP[idx] = clampi(thickness, 1, 256);
}

COLORREF ColorBox::currentColor() { return getColorNum(colorSelect); }
int      ColorBox::currentThickness() { return getThicknessNum(colorSelect); }

// ==== 드로잉 ====
void ColorBox::DrawColorWheel(HDC hdc, int centerX, int centerY, int radius)
{
    const int diameter = radius * 2;
    std::vector<COLORREF> buffer(diameter * diameter, RGB(234, 232, 224));

    for (int y = -radius; y < radius; ++y) {
        for (int x = -radius; x < radius; ++x) {
            const double dist = std::sqrt(double(x) * x + double(y) * y);
            if (dist <= radius) {
                const double hue = atan2(-(double)y, (double)x) * 180.0 / 3.141592653589793 + 180.0;
                const double sat = dist / radius;
                const COLORREF color = HSVtoRGB(hue, sat, 1.0);
                const int bufX = x + radius;
                const int bufY = y + radius; // top-down DIB로 그대로
                buffer[bufY * diameter + bufX] = color;
            }
        }
    }

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = diameter;
    bmi.bmiHeader.biHeight = -diameter; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    SetDIBitsToDevice(hdc, centerX - radius, centerY - radius, diameter, diameter,
        0, 0, 0, diameter, buffer.data(), &bmi, DIB_RGB_COLORS);
}

void ColorBox::DrawValueBarH(HDC hdc, RECT rect, double hue, double sat)
{
    const int w = rect.right - rect.left;
    const int h = rect.bottom - rect.top;
    if (w <= 0 || h <= 0) return;

    // 한 열(column)씩 찍기: v는 x에 따라 1 -> 0 으로 감소 (왼→오)
    std::vector<COLORREF> col(h);
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = 1;
    bmi.bmiHeader.biHeight = -h; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    for (int x = 0; x < w; ++x) {
        double t = double(x) / double(w - 1); // 0..1
        double v = 1.0 - t;                   // 왼쪽=밝음, 오른쪽=어두움
        COLORREF c = HSVtoRGB(hue, sat, v);
        for (int y = 0; y < h; ++y) col[y] = c;

        SetDIBitsToDevice(
            hdc, rect.left + x, rect.top, 1, h,
            0, 0, 0, h, col.data(), &bmi, DIB_RGB_COLORS);
    }

    // 외곽 테두리
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN old = (HPEN)SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(hdc, old);
    DeleteObject(pen);
}


void ColorBox::DrawSelectedColor(HDC hdc)
{
    const COLORREF selected = currentColor();
    HBRUSH hBrush = CreateSolidBrush(selected);
    RECT rc = { 50, 280, 310, 330 }; // 로컬 RECT 사용
    FillRect(hdc, &rc, hBrush);
    DeleteObject(hBrush);
}

void ColorBox::DrawSlider(HDC hdc, RECT rect, int value, int max)
{
    max = (max <= 0) ? 1 : max;
    value = clampi(value, 0, max);

    HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &rect, bg);
    DeleteObject(bg);

    const int barW = int((rect.right - rect.left) * double(value) / double(max));
    RECT bar = { rect.left, rect.top, rect.left + barW, rect.bottom };
    HBRUSH hb = CreateSolidBrush(RGB(0, 122, 204));
    FillRect(hdc, &bar, hb);
    DeleteObject(hb);

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN old = (HPEN)SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(hdc, old);
    DeleteObject(pen);
}

void ColorBox::DrawThicknessSlider(HDC hdc, RECT rect, RoundRECT rr, int thicknessValue, int maxThickness)
{
    maxThickness = (maxThickness <= 0) ? 1 : maxThickness;
    thicknessValue = clampi(thicknessValue, 0, maxThickness);

    HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
    HBRUSH oldB = (HBRUSH)SelectObject(hdc, bg);
    HPEN   oldP = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));

    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, rr.radiusX, rr.radiusY);

    const int barW = int((rect.right - rect.left) * double(thicknessValue) / double(maxThickness));
    RECT bar = { rect.left, rect.top, rect.left + barW, rect.bottom };
    HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, white);
    RoundRect(hdc, bar.left, bar.top, bar.right, bar.bottom, rr.radiusX, rr.radiusY);

    SelectObject(hdc, oldP);
    SelectObject(hdc, oldB);
    DeleteObject(white);
    DeleteObject(bg);
}

// ==== HSV <-> RGB ====
COLORREF ColorBox::HSVtoRGB(double h, double s, double v)
{
    // C++14 호환: std::clamp 미사용
    if (h < 0.0) h = std::fmod(h, 360.0) + 360.0;
    if (h >= 360.0) h = std::fmod(h, 360.0);
    s = (s < 0.0) ? 0.0 : (s > 1.0 ? 1.0 : s);
    v = (v < 0.0) ? 0.0 : (v > 1.0 ? 1.0 : v);

    int i = int(h / 60.0) % 6;
    double f = (h / 60.0) - std::floor(h / 60.0);
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    double r, g, b;
    switch (i) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default:r = v; g = p; b = q; break;
    }
    return RGB(BYTE(r * 255.0 + 0.5), BYTE(g * 255.0 + 0.5), BYTE(b * 255.0 + 0.5));
}

void ColorBox::RGBtoHSV(COLORREF rgb, double& h, double& s, double& v)
{
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;

    // Windows min/max 매크로 충돌 회피: (std::max)(...), (std::min)(...)
    double maxc = (std::max)(r, (std::max)(g, b));
    double minc = (std::min)(r, (std::min)(g, b));
    double delta = maxc - minc;

    v = maxc;
    if (maxc <= 0.0) { s = 0.0; h = 0.0; return; }

    s = (delta <= 0.0) ? 0.0 : (delta / maxc);
    if (delta <= 0.0) { h = 0.0; return; }

    if (maxc == r) h = 60.0 * std::fmod(((g - b) / delta), 6.0);
    else if (maxc == g) h = 60.0 * (((b - r) / delta) + 2.0);
    else                h = 60.0 * (((r - g) / delta) + 4.0);

    if (h < 0.0) h += 360.0;
}
