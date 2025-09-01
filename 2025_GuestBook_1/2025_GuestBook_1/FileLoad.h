#pragma once
#include <windows.h>
#include <commdlg.h>

class FileLoad {
public:
    // �ҷ����� ���� �Լ�
    bool Run(HWND hWnd);

private:
    bool GetOpenPath(HWND hWnd, wchar_t* path, DWORD size); // ���� ��� ����
    bool LoadBitmapToWindow(HWND hWnd, const wchar_t* path); // ���� �ε� �� ȭ�鿡 �׸�
};
