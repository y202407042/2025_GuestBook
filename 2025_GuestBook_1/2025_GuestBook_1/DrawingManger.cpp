
#include "PenView.h"
#include "DrawingManger.h"
#include "DrawPoints.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv, HWND hWnd) // 생성자 구현 추가
	: penView(pv)
{
	/// time 관련 변수들 초기화

	lastPoint.x = 0;
	lastPoint.y = 0;
	isDrawing = false;

	penWidth = penView->getPenStrock();
	penColor = penView->getPenColor();

	/// WM_MOUSELEAVE를 사용하기 위한 구조체 변수 추가
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;   // 떠날 때 알려달라
	tme.hwndTrack = hWnd;

}

DrawingManager::~DrawingManager() // 소멸자 구현 추가
{
	// penView는 외부에서 관리되므로 삭제하지 않음
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
		/// 그릴 그릴 때 함수를 통해 벡터로 좌표 전달
		/// 필요한 데이터들 벡터에 저장.
		
		DrawPoints::saveToPoint(x, y, penWidth);
	}
	SelectObject(hdc, oldPen);

}
/// L마우스 다운 업 이벤트 생성. 추후 메세지에 함수 불러와서 사용할 수 있게.
void DrawingManager::lMouseDownEvent()
{
	isDrawing = TRUE;
	drawStart = Clock::now();
	/// 마우스가 떠나게 되면 이벤트 메세지 발생
	TrackMouseEvent(&tme);
	SetCapture(hWnd);
}

void DrawingManager::lMouseUpEvent()
{
	isDrawing = FALSE;
	ReleaseCapture();
}

