
#include "DrawPoints.h"
#include <chrono>
#include <vector>


/// ������ ���� �� ����ü ����
DrawPoints::DrawPoints()
{
 
}

/// COLORREF ���� ��
/// storck ����

void DrawPoints::saveToPoint(int x, int y, int strock, double startTime, double endTime)
{
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	PenInit.push_back({ x, y,strock});
}


void DrawPoints::drawToPoint(HDC hdc)
{
    if (PenInit.size() < 2) return; // �� 2�� �̻� �־�� �� �׷���

    // ù ��ǥ�� �̵�
    MoveToEx(hdc, PenInit[0].x, PenInit[0].y, NULL);

    // ������� �� �׸���
    for (size_t i = 1; i < PenInit.size(); ++i) {
        LineTo(hdc, PenInit[i].x, PenInit[i].y);
    }
}

/// chrono �߰� ���� �ʿ�
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