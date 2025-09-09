#include "PenView.h"
#include "windows.h"

#define IDD_PLUS_SIZE 30000
#define IDD_MIUS_SIZE 30001
PenView::PenView()
{
	/// �� �ʱ�ȭ
	penStrock = 1;
	penColor = RGB(0, 0, 0);

	/// ����� ������ ����
	normalPen = CreatePen(PS_SOLID, penStrock, penColor);
	brushPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

	selectPen = normalPen;
	currentPenType = PEN_TYPE_NORMAL;
}
PenView::~PenView() // �Ҹ��� ���� �߰�
{
	DeleteObject(normalPen);
	DeleteObject(brushPen);
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

INT_PTR CALLBACK PenDlgProc(HWND hDig, UINT message)
{
	int penPointSize = 1;
	switch (message)
	{
	case IDD_PLUS_SIZE:
	{
		return penPointSize++;
	}
	case IDD_MIUS_SIZE:
	{
		return penPointSize--;
	}
	}
}
