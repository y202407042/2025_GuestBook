#include "DrawPoints.h"
#include <vector>

/// COLORREF Àû¿ë Àü
void DrawPoints::saveToPoint(int x, int y, int strock)
{
	PenInit.push_back({ x, y, strock });
}