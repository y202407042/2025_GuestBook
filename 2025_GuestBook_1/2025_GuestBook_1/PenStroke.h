#pragma once
#include "Windows.h"
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
	int  value{ 10 };                           // 현재 값(펜 굵기)
	std::function<void(int)> onApply;         // 값 변경 콜백

public:
    PenStroke();
    void SetStrokeSlider(HWND parent, HINSTANCE hInst);

    /// 외부에서 콜백 등록
    void SetApplyCallback(std::function<void(int)> cb) { onApply = std::move(cb); }
    void Apply(int v) { if (onApply) onApply(v); }

    /// 외부에서 값 설정/조회
    void SetStroke(int v);
    int  GetStroke() const { return value; }

    ///부모(WndProc/WindowTool)에서 WM_HSCROLL 받을 때 호출, 처리했으면 true, 아니면 false 반환
    bool OnScroll(WPARAM wParam, LPARAM lParam);

    /// (선택) 필요시 핸들 얻기
    HWND Handle() const { return hStrokeSlider; }
};

