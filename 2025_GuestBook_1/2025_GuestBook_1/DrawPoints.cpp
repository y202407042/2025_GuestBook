
#include "DrawPoints.h"
#include <chrono>
#include <vector>


/// 생성자 생성 및 구조체 정의
DrawPoints::DrawPoints()
{
 
}

/// COLORREF 적용 전
/// storck 제거

void DrawPoints::saveToPoint(int x, int y, int strock, double startTime, double endTime)
{
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	PenInit.push_back({ x, y,strock});
}


void DrawPoints::drawToPoint(HDC hdc)
{
    if (PenInit.size() < 2) return; // 점 2개 이상 있어야 선 그려짐

    // 첫 좌표로 이동
    MoveToEx(hdc, PenInit[0].x, PenInit[0].y, NULL);

    // 순서대로 선 그리기
    for (size_t i = 1; i < PenInit.size(); ++i) {
        LineTo(hdc, PenInit[i].x, PenInit[i].y);
    }
}

/// chrono 추가 공부 필요
double DrawPoints::getStartTime()
{
    auto time = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        time.time_since_epoch()
    ).count();
}

double DrawPoints::getEndTime()
{
    auto time = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        time.time_since_epoch()
    ).count();
}