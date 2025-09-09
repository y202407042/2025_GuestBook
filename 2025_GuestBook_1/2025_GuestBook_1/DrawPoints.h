#pragma once
#include "PenView.h"
#include <chrono>
#include <vector>

struct PenData {
	int x, y;
	uint32_t drawTime;
	/// 색상을 이후 color와 연결 후 적용
	COLORREF penColor;
};


class DrawPoints
{
private:
	std::vector<PenData> PenInit;

public:

	DrawPoints();
	/// COLORREF 적용 전
	/// 백터에 데이터 저장 시 펜 굵기, 시간도 함께 저장
	/// 시간은 조금 더 공부 후 chrono 활용해서 저장 예정.
	void saveToPoint(int x, int y,uint32_t drawTime);
	void drawToPoint(HDC hdc);

};

