#include "PenReplay.h"

void PenReplay::addCoord(const Pen& pen)
{
	/// 펜값 저장
	if (isReplaying.load())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(mtx);
	originalBuffer.push_back(pen);
}

void PenReplay::replayStart()
{
	if (originalBuffer.empty() || isReplaying.load())
	{
		return;
	}

	isReplaying.store(true);
	std::lock_guard<std::mutex> lock(mtx);

	rpThread = std::thread([this]() {
		replayBuffer = originalBuffer;
		
		});

}

void PenReplay::replayPause()
{
	/// 일시정지
}