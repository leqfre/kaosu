#pragma once
#include <vector>
#include <string>
#include <unordered_map>

enum NoteType
{
    Circle,
    Slider,
    Spinner
};

enum NoteEffect
{
    None,
    Whistle,
    Finish,
    Clap
};

enum Tag
{
    None,
    General,
    Editor,
    Metadata,
    Difficulty,
    Events,
    TimingPoints,
    HitObjects
};

typedef struct
{
    int x;
    int y;
    int vx;
    int vy;

    NoteType type;
    NoteEffect effect;
} Note;

class OsuFileLoader {
public:
    OsuFileLoader(const std::string&);
    ~OsuFileLoader();

    void load();
private:
    std::string fileName_;
    std::unordered_map<std::string, std::string> general_;
    std::unordered_map<std::string, std::string> editor_;
    std::unordered_map<std::string, std::string> metadata_;
    std::unordered_map<std::string, std::string> difficulty_;
    std::unordered_map<std::string, std::string> events_;
    std::unordered_map<std::string, std::string> timingPoints_;
    std::unordered_map<std::string, std::string> hitObjects_;

    Tag analyzeTag(const std::string&);
};