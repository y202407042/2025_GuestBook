#pragma once
#include <vector>
#include <windows.h>
#include <mutex>
class PenReplay
{
private:
	std::vector<int> originalBuffer;
	std::vector<int> replayBuffer;
	std::mutex originalMutex;
	std::mutex replayMutex;
	bool replayFlag = false;
	HDC memoryDC = nullptr;		/// �޸� DC
	HBITMAP backBufferBitmap = nullptr;	/// ����� ��Ʈ��
	HBITMAP replayBitmap = nullptr;	/// ������ ��Ʈ��

public:
	void addCoord(int x, int y);

	bool isReplaying() const { return replayFlag; }
};


