#pragma once
#include <windows.h>
#include "PenView.h"
#include "DrawPoints.h"
#include <chrono>

class DrawingManager: public DrawPoints 
{
private:
    POINT lastPoint;
    PenView* penView;
    HWND hWnd;
    int penWidth;

    /// 시간 저장을 위해 변수 선언
    using Clock = std::chrono::steady_clock;
    Clock::time_point drawStart;
    Clock::time_point drawEnd;
    uint32_t drawTime;

    COLORREF penColor;
    /// 창에서 마우스가 나가지면 비활성화 하기 위한 코드
    TRACKMOUSEEVENT tme = {};


public:
    BOOL isDrawing;
    DrawingManager(PenView* pv, HWND hWnd);
    ~DrawingManager();
    void drawingEvent(HDC hdc, HWND hWnd, LPARAM lParam);
    /// L마우스 다운 업 이벤트 생성. 추후 메세지에 함수 불러와서 사용할 수 있게.
    void lMouseDownEvent(int x, int y);
    void lMouseUpEvent();

};