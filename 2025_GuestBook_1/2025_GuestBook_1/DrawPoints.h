#pragma once
#include "PenView.h"
#include "DrawingManger.h"
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
	void saveToPoint(int x, int y, int strock);
};

