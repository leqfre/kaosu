#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

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
    double bpm;
    double offset;
    int music;
    std::unordered_map<std::string, std::string> general;
    std::unordered_map<std::string, std::string> editor;
    std::unordered_map<std::string, std::string> metadata;
    std::unordered_map<std::string, std::string> difficulty;
    std::unordered_map<std::string, std::string> events;
    std::vector<std::vector<double>> timingPoints;
    std::vector<std::vector<double>> hitObjects;
} Beatmap;

class OsuFileLoader {
public:
    OsuFileLoader(const std::string&);
    ~OsuFileLoader();

    std::unique_ptr<Beatmap> load();

private:
    std::string path_;
    
    Tag analyzeTag(const std::string&) const;
    bool isComment(const std::string&) const;
};