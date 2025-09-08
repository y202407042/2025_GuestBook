#include "PenView.h"
#include "windows.h"
PenView::PenView()
{
	/// 펜 초기화
	penStrock = 1;
	penColor = RGB(0, 0, 0);
	drawTime = 0;

	/// 상수를 변수로 수정
	normalPen = CreatePen(PS_SOLID, penStrock, penColor);
	brushPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

	currentPenType = PEN_TYPE_NORMAL;;
}
PenView::~PenView() // 소멸자 구현 추가
{
	DeleteObject(normalPen);
	DeleteObject(brushPen);
}
void PenView::getChangePen(WPARAM wParam)  // message 매개변수 제거
{
	int commandId = LOWORD(wParam);  // 직접 commandId 추출

	switch (commandId) {
	case IDI_PEN_BUTTON:
		switchPen(PEN_TYPE_NORMAL);
		break;
	case IDI_BRUSH_BUTTON:
		switchPen(PEN_TYPE_BRUSH);
		break;
	case IDI_SPRAY_BUTTON:
		switchPen(PEN_TYPE_SPRAY);
		break;
	case IDI_ERASER_BUTTON:
		switchPen(PEN_TYPE_ERASER);
		break;
	}
}

void PenView::switchPen(int type)
{
	currentPenType = type; // 현재 펜 타입 저장

	switch (type) { // 올바른 펜 선택 로직으로 변경
	case PEN_TYPE_NORMAL:
		selectPen = normalPen;
		break;
	case PEN_TYPE_BRUSH:
		selectPen = brushPen;
		break;
	default:
		selectPen = normalPen;
		break;
	}
}
HPEN PenView::getCurrentPen()
{
	return selectPen;
}

int PenView::getCurrentPenType()
{
	return currentPenType;
}

int PenView::getPenStrock() const
{ 
	return penStrock; 
}
COLORREF PenView::getPenColor() const 
{
	return penColor; 
}


