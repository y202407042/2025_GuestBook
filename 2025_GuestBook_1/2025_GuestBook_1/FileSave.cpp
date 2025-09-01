#include "FileSave.h"

// ���� ��ư ������ �� ����
bool FileSave::Run(HWND hWnd) {
    wchar_t filePath[MAX_PATH] = L"";

    // ���� ��� ����
    if (!GetSavePath(hWnd, filePath, MAX_PATH))
        return false;

    // ȭ�� ĸó �� ����
    return SaveBitmap(hWnd, filePath);
}

// ���� ���� ��ȭ���� ����
bool FileSave::GetSavePath(HWND hWnd, wchar_t* path, DWORD size) {
    OPENFILENAMEW ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.nMaxFile = size;
    ofn.lpstrFilter = L"BMP ����\0*.bmp\0��� ����\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"bmp";

    path[0] = '\0';

    return GetSaveFileNameW(&ofn);
}

// ���� ������ ȭ���� ĸó�� BMP ���Ϸ� ����
bool FileSave::SaveBitmap(HWND hWnd, const wchar_t* path) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    HDC hdc = GetDC(hWnd);
    HDC memDC = CreateCompatibleDC(hdc);

    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    BitBlt(memDC, 0, 0, rc.right, rc.bottom, hdc, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
    ReleaseDC(hWnd, hdc);

    // ��Ʈ�� ������ ��������
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 24;   // 24��Ʈ BMP
    bi.biCompression = BI_RGB;
    bi.biSizeImage = ((bmp.bmWidth * 3 + 3) & ~3) * bmp.bmHeight;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = bi.biSizeImage;
    BYTE* lpBitmap = new BYTE[dwBmpSize];

    HDC hdcScreen = GetDC(NULL);
    GetDIBits(hdcScreen, hBitmap, 0, (UINT)bmp.bmHeight, lpBitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdcScreen);

    HANDLE hFile = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        delete[] lpBitmap;
        DeleteObject(hBitmap);
        return false;
    }

    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42; // "BM"

    DWORD dwWritten;
    WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, lpBitmap, dwBmpSize, &dwWritten, NULL);

    CloseHandle(hFile);
    delete[] lpBitmap;
    DeleteObject(hBitmap);

    return true;
}
