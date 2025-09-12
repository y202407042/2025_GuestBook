#include "PenReplay.h"

void PenReplay::replayThread() {
    std::vector<PenData> localBuffer;
    {
        std::lock_guard<std::mutex> lock(mtx);
        localBuffer = replayBuffer;
    }

    /// 그릴 핸들
    HDC hdc = GetDC(targetHwnd);
    if (!localBuffer.empty()) {
        MoveToEx(hdc, localBuffer[0].x, localBuffer[0].y, nullptr);
    }

    for (const auto& p : localBuffer) {
        if (!isReplaying.load()) break;

        LineTo(hdc, p.x, p.y);

        /// 속도 조절
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_INTERVAL));
    }

    ReleaseDC(targetHwnd, hdc);
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