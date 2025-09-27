#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "DrawPoints.h"   

class FileSave {
public:
    FileSave(HWND hWnd);
    ~FileSave();

    /// 벡터에 담긴 PenData를 파일로 저장
    /// 저장이 성공하면 true 반환
    bool saveToFile(const std::vector<PenData>& penData);

private:
    HWND hWnd;  // 다이얼로그 부모 윈도우 핸들
};
