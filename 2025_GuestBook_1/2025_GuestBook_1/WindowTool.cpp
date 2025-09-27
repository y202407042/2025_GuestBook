#include "WindowTool.h"
#include "Resource.h"
#include <windowsx.h>

#define TOP_PANEL_HEIGHT 50
#define TOOLBAR_HEIGHT   60
#define BUTTON_MARGIN    8
#define HEADER_BG RGB(180,220,180)

WindowTool::WindowTool(HINSTANCE hInst) : hInstance(hInst),
mainWindow(NULL), headerPanel(NULL), toolbarPanel(NULL), canvasArea(NULL),
isDrawing(false)
{
    lastPt.x = lastPt.y = 0;
}

WindowTool::~WindowTool() {
    for (ButtonTool* b : buttons) {
        delete b;
    }
    buttons.clear();
    canvas.destroy();
}

bool WindowTool::createMainWindow(int width, int height) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowTool::windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GuestBookMain";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) return false;

    WNDCLASS wcc = {};
    wcc.lpfnWndProc = WindowTool::canvasProc;
    wcc.hInstance = hInstance;
    wcc.lpszClassName = L"GuestBookCanvas";
    wcc.hCursor = LoadCursor(NULL, IDC_CROSS);
    if (!RegisterClass(&wcc)) return false;

    mainWindow = CreateWindowEx(
        0, wc.lpszClassName, L"GuestBook Drawing Tool",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, this
    );
    return mainWindow != NULL;
}

void WindowTool::showWindow(int nCmdShow) {
    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);
}

void WindowTool::createChildWindows(HWND parent) {
    headerPanel = CreateWindowEx(
        0, L"STATIC", L"유한대",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
        0, 0, 0, 0, parent, NULL, hInstance, NULL
    );

    toolbarPanel = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0, parent, NULL, hInstance, NULL
    );

    struct BtnDef { int id; LPCWSTR text; };
    BtnDef defs[] = {
        { IDC_BTN_PEN,    L"펜" },
        { IDC_BTN_SPRAY,  L"스프레이" },
        { IDC_BTN_COLOR,  L"색상" },
        { IDC_BTN_REPLAY, L"리플레이" },
        { IDC_BTN_SAVE,   L"저장" },
        { IDC_BTN_LOAD,   L"불러오기" },
    };

    for (int i = 0; i < (int)_countof(defs); i++) {
        ButtonTool* b = new ButtonTool(hInstance, toolbarPanel, defs[i].id, defs[i].text);
        b->create(0, 0, 0, 0);
        buttons.push_back(b);
    }

    canvasArea = CreateWindowEx(
        0, L"GuestBookCanvas", NULL,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0, parent, NULL, hInstance, NULL
    );
}

void WindowTool::resizeChildWindows(RECT rc) {
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    MoveWindow(headerPanel, 0, 0, w, TOP_PANEL_HEIGHT, TRUE);
    MoveWindow(toolbarPanel, 0, TOP_PANEL_HEIGHT, w, TOOLBAR_HEIGHT, TRUE);
    MoveWindow(canvasArea, 0, TOP_PANEL_HEIGHT + TOOLBAR_HEIGHT,
        w, h - (TOP_PANEL_HEIGHT + TOOLBAR_HEIGHT), TRUE);

    layoutToolbarButtons(w, TOOLBAR_HEIGHT);

    InvalidateRect(canvasArea, NULL, TRUE);
}

void WindowTool::layoutToolbarButtons(int width, int height) {
    int count = (int)buttons.size();
    if (count <= 0) return;

    int margin = BUTTON_MARGIN;
    int availW = width - (margin * (count + 1));
    int btnW = (availW > 0) ? max(80, availW / count) : 80;
    int btnH = max(24, height - margin * 2);

    int x = margin;
    for (int i = 0; i < count; i++) {
        ButtonTool* b = buttons[i];
        if (b) b->move(x, margin, btnW, btnH);
        x += btnW + margin;
    }
}

LRESULT CALLBACK WindowTool::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowTool* self = NULL;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = static_cast<WindowTool*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    }
    else {
        self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self == NULL) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg) {
    case WM_CREATE:
        self->createChildWindows(hwnd);
        return 0;

    case WM_SIZE: {
        RECT rc;
        GetClientRect(hwnd, &rc);
        self->resizeChildWindows(rc);
        return 0;
    }

    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        HWND ctrl = (HWND)lParam;
        if (ctrl == self->headerPanel) {
            static HBRUSH brush = NULL;
            if (brush == NULL) brush = CreateSolidBrush(HEADER_BG);
            SetBkColor(hdc, HEADER_BG);
            SetTextColor(hdc, RGB(0, 0, 0));
            return (LRESULT)brush;
        }
        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        switch (id) {
        /*case IDC_BTN_PEN:    self->penView.switchPen(PEN_TYPE_NORMAL); return 0;
        case IDC_BTN_SPRAY:  self->penView.switchPen(PEN_TYPE_SPRAY);  return 0;
        case IDC_BTN_BRUSH:  self->penView.switchPen(PEN_TYPE_BRUSH);  return 0;
        case IDC_BTN_COLOR: {
            COLORREF c = self->colorPicker.Show(hwnd);
            self->colorMgr.SetColor(c);
            InvalidateRect(self->canvasArea, NULL, FALSE);
            return 0;
        }*/
        /*case IDC_BTN_SAVE:   self->fileSave.Run(self->canvasArea); return 0;
        case IDC_BTN_LOAD:   self->fileLoad.Run(self->canvasArea);
            InvalidateRect(self->canvasArea, NULL, TRUE);
            return 0;
        case IDC_BTN_REPLAY: self->penReplay.replayStart(self->drawPoints.getPoints()); return 0;
        default: break;
        }
        break;*/
    }

    case WM_DESTROY:
        self->canvas.destroy();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//LRESULT CALLBACK WindowTool::canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//    HWND parent = GetParent(hwnd);
//    WindowTool* self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(parent, GWLP_USERDATA));
//    if (self == NULL) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg) {
    case WM_ERASEBKGND:
        return 1;

    case WM_CREATE:
        self->canvas.resizeTo(hwnd);
        return 0;

    case WM_SIZE:
        self->canvas.resizeTo(hwnd);
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_LBUTTONDOWN:
        self->isDrawing = true;
        self->lastPt.x = GET_X_LPARAM(lParam);
        self->lastPt.y = GET_Y_LPARAM(lParam);
        /*self->drawPoints.saveToPoint(self->lastPt.x, self->lastPt.y);*/
        SetCapture(hwnd);
        return 0;

    case WM_MOUSEMOVE:
        if (!self->isDrawing) return 0;
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            /*int penWidth = (self->penView.getCurrentPenType() == PEN_TYPE_BRUSH) ? 3 : 1;
            HPEN pen = CreatePen(PS_SOLID, penWidth, self->colorMgr.GetColor());
            HGDIOBJ old = SelectObject(self->canvas.memDC, pen);
            MoveToEx(self->canvas.memDC, self->lastPt.x, self->lastPt.y, NULL);
            LineTo(self->canvas.memDC, x, y);
            SelectObject(self->canvas.memDC, old);
            DeleteObject(pen);*/

            /*self->drawPoints.saveToPoint(x, y);*/
            self->lastPt.x = x;
            self->lastPt.y = y;

            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;

    case WM_LBUTTONUP:
        self->isDrawing = false;
        ReleaseCapture();
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        BitBlt(hdc, 0, 0, self->canvas.w, self->canvas.h,
            self->canvas.memDC, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        self->canvas.destroy();
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}