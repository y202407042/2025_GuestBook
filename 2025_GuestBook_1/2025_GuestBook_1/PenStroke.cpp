#include "PenStroke.h"
PenStroke::PenStroke()
{
	/// 슬라이더 기본 위치 설정, 추후 변경 가능
	sliderX = 8;
	sliderY = 150;
	sliderW = 50;
	sliderH = 50 * 5;
	/// 변수 추가
	penStroke = 1;

	/// 공용 클래스 초기화. 하지 않을 경우 기존 클래스를 가지고 오기 떄문에
	/// 쓰레기값 or 컴파일 에러
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_BAR_CLASSES;  // 트랙바, 툴바, 프로그레스바
	InitCommonControlsEx(&icc);
}

void PenStroke::setStrokeSlider(HWND hStroke, HINSTANCE hInst)
{
	hStrokeSlider = CreateWindowExW(0, TRACKBAR_CLASSW,
		L"", WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_AUTOTICKS | TBS_TOOLTIPS, sliderX,
		sliderY, sliderW, sliderH, hStroke, (HMENU)IDC_TRACK_SLIDER, hInst, nullptr);
	
	SendMessage(hStrokeSlider, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));
	SendMessage(hStrokeSlider, TBM_SETPOS, TRUE, 10);

	SetWindowPos(hStrokeSlider, HWND_TOP, sliderX, sliderY, sliderW, sliderH,
		SWP_SHOWWINDOW);

	// 내부 상태/표시를 한 곳에서만 세팅
	setStroke(10);

	/// windowtool.cpp에 정의해서 사용하면 나옴.
}
void PenStroke::setStroke(int v)
{
	if (v < 1) v = 1;
	if (v > 30) v = 30;
	penStroke = v;

	if (hStrokeSlider) {
		/// 트랙바 표시도 동기화
		SendMessage(hStrokeSlider, TBM_SETPOS, TRUE, penStroke);
	}

	/// 바뀐 값을 외부(PenView)에 반영
	if (onApply) onApply(penStroke);
}

bool PenStroke::onScroll(WPARAM wParam, LPARAM lParam)
{
	// 내 트랙바가 아니면 무시
	if ((HWND)lParam != hStrokeSlider) return false;

	// 위치 읽어서 내부값 및 PenView에 반영
	int pos = (int)SendMessage(hStrokeSlider, TBM_GETPOS, 0, 0);

	if (pos != penStroke) {
		penStroke = pos;
		if (onApply) onApply(penStroke);
	}
	return true; // 내가 처리함
}