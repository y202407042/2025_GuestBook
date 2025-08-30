#include "DrawingManger.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv) // 생성자 구현 추가
    : penView(pv), isDrawing(false)
{
    lastPoint.x = 0;
    lastPoint.y = 0;
    lastTime = 0;
    currentWidth = 2.0f;
}

DrawingManager::~DrawingManager() // 소멸자 구현 추가
{
    // penView는 외부에서 관리되므로 삭제하지 않음
}
void DrawingManager::StartDrawing(HDC hdc, int x, int y) {
    isDrawing = true;

    // 현재 펜 선택
    HPEN oldPen = (HPEN)SelectObject(hdc, penView->GetCurrentPen());

    // 시작점 설정
    MoveToEx(hdc, x, y, NULL);
    lastPoint.x = x;
    lastPoint.y = y;

    SelectObject(hdc, oldPen);
}

void DrawingManager::ContinueDrawing(HDC hdc, int x, int y) {
    if (!isDrawing) return;

    if (penView->GetCurrentPenType() == PEN_TYPE_BRUSH) {
        DrawFountainPenStroke(hdc, x, y);
    }
    else {
        // 기존 그리기 로직
        HPEN oldPen = (HPEN)SelectObject(hdc, penView->GetCurrentPen());
        MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
        LineTo(hdc, x, y);
        SelectObject(hdc, oldPen);
    }

    lastPoint.x = x;
    lastPoint.y = y;
}
void DrawingManager::EndDrawing(HDC hdc, int x, int y) { 
    if (!isDrawing) return;

    isDrawing = false;

    // 마지막 점까지 선 그리기
    HPEN oldPen = (HPEN)SelectObject(hdc, penView->GetCurrentPen());
    MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
    LineTo(hdc, x, y);
    SelectObject(hdc, oldPen);
}
void DrawingManager::DrawFountainPenStroke(HDC hdc, int x, int y) {
    DWORD currentTime = GetTickCount64();

    // 속도 계산 (픽셀/밀리초)
    float speed = CalculateSpeed(x, y, currentTime);

    // 속도에 따른 굵기 조절 (느릴수록 굵게, 빠를수록 얇게)
    float targetWidth = MAX_WIDTH - (speed * 2.0f);
    if (targetWidth < MIN_WIDTH) targetWidth = MIN_WIDTH;
    if (targetWidth > MAX_WIDTH) targetWidth = MAX_WIDTH;

    // 부드러운 굵기 변화를 위한 보간
    currentWidth = currentWidth * 0.7f + targetWidth * 0.3f;

    // 가변 굵기로 선 그리기
    DrawVariableWidthLine(hdc, lastPoint.x, lastPoint.y, x, y, currentWidth);

    lastTime = currentTime;
}

float DrawingManager::CalculateSpeed(int x, int y, DWORD currentTime) {
    if (lastTime == 0) {
        lastTime = currentTime;
        return 0.0f;
    }

    DWORD timeDiff = currentTime - lastTime;
    if (timeDiff == 0) return 0.0f;

    float distance = sqrt(pow(x - lastPoint.x, 2) + pow(y - lastPoint.y, 2));
    return distance / timeDiff; // 픽셀/밀리초
}

void DrawingManager::DrawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width) {
    // 굵기에 따른 펜 생성
    int penWidth = (int)(width + 0.5f); // 반올림
    HPEN variablePen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 139)); // 진한 파란색
    HPEN oldPen = (HPEN)SelectObject(hdc, variablePen);

    // 선 그리기
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    // 잉크 번짐 효과를 위한 추가 점들
    if (width > 3.0f) {
        // 굵은 선일 때 주변에 연한 점들 추가
        HPEN inkPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 200)); // 연한 파란색
        SelectObject(hdc, inkPen);

        // 주변에 연한 점들 그리기
        SetPixel(hdc, x2 - 1, y2, RGB(150, 150, 220));
        SetPixel(hdc, x2 + 1, y2, RGB(150, 150, 220));
        SetPixel(hdc, x2, y2 - 1, RGB(150, 150, 220));
        SetPixel(hdc, x2, y2 + 1, RGB(150, 150, 220));

        DeleteObject(inkPen);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(variablePen);
}
