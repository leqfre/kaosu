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
    void checkKeyInput();
    void judge(const NoteType);

    bool isTargetNoteElapsed() const;

    double calcElapsed() const;

    void drawJudgeCircle() const;
    void drawCombo() const;
    void drawBarLine(const int);
    void drawNote(const int);
    void drawHitEffect(const HitEffectType type);

    void playHitSound(const NoteType) const;

    ResourceLoader *rl_;
    GameController *gc_;
    std::string path_;
    std::unique_ptr<Beatmap> bm_;
    std::vector<std::unique_ptr<Note>> notes_;
    std::vector<std::unique_ptr<BarLine>> barLines_;

    std::chrono::system_clock::time_point start_;
    double elapsed_;

    int targetNoteIndex_;

    int hitEffectCount_;
    HitEffectType currentHitEffectType_;

    int combo_;

    bool isPlayingMusic_;
};