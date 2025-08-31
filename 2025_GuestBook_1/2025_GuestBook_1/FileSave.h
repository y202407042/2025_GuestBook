#pragma once
#include <windows.h>
#include <vector>
#include "DrawData.h"

class FileSave {
public:
    bool SaveToFile(HWND hWnd, const std::vector<DrawPoint>& points);
};