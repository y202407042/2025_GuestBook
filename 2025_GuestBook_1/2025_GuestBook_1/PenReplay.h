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

public:
	void addCoord(/*미정*/);
	void replayStart(const std::vector<PenData>& sourceBuffer);
	void replayThread();
	void replayPause();
	// 오류 보려고 일단 주석 달아놧습니다...
	/*void replayThread();*/
};


