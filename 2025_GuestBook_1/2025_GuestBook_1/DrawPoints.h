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

public:
	/// ���� PenInit public���� �̵� 
	std::vector<PenData> PenInit;

	DrawPoints();
	/// COLORREF ���� ��
	/// ���Ϳ� ������ ���� �� �� ����, �ð��� �Բ� ����
	/// �ð��� ���� �� ���� �� chrono Ȱ���ؼ� ���� ����.
	void saveToPoint(int x, int y,uint32_t drawTime);
	void drawToPoint(HDC hdc);

};

