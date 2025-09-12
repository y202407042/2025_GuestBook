#include "FileLoad.h"
#include <commdlg.h>
#include <fstream>

FileLoad::FileLoad(HWND hWnd) : hWnd(hWnd) {}
FileLoad::~FileLoad() {}

bool FileLoad::loadFromFile(std::vector<PenData>& penData) {
    // ���� ���̾�α� ����
    WCHAR fileName[MAX_PATH] = L"";
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = L"Guest Book(*.gp)\0*.gp\0All Files\0*.*\0";
    ofn.lpstrDefExt = L"gp";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&ofn)) {
        std::ifstream inFile(fileName, std::ios::binary);
        if (!inFile.is_open()) return false;

        // ������ ���� �б�
        size_t size = 0;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (size == 0) return false;

        // ���� ũ�� ���� �� �б�
        penData.resize(size);
        inFile.read(reinterpret_cast<char*>(penData.data()), size * sizeof(PenData));

        inFile.close();
        return true;
    }
    return false;
}
