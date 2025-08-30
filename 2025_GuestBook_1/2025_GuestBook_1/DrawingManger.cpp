#include "DrawingManger.h"
#include <cmath>

DrawingManager::DrawingManager(PenView* pv) // ������ ���� �߰�
    : penView(pv), isDrawing(false)
{
    lastPoint.x = 0;
    lastPoint.y = 0;
    lastTime = 0;
    currentWidth = 2.0f;
}

DrawingManager::~DrawingManager() // �Ҹ��� ���� �߰�
{
    // penView�� �ܺο��� �����ǹǷ� �������� ����
}
void DrawingManager::StartDrawing(HDC hdc, int x, int y) {
    isDrawing = true;

    // ���� �� ����
    HPEN oldPen = (HPEN)SelectObject(hdc, penView->GetCurrentPen());

    // ������ ����
    MoveToEx(hdc, x, y, NULL);
    lastPoint.x = x;
    lastPoint.y = y;

    SelectObject(hdc, oldPen);
}

void DrawingManager::ContinueDrawing(HDC hdc, int x, int y) {
    if (!isDrawing) return;

    if (penView->GetCurrentPenType() == PEN_TYPE_BRUSH) {
        DrawFountainPenStroke(hdc, x, y);
    }
    else {
        // ���� �׸��� ����
        HPEN oldPen = (HPEN)SelectObject(hdc, penView->GetCurrentPen());
        MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
        LineTo(hdc, x, y);
        SelectObject(hdc, oldPen);
    }

    lastPoint.x = x;
    lastPoint.y = y;
}
void DrawingManager::EndDrawing(HDC hdc, int x, int y) { 
    if (!isDrawing) return;

    isDrawing = false;

    // ������ ������ �� �׸���
    HPEN oldPen = (HPEN)SelectObject(hdc, penView->GetCurrentPen());
    MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);
    LineTo(hdc, x, y);
    SelectObject(hdc, oldPen);
}
void DrawingManager::DrawFountainPenStroke(HDC hdc, int x, int y) {
    DWORD currentTime = GetTickCount64();

    // �ӵ� ��� (�ȼ�/�и���)
    float speed = CalculateSpeed(x, y, currentTime);

    // �ӵ��� ���� ���� ���� (�������� ����, �������� ���)
    float targetWidth = MAX_WIDTH - (speed * 2.0f);
    if (targetWidth < MIN_WIDTH) targetWidth = MIN_WIDTH;
    if (targetWidth > MAX_WIDTH) targetWidth = MAX_WIDTH;

    // �ε巯�� ���� ��ȭ�� ���� ����
    currentWidth = currentWidth * 0.7f + targetWidth * 0.3f;

    // ���� ����� �� �׸���
    DrawVariableWidthLine(hdc, lastPoint.x, lastPoint.y, x, y, currentWidth);

    lastTime = currentTime;
}

float DrawingManager::CalculateSpeed(int x, int y, DWORD currentTime) {
    if (lastTime == 0) {
        lastTime = currentTime;
        return 0.0f;
    }

    DWORD timeDiff = currentTime - lastTime;
    if (timeDiff == 0) return 0.0f;

    float distance = sqrt(pow(x - lastPoint.x, 2) + pow(y - lastPoint.y, 2));
    return distance / timeDiff; // �ȼ�/�и���
}

void DrawingManager::DrawVariableWidthLine(HDC hdc, int x1, int y1, int x2, int y2, float width) {
    // ���⿡ ���� �� ����
    int penWidth = (int)(width + 0.5f); // �ݿø�
    HPEN variablePen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 139)); // ���� �Ķ���
    HPEN oldPen = (HPEN)SelectObject(hdc, variablePen);

    // �� �׸���
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    // ��ũ ���� ȿ���� ���� �߰� ����
    if (width > 3.0f) {
        // ���� ���� �� �ֺ��� ���� ���� �߰�
        HPEN inkPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 200)); // ���� �Ķ���
        SelectObject(hdc, inkPen);

        // �ֺ��� ���� ���� �׸���
        SetPixel(hdc, x2 - 1, y2, RGB(150, 150, 220));
        SetPixel(hdc, x2 + 1, y2, RGB(150, 150, 220));
        SetPixel(hdc, x2, y2 - 1, RGB(150, 150, 220));
        SetPixel(hdc, x2, y2 + 1, RGB(150, 150, 220));

        DeleteObject(inkPen);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(variablePen);
}
