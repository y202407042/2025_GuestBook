#pragma once
#include "DrawPoints.h"
#include <vector>
#include <windows.h>
#include <mutex>
#include <atomic>
#include <thread>

#define TIME_INTERVAL 50

class PenReplay
{
private:
	std::vector<PenData> replayBuffer;
	std::thread rpThread;
	std::mutex mtx;
	std::atomic<bool> isReplaying{ false };
	/// 좌표를 던져줄 윈도우
	HWND targetHwnd = nullptr;

public:
	void replayStart(const std::vector<PenData>& sourceBuffer);
	void replayThread();
	void replayPause();
	void setTargetHwnd(HWND h) { targetHwnd = h; }
};


