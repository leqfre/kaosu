#include "taiko.hpp"
#include <cstdlib>
#include "taiko_common.hpp"
#include "common_const.hpp"

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int notesOffset = -5;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (4 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * 10;

    constexpr double musicVolume = 0.75;

    constexpr int hitEffectSize = 260;

    constexpr int judgeCircleX = 200;
    constexpr int judgeCircleY = 300;

    constexpr int hitEffectX = judgeCircleX - 66;
    constexpr int hitEffectY = judgeCircleY - 66;

    constexpr int maxhitEffectCount_ = 8;

    constexpr int maxDisplayNotes = 50;
}

Taiko::Taiko(const std::string& song)
    : rl_(ResourceLoader::getInstance()), gc_(GameController::getInstance()), path_("./songs/" + song)
{
}

Taiko::~Taiko()
{
}

void Taiko::load()
{
    bm_ = std::make_unique<OsuFileLoader>(path_)->load();
    ChangeVolumeSoundMem((int) (255 * musicVolume), bm_->music);
    
    auto tp = std::make_unique<TaikoParser>();

    notes_ = tp->makeNotes(bm_->hitObjects, bm_->timingPoints);
    barLines_ = tp->makeBarLines(bm_->timingPoints, bm_->hitObjects.back()[2]);

    targetNoteIndex_ = 0;
    hitEffectCount_ = -1;

    combo_ = 0;

    isPlayingMusic_ = false;

    start_ = std::chrono::system_clock::now();
}

void Taiko::update()
{
    DrawFormatString(0,   0, GetColor(0, 255, 0), "%d", notes_.size());
    DrawFormatString(0,  16, GetColor(0, 255, 0), "%lf", notes_[targetNoteIndex_]->velocity);

    elapsed_ = calcElapsed();

    if (gc_->getKey(KEY_INPUT_Q) == 1)
    {
        DeleteSoundMem(bm_->music);
        load();
    }

    if (!isPlayingMusic_ && elapsed_ >= blankPeriodMs)
    {
        PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);
        isPlayingMusic_ = true;
    }

    if (isTargetNoteElapsed())
    {
        combo_ = 0;
        hitEffectCount_ = 0;
        currentHitEffectType_ = HE0;
        ++targetNoteIndex_;
    }

    checkKeyInput();

    drawJudgeCircle();

    if (hitEffectCount_ >= 0)
    {
        drawHitEffect(currentHitEffectType_);
    }

    for (unsigned int i = 0; i < barLines_.size(); ++i)
    {
        drawBarLine(i);
    }

    for (int i = min(notes_.size() - 1, maxDisplayNotes); i > -1 ; --i)
    {
        drawNote(i);
    }

    drawCombo();
}

void Taiko::checkKeyInput()
{
    static bool isAutoPlay;
    auto donL   = false;
    auto donR   = false;
    auto katsuL = false;
    auto katsuR = false;

    if (gc_->getKey(KEY_INPUT_TAB) == 1)
    {
        isAutoPlay = !isAutoPlay;
    }

    if (isAutoPlay && elapsed_ >= notes_[targetNoteIndex_]->timing)
    {
        auto noteType = notes_[targetNoteIndex_]->type;

        switch (noteType)
        {
        case Don:      donL   = true; break;
        case DonBig:   donL   = true; donR   = true; break;
        case Katsu:    katsuL = true; break;
        case KatsuBig: katsuL = true; katsuR = true; break;
        }
    }

    if (gc_->getKey(KEY_INPUT_F) == 1 || donL)
    {
        playHitSound(Don);
        judge(Don, isAutoPlay);
    }

    if (gc_->getKey(KEY_INPUT_J) == 1 || donR)
    {
        playHitSound(Don);
        judge(Don, isAutoPlay);
    }

    if (gc_->getKey(KEY_INPUT_D) == 1 || katsuL)
    {
        playHitSound(Katsu);
        judge(Katsu, isAutoPlay);
    }

    if (gc_->getKey(KEY_INPUT_K) == 1 || katsuR)
    {
        playHitSound(Katsu);
        judge(Katsu, isAutoPlay);
    }
}

void Taiko::judge(const NoteType inputedNoteType, const bool isAutoPlay)
{
    auto diff = std::abs(notes_[targetNoteIndex_]->timing - elapsed_);

    if (diff >= (badJudgeMs / 2))
    {
        return;
    }

    auto targetNoteType = notes_[targetNoteIndex_]->type;
    notes_.erase(notes_.begin() + targetNoteIndex_);

    hitEffectCount_ = 0;

    if ((inputedNoteType == Don && (targetNoteType != Don && targetNoteType != DonBig))
        || (inputedNoteType == Katsu && (targetNoteType != Katsu && targetNoteType != KatsuBig)))
    {
        combo_ = 0;
        currentHitEffectType_ = HE0;
        return;
    }

    auto isBig = ((targetNoteType == DonBig) || (targetNoteType == KatsuBig)
        ) && ((gc_->getKey(KEY_INPUT_F) == 1 && gc_->getKey(KEY_INPUT_J) == 1)
            || (gc_->getKey(KEY_INPUT_D) == 1 && gc_->getKey(KEY_INPUT_K) == 1)
            || isAutoPlay
        );

    if (diff < (perfectJudgeMs / 2))
    {
        ++combo_;
        if (isBig)
        {
            currentHitEffectType_ = HE300k;
        }
        else
        {
            currentHitEffectType_ = HE300;
        }
    }
    else if (diff < (goodJudgeMs / 2))
    {
        ++combo_;
        if (isBig)
        {
            currentHitEffectType_ = HE100k;
        }
        else
        {
            currentHitEffectType_ = HE100;
        }
    }
    else
    {
        combo_ = 0;
        currentHitEffectType_ = HE0;
    }
}

bool Taiko::isTargetNoteElapsed() const
{
    return (notes_[targetNoteIndex_]->timing + (badJudgeMs / 2) < elapsed_);
}

double Taiko::calcElapsed() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count());
}

void Taiko::drawCombo() const
{
    auto width = GetDrawFormatStringWidthToHandle(rl_->getFont(), "%d", combo_);
    auto x = judgeCircleX / 2 - width + 50;
    auto y = judgeCircleY + (noteBigSize - COMBO_FONT_SIZE) / 2;
    DrawFormatStringToHandle(x, y, GetColor(255, 255, 255), rl_->getFont(), "%d", combo_);
}

void Taiko::drawJudgeCircle() const
{
    DrawGraph(judgeCircleX, judgeCircleY, rl_->getJudgeCircleImage(), TRUE);
}

void Taiko::drawBarLine(const int index)
{
    auto time = barLines_[index]->timing - elapsed_;

    auto adjust = (defaultNoteSize) / 2;

    auto x = (int) (time * barLines_[index]->velocity + judgeCircleX + notesOffset) + adjust;
    auto y = judgeCircleY - 8;

    DrawLine(x, y, x, y + noteBigSize, GetColor(255, 255, 255));
}

void Taiko::drawNote(const int index)
{
    auto time = notes_[index]->timing - elapsed_;

    auto isBig = (notes_[index]->type == DonBig || notes_[index]->type == KatsuBig);
    auto noteSize = isBig ? noteBigSize : noteSmallSize;

    auto adjust = (defaultNoteSize - noteSize) / 2;

    auto x1 = (int) (time * notes_[index]->velocity + judgeCircleX + notesOffset) + adjust;
    auto x2 = x1 + noteSize;
    auto y1 = judgeCircleY + adjust;
    auto y2 = y1 + noteSize;

    if (x2 < 0)
    {
        notes_.erase(notes_.begin() + index);
        --targetNoteIndex_;
        return;
    }

    DrawExtendGraph(x1, y1, x2, y2, rl_->getTaikoNoteImages()[notes_[index]->type - 1], TRUE);
}

void Taiko::drawHitEffect(const HitEffectType type)
{
    auto diff = pow(hitEffectCount_ - (maxhitEffectCount_ / 2), 2);

    auto x1 = (int) (hitEffectX + diff);
    auto x2 = (int) (hitEffectX + hitEffectSize - diff);
    auto y1 = (int) (hitEffectY + diff);
    auto y2 = (int) (hitEffectY + hitEffectSize - diff);

    auto transparent = (int) (255 * (1 - ((double) hitEffectCount_ / maxhitEffectCount_)));

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
    DrawExtendGraph(x1, y1, x2, y2, rl_->getTaikoHitEffectImages()[type], TRUE) ;
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (++hitEffectCount_ > maxhitEffectCount_)
    {
        hitEffectCount_ = -1;
    }
}

void Taiko::playHitSound(const NoteType type) const
{
    auto index = (type == Don || type == DonBig) ? 0 : 1;
    PlaySoundMem(rl_->getTaikoHitSounds()[index], DX_PLAYTYPE_BACK);
}