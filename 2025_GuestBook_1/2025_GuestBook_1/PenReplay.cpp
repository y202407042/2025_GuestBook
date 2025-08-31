#include "PenReplay.h"

void PenReplay::addCoord(const Pen& pen)
{
	/// �氪 ����
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

	std::lock_guard<std::mutex> lock(mtx);
	isReplaying.store(true);
	replayBuffer = originalBuffer;

}

void PenReplay::replayPause()
{
	/// �Ͻ�����
}

void PenReplay::replayThread()
{
	/// ���÷��̿� ������
	if (originalBuffer.empty() || isReplaying.load())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(mtx);
	isReplaying.store(true);

}