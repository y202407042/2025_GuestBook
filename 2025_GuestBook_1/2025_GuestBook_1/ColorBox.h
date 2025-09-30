#pragma once
#define NOMINMAX            // Windows min/max 매크로 비활성화 (중요)
#include <Windows.h>
#include <cmath>

// 둥근 사각형 정의 구조체
struct RoundRECT {
    int left;
    int top;
    int right;
    int bottom;
    int radiusX;
    int radiusY;
};

class ColorBox
{
public:
    enum { kSlots = 3 };                 // C++14 호환 상수

    static int      colorSelect;         // 현재 선택된 색상 인덱스 [0..kSlots-1]
    static COLORREF colorP[kSlots];      // 색상 팔레트 저장
    static int      thicknessP[kSlots];  // 펜 두께 저장

    // 접근자(인덱스 안전 클램프)
    static COLORREF getColorNum(int idx);
    static void     setColorNum(int idx, COLORREF color);
    static int      getThicknessNum(int idx);
    static void     setThicknessNum(int idx, int thickness);

    // 현재 슬롯 헬퍼
    static COLORREF currentColor();
    static int      currentThickness();

    // UI 그리기 함수들
    void DrawColorWheel(HDC hdc, int centerX, int centerY, int radius);
    void DrawValueBarH(HDC hdc, RECT rect, double hue, double sat);
    void DrawSelectedColor(HDC hdc);
    void DrawSlider(HDC hdc, RECT rect, int value, int max);
    void DrawThicknessSlider(HDC hdc, RECT rect, RoundRECT roundrect, int thicknessValue, int maxThickness);

    // 색상 변환
    COLORREF HSVtoRGB(double h, double s, double v);
    void     RGBtoHSV(COLORREF rgb, double& h, double& s, double& v);
};
