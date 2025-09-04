#include "ColorManager.h"

ColorManager::ColorManager() : currentColor(RGB(0, 0, 0)) {}

void ColorManager::SetColor(COLORREF color) {
    currentColor = color;
}

COLORREF ColorManager::GetColor() const {
    return currentColor;
}
