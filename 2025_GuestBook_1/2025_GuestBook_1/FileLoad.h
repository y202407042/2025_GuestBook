#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "DrawPoints.h" 

class FileLoad {
public:
    FileLoad(HWND hWnd);
    ~FileLoad();

    /// ���Ͽ��� PenData�� �о� ���ͷ� ��ȯ
    /// �ε� ���� �� true ��ȯ
    bool loadFromFile(std::vector<PenData>& penData);

private:
    HWND hWnd;
};
