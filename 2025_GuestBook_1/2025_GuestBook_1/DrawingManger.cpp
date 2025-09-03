#include "DrawingManger.h"
#include "DrawPoints.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv) // ������ ���� �߰�
	: penView(pv)
{
	lastPoint.x = 0;
	lastPoint.y = 0;
	isDrawing = false;
	/* �귯�� ��� ��Ȱ��ȭ
	currentWidth = 2.0f;
	lastTime = 0;
	*/
}

DrawingManager::~DrawingManager() // �Ҹ��� ���� �߰�
{
	// penView�� �ܺο��� �����ǹǷ� �������� ����
}

void DrawingManager::testDrawing(HDC hdc, int x, int y)
{
	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
	if (isDrawing)
	{
		MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
		LineTo(hdc, x, y);
	}
		lastPoint.x = x;
		lastPoint.y = y;
	DrawPoints::saveToPoint(x, y);
		// SelectObject(hdc, oldPen);
}
//}
//void DrawingManager::startDrawing(HDC hdc, int x, int y) {
//	isDrawing = true;
//
//	// ���� �� ����
//	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
//
//	// ������ ����
//	MoveToEx(hdc, x, y, NULL);
//	lastPoint.x = x;
//	lastPoint.y = y;
//
//	/// ���Ϳ� ��ǥ ����
//	DrawPoints::saveToPoint(lastPoint.x, lastPoint.y);
//	SelectObject(hdc, oldPen);
//}
//
//void DrawingManager::continueDrawing(HDC hdc, int x, int y) {
//	if (!isDrawing) return;
//	/* �귯�� ��� ��Ȱ��ȭ, ���� �Ʒ� �ڵ� ���� �� �ٽ�; Ȱ��ȭ
//	if (penView->getCurrentPenType() == PEN_TYPE_BRUSH) {
//		drawFountainPenStroke(hdc, x, y);
//	}
//	else {
//		// ���� �׸��� ����
//		HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
//		MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
//		LineTo(hdc, x, y);
//
//		/// ���Ϳ� ��ǥ ����
//		/// DrawPoints::saveToPoint(lastPoint.x, lastPoint.x, currentWidth);
//		SelectObject(hdc, oldPen);
//	}
//	*/
//
//	// ���� �׸��� ����
//	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());s
//	MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
//	LineTo(hdc, x, y);
//
//	/// ���Ϳ� ��ǥ ����
//	DrawPoints::saveToPoint(lastPoint.x, lastPoint.y);
//	SelectObject(hdc, oldPen);
//
//	lastPoint.x = x;
//	lastPoint.y = y;
//}
//void DrawingManager::endDrawing(HDC hdc, int x, int y) {
//	if (!isDrawing) return;
//
//	isDrawing = false;
//
//	// ������ ������ �� �׸���
//	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
//	MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
//	LineTo(hdc, x, y);
//
//	/// ���Ϳ� ��ǥ ����
//	DrawPoints::saveToPoint(lastPoint.x, lastPoint.y);
//	SelectObject(hdc, oldPen);
//}



/// ����� ���ͷ� ��ǥ �����ϴ°� ���� �̿ϼ�. ���� ������.
/* �귯�� ��� ��Ȱ��ȭ
void DrawingManager::drawFountainPenStroke(HDC hdc, int x, int y) {
	DWORD currentTime = GetTickCount64();

	// �ӵ� ��� (�ȼ�/�и���)
	float speed = calculateSpeed(x, y, currentTime);

	// �ӵ��� ���� ���� ���� (�������� ����, �������� ���)
	float targetWidth = MAX_WIDTH - (speed * 4.0f);
	if (targetWidth < MIN_WIDTH) targetWidth = MIN_WIDTH;
	if (targetWidth > MAX_WIDTH) targetWidth = MAX_WIDTH;

	// �ε巯�� ���� ��ȭ�� ���� ����
	currentWidth = currentWidth * 0.7f + targetWidth * 0.3f;

	// ���� ����� �� �׸���
	drawVariableWidthLine(hdc, lastPoint.x, lastPoint.y, x, y, currentWidth);

	lastTime = currentTime;
}

float DrawingManager::calculateSpeed(int x, int y, DWORD currentTime) {
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0.0f;
	}

	DWORD timeDiff = currentTime - lastTime;
	if (timeDiff == 0) return 0.0f;

	float distance = sqrt(pow(x - lastPoint.x, 2) + pow(y - lastPoint.y, 2));
	return distance / timeDiff; // �ȼ�/�и���
}

void DrawingManager::drawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width) {
	// ���⿡ ���� �� ����
	int penWidth = (int)(width + 0.5f); // �ݿø�
	HPEN variablePen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 139)); // ���� �Ķ���
	HPEN oldPen = (HPEN)SelectObject(hdc, variablePen);

	// �� �׸���
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

}
*/
