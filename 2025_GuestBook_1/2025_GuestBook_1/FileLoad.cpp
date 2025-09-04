#include "FileLoad.h"

// 불러오기 버튼 눌렀을 때 실행
bool FileLoad::Run(HWND hWnd) {
    wchar_t filePath[MAX_PATH] = L"";

    // 파일 경로 선택
    if (!GetOpenPath(hWnd, filePath, MAX_PATH))
        return false;

    // 화면에 그림
    return LoadBitmapToWindow(hWnd, filePath);
}

// 파일 열기 대화상자 열기
bool FileLoad::GetOpenPath(HWND hWnd, wchar_t* path, DWORD size) {
    OPENFILENAMEW ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.nMaxFile = size;
    ofn.lpstrFilter = L"BMP 파일\0*.bmp\0모든 파일\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_FILEMUSTEXIST;

    path[0] = '\0';

    return GetOpenFileNameW(&ofn);
}

// 선택한 비트맵을 윈도우에 그리기
bool FileLoad::LoadBitmapToWindow(HWND hWnd, const wchar_t* path) {
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitmap) return false;

    HDC hdc = GetDC(hWnd);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
    ReleaseDC(hWnd, hdc);
    DeleteObject(hBitmap);

    return true;
}
