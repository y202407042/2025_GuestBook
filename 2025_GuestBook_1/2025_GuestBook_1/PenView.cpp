#include "PenView.h"

void PenView::getChangePen(WPARAM wParam)  // message 매개변수 제거
{
    int commandId = LOWORD(wParam);  // 직접 commandId 추출

    switch (commandId) {
    case IDI_PEN_BUTTON:
        switchPen(PEN_TYPE_NORMAL);
        break;
    case IDI_BRUSH_BUTTON:
        switchPen(PEN_TYPE_BRUSH);
        break;
    case IDI_SPRAY_BUTTON:
        switchPen(PEN_TYPE_SPRAY);
        break;
    case IDI_ERASER_BUTTON:
        switchPen(PEN_TYPE_ERASER);
        break;
    }
}

void PenView::switchPen(int type)
{
    selectPen = (HBRUSH)type;
}

