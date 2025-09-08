#include "PenView.h"
#include "windows.h"
PenView::PenView()
{
	/// �� �ʱ�ȭ
	penStrock = 1;
	penColor = RGB(0, 0, 0);
	drawTime = 0;

	/// ����� ������ ����
	normalPen = CreatePen(PS_SOLID, penStrock, penColor);
	brushPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

	currentPenType = PEN_TYPE_NORMAL;;
}
PenView::~PenView() // �Ҹ��� ���� �߰�
{
	DeleteObject(normalPen);
	DeleteObject(brushPen);
}
void PenView::getChangePen(WPARAM wParam)  // message �Ű����� ����
{
	int commandId = LOWORD(wParam);  // ���� commandId ����

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
	currentPenType = type; // ���� �� Ÿ�� ����

	switch (type) { // �ùٸ� �� ���� �������� ����
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


