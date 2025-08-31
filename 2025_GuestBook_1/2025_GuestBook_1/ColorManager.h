#pragma once
#include <windows.h>

class ColorManager {
private:
    COLORREF currentColor;  // ���� ���õ� ����

public:
    ColorManager();
    void SetColor(COLORREF color);
    COLORREF GetColor() const;
};
