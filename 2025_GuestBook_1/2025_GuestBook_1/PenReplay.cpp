#include "PenReplay.h"

void PenReplay::replayThread()
{
    /// 리플레이 스레드에서 사용될 버퍼
    std::vector<PenData> localBuffer;

    {
        std::lock_guard<std::mutex> lock(mtx);
        localBuffer = replayBuffer;
    }

    if (localBuffer.empty() || targetHwnd == nullptr)
    {
        isReplaying.store(false);
        return;
    }

    /// 그릴 핸들
    HDC hdc = GetDC(targetHwnd);

    MoveToEx(hdc, localBuffer[0].x, localBuffer[0].y, nullptr);

    for (size_t i = 1; i < localBuffer.size(); ++i)
    {
        if (!isReplaying.load())
        {
            break;
        }

        /// 일시정지 상태인 경우 대기하는 루프문
        while (isPaused.load())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            /// 일시정지 상태가 해제 되면 빠져나옴
            if (!isReplaying.load())
            {
                break;
            }
        }

        LineTo(hdc, localBuffer[i].x, localBuffer[i].y);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_INTERVAL));
    }

    ReleaseDC(targetHwnd, hdc);
    isReplaying.store(false);
}

void PenReplay::replayStart(const std::vector<PenData>& sourceBuffer, HWND hwnd)
{
    if (sourceBuffer.empty() || isReplaying.load())
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        /// 원본의 데이터를 리플레이용 버퍼에 저장
        /// sourceBuffer는 PenInit의 값
        replayBuffer = sourceBuffer;
    }

    /// 핸들을 저장
    targetHwnd = hwnd;
    isReplaying.store(true);
    isPaused.store(false);

    if (rpThread.joinable())
    {
        rpThread.join();
    }
    rpThread = std::thread(&PenReplay::replayThread, this);
}

void PenReplay::replayPause()
{
	/// 일시정지
    isPaused.store(true);
}

void PenReplay::replayResume()
{
    /// 재개
    isPaused.store(false);
}

void PenReplay::replayStop()
{
    isReplaying.store(false);

    if (rpThread.joinable())
    {
        rpThread.join();
    }
}