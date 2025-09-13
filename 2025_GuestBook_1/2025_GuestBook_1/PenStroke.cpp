#include "PenStroke.h"
PenStroke::PenStroke()
{
	sliderX = 200;
	sliderY = 200;
	sliderW = 400;
	sliderH = 2000;

	/// ���� Ŭ���� �ʱ�ȭ. ���� ���� ��� ���� Ŭ������ ������ ���� ������
	/// �����Ⱚ or ������ ����
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_BAR_CLASSES;  // Ʈ����, ����, ���α׷�����
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
	/// windowtool.cpp�� �����ؼ� ����ϸ� ����.
}
