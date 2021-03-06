#include "taiko_parser.hpp"
#include "DxLib.h"
#include "taiko_common.hpp"

TaikoParser::TaikoParser()
{
}

TaikoParser::~TaikoParser()
{
}

std::vector<std::unique_ptr<Note>> TaikoParser::makeNotes(std::vector<std::vector<double>>& hitObjects, const std::vector<std::vector<double>>& timingPoints)
{
    bool isRandom = true;

    int timingPointsIndex = 0;

    beatmapHiSpeed_ = 1.f;
    notesInterval_ = timingPoints[timingPointsIndex][1];

    std::vector<std::unique_ptr<Note>> notes(hitObjects.size());

    for (unsigned int i = 0; i < notes.size(); ++i)
    {
        notes[i] = std::make_unique<Note>();

        while ((unsigned) timingPointsIndex < timingPoints.size() && hitObjects[i][2] >= timingPoints[timingPointsIndex][0])
        {
            updateSettings(timingPoints[timingPointsIndex]);
            ++timingPointsIndex;
        }
        
        notes[i]->velocity = calcVelocity();
        notes[i]->type = getNoteType(hitObjects[i]);
        notes[i]->timing = (int) hitObjects[i][2] + blankPeriodMs + timingOffset;

        if (isRandom)
        {
            if (notes[i]->type == Don || notes[i]->type == Katsu)
            {
                notes[i]->type = (NoteType) (GetRand(1) + 1);
            }
            else
            {
                notes[i]->type = (NoteType) (GetRand(1) + 3);
            }
        }
    }

    return notes;
}

std::vector<std::unique_ptr<BarLine>> TaikoParser::makeBarLines(std::vector<std::vector<double>>& timingPoints, const double lastNoteTiming)
{
    double elapsed = timingPoints[0][1];

    std::vector<std::unique_ptr<BarLine>> barLines;

    
    for (unsigned int i = 0; i < timingPoints.size() || elapsed <= lastNoteTiming;)
    {
        while (i < timingPoints.size() && elapsed >= timingPoints[i][0])
        {
            updateSettings(timingPoints[i]);

            if (timingPoints[i][6] == 1)
            {
                elapsed = timingPoints[i][0];
            }
            ++i;
        }

        barLines.push_back(std::make_unique<BarLine>());
        barLines.back()->velocity = calcVelocity();
        barLines.back()->timing = (int) elapsed + blankPeriodMs + timingOffset;

        elapsed += notesInterval_ * 4.f;
    }

    return barLines;
}

double TaikoParser::calcVelocity() const
{
    return (noteSmallSize * 4 * systemHiSpeed * beatmapHiSpeed_) / notesInterval_;
}

void TaikoParser::updateSettings(const std::vector<double>& timingPoint)
{
    if (timingPoint[6] == 1)
    {
        notesInterval_ = timingPoint[1] * 1.f;
    }
    else
    {
        beatmapHiSpeed_ = 100.f / (timingPoint[1] * -1.f);
    }
}

NoteType TaikoParser::getNoteType(const std::vector<double>& hitObject) const
{
    switch ((int) hitObject[4])
    {
    case  0:
    case  1: return Don;
    case  2:
    case  3:
    case  8:
    case  9:
    case 10:
    case 11: return Katsu;
    case  4:
    case  5: return DonBig;
    case  6:
    case  7:
    case 12: return KatsuBig;
    default: return None;
    }
}