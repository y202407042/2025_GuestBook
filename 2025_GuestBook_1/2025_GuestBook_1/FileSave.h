#pragma once
#include <windows.h>
#include <commdlg.h>

class FileSave {
public:
    // 저장 실행 함수
    bool Run(HWND hWnd);

private:
    bool GetSavePath(HWND hWnd, wchar_t* path, DWORD size); // 저장 경로 선택
    bool SaveBitmap(HWND hWnd, const wchar_t* path);        // 비트맵 저장
};
