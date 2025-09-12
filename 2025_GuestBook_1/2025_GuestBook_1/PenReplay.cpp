#include "PenReplay.h"

void PenReplay::replayThread() {
    std::vector<PenData> localBuffer;
    {
        std::lock_guard<std::mutex> lock(mtx);
        localBuffer = replayBuffer;
    }

    /// �׸� �ڵ�
    HDC hdc = GetDC(targetHwnd);
    if (!localBuffer.empty()) {
        MoveToEx(hdc, localBuffer[0].x, localBuffer[0].y, nullptr);
    }

    for (const auto& p : localBuffer) {
        if (!isReplaying.load()) break;

        LineTo(hdc, p.x, p.y);

        /// �ӵ� ����
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