#include "FileSave.h"
#include <commdlg.h>
#include <fstream>

bool FileSave::SaveToFile(HWND hWnd, const std::vector<DrawPoint>& points) {
    wchar_t filePath[MAX_PATH] = { 0 };

    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"txt";

    if (GetSaveFileName(&ofn)) {
        std::wofstream out(filePath);
        if (!out.is_open()) return false;

        for (const auto& pt : points) {
            out << pt.x << L" " << pt.y << L" " << pt.penType << std::endl;
        }

        out.close();
        return true;
    }

    return false;
}
