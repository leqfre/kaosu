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
    NoneEffect,
    Whistle,
    Finish,
    Clap
};

enum Tag
{
    NoneTag,
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
    std::vector<std::vector<double>> timingPoints_;
    std::vector<std::vector<double>> hitObjects_;

    Tag analyzeTag(const std::string&);
    bool isComment(const std::string&);
};