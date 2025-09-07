#pragma once
#include "PenView.h"
#include <vector>

struct PenData {
	int x, y, strock,time;
	/// ������ ���� color�� ���� �� ����
	COLORREF penColor;
};

class DrawPoints
{
private:
	std::vector<PenData> PenInit;

public:
	/// COLORREF ���� ��
	/// ���Ϳ� ������ ���� �� �� ����, �ð��� �Բ� ����
	/// �ð��� ���� �� ���� �� chrono Ȱ���ؼ� ���� ����.
	void saveToPoint(int x, int y, int strock, int time);
	void drawToPoint(HDC hdc);
};

