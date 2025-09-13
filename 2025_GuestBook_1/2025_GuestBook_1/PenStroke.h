#pragma once
#include "Windows.h"
#include <functional>
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
	HWND hStrokeSlider{};
	int  value{ 10 };                           // ���� ��(�� ����)
	std::function<void(int)> onApply;         // �� ���� �ݹ�

public:
    PenStroke();
    void SetStrokeSlider(HWND parent, HINSTANCE hInst);

    /// �ܺο��� �ݹ� ���
    void SetApplyCallback(std::function<void(int)> cb) { onApply = std::move(cb); }
    void Apply(int v) { if (onApply) onApply(v); }

    /// �ܺο��� �� ����/��ȸ
    void SetStroke(int v);
    int  GetStroke() const { return value; }

    ///�θ�(WndProc/WindowTool)���� WM_HSCROLL ���� �� ȣ��, ó�������� true, �ƴϸ� false ��ȯ
    bool OnScroll(WPARAM wParam, LPARAM lParam);

    /// (����) �ʿ�� �ڵ� ���
    HWND Handle() const { return hStrokeSlider; }
};

