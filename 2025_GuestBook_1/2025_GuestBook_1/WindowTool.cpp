//#include <commdlg.h>
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
        nullptr, nullptr, hInstance, this
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

    switch (msg) {
    case WM_CREATE:
        self->createChildWindows(hwnd);
        return 0;

    case WM_SIZE: {
        RECT r; GetClientRect(hwnd, &r);
        self->resizeChildWindows(r);
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
            MessageBox(hwnd, L"��", L"INFO", MB_OK);
            return 0;
        case IDC_BTN_SPRAY:
            MessageBox(hwnd, L"��������", L"INFO", MB_OK);
            return 0;
        case IDC_BTN_COLOR:
            MessageBox(hwnd, L"����", L"INFO", MB_OK);
            return 0;
        case IDC_BTN_BRUSH:
            MessageBox(hwnd, L"�귯��", L"INFO", MB_OK);
            return 0;
        case IDC_BTN_SAVE:
            MessageBox(hwnd, L"����", L"INFO", MB_OK);
            return 0;
        case IDC_BTN_LOAD:
            MessageBox(hwnd, L"�ҷ�����", L"INFO", MB_OK);
            return 0;
        case IDC_BTN_REPLAY:
            MessageBox(hwnd, L"���÷���", L"INFO", MB_OK);
            return 0;
        default:
            break;
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/// ĵ���� ���� ���ν��� : ����� �׻� �����ϰ� ĥ��
LRESULT CALLBACK WindowTool::canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    /*HWND parent = GetParent(hwnd);
    auto* self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(parent, GWLP_USERDATA));*/

    switch (msg) {
    case WM_ERASEBKGND:
        // ��� ����� ���� ó�� �� 1 ��ȯ(�ý��� �߰� ����� ����)
        return 1;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

