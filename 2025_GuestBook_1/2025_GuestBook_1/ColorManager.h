#pragma once
#include <Windows.h>

class ColorManager {
    COLORREF current;
public:
    ColorManager();
    void SetColor(COLORREF c);
    COLORREF GetColor() const;
};
