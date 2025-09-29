
#include "DrawPoints.h"
#include <chrono>
#include <vector>

/// COLORREF 적용 전
/// storck 제거

DrawPoints::DrawPoints() = default;

void DrawPoints::saveToPoint(int x, int y,uint32_t drawTime)
{
	PenInit.push_back({ x, y,drawTime});
}

/// 리플레이를 위한 getter 메서드 생성
const std::vector<PenData>& DrawPoints::getPenInit()
{
    return PenInit;

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