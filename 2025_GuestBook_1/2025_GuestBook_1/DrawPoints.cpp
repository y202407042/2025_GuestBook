#include "DrawPoints.h"
#include <vector>

/// COLORREF ���� ��
/// storck ����

DrawPoints::DrawPoints()
{
	/// �׳� �����f���
}
void DrawPoints::saveToPoint(int x, int y)
{
	PenInit.push_back({ x, y});
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