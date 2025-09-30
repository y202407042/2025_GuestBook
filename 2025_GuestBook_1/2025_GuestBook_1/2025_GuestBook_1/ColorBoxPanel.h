#pragma once
#define NOMINMAX
#include <Windows.h>
#include <functional>
#include "ColorBox.h"

// ColorBox 기반 색/두께 패널 (자식 윈도우)
// onChanged(color, thickness) 콜백으로 즉시 통지
class ColorBoxPanel {
public:
    // 패널 생성: 부모, 위치/크기
    static HWND Create(HWND parent, int x, int y, int w, int h);

    // 색/두께 변경 콜백 등록
    static void SetOnChanged(HWND panelHwnd, std::function<void(COLORREF, int)> cb);

    // 두께 범위 설정(기본 1~30)
    static void SetThicknessRange(HWND panelHwnd, int minT, int maxT);

private:
    // WndProc는 cpp에서 정의 (여기서는 선언만)
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};
