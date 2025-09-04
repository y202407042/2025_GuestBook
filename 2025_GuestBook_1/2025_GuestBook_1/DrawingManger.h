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

    /// �ӽ� �׸��� ���� 
    /// ���߿� �ڵ� ������ �����Դϴ�.
    RECT DrawRect;

    /* �� �귯�� ��� ��Ȱ��ȭ
    DWORD lastTime;        // ������ �׸��� �ð�
    float currentWidth;    // ���� �� ����
    const float MIN_WIDTH = 1.0;  // �ּ� ����
    const float MAX_WIDTH = 8.0f;  // �ִ� ����
    */

public:
    BOOL isDrawing;
    DrawingManager(PenView* pv, HWND hWnd);
    ~DrawingManager();
    void testDrawing(HDC hdc, int x, int y);
    
    /*void startDrawing(HDC hdc, int x, int y);
    void continueDrawing(HDC hdc, int x, int y);
    void endDrawing(HDC hdc, int x, int y);*/

    /* �� �귯�� ��� ��Ȱ��ȭ
    void drawFountainPenStroke(HDC hdc, int x, int y);
    float calculateSpeed(int x, int y, DWORD currentTime);
    void drawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width);
    */
};