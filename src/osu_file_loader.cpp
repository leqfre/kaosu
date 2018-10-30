#include "osu_file_loader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

constexpr int maxTimingPoints = 50000;
constexpr int maHitObjects    = 50000;

OsuFileLoader::OsuFileLoader(const std::string &fileName)
    : fileName_(fileName)
{
    timingPoints_.resize(maxTimingPoints);
    hitObjects_.resize(maHitObjects);
}

OsuFileLoader::~OsuFileLoader()
{
}

void OsuFileLoader::load()
{
    std::ifstream ifs(fileName_);

    if (ifs.fail())
    {
        std::cerr << "failed to open file" << std::endl;
        return;
    }

    std::string s;
    while (std::getline(ifs, s))
    {
        if (s == "" || isComment(s))
        {
            continue;
        }

        std::smatch match;
        std::regex reColonSplit("(.*?):\\s*(.*?)");
        std::regex reCommaSplit("([\\d|\\.]*?),*");

        switch (analyzeTag(s))
        {
        case General:
            do
            {
                general_[match[1]] = match[2];
                std::getline(ifs, s);
            }while(regex_match(s, match, reColonSplit));
            break;
        case Editor:
            do
            {
                editor_[match[1]] = match[2];
                std::getline(ifs, s);
            }while(regex_match(s, match, reColonSplit));
            break;
        case Metadata:
            do
            {
                metadata_[match[1]] = match[2];
                std::getline(ifs, s);
            }while(regex_match(s, match, reColonSplit));
            break;
        case Difficulty:
            do
            {
                difficulty_[match[1]] = match[2];
                std::getline(ifs, s);
            }while(regex_match(s, match, reColonSplit));
            break;
        case Events:
            break;
        case TimingPoints:
            for (int i = 0; std::getline(ifs, s); ++i)
            {
                if (s == "")
                {
                    break;
                }

                if (isComment(s))
                {
                    continue;
                }

                std::stringstream ss{s};
                timingPoints_[i].reserve(10);
                while (std::getline(ss, s, ','))
                {
                    timingPoints_[i].push_back(std::stod(s));
                }
            }
            break;
        case HitObjects:
            for (int i = 0; std::getline(ifs, s); ++i)
            {
                if (s == "")
                {
                    break;
                }

                if (isComment(s))
                {
                    continue;
                }

                std::stringstream ss{s};
                hitObjects_[i].reserve(10);
                while (std::getline(ss, s, ','))
                {
                    hitObjects_[i].push_back(std::stod(s));
                }
            }
            break;
        default:
            break;
        }
    }
}

bool OsuFileLoader::isComment(const std::string &s)
{
    return (s.length() >= 2) && (s[0] == '/') && (s[1] == '/');
}

Tag OsuFileLoader::analyzeTag(const std::string &s)
{
    std::smatch match;

    std::regex reGeneral("\\[General\\]");
    if(regex_match(s, match, reGeneral))
    {
        return Tag::General;
    }

    std::regex reEditor("\\[Editor\\]");
    if(regex_match(s, match, reEditor))
    {
        return Tag::Editor;
    }

    std::regex reMetadata("\\[Metadata\\]");
    if(regex_match(s, match, reMetadata))
    {
        return Tag::Metadata;
    }

    std::regex reDifficulty("\\[Difficulty\\]");
    if(regex_match(s, match, reDifficulty))
    {
        return Tag::Difficulty;
    }

    std::regex reEvents("\\[Events\\]");
    if(regex_match(s, match, reEvents))
    {
        return Tag::Events;
    }

    std::regex reTimingPoints("\\[TimingPoints\\]");
    if(regex_match(s, match, reTimingPoints))
    {
        return Tag::TimingPoints;
    }

    std::regex reHitObjects("\\[HitObjects\\]");
    if(regex_match(s, match, reHitObjects))
    {
        return Tag::HitObjects;
    }

    return Tag::NoneTag;
}