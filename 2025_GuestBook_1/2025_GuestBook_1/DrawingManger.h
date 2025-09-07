#pragma once
#include <windows.h>
#include "PenView.h"
#include "DrawPoints.h"

class DrawingManager: public DrawPoints 
{
private:
    POINT lastPoint;
    PenView* penView;
    HWND hWnd;

    /// 창에서 마우스가 나가지면 비활성화 하기 위한 코드
    TRACKMOUSEEVENT tme = {};


public:
    BOOL isDrawing;
    DrawingManager(PenView* pv, HWND hWnd);
    ~DrawingManager();
    void drawingEvent(HDC hdc, int x, int y, int strock, int penTime, COLORREF penColor);
    /// L마우스 다운 업 이벤트 생성. 추후 메세지에 함수 불러와서 사용할 수 있게.
    void lMouseDownEvent();
    void lMouseUpEvent();

};