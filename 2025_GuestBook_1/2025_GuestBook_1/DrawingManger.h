#pragma once
#include <windows.h>
#include "PenView.h"

class DrawingManager {
private:
    POINT lastPoint;
    bool isDrawing;
    PenView* penView;
    DWORD lastTime;        // 마지막 그리기 시간
    float currentWidth;    // 현재 펜 굵기
    const float MIN_WIDTH = 1.0;  // 최소 굵기
    const float MAX_WIDTH = 8.0f;  // 최대 굵기

public:
    DrawingManager(PenView* pv);
    ~DrawingManager();
    void StartDrawing(HDC hdc, int x, int y);
    void ContinueDrawing(HDC hdc, int x, int y);
    void EndDrawing(HDC hdc, int x, int y);
    void DrawFountainPenStroke(HDC hdc, int x, int y);
    float CalculateSpeed(int x, int y, DWORD currentTime);
    void DrawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width);
};