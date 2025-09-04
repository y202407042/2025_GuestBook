#pragma once
#include "Pen.h"
#include <vector>
#include <windows.h>
#include <mutex>
#include <atomic>
#include <thread>

#define TIME_INTERVAL 50;
#define TIME_TIMER 100;

class PenReplay
{
private:
	std::vector<Pen> replayBuffer;
	std::thread rpThread;
	std::mutex mtx;
	std::atomic<bool> isReplaying{ false };

public:
	void addCoord(const Pen& pen);
	void replayStart();
	void replayPause();
	void replayThread();
};


