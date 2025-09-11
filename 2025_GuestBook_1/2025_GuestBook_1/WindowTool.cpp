//#include <commdlg.h>
#include <windowsx.h>
#include "WindowTool.h"
#include "Resource.h"

#define TOP_PANEL_HEIGHT 50
#define LEFT_PANEL_WIDTH 100

WindowTool::WindowTool(HINSTANCE hInst) : hInstance(hInst) {}

/// ���� ������ ����
bool WindowTool::createMainWindow(int width, int height) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowTool::windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GuestBookWindowClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    /// ĵ���� ���� Ŭ���� ���
    WNDCLASS wcc = {};
    wcc.lpfnWndProc = WindowTool::canvasProc;
    wcc.hInstance = hInstance;
    wcc.lpszClassName = L"CanvasAreaClass";
    wcc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wcc.hbrBackground = nullptr; // WM_PAINT���� ���� ĥ�� (������� ����� ȿ��)
    RegisterClass(&wcc);

   /// �θ� WS_CLIPCHILDREN �߰�(�ڽ� �� ����ĥ ���� ����)
    DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;

    mainWindow = CreateWindowEx(
        0, wc.lpszClassName, L"GuestBook Drawing Tool",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance,
        this
    );
    return mainWindow != nullptr;
}

/// ������ ǥ��
void WindowTool::showWindow(int nCmdShow) {
    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);
}

/// �ڽ� ������ ����
void WindowTool::createChildWindows(HWND parent) {
    /// ��� (���� ��ħ ���� WS_CLIPSIBLINGS)
    headerPanel = CreateWindowEx(
        0, L"STATIC", L"���Ѵ�",
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_CENTER | SS_CENTERIMAGE,
        0, 0, 0, 0, parent, nullptr, hInstance, nullptr
    );
    /// ����
    toolbarPanel = CreateWindowEx(
        0, L"STATIC", nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, 0, 0, parent, nullptr, hInstance, nullptr
    );

    /// ��ư
    struct BtnDef { int id; LPCWSTR text; };
    BtnDef defs[] = {
        { IDC_BTN_PEN,     L"��" },
        { IDC_BTN_SPRAY,   L"��������" },
        { IDC_BTN_COLOR,   L"����" },
        { IDC_BTN_REPLAY,  L"���÷���" },
        { IDC_BTN_SAVE,    L"����" },
        { IDC_BTN_LOAD,    L"�ҷ�����" },
    };
    buttons.reserve(_countof(defs));
    for (auto& d : defs) {
        auto* btn = new ButtonTool(hInstance, toolbarPanel, d.id, d.text);
        btn->create(0, 0, 0, 0);
        buttons.push_back(btn);
    }

    /// ĵ����: ���� Ŭ���� ��� + WS_CLIPSIBLINGS
    canvasArea = CreateWindowEx(
        0, L"CanvasAreaClass", nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, 0, 0, parent, nullptr, hInstance, nullptr
    );
}

/// �������� �� ���̾ƿ� ����
void WindowTool::resizeChildWindows(RECT rc) {
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    MoveWindow(headerPanel, 0, 0, w, TOP_HEADER_HEIGHT, TRUE);
    MoveWindow(toolbarPanel, 0, TOP_HEADER_HEIGHT, w, TOOLBAR_HEIGHT, TRUE);
    MoveWindow(canvasArea, 0, TOP_HEADER_HEIGHT + TOOLBAR_HEIGHT,
        w, h - (TOP_HEADER_HEIGHT + TOOLBAR_HEIGHT), TRUE);

    layoutToolbarButtons(w, TOOLBAR_HEIGHT);

    /// �������� �� ĵ���� ��ü ��ȿȭ. �ܻ� ����
    InvalidateRect(canvasArea, nullptr, TRUE);
}

/// ���� ���� ��ư�� ���� �յ� ��ġ
void WindowTool::layoutToolbarButtons(int width, int height) {
    const int count = static_cast<int>(buttons.size());
    const int margin = BUTTON_MARGIN;
    const int availW = width - (margin * (count + 1));
    const int btnW = max(80, availW / max(1, count));
    const int btnH = height - margin * 2;

    int x = margin;
    for (auto* b : buttons) {
        b->move(x, margin, btnW, btnH);
        x += btnW + margin;
    }
}

/// ���� ������ ���ν���
LRESULT CALLBACK WindowTool::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowTool* self = nullptr;
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = static_cast<WindowTool*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    }
    else {
        self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!self) {
        // ���� �ʱ⿡ ������ ��� �޽��� ���
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
    case WM_CREATE:
        self->createChildWindows(hwnd);
        return 0;

    case WM_SIZE: {
        if (self && self->canvasArea) {
            RECT rc; GetClientRect(hwnd, &rc);
            int toolbarH = 60; // ���� ���̿� �°�
            MoveWindow(self->canvasArea,
                0, toolbarH,
                rc.right, rc.bottom - toolbarH,
                TRUE);
        }
        return 0;
    }

                // STATIC(���) ����� �ʷ����� ĥ�ϰ� �����Ѵ롱�� ������ �ؽ�Ʈ
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        HWND ctrl = (HWND)lParam;
        if (ctrl == self->headerPanel) {
            SetBkColor(hdc, HEADER_BG);
            SetTextColor(hdc, RGB(0, 0, 0));
            static HBRUSH greenBrush = CreateSolidBrush(HEADER_BG);
            return (LRESULT)greenBrush;
        }
        break;
    }

    // ��ư Ŭ�� ó��
    case WM_COMMAND: {
        const int id = LOWORD(wParam);
        switch (id) {
        case IDC_BTN_PEN:
            self->penView.switchPen(PEN_TYPE_NORMAL);
            return 0;
        case IDC_BTN_SPRAY:
            self->penView.switchPen(PEN_TYPE_SPRAY);
            return 0;
        case IDC_BTN_COLOR:
        {
            COLORREF c = self->colorPicker.Show(hwnd);
            self->colorMgr.SetColor(c);
            InvalidateRect(self->canvasArea, nullptr, FALSE);
            return 0;
        }
        case IDC_BTN_BRUSH:
            self->penView.switchPen(PEN_TYPE_BRUSH);
            return 0;
        case IDC_BTN_SAVE:
            self->fileSave.Run(self->canvasArea);
            return 0;
        case IDC_BTN_LOAD:
            self->fileLoad.Run(self->canvasArea);
            InvalidateRect(self->canvasArea, nullptr, TRUE);
            return 0;
        case IDC_BTN_REPLAY:
            self->penReplay.replayStart(self->drawPoints.getPoints());
            return 0;
        default:
            break;
        }
        break;
    }

    case WM_DESTROY:
        if (self) {
            self->canvas.destroy();
        }
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/// ĵ���� ���� ���ν��� : ����� �׻� �����ϰ� ĥ��
LRESULT CALLBACK WindowTool::canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND parent = GetParent(hwnd);
    auto* self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(parent, GWLP_USERDATA));
    if (!self) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg)
    {
    case WM_ERASEBKGND:
        // ������ ����: �츮�� ���� �׸� �Ŷ� ��� ������ ����
        return 1;

    case WM_CREATE:
        // ���� ���� �� ���� �غ�
        self->canvas.resizeTo(hwnd);
        return 0;

    case WM_SIZE:
        // ������ ���� �� ���� �����
        self->canvas.resizeTo(hwnd);
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;

    case WM_LBUTTONDOWN: {
        self->isDrawing = true;
        self->lastPt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        self->drawPoints.saveToPoint(self->lastPt.x, self->lastPt.y);
        SetCapture(hwnd);
        return 0;
    }

    case WM_MOUSEMOVE: {
        if (!self->isDrawing) return 0;

        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        /// ���� DC�� �׸���
        HPEN drawPen = CreatePen(
            PS_SOLID,
            (self->penView.getCurrentPenType() == PEN_TYPE_BRUSH ? 3 : 1),
            self->colorMgr.GetColor()
        );
        HGDIOBJ old = SelectObject(self->canvas.memDC, drawPen);
        MoveToEx(self->canvas.memDC, self->lastPt.x, self->lastPt.y, nullptr);
        LineTo(self->canvas.memDC, x, y);
        SelectObject(self->canvas.memDC, old);
        DeleteObject(drawPen);

        self->drawPoints.saveToPoint(x, y);
        self->lastPt = { x,y };

        /// ����� ������ �����ϰ� ������ InvalidateRect�� �һ簢 ����
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    }

    case WM_LBUTTONUP:
        self->isDrawing = false;
        ReleaseCapture();
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // �ʿ� �� ��ü�� �ٽ� �׷��� �� ��(���÷��� ����):
        // self->canvas.clear(); self->drawPoints.drawToPoint(self->canvas.memDC);

        //������ũ�� ���� ȭ������ ����
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

