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
	/// 리플레이 상태 확인
	std::atomic<bool> isReplaying { false };
	/// 일시정지 상태 확인
	std::atomic<bool> isPaused { false };
	/// 좌표를 던져줄 윈도우
	HWND targetHwnd = nullptr;

public:
	void replayStart(const std::vector<PenData>& sourceBuffer, HWND hwnd);
	void replayThread();
	void replayPause();
	void replayResume();
	void replayStop();
};


