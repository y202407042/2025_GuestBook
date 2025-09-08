
#include "PenView.h"
#include "DrawingManger.h"
#include "DrawPoints.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv, HWND hWnd) // ������ ���� �߰�
	: penView(pv)
{
	/// time ���� ������ �ʱ�ȭ

	lastPoint.x = 0;
	lastPoint.y = 0;
	isDrawing = false;

	penWidth = penView->getPenStrock();
	penColor = penView->getPenColor();

	/// WM_MOUSELEAVE�� ����ϱ� ���� ����ü ���� �߰�
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;   // ���� �� �˷��޶�
	tme.hwndTrack = hWnd;

}

DrawingManager::~DrawingManager() // �Ҹ��� ���� �߰�
{
	// penView�� �ܺο��� �����ǹǷ� �������� ����
}

void DrawingManager::drawingEvent(HDC hdc, HWND hWnd, LPARAM lParam)
{
	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
	if (isDrawing) {

		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
		LineTo(hdc, x, y);
		lastPoint.x = x;
		lastPoint.y = y;
		/// �׸� �׸� �� �Լ��� ���� ���ͷ� ��ǥ ����
		/// �ʿ��� �����͵� ���Ϳ� ����.
		
		DrawPoints::saveToPoint(x, y, penWidth);
	}
	SelectObject(hdc, oldPen);

}
/// L���콺 �ٿ� �� �̺�Ʈ ����. ���� �޼����� �Լ� �ҷ��ͼ� ����� �� �ְ�.
void DrawingManager::lMouseDownEvent()
{
	isDrawing = TRUE;
	drawStart = Clock::now();
	/// ���콺�� ������ �Ǹ� �̺�Ʈ �޼��� �߻�
	TrackMouseEvent(&tme);
	SetCapture(hWnd);
}

void DrawingManager::lMouseUpEvent()
{
	isDrawing = FALSE;
	ReleaseCapture();
}

