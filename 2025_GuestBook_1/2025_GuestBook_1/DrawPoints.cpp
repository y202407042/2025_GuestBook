#include "DrawPoints.h"
#include <vector>

/// COLORREF ���� ��
void DrawPoints::saveToPoint(int x, int y, int strock)
{
	PenInit.push_back({ x, y, strock });
}