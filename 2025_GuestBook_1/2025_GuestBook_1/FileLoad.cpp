#include "FileLoad.h"
#include <commdlg.h>
#include <fstream>

bool FileLoad::LoadFromFile(HWND hWnd, std::vector<DrawPoint>& outPoints) {
    wchar_t filePath[MAX_PATH] = { 0 };

    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = L"txt";

    if (GetOpenFileName(&ofn)) {
        std::wifstream in(filePath);
        if (!in.is_open()) return false;

        outPoints.clear();
        DrawPoint pt;
        while (in >> pt.x >> pt.y >> pt.penType) {
            outPoints.push_back(pt);
        }

        in.close();
        return true;
    }

    return false;
}
