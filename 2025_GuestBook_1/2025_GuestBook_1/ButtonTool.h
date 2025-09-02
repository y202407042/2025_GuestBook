#pragma once
#include <windows.h>

/// ButtonTool: ���� ��ư ���� Ŭ����
class ButtonTool {
public:
    ButtonTool(HINSTANCE hInstance, HWND parent, int id, LPCWSTR text);

    void create(int x, int y, int width, int height);
    void move(int x, int y, int width, int height);
    HWND getHandle() const { return hWnd; }
    /*void setIcon(HICON icon);*/  /// ������ �� �� Ȱ��ȭ
private:
    HINSTANCE hInstance;
    HWND parent;
    HWND hWnd;
    int id;
    LPCWSTR text;
};