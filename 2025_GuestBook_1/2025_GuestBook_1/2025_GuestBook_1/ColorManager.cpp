#include "ColorManager.h"

ColorManager::ColorManager()
    : current(RGB(0, 0, 0)) // 기본 색상은 검정
{
}

void ColorManager::SetColor(COLORREF c)
{
    current = c;
}

COLORREF ColorManager::GetColor() const
{
    return current;
}
