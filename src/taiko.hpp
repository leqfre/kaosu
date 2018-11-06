#pragma once
#include "DxLib.h"
#include <memory>
#include <chrono>
#include "resource_loader.hpp"
#include "game_controller.hpp"
#include "osu_file_loader.hpp"
#include "taiko_parser.hpp"

enum HitEffectType
{
    HE300,
    HE100,
    HE0
};

class Taiko
{
public:
    Taiko(const std::string&);
    ~Taiko();

    void load();
    void update();

private:
    void judge(const double, const NoteType);

    bool isTargetTimingElapsed(const double) const;
    bool isTargetNoteElapsed(const double) const;

    double calcElapsed() const;
    double calcBeatmapHiSpeed();

    NoteType getNoteType(const std::vector<double>&) const;

    void drawJudgeCircle() const;
    void drawNote(const int, const double) const;
    void drawHitEffect(const HitEffectType type);

    void playHitSound(const NoteType) const;

    ResourceLoader *rl_;
    GameController *gc_;
    std::string path_;
    std::unique_ptr<Beatmap> bm_;

    std::chrono::system_clock::time_point start_;

    int targetTimingIndex_;
    int targetNoteIndex_;

    double beatmapHiSpeed_;
    double notesInterval_;

    std::vector<std::unique_ptr<Note>> notes_;

    int hitEffectCount_;
    HitEffectType currentHitEffectType_;
};