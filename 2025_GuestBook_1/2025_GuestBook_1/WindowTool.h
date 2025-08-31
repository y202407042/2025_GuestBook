#pragma once
#include <windows.h>
#include <vector>
#include "ButtonTool.h"

/// WindowTool Ŭ����: ���� ������� ���� ���� ����
class WindowTool {
public:
    WindowTool(HINSTANCE hInstance);
    bool createMainWindow(int width, int height);
    void showWindow(int nCmdShow);
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    static constexpr int TOP_HEADER_HEIGHT = 50;
    static constexpr int TOOLBAR_HEIGHT = 48;
    static constexpr int BUTTON_MARGIN = 8;
    static constexpr COLORREF HEADER_BG = RGB(144, 238, 144);

    HINSTANCE hInstance;
    HWND mainWindow{};
    HWND headerPanel{};
    HWND toolbarPanel{};
    HWND canvasArea{};

    std::vector<ButtonTool*> buttons;

    void createChildWindows(HWND parent);
    void resizeChildWindows(RECT clientRect);
    void layoutToolbarButtons(int width, int height);

    /// ĵ���� ���� ������ ���ν���
    static LRESULT CALLBACK canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};