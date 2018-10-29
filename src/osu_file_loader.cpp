#include "osu_file_loader.hpp"
#include <iostream>
#include <fstream>
#include <regex>

OsuFileLoader::OsuFileLoader(const std::string &fileName)
    : fileName_(fileName)
{
}

OsuFileLoader::~OsuFileLoader()
{
}

void OsuFileLoader::load()
{
    std::ifstream ifs(this->fileName_);

    if (ifs.fail())
    {
        std::cerr << "failed to open file" << std::endl;
        return;
    }

    std::string s;
    while (getline(ifs, s))
    {
        std::unordered_map<std::string, std::string> map;

        switch (analyzeTag(s))
        {
        case General:
            map = this->general_;
            break;
        case Editor :
            map = this->editor_;
            break;
        case Metadata:
            map = this->metadata_;
            break;
        case Difficulty:
            map = this->difficulty_;
            break;
        case Events:
            map = this->events_;
            break;
        case TimingPoints:
            map = this->timingPoints_;
            break;
        case HitObjects:
            map = this->hitObjects_;
            break;
        default:
            break;
        }
    }
}

Tag OsuFileLoader::analyzeTag(const std::string &s)
{
    std::smatch match;

    std::regex re("\[General\]");
    if(regex_match(s, match, re))
    {
        return Tag::General;
    }

    std::regex re("\[Editor\]");
    if(regex_match(s, match, re))
    {
        return Tag::Editor;
    }

    std::regex re("\[Metadata\]");
    if(regex_match(s, match, re))
    {
        return Tag::Metadata;
    }

    std::regex re("\[Difficulty\]");
    if(regex_match(s, match, re))
    {
        return Tag::Difficulty;
    }

    std::regex re("\[Events\]");
    if(regex_match(s, match, re))
    {
        return Tag::Events;
    }

    std::regex re("\[TimingPoints\]");
    if(regex_match(s, match, re))
    {
        return Tag::TimingPoints;
    }

    std::regex re("\[HitObjects\]");
    if(regex_match(s, match, re))
    {
        return Tag::HitObjects;
    }

    return Tag::None;
}