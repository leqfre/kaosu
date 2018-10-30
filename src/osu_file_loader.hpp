#pragma once
#include <vector>
#include <string>
#include <unordered_map>

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

class OsuFileLoader {
public:
    OsuFileLoader(const std::string&);
    ~OsuFileLoader();

    void load();
    std::unordered_map<std::string, std::string> getGeneral();
    std::unordered_map<std::string, std::string> getEditor();
    std::unordered_map<std::string, std::string> getMetadata();
    std::unordered_map<std::string, std::string> getDifficulty();
    std::unordered_map<std::string, std::string> getEvents();
    std::vector<std::vector<double>> getTimingPoints();
    std::vector<std::vector<double>> getHitObjects();

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