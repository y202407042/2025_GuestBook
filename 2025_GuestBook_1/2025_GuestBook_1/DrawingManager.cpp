
#include "PenView.h"
#include "DrawingManager.h"
#include "DrawPoints.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv, HWND hWnd) // ������ ���� �߰�
	: penView(pv)
{
	/// time ���� ������ �ʱ�ȭ
	drawStart = {};
	drawEnd = {};
	drawTime = {};

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
	if (!isDrawing) return;

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());

	MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
	LineTo(hdc, x, y);
	lastPoint.x = x;
	lastPoint.y = y;
	/// �׸� �׸� �� �Լ��� ���� ���ͷ� ��ǥ ����
	/// �ʿ��� �����͵� ���Ϳ� ����.

	uint32_t drawTime = (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(
		Clock::now() - drawStart).count();

	DrawPoints::saveToPoint(x, y, drawTime);
	SelectObject(hdc, oldPen);

}
/// L���콺 �ٿ� �� �̺�Ʈ ����. ���� �޼����� �Լ� �ҷ��ͼ� ����� �� �ְ�.
void DrawingManager::lMouseDownEvent(int x, int y)
{
	isDrawing = TRUE;
	drawStart = Clock::now();
	lastPoint = { x,y };
	/// ���콺�� ������ �Ǹ� �̺�Ʈ �޼��� �߻�
	TrackMouseEvent(&tme);
	SetCapture(hWnd);
}

void DrawingManager::lMouseUpEvent()
{
	isDrawing = FALSE;
	ReleaseCapture();
}

