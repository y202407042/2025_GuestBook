#include "PenReplay.h"

void PenReplay::addCoord(const Pen& pen)
{
	/// 펜값 저장
	if (isReplaying.load())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(mtx);
	/// replayBuffer = pen.getPoints();
}

void PenReplay::replayStart()
{
	if (replayBuffer.empty() || isReplaying.load())
	{
		return;
	}

	isReplaying.store(true);
	std::lock_guard<std::mutex> lock(mtx);

	rpThread = std::thread([this]() {
		for (const auto& p : replayBuffer)
		{
			if (!isReplaying.load())
			{
				break;
			}

		}
		});

}

void PenReplay::replayPause()
{
	/// 일시정지
}