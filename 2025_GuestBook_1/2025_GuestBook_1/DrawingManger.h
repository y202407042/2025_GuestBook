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
    int penWidth;

    /// �ð� ������ ���� ���� ����
    using Clock = std::chrono::steady_clock;
    Clock::time_point drawStart;
    Clock::time_point drawEnd;

    COLORREF penColor;
    /// â���� ���콺�� �������� ��Ȱ��ȭ �ϱ� ���� �ڵ�
    TRACKMOUSEEVENT tme = {};


public:
    BOOL isDrawing;
    DrawingManager(PenView* pv, HWND hWnd);
    ~DrawingManager();
    void drawingEvent(HDC hdc, HWND hWnd, LPARAM lParam);
    /// L���콺 �ٿ� �� �̺�Ʈ ����. ���� �޼����� �Լ� �ҷ��ͼ� ����� �� �ְ�.
    void lMouseDownEvent();
    void lMouseUpEvent();

};