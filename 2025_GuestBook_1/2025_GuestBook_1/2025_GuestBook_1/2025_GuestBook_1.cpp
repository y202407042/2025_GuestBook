// 2025_GuestBook_1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "2025_GuestBook_1.h"
#include "resource.h"
#include "WindowTool.h"
#include "ButtonTool.h"

#include "ColorManager.h"
#include "ColorBoxPanel.h"   // 새로 추가
#define MAX_LOADSTRING 100


static WindowTool* gWindowTool = nullptr;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

ColorManager g_colorManager;  // 현재 색상 저장
HWND gColorPanel = nullptr;   // ColorBox 패널 핸들

// 전역 그리기 상태
bool isDrawing = false;
int lastX = 0, lastY = 0;

// 캔버스 영역 정의
RECT g_logoRect = { 0, 0, 800, 40 };
RECT g_toolbarRect = { 0, 40, 800, 80 };
RECT g_canvasRect = { 20, 100, 780, 580 };

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2025GUESTBOOK1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2025GUESTBOOK1));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
//
//  함수: MyRegisterClass()
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2025GUESTBOOK1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY2025GUESTBOOK1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 900, 700, nullptr, nullptr, hInstance, nullptr);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
    /// WindowTool로 메인 윈도우 생성/표시 (템플릿 CreateWindowW 대체)
    gWindowTool = new WindowTool(hInstance);
    if (!gWindowTool->createMainWindow(1000, 700)) {
        return FALSE;
    }
    gWindowTool->showWindow(nCmdShow);
    return TRUE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // ColorBoxPanel 생성 (툴바 밑에 띄움)
        gColorPanel = ColorBoxPanel::Create(hWnd, 600, 40, 280, 300);
        // 변경 콜백 등록
        ColorBoxPanel::SetOnChanged(gColorPanel, [](COLORREF c, int thick) {
            g_colorManager.SetColor(c);
            // 여기서 펜 두께까지 관리할 수 있음
            });
        break;

    case WM_LBUTTONDOWN: {
        POINT pt{ LOWORD(lParam), HIWORD(lParam) };
        if (PtInRect(&g_canvasRect, pt)) {
            isDrawing = true;
            lastX = pt.x; lastY = pt.y;
            SetCapture(hWnd);                  // 캔버스에서만 캡처
        }
        else {
            isDrawing = false;                 // 패널 등 다른 영역 클릭 시 드로잉 OFF
        }
        return 0;
    }


    case WM_LBUTTONUP:
        if (isDrawing) {
            isDrawing = false;
            if (GetCapture() == hWnd) ReleaseCapture();
        }
        return 0;


    case WM_MOUSEMOVE:
        if (isDrawing && GetCapture() == hWnd) {   // 메인창이 캡처 보유시만 그리기
            int x = LOWORD(lParam), y = HIWORD(lParam);
            if (PtInRect(&g_canvasRect, { x, y })) {
                HDC hdc = GetDC(hWnd);
                int thick = ColorBox::getThicknessNum(ColorBox::colorSelect);
                HPEN hPen = CreatePen(PS_SOLID, thick, g_colorManager.GetColor());
                HGDIOBJ oldPen = SelectObject(hdc, hPen);
                MoveToEx(hdc, lastX, lastY, NULL);
                LineTo(hdc, x, y);
                SelectObject(hdc, oldPen);
                DeleteObject(hPen);
                ReleaseDC(hWnd, hdc);
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
        /// WM_DESTROY에서 gWindowTool 삭제 고려
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (gWindowTool) { delete gWindowTool; gWindowTool = nullptr; }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;

}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}