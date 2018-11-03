#pragma once
#include "DxLib.h"
#include <memory>
#include <chrono>
#include "resource_loader.hpp"
#include "game_controller.hpp"
#include "osu_file_loader.hpp"

enum NoteType
{
    Don,
    Katsu
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

    ResourceLoader *rl_;
    GameController *gc_;
    std::unique_ptr<Beatmap> bm_;

    std::chrono::system_clock::time_point start_;

    std::string path_;

    double position_;
    int targetNoteIndex_;
};