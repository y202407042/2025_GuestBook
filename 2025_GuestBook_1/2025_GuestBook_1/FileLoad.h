#pragma once
#include <windows.h>
#include <vector>
#include "DrawData.h"

class FileLoad {
public:
    bool LoadFromFile(HWND hWnd, std::vector<DrawPoint>& outPoints);
};
