#pragma once
#include "PenView.h"
#include <vector>

struct PenData {
	int x, y, strock;
	/// 색상을 이후 color와 연결 후 적용
	// COLORREF penColor;
};

class DrawPoints
{
private:
	std::vector<PenData> PenInit;
public:
	/// COLORREF 적용 전
	DrawPoints();
	void saveToPoint(int x, int y);
	void drawToPoint(HDC hdc);
};

