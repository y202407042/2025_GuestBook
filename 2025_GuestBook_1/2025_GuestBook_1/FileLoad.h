#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "DrawPoints.h" 

class FileLoad {
public:
    FileLoad(HWND hWnd);
    ~FileLoad();

    /// 파일에서 PenData를 읽어 벡터로 반환
    /// 로드 성공 시 true 반환
    bool loadFromFile(std::vector<PenData>& penData);

private:
    HWND hWnd;
};
