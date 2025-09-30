#include "ButtonTool.h"

ButtonTool::ButtonTool(HINSTANCE hInst, HWND prnt, int btnId, LPCWSTR btnText)
    : hInstance(hInst), parent(prnt), hWnd(NULL), id(btnId), text(btnText)
{
}

void ButtonTool::create(int x, int y, int width, int height) {
    hWnd = CreateWindowEx(
        0, L"BUTTON", text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height,
        parent, (HMENU)(INT_PTR)id, hInstance, NULL
    );
}

void ButtonTool::move(int x, int y, int width, int height) {
    if (hWnd) {
        MoveWindow(hWnd, x, y, width, height, TRUE);
    }
};