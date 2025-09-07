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

    /// â���� ���콺�� �������� ��Ȱ��ȭ �ϱ� ���� �ڵ�
    TRACKMOUSEEVENT tme = {};


public:
    BOOL isDrawing;
    DrawingManager(PenView* pv, HWND hWnd);
    ~DrawingManager();
    void drawingEvent(HDC hdc, int x, int y, int strock, int penTime, COLORREF penColor);
    /// L���콺 �ٿ� �� �̺�Ʈ ����. ���� �޼����� �Լ� �ҷ��ͼ� ����� �� �ְ�.
    void lMouseDownEvent();
    void lMouseUpEvent();

};