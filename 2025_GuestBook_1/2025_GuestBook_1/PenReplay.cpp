#include "PenReplay.h"

void PenReplay::addCoord(int x, int y)
{
	if (!isReplaying())
	{
		replayMutex.lock();

	}
}
