#include "FileSave.h"
#include <commdlg.h>
#include <fstream>

FileSave::FileSave(HWND hWnd) : hWnd(hWnd) {}
FileSave::~FileSave() {}

bool FileSave::saveToFile(const std::vector<PenData>& penData) {
    if (penData.empty()) return false;

    // ���� ���̾�α� ����
    WCHAR fileName[MAX_PATH] = L"";
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = L"Guest Book(*.gp)\0*.gp\0All Files\0*.*\0";
    ofn.lpstrDefExt = L"gp";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

    if (GetSaveFileNameW(&ofn)) {
        std::ofstream outFile(fileName, std::ios::binary);
        if (!outFile.is_open()) return false;

        // ������ ���� ����
        size_t size = penData.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // PenData ��ü ����
        outFile.write(reinterpret_cast<const char*>(penData.data()), size * sizeof(PenData));

        outFile.close();
        return true;
    }
    return false;
}
