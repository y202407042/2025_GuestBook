#include "PenStrokeDlglog.h"
#include <string>

bool PenStrokeDlg::Create() {
    if (hDlg) return true;

    hDlg = CreateDialogParamW(
        hInstance,
        MAKEINTRESOURCEW(IDD_PEN_STROKE_DLG),
        hOwner,
        PenStrokeDlg::DlgProc,
        (LPARAM)this
    );
    if (!hDlg) return false;

    ShowWindow(hDlg, SW_SHOW);
    return true;
}

void PenStrokeDlg::Destroy() {
    if (hDlg) {
        DestroyWindow(hDlg);
        hDlg = nullptr; hTrk = nullptr; hLabel = nullptr;
    }
}

void PenStrokeDlg::Show(bool show) {
    if (!hDlg) return;
    ShowWindow(hDlg, show ? SW_SHOW : SW_HIDE);
}

void PenStrokeDlg::SetStroke(int v) {
    // ¹üÀ§ Å¬·¥ÇÁ(1~30)
    if (v < 1) v = 1;
    if (v > 30) v = 30;
    value = v;
    if (hTrk) SendMessage(hTrk, TBM_SETPOS, TRUE, value);
    syncLabel();
}

INT_PTR CALLBACK PenStrokeDlg::DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    PenStrokeDlg* self = nullptr;
    if (msg == WM_INITDIALOG) {
        self = reinterpret_cast<PenStrokeDlg*>(lParam);
        SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)self);
    }
    else {
        self = reinterpret_cast<PenStrokeDlg*>(GetWindowLongPtrW(hDlg, GWLP_USERDATA));
    }
    if (self) return self->OnMessage(hDlg, msg, wParam, lParam);
    return FALSE;
}

INT_PTR PenStrokeDlg::OnMessage(HWND, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_INITDIALOG:
        initControls();
        return TRUE;

    case WM_HSCROLL:
    case WM_VSCROLL:
        if ((HWND)lParam == hTrk) {
            value = (int)SendMessage(hTrk, TBM_GETPOS, 0, 0);
            syncLabel();
            if (onChanged) onChanged(value);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        Show(false);  // X ´­·¶À» ¶§ ¼û±è¸¸
        return TRUE;
    }
    return FALSE;
}

void PenStrokeDlg::initControls() {
    hTrk = GetDlgItem(hDlg, IDC_TRK_STROKE);
    hLabel = GetDlgItem(hDlg, IDC_STC_VALUE);

    SendMessage(hTrk, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));
    SendMessage(hTrk, TBM_SETPOS, TRUE, value);
    syncLabel();
}

void PenStrokeDlg::syncLabel() {
    if (!hLabel) return;
    std::wstring text = L"±½±â: " + std::to_wstring(value);
    SetWindowTextW(hLabel, text.c_str());
}
