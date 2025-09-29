#include "PenView.h"
#include "windows.h"
#include "Resource.h"

PenView::PenView()
{
    normalPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    brushPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    /// 스프레이 기능은 별도의 창을 만들어서 넣어야 한다고 함.
    /// sprayPen = ;
    currentPenType = PEN_TYPE_NORMAL;;
}
PenView::~PenView() // 소멸자 구현 추가
{
    DeleteObject(normalPen);
    DeleteObject(brushPen);
    DeleteObject(sprayPen);
}
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
    currentPenType = type; // 현재 펜 타입 저장

    switch (type) { // 올바른 펜 선택 로직으로 변경
    case PEN_TYPE_NORMAL:
        selectPen = normalPen;
        break;
    case PEN_TYPE_BRUSH:
        selectPen = brushPen;
        break;
    case PEN_TYPE_SPRAY:
        selectPen = sprayPen;
        break;
    default:
        selectPen = normalPen;
        break;
    }
}
HPEN PenView::getCurrentPen() 
{
    return selectPen;
}

int PenView::getCurrentPenType() 
{
    return currentPenType;
}


