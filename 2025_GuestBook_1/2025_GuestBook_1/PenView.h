#pragma once
#include <windows.h>

///��ư ���ó����.
/// resource.h���� �̵� ����.
#define IDI_PEN_BUTTON 301
#define IDI_BRUSH_BUTTON 302
#define IDI_SPRAY_BUTTON 303
#define IDI_ERASER_BUTTON 304

/// �� ��� ��� ó����
/// resource.h�� �̵� ����
#define PEN_TYPE_NORMAL 401
#define PEN_TYPE_BRUSH 402
#define PEN_TYPE_SPRAY 403
#define PEN_TYPE_ERASER 404

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

	/// �� ���̾�α� ����
	INT_PTR CALLBACK PenDlgProc(HWND hDig, UINT message);

};

