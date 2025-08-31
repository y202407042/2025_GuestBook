#pragma once
#include <windows.h>

class ColorManager {
private:
    COLORREF currentColor;  // 현재 선택된 색상

public:
    ColorManager();
    void SetColor(COLORREF color);
    COLORREF GetColor() const;
};
