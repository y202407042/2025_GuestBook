#include "PenStroke.h"
PenStroke::PenStroke()
{
	sliderX = 200;
	sliderY = 200;
	sliderW = 400;
	sliderH = 2000;

	/// 공용 클래스 초기화. 하지 않을 경우 기존 클래스를 가지고 오기 떄문에
	/// 쓰레기값 or 컴파일 에러
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_BAR_CLASSES;  // 트랙바, 툴바, 프로그레스바
	InitCommonControlsEx(&icc);
}

void PenStroke::SetStrokeSlider(HWND hStroke, HINSTANCE hInst)
{
	hStrokeSlider = CreateWindowExW(0, TRACKBAR_CLASSW,
		L"", WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_AUTOTICKS | TBS_TOOLTIPS, sliderX,
		sliderY, sliderW, sliderH, hStroke, (HMENU)IDC_TRACK_SLIDER, hInst, nullptr);
	
	SendMessage(hStrokeSlider, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));
	SendMessage(hStrokeSlider, TBM_SETPOS, TRUE, 10);

	SetWindowPos(hStrokeSlider, HWND_TOP, sliderX, sliderY, sliderW, sliderH,
		SWP_SHOWWINDOW);
	/// windowtool.cpp에 정의해서 사용하면 나옴.
}
