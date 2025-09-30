//#define NOMINMAX
//#include <windows.h>
//#include <windowsx.h>
//#include "ColorBox.h"
//
//// ===== ·¹ÀÌ¾Æ¿ô =====
//static RECT g_rcPalette[ColorBox::kSlots];
//static RECT g_rcCanvas;
//
//// ===== »óÅÂ =====
//static bool  g_drawing = false;
//static POINT g_lastPt = { 0,0 };
//static HPEN  g_hPen = nullptr;
//
//// À¯Æ¿
//static bool PtInRectI(const RECT& rc, int x, int y) {
//    return x >= rc.left && x < rc.right && y >= rc.top && y < rc.bottom;
//}
//
//static void ComputeLayout(HWND hWnd) {
//    RECT rc; GetClientRect(hWnd, &rc);
//    const int W = rc.right - rc.left;
//    const int H = rc.bottom - rc.top;
//
//    const int topBarH = 60;
//    const int pad = 10;
//    const int slotW = 80;
//    const int slotH = 40;
//
//    int startX = pad;
//    for (int i = 0; i < ColorBox::kSlots; i++) {
//        g_rcPalette[i] = { startX, pad, startX + slotW, pad + slotH };
//        startX += slotW + pad;
//    }
//    g_rcCanvas = { pad, topBarH, W - pad, H - pad };
//}
//
//static void RecreatePen() {
//    if (g_hPen) { DeleteObject(g_hPen); g_hPen = nullptr; }
//    g_hPen = CreatePen(PS_SOLID, ColorBox::currentThickness(), ColorBox::currentColor());
//}
//
//static void PaintUI(HDC hdc) {
//    for (int i = 0; i < ColorBox::kSlots; i++) {
//        HBRUSH b = CreateSolidBrush(ColorBox::getColorNum(i));
//        FillRect(hdc, &g_rcPalette[i], b);
//        DeleteObject(b);
//
//        HPEN pen = CreatePen(PS_SOLID, (i == ColorBox::colorSelect) ? 3 : 1, RGB(0, 0, 0));
//        HPEN old = (HPEN)SelectObject(hdc, pen);
//        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
//        Rectangle(hdc, g_rcPalette[i].left, g_rcPalette[i].top, g_rcPalette[i].right, g_rcPalette[i].bottom);
//        SelectObject(hdc, old);
//        DeleteObject(pen);
//    }
//
//    HPEN pen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
//    HPEN old = (HPEN)SelectObject(hdc, pen);
//    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
//    Rectangle(hdc, g_rcCanvas.left, g_rcCanvas.top, g_rcCanvas.right, g_rcCanvas.bottom);
//    SelectObject(hdc, old);
//    DeleteObject(pen);
//}
//
//static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//    switch (msg)
//    {
//    case WM_CREATE:
//        ColorBox::colorSelect = 0; // ±âº» ½½·Ô
//        ComputeLayout(hWnd);
//        RecreatePen();
//        return 0;
//
//    case WM_SIZE:
//        ComputeLayout(hWnd);
//        InvalidateRect(hWnd, nullptr, FALSE);
//        return 0;
//
//    case WM_PAINT:
//    {
//        PAINTSTRUCT ps;
//        HDC hdc = BeginPaint(hWnd, &ps);
//        PaintUI(hdc);
//        EndPaint(hWnd, &ps);
//        return 0;
//    }
//
//    case WM_LBUTTONDOWN:
//    {
//        const int x = GET_X_LPARAM(lParam);
//        const int y = GET_Y_LPARAM(lParam);
//
//        // ÆÈ·¹Æ® Å¬¸¯
//        for (int i = 0; i < ColorBox::kSlots; i++) {
//            if (PtInRectI(g_rcPalette[i], x, y)) {
//                ColorBox::colorSelect = i;
//                RecreatePen();
//                InvalidateRect(hWnd, nullptr, FALSE);
//                return 0;
//            }
//        }
//
//        // Äµ¹ö½º¿¡¼­ µå·ÎÀ× ½ÃÀÛ
//        if (PtInRectI(g_rcCanvas, x, y)) {
//            g_drawing = true;
//            g_lastPt = { x, y };
//            SetCapture(hWnd);
//        }
//        return 0;
//    }
//
//    case WM_MOUSEMOVE:
//        if (g_drawing) {
//            const int x = GET_X_LPARAM(lParam);
//            const int y = GET_Y_LPARAM(lParam);
//
//            if (PtInRectI(g_rcCanvas, x, y)) {
//                HDC hdc = GetDC(hWnd);
//                HPEN old = (HPEN)SelectObject(hdc, g_hPen);
//                MoveToEx(hdc, g_lastPt.x, g_lastPt.y, nullptr);
//                LineTo(hdc, x, y);
//                SelectObject(hdc, old);
//                ReleaseDC(hWnd, hdc);
//                g_lastPt = { x, y };
//            }
//        }
//        return 0;
//
//    case WM_LBUTTONUP:
//        if (g_drawing) {
//            g_drawing = false;
//            ReleaseCapture();
//        }
//        return 0;
//
//    case WM_KEYDOWN:
//        // 1/2/3 ½½·Ô ÀüÈ¯
//        if (wParam == '1' || wParam == '2' || wParam == '3') {
//            int idx = int(wParam - '1');
//            if (idx >= 0 && idx < ColorBox::kSlots) {
//                ColorBox::colorSelect = idx;
//                RecreatePen();
//                InvalidateRect(hWnd, nullptr, FALSE);
//            }
//        }
//        return 0;
//
//    case WM_DESTROY:
//        if (g_hPen) { DeleteObject(g_hPen); g_hPen = nullptr; }
//        PostQuitMessage(0);
//        return 0;
//    }
//    return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//
//int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
//{
//    const wchar_t* kClass = L"ColorDrawDemoWnd";
//    WNDCLASS wc = {};
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInst;
//    wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
//    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wc.lpszClassName = kClass;
//    RegisterClass(&wc);
//
//    HWND hWnd = CreateWindow(
//        kClass, L"Color Draw Demo (Click palette, draw on canvas)",
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
//        nullptr, nullptr, hInst, nullptr);
//
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//
//    MSG msg;
//    while (GetMessage(&msg, nullptr, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    return (int)msg.wParam;
//}
