#include "PenView.h"
#include "windows.h"
#include "PenStrokeDlglog.h"

#define IDD_PLUS_SIZE 30000
#define IDD_MIUS_SIZE 30001

PenView* gPenView = nullptr; 

PenView::PenView()
{
	/// 펜 초기화
	penStrock = 1;
	penColor = RGB(0, 0, 0);

	/// 상수를 변수로 수정
	normalPen = CreatePen(PS_SOLID, penStrock, penColor);
	brushPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

	selectPen = normalPen;
	currentPenType = PEN_TYPE_NORMAL;
}
PenView::~PenView() // 소멸자 구현 추가
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

void PenView::setPenStroke(int s)
{
	if (s < 1) s = 1;
	if (s > 30) s = 30;

	penStrock = s;

	// 기존 펜 삭제 후 재생성
	if (normalPen) DeleteObject(normalPen);
	normalPen = CreatePen(PS_SOLID, penStrock, penColor);

	// 현재 선택 펜이 일반펜이면 교체
	if (currentPenType == PEN_TYPE_NORMAL) {
		selectPen = normalPen;
	}
}


/// WindowTool.h
/// Add: #include "PenStrokDlg.h",  #include "PenStroke.h" 추가
/// Add: private 영역에 PenStrokeDlg* strokeDlg{nullptr};

/// WindowTool.cpp
/*
case WM_CREATE:
	self->createChildWindows(hwnd);
	penView->bindStrokeDialog(strokeDlg, // initStroke=10);

	// 펜 굵기 다이얼로그 생성
	self->strokeDlg = new PenStrokeDlg(self->hInstance, hwnd);
	if (self->strokeDlg->Create()) {
		self->strokeDlg->Show(false);
	}
	return 0;

	case WM_COMMAND: {
	const int id = LOWORD(wParam);
	switch (id) {
	case IDC_BTN_PEN:
		if (self->strokeDlg) self->strokeDlg->Show(true);
		return 0;

		case WM_DESTROY:
	if (self->strokeDlg) { self->strokeDlg->Destroy(); delete self->strokeDlg; self->strokeDlg = nullptr; }
	PostQuitMessage(0);
	return 0;


	*/
