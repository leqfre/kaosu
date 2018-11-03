#pragma once
#include "DxLib.h"
#include <memory>
#include <chrono>
#include "resource_loader.hpp"
#include "osu_file_loader.hpp"

enum ProcessSequence
{
    None,
    Load,
    Play
};

class Taiko
{
public:
    Taiko(const std::string&);
    ~Taiko();

    void load();
    void update();

private:
    ResourceLoader *rl_;
    std::unique_ptr<Beatmap> bm_;

    std::chrono::system_clock::time_point start_;

    ProcessSequence process_;
    std::string path_;

    double position_;
    int targetNote_;
};