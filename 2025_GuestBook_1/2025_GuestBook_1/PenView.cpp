#include "PenView.h"
#include "windows.h"
#include "Resource.h"

PenView::PenView()
{
    normalPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    brushPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    /// �������� ����� ������ â�� ���� �־�� �Ѵٰ� ��.
    /// sprayPen = ;
    currentPenType = PEN_TYPE_NORMAL;;
}
PenView::~PenView() // �Ҹ��� ���� �߰�
{
    DeleteObject(normalPen);
    DeleteObject(brushPen);
    DeleteObject(sprayPen);
}
void PenView::getChangePen(WPARAM wParam)  // message �Ű����� ����
{
    int commandId = LOWORD(wParam);  // ���� commandId ����

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
    currentPenType = type; // ���� �� Ÿ�� ����

    switch (type) { // �ùٸ� �� ���� �������� ����
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


