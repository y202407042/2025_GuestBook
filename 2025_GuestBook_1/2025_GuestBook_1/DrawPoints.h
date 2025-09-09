#pragma once
#include "PenView.h"
#include <chrono>
#include <vector>

struct PenData {
	int x, y;
	uint32_t drawTime;
	/// ������ ���� color�� ���� �� ����
	COLORREF penColor;
};


class DrawPoints
{
private:
	std::vector<PenData> PenInit;

public:

	DrawPoints();
	/// COLORREF ���� ��
	/// ���Ϳ� ������ ���� �� �� ����, �ð��� �Բ� ����
	/// �ð��� ���� �� ���� �� chrono Ȱ���ؼ� ���� ����.
	void saveToPoint(int x, int y,uint32_t drawTime);
	void drawToPoint(HDC hdc);

};

