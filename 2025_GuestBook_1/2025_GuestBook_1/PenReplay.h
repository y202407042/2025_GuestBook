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
	HDC memoryDC = nullptr;		/// 메모리 DC
	HBITMAP backBufferBitmap = nullptr;	/// 백버퍼 비트맵
	HBITMAP replayBitmap = nullptr;	/// 복원용 비트맵

public:
	void addCoord(int x, int y);

	bool isReplaying() const { return replayFlag; }
};


