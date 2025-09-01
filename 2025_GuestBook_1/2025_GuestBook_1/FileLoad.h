#pragma once
#include <windows.h>
#include <commdlg.h>

class FileLoad {
public:
    // 불러오기 실행 함수
    bool Run(HWND hWnd);

private:
    bool GetOpenPath(HWND hWnd, wchar_t* path, DWORD size); // 파일 경로 선택
    bool LoadBitmapToWindow(HWND hWnd, const wchar_t* path); // 파일 로드 후 화면에 그림
};
