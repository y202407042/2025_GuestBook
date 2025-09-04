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
	void addCoord(/*πÃ¡§*/);
	void replayStart(const std::vector<PenData>& sourceBuffer);
	void replayThread();
	void replayPause();
	void replayThread();
};


