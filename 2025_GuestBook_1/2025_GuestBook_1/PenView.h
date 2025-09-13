#pragma once
#include <windows.h>

/// 바인딩 구현
#include "PenStrokeDlglog.h"

///버튼 상수처리문.
/// resource.h으로 이동 예정.
#define IDI_PEN_BUTTON 301
#define IDI_BRUSH_BUTTON 302
#define IDI_SPRAY_BUTTON 303
#define IDI_ERASER_BUTTON 304

/// 펜 모양 상수 처리문
/// resource.h로 이동 예정
#define PEN_TYPE_NORMAL 401
#define PEN_TYPE_BRUSH 402
#define PEN_TYPE_SPRAY 403
#define PEN_TYPE_ERASER 404

class PenStrokeDlg; // 다이얼로그 클래스 전방선언

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

	/// 굵기 관련 데이터 추가
	 void setPenStroke(int s);

};

