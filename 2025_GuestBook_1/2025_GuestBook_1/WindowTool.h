#pragma once
#include <windows.h>
#include <vector>
#include "ButtonTool.h"
#include "CanvasBuffer.h"

class WindowTool {
public:
    explicit WindowTool(HINSTANCE hInst);
    ~WindowTool();

    bool createMainWindow(int width, int height);
    void showWindow(int nCmdShow);

    void createChildWindows(HWND parent);
    void resizeChildWindows(RECT rc);
    void layoutToolbarButtons(int width, int height);

    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK canvasProc(HWND, UINT, WPARAM, LPARAM);

private:
    HINSTANCE hInstance;
    HWND mainWindow;

    HWND headerPanel;
    HWND toolbarPanel;
    HWND canvasArea;

    std::vector<ButtonTool*> buttons;

    bool isDrawing;
    POINT lastPt;

   /* PenView penView;
    ColorManager colorMgr;
    ColorPicker colorPicker;
    FileSave fileSave;
    FileLoad fileLoad;
    PenReplay penReplay;
    DrawPoints drawPoints;*/
    CanvasBuffer canvas;
};