#pragma once
#include "Windows.h"

/// �����̴��� ����ϱ� ���� api, �������
#include <commctrl.h>
#pragma comment (lib, "comctl32.lib")
/// �����̴� �ӽ� ��� �ο�
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

