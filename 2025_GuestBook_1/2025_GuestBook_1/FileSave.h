#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "DrawPoints.h"   

class FileSave {
public:
    FileSave(HWND hWnd);
    ~FileSave();

    /// ���Ϳ� ��� PenData�� ���Ϸ� ����
    /// ������ �����ϸ� true ��ȯ
    bool saveToFile(const std::vector<PenData>& penData);

private:
    HWND hWnd;  // ���̾�α� �θ� ������ �ڵ�
};
