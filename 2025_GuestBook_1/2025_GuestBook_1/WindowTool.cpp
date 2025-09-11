//#include <commdlg.h>
#include <windowsx.h>
#include "WindowTool.h"
#include "Resource.h"

#define TOP_PANEL_HEIGHT 50
#define LEFT_PANEL_WIDTH 100

WindowTool::WindowTool(HINSTANCE hInst) : hInstance(hInst) {}

/// 메인 윈도우 생성
bool WindowTool::createMainWindow(int width, int height) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowTool::windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GuestBookWindowClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    /// 캔버스 전용 클래스 등록
    WNDCLASS wcc = {};
    wcc.lpfnWndProc = WindowTool::canvasProc;
    wcc.hInstance = hInstance;
    wcc.lpszClassName = L"CanvasAreaClass";
    wcc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wcc.hbrBackground = nullptr; // WM_PAINT에서 직접 칠함 (더블버퍼 비슷한 효과)
    RegisterClass(&wcc);

   /// 부모에 WS_CLIPCHILDREN 추가(자식 위 덮어칠 위험 제거)
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

/// 윈도우 표시
void WindowTool::showWindow(int nCmdShow) {
    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);
}

/// 자식 윈도우 생성
void WindowTool::createChildWindows(HWND parent) {
    /// 헤더 (형제 겹침 방지 WS_CLIPSIBLINGS)
    headerPanel = CreateWindowEx(
        0, L"STATIC", L"유한대",
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_CENTER | SS_CENTERIMAGE,
        0, 0, 0, 0, parent, nullptr, hInstance, nullptr
    );
    /// 툴바
    toolbarPanel = CreateWindowEx(
        0, L"STATIC", nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, 0, 0, parent, nullptr, hInstance, nullptr
    );

    /// 버튼
    struct BtnDef { int id; LPCWSTR text; };
    BtnDef defs[] = {
        { IDC_BTN_PEN,     L"펜" },
        { IDC_BTN_SPRAY,   L"스프레이" },
        { IDC_BTN_COLOR,   L"색상" },
        { IDC_BTN_REPLAY,  L"리플레이" },
        { IDC_BTN_SAVE,    L"저장" },
        { IDC_BTN_LOAD,    L"불러오기" },
    };
    buttons.reserve(_countof(defs));
    for (auto& d : defs) {
        auto* btn = new ButtonTool(hInstance, toolbarPanel, d.id, d.text);
        btn->create(0, 0, 0, 0);
        buttons.push_back(btn);
    }

    /// 캔버스: 전용 클래스 사용 + WS_CLIPSIBLINGS
    canvasArea = CreateWindowEx(
        0, L"CanvasAreaClass", nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, 0, 0, parent, nullptr, hInstance, nullptr
    );
}

/// 리사이즈 시 레이아웃 조정
void WindowTool::resizeChildWindows(RECT rc) {
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    MoveWindow(headerPanel, 0, 0, w, TOP_HEADER_HEIGHT, TRUE);
    MoveWindow(toolbarPanel, 0, TOP_HEADER_HEIGHT, w, TOOLBAR_HEIGHT, TRUE);
    MoveWindow(canvasArea, 0, TOP_HEADER_HEIGHT + TOOLBAR_HEIGHT,
        w, h - (TOP_HEADER_HEIGHT + TOOLBAR_HEIGHT), TRUE);

    layoutToolbarButtons(w, TOOLBAR_HEIGHT);

    /// 리사이즈 때 캔버스 전체 무효화. 잔상 제거
    InvalidateRect(canvasArea, nullptr, TRUE);
}

/// 툴바 내부 버튼을 가로 균등 배치
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

/// 메인 윈도우 프로시저
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
        // 생성 초기에 들어오는 몇몇 메시지 방어
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
    case WM_CREATE:
        self->createChildWindows(hwnd);
        return 0;

    case WM_SIZE: {
        if (self && self->canvasArea) {
            RECT rc; GetClientRect(hwnd, &rc);
            int toolbarH = 60; // 툴바 높이에 맞게
            MoveWindow(self->canvasArea,
                0, toolbarH,
                rc.right, rc.bottom - toolbarH,
                TRUE);
        }
        return 0;
    }

                // STATIC(헤더) 배경을 초록으로 칠하고 “유한대”는 검정색 텍스트
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

    // 버튼 클릭 처리
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

/// 캔버스 전용 프로시저 : 배경을 항상 깨끗하게 칠함
LRESULT CALLBACK WindowTool::canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND parent = GetParent(hwnd);
    auto* self = reinterpret_cast<WindowTool*>(GetWindowLongPtr(parent, GWLP_USERDATA));
    if (!self) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg)
    {
    case WM_ERASEBKGND:
        // 깜빡임 방지: 우리가 직접 그릴 거라서 배경 지우지 않음
        return 1;

    case WM_CREATE:
        // 최초 생성 시 버퍼 준비
        self->canvas.resizeTo(hwnd);
        return 0;

    case WM_SIZE:
        // 사이즈 변경 시 버퍼 재생성
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

        /// 버퍼 DC에 그리기
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

        /// 변경된 영역만 갱신하고 싶으면 InvalidateRect에 소사각 지정
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

        // 필요 시 전체를 다시 그려야 할 때(리플레이 없이):
        // self->canvas.clear(); self->drawPoints.drawToPoint(self->canvas.memDC);

        //오프스크린 버퍼 화면으로 복사
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

