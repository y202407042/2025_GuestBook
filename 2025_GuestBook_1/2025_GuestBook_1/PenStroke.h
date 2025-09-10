#pragma once
#include "Windows.h"

/// 슬라이더를 사용하기 위한 api, 헤더파일
#include <commctrl.h>
#pragma comment (lib, "comctl32.lib")
/// 슬라이더 임시 상수 부여
#define IDC_TRACK_SLIDER 50001

class PenStroke
{
private:
	int sliderX, sliderY, sliderW, sliderH;
	INITCOMMONCONTROLSEX icc;
	HWND hStrokeSlider;

public:
	PenStroke();
	void SetStrokeSlider(HWND hStrock, HINSTANCE hInst);
};

