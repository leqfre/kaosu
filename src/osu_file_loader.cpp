#include "osu_file_loader.hpp"
#include "DxLib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "utils.hpp"

OsuFileLoader::OsuFileLoader(const std::string& path)
    : path_(path)
{
}

OsuFileLoader::~OsuFileLoader()
{
}

std::unique_ptr<Beatmap> OsuFileLoader::load()
{
    auto fileNames = Util::getFileNames(path_, Util::FILES_ONLY, ".osu");
    std::ifstream ifs(path_ + "/" + fileNames[0]);
    std::unique_ptr<Beatmap> bm = std::make_unique<Beatmap>();

    if (ifs.fail())
    {
        std::cerr << "failed to open file" << std::endl;
        return nullptr;
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

        switch (analyzeTag(s))
        {
        case General:
            do
            {
                bm->general[match[1]] = match[2];
                std::getline(ifs, s);
            } while (regex_match(s, match, reColonSplit));
            break;

        case Editor:
            do
            {
                bm->editor[match[1]] = match[2];
                std::getline(ifs, s);
            } while (regex_match(s, match, reColonSplit));
            break;

        case Metadata:
            do
            {
                bm->metadata[match[1]] = match[2];
                std::getline(ifs, s);
            } while (regex_match(s, match, reColonSplit));
            break;

        case Difficulty:
            do
            {
                bm->difficulty[match[1]] = match[2];
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

                bm->timingPoints.push_back(timingPoint);
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
                    if (s.find(':') != std::string::npos)
                    {
                        break;
                    }
                    hitObject.push_back(std::stod(s));
                }

                bm->hitObjects.push_back(hitObject);
            }
            break;

        default:
            break;
        }
    }

    bm->bpm = (1000 * 60) / bm->timingPoints[0][1];

    std::string audioFilePath = path_ + "/" + bm->general["AudioFilename"];
    bm->music = LoadSoundMem(audioFilePath.c_str());

    return bm;
}

bool OsuFileLoader::isComment(const std::string& s) const
{
    return (s.length() >= 2) && (s[0] == '/') && (s[1] == '/');
}

Tag OsuFileLoader::analyzeTag(const std::string& s) const
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