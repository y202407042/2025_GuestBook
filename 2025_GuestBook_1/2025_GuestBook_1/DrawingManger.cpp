#include "DrawingManger.h"
#include "DrawPoints.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv) // 생성자 구현 추가
	: penView(pv)
{
	lastPoint.x = 0;
	lastPoint.y = 0;
	isDrawing = false;
	/* 브러쉬 기능 비활성화
	currentWidth = 2.0f;
	lastTime = 0;
	*/
}

DrawingManager::~DrawingManager() // 소멸자 구현 추가
{
	// penView는 외부에서 관리되므로 삭제하지 않음
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
//	// 현재 펜 선택
//	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
//
//	// 시작점 설정
//	MoveToEx(hdc, x, y, NULL);
//	lastPoint.x = x;
//	lastPoint.y = y;
//
//	/// 벡터에 좌표 저장
//	DrawPoints::saveToPoint(lastPoint.x, lastPoint.y);
//	SelectObject(hdc, oldPen);
//}
//
//void DrawingManager::continueDrawing(HDC hdc, int x, int y) {
//	if (!isDrawing) return;
//	/* 브러쉬 기능 비활성화, 이후 아래 코드 삭제 후 다시; 활성화
//	if (penView->getCurrentPenType() == PEN_TYPE_BRUSH) {
//		drawFountainPenStroke(hdc, x, y);
//	}
//	else {
//		// 기존 그리기 로직
//		HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
//		MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
//		LineTo(hdc, x, y);
//
//		/// 벡터에 좌표 저장
//		/// DrawPoints::saveToPoint(lastPoint.x, lastPoint.x, currentWidth);
//		SelectObject(hdc, oldPen);
//	}
//	*/
//
//	// 기존 그리기 로직
//	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());s
//	MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
//	LineTo(hdc, x, y);
//
//	/// 벡터에 좌표 저장
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
//	// 마지막 점까지 선 그리기
//	HPEN oldPen = (HPEN)SelectObject(hdc, penView->getCurrentPen());
//	MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
//	LineTo(hdc, x, y);
//
//	/// 벡터에 좌표 저장
//	DrawPoints::saveToPoint(lastPoint.x, lastPoint.y);
//	SelectObject(hdc, oldPen);
//}



/// 여기는 백터로 좌표 저장하는거 아직 미완성. 감이 안잡힘.
/* 브러쉬 기능 비활성화
void DrawingManager::drawFountainPenStroke(HDC hdc, int x, int y) {
	DWORD currentTime = GetTickCount64();

	// 속도 계산 (픽셀/밀리초)
	float speed = calculateSpeed(x, y, currentTime);

	// 속도에 따른 굵기 조절 (느릴수록 굵게, 빠를수록 얇게)
	float targetWidth = MAX_WIDTH - (speed * 4.0f);
	if (targetWidth < MIN_WIDTH) targetWidth = MIN_WIDTH;
	if (targetWidth > MAX_WIDTH) targetWidth = MAX_WIDTH;

	// 부드러운 굵기 변화를 위한 보간
	currentWidth = currentWidth * 0.7f + targetWidth * 0.3f;

	// 가변 굵기로 선 그리기
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
	return distance / timeDiff; // 픽셀/밀리초
}

void DrawingManager::drawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width) {
	// 굵기에 따른 펜 생성
	int penWidth = (int)(width + 0.5f); // 반올림
	HPEN variablePen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 139)); // 진한 파란색
	HPEN oldPen = (HPEN)SelectObject(hdc, variablePen);

	// 선 그리기
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

}
*/
