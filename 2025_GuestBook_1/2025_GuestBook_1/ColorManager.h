#pragma once
#include <windows.h>

class ColorManager {
private:
    COLORREF currentColor;  

public:
    ColorManager();
    void SetColor(COLORREF color);
    COLORREF GetColor() const;
};
