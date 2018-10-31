#pragma once
#include "DxLib.h"
#include <memory>
#include "resource_loader.hpp"
#include "osu_file_loader.hpp"

class Taiko
{
public:
    Taiko();
    ~Taiko();

    void update();

private:
    ResourceLoader *rl_;
    std::unique_ptr<OsuFileLoader> ofl_;
    std::vector<std::vector<double>> hitObjects_;

    double bpm_;
    double offset_;
    double position_;
};