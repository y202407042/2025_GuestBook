#pragma once
#include <windows.h>
#include "PenView.h"
#include "DrawPoints.h"

class DrawingManager: public DrawPoints 
{
private:
    POINT lastPoint;
    PenView* penView;
    /* 펜 브러쉬 기능 비활성화
    DWORD lastTime;        // 마지막 그리기 시간
    float currentWidth;    // 현재 펜 굵기
    const float MIN_WIDTH = 1.0;  // 최소 굵기
    const float MAX_WIDTH = 8.0f;  // 최대 굵기
    */

public:
    BOOL isDrawing;
    DrawingManager(PenView* pv);
    ~DrawingManager();
    void testDrawing(HDC hdc, int x, int y);
    /*void startDrawing(HDC hdc, int x, int y);
    void continueDrawing(HDC hdc, int x, int y);
    void endDrawing(HDC hdc, int x, int y);*/

    /* 펜 브러쉬 기능 비활성화
    void drawFountainPenStroke(HDC hdc, int x, int y);
    float calculateSpeed(int x, int y, DWORD currentTime);
    void drawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width);
    */
};