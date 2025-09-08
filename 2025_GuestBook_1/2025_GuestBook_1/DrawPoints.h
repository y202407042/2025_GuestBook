#pragma once
#include "PenView.h"
#include <chrono>
#include <vector>

struct PenData {
	int x, y, strock;
	/// 색상을 이후 color와 연결 후 적용
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
	/// COLORREF 적용 전
	/// 백터에 데이터 저장 시 펜 굵기, 시간도 함께 저장
	/// 시간은 조금 더 공부 후 chrono 활용해서 저장 예정.
	void saveToPoint(int x, int y, int strock,  startTime, double endTime);
	void drawToPoint(HDC hdc);
	/// 시작 시간 확인하는 함수 제작
	double getStartTime();
	double getEndTime();

};

