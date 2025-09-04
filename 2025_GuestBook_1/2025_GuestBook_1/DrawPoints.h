#pragma once
#include "PenView.h"
#include <vector>

struct PenData {
	int x, y, strock;
	/// ������ ���� color�� ���� �� ����
	// COLORREF penColor;
};

class DrawPoints
{
private:
	std::vector<PenData> PenInit;
public:
	/// COLORREF ���� ��
	DrawPoints();
	void saveToPoint(int x, int y);
	void drawToPoint(HDC hdc);
};

