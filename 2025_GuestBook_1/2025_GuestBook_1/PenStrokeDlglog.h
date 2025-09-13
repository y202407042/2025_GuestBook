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

    bool Create();     // �𵨸��� ���̾�α� ����
    void Destroy();    // �ı�
    void Show(bool show);
    void SetStroke(int v);
    int  GetStroke() const { return value; }

    // �� ���� �� �ܺη� �˸�(�ݹ�)
    std::function<void(int)> onChanged;

private:
    HINSTANCE hInstance{};
    HWND hOwner{};
    HWND hDlg{};
    HWND hTrk{};
    HWND hLabel{};
    int  value{ 10 };     // �⺻��(����/���� �� ��ü)

    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    INT_PTR OnMessage(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    void initControls();
    void syncLabel();
};

/// #define ����� ���ҽ��� �Űܾ� ��.
