#pragma once
#include <windows.h>
#include <vector>
#include "ButtonTool.h"
#include "PenView.h"
#include "ColorManager.h"
#include "ColorPicker.h"
#include "FileSave.h"
#include "FileLoad.h"
#include "PenReplay.h"
#include "DrawPoints.h"
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

    PenView penView;
    ColorManager colorMgr;
    ColorPicker colorPicker;
    PenReplay penReplay;
    FileSave fileSave;
    FileLoad fileLoad;
    DrawPoints drawPoints;
    CanvasBuffer canvas;

    std::vector<PenData> pointsMirror;
};