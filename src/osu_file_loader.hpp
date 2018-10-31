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
    std::unordered_map<std::string, std::string> getGeneral() const;
    std::unordered_map<std::string, std::string> getEditor() const;
    std::unordered_map<std::string, std::string> getMetadata() const;
    std::unordered_map<std::string, std::string> getDifficulty() const;
    std::unordered_map<std::string, std::string> getEvents() const;
    std::vector<std::vector<double>> getTimingPoints() const;
    std::vector<std::vector<double>> getHitObjects() const;

private:
    std::string fileName_;
    std::unordered_map<std::string, std::string> general_;
    std::unordered_map<std::string, std::string> editor_;
    std::unordered_map<std::string, std::string> metadata_;
    std::unordered_map<std::string, std::string> difficulty_;
    std::unordered_map<std::string, std::string> events_;
    std::vector<std::vector<double>> timingPoints_;
    std::vector<std::vector<double>> hitObjects_;

    Tag analyzeTag(const std::string&) const;
    bool isComment(const std::string&) const;
};