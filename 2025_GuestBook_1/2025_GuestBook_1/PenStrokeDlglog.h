#pragma once
#include <windows.h>
#include <commctrl.h>
#include <functional>
#pragma comment(lib, "comctl32.lib")

#define IDD_PEN_STROKE_DLG   5100
#define IDC_TRK_STROKE       5101
#define IDC_STC_VALUE        5102

class PenStrokeDlg {
public:
    PenStrokeDlg(HINSTANCE hInst, HWND hOwner)
        : hInstance(hInst), hOwner(hOwner) {}

    bool Create();     // 모델리스 다이얼로그 생성
    void Destroy();    // 파괴
    void Show(bool show);
    void SetStroke(int v);
    int  GetStroke() const { return value; }

    // 값 변경 시 외부로 알림(콜백)
    std::function<void(int)> onChanged;

private:
    HINSTANCE hInstance{};
    HWND hOwner{};
    HWND hDlg{};
    HWND hTrk{};
    HWND hLabel{};
    int  value{ 10 };     // 기본값(저장/복원 시 교체)

    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    INT_PTR OnMessage(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    void initControls();
    void syncLabel();
};

/// #define 상수들 리소스로 옮겨야 함.
