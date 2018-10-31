#include "osu_file_loader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
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
            } while (regex_match(s, match, reColonSplit));
            break;

        case Editor:
            do
            {
                editor_[match[1]] = match[2];
                std::getline(ifs, s);
            } while (regex_match(s, match, reColonSplit));
            break;

        case Metadata:
            do
            {
                metadata_[match[1]] = match[2];
                std::getline(ifs, s);
            } while (regex_match(s, match, reColonSplit));
            break;

        case Difficulty:
            do
            {
                difficulty_[match[1]] = match[2];
                std::getline(ifs, s);
            } while (regex_match(s, match, reColonSplit));
            break;

        case Events:
            break;

        case TimingPoints:
            while (std::getline(ifs, s))
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
                std::vector<double> timingPoint;

                while (std::getline(ss, s, ','))
                {
                    timingPoint.push_back(std::stod(s));
                }

                timingPoints_.push_back(timingPoint);
            }
            break;

        case HitObjects:
            while (std::getline(ifs, s))
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
                std::vector<double> hitObject;

                while (std::getline(ss, s, ','))
                {
                    hitObject.push_back(std::stod(s));
                }

                hitObjects_.push_back(hitObject);
            }
            break;

        default:
            break;
        }
    }
}

bool OsuFileLoader::isComment(const std::string &s) const
{
    return (s.length() >= 2) && (s[0] == '/') && (s[1] == '/');
}

Tag OsuFileLoader::analyzeTag(const std::string &s) const
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

std::unordered_map<std::string, std::string> OsuFileLoader::getGeneral() const
{
    return general_;
}

std::unordered_map<std::string, std::string> OsuFileLoader::getEditor() const
{
    return editor_;
}

std::unordered_map<std::string, std::string> OsuFileLoader::getMetadata() const
{
    return metadata_;
}

std::unordered_map<std::string, std::string> OsuFileLoader::getDifficulty() const
{
    return difficulty_;
}

std::unordered_map<std::string, std::string> OsuFileLoader::getEvents() const
{
    return events_;
}

std::vector<std::vector<double>> OsuFileLoader::getTimingPoints() const
{
    return timingPoints_;
}

std::vector<std::vector<double>> OsuFileLoader::getHitObjects() const
{
    return hitObjects_;
}