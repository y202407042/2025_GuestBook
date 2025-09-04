#include "PenReplay.h"


void PenReplay::replayThread()
{
    std::vector<PenData> localBuffer;

    {
        /// �� �ɰ� ����
        std::lock_guard<std::mutex> lock(mtx);
        localBuffer = replayBuffer;
    }

    for (const auto& p : localBuffer)
    {
        if (!isReplaying.load())
        {
            break;
        }

        /// ���⼭ ó�� �ؾߵ�

        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_INTERVAL));

    }
    isReplaying.store(false);
}

void PenReplay::replayStart(const std::vector<PenData>& sourceBuffer)
{
    if (sourceBuffer.empty() || isReplaying.load())
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        /// ���� ������ ����
        replayBuffer = sourceBuffer;
    }

    isReplaying.store(true);

    /// ������ ����
    rpThread = std::thread(&PenReplay::replayThread, this);
    rpThread.detach();
}

void PenReplay::replayPause()
{
	/// �Ͻ�����
}