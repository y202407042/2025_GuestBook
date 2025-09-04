#include "PenReplay.h"


void PenReplay::replayThread()
{
    std::vector<PenData> localBuffer;

    {
        /// 락 걸고 복사
        std::lock_guard<std::mutex> lock(mtx);
        localBuffer = replayBuffer;
    }

    for (const auto& p : localBuffer)
    {
        if (!isReplaying.load())
        {
            break;
        }

        /// 여기서 처리 해야됨

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
        /// 원본 데이터 복사
        replayBuffer = sourceBuffer;
    }

    isReplaying.store(true);

    /// 스레드 실행
    rpThread = std::thread(&PenReplay::replayThread, this);
    rpThread.detach();
}

void PenReplay::replayPause()
{
	/// 일시정지
}