#include "ButtonTool.h"

ButtonTool::ButtonTool(HINSTANCE hInst, HWND prnt, int btnId, LPCWSTR btnText)
    : hInstance(hInst), parent(prnt), hWnd(nullptr), id(btnId), text(btnText) {
}

void ButtonTool::create(int x, int y, int width, int height) {
    hWnd = CreateWindowEx(
        0, L"BUTTON", text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height,
        parent, (HMENU)id, hInstance, nullptr
    );
}

void ButtonTool::move(int x, int y, int width, int height) {
    if (hWnd) {
        MoveWindow(hWnd, x, y, width, height, TRUE);
    }
}
/// 아이콘 들어갈 때 코드 적용
//void ButtonTool::setIcon(HICON icon) {
//    if (hWnd && icon) {
//        SendMessage(hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)icon);
//    }
//}