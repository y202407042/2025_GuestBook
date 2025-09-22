#pragma once
#include "Windows.h"

/// functional 헤더파일 추가 
#include <functional>

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
	HWND hStrokeSlider{};
	int penStroke { 10 };                           // 현재 값(펜 굵기)
	std::function<void(int)> onApply;         // 값 변경 콜백

public:
	PenStroke();
	void setStrokeSlider(HWND hStrock, HINSTANCE hInst);
	/// 펜 굵기 값 변경 함수 선언
	bool onScroll(WPARAM wParam, LPARAM lParam);

	// 외부에서 콜백 등록
	void setApplyCallback(std::function<void(int)> cb) { onApply = std::move(cb); }

	// 외부에서 값 설정/조회
	void setStroke(int v);
	int  getStroke() const { return penStroke; }
};

