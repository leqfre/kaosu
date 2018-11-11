#pragma once
#include <array>

namespace
{
    constexpr int taikoNoteTypes = 5;
}

class ResourceLoader
{
public:
    ResourceLoader(const ResourceLoader&) = delete;
    ResourceLoader& operator=(const ResourceLoader&) = delete;

    static ResourceLoader *getInstance();
    void load();

    std::array<int, taikoNoteTypes> getTaikoNoteImages() const;
    std::array<int, taikoNoteTypes> getTaikoHitEffectImages() const;
    int getJudgeCircleImage() const;
    int getFont() const;


    std::array<int, taikoNoteTypes> getTaikoHitSounds() const;

private:
    ResourceLoader() {};
    ~ResourceLoader() {};

    std::array<int, taikoNoteTypes> taikoNoteImages_;
    std::array<int, taikoNoteTypes> taikoHitEffectImages_;
    int judgeCircleImage_;
    int font_;

    std::array<int, taikoNoteTypes> taikoHitSounds_;
};