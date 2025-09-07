#include "DrawingManger.h"
#include "DrawPoints.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv, HWND hWnd) // ������ ���� �߰�
	: penView(pv)
{
	lastPoint.x = 0;
	lastPoint.y = 0;
	isDrawing = false;

	/// WM_MOUSELEAVE�� ����ϱ� ���� ����ü ���� �߰�
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;   // ���� �� �˷��޶�
	tme.hwndTrack = hWnd;

}

DrawingManager::~DrawingManager() // �Ҹ��� ���� �߰�
{
	// penView�� �ܺο��� �����ǹǷ� �������� ����
}

void DrawingManager::drawingEvent(HDC hdc, int x, int y,int strock,int penTime,COLORREF penColor)
{
	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
	if (isDrawing) {

		/// ���콺�� ������ �Ǹ� �̺�Ʈ �޼��� �߻�
		MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
		LineTo(hdc, x, y);
		lastPoint.x = x;
		lastPoint.y = y;
		/// �׸� �׸� �� �Լ��� ���� ���ͷ� ��ǥ ����
		/// �ʿ��� �����͵� ���Ϳ� ����.
		DrawPoints::saveToPoint(x, y,strock,penTime);
	}
	SelectObject(hdc, oldPen);

}
/// L���콺 �ٿ� �� �̺�Ʈ ����. ���� �޼����� �Լ� �ҷ��ͼ� ����� �� �ְ�.
void DrawingManager::lMouseDownEvent()
{
	isDrawing = TRUE;
	TrackMouseEvent(&tme);
	SetCapture(hWnd);
}

void DrawingManager::lMouseUpEvent()
{
	isDrawing = FALSE;
	ReleaseCapture();
}

