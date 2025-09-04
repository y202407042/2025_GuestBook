#pragma once
#include <windows.h>
#include <commdlg.h>

class FileSave {
public:
    // ���� ���� �Լ�
    bool Run(HWND hWnd);

private:
    bool GetSavePath(HWND hWnd, wchar_t* path, DWORD size); // ���� ��� ����
    bool SaveBitmap(HWND hWnd, const wchar_t* path);        // ��Ʈ�� ����
};
