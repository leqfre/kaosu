#pragma once

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (4 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * 10;

    constexpr double systemHiSpeed = 0.85;

    constexpr double musicVolume = 0.7;

    constexpr int defaultNoteSize = 128;
    constexpr int noteSmallSize   = 100;
    constexpr int noteBigSize     = 160;

    constexpr int hitEffectSize = 260;

    constexpr int judgeCircleX = 120;
    constexpr int judgeCircleY = 300;

    constexpr int hitEffectX = judgeCircleX - 66;
    constexpr int hitEffectY = judgeCircleY - 66;

    constexpr int maxhitEffectCount_ = 8;
}