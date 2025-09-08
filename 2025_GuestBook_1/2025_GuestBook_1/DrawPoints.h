#pragma once
#include "PenView.h"
#include <chrono>
#include <vector>

struct PenData {
	int x, y, strock;
	/// ������ ���� color�� ���� �� ����
	COLORREF penColor;
};

struct PenTime
{
	using Clock = std::chrono::steady_clock;
	Clock::time_point drawStart;
	Clock::time_point drawEnd;
};

class DrawPoints
{
private:
	std::vector<PenData> PenInit;
	std::vector<PenTime> PenTimeVector;
	double drawTime;

public:

	DrawPoints();
	/// COLORREF ���� ��
	/// ���Ϳ� ������ ���� �� �� ����, �ð��� �Բ� ����
	/// �ð��� ���� �� ���� �� chrono Ȱ���ؼ� ���� ����.
	void saveToPoint(int x, int y, int strock,  startTime, double endTime);
	void drawToPoint(HDC hdc);
	/// ���� �ð� Ȯ���ϴ� �Լ� ����
	double getStartTime();
	double getEndTime();

};

