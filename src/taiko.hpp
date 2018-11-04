#pragma once
#include "DxLib.h"
#include <memory>
#include <chrono>
#include "resource_loader.hpp"
#include "game_controller.hpp"
#include "osu_file_loader.hpp"

enum NoteType
{
    None,
    Don,
    Katsu,
    DonBig,
    KatsuBig
};

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
    void judge(double, NoteType);

    bool isTargetNoteOutOfJudgeRange(double) const;
    double calcElapsed() const;

    NoteType getNoteType(const std::vector<double>&) const;

    void drawJudgeCircle() const;
    void drawNote(const int) const;
    void drawHitEffect(HitEffectType type) const;

    void playHitSound(NoteType) const;

    ResourceLoader *rl_;
    GameController *gc_;
    std::unique_ptr<Beatmap> bm_;

    std::chrono::system_clock::time_point start_;

    std::string path_;

    double position_;
    int targetNoteIndex_;
};