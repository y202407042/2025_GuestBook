#include "PenReplay.h"

void PenReplay::addCoord(const Pen& pen)
{
	if (isReplaying.load())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(mtx);
	originalBuffer.push_back(pen);
}

