#pragma once
#include <windows.h>
#include "Resource.h"


class PenView
{
private:
	HPEN normalPen;
	HPEN brushPen;
	HPEN selectPen;
	int currentPenType;
	
	int penStrock;
	COLORREF penColor;

public:
	PenView();
	~PenView();
	HPEN getCurrentPen();
	int getCurrentPenType();

	int getPenStrock() const;
	COLORREF getPenColor() const;
	void setPenColor(COLORREF penColorData);
	/*void switchPen(int width, COLORREF color);*/

	/// 굵기 관련 데이터 추가
	 void setPenStroke(int s);

};

