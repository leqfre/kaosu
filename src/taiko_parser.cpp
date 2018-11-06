#include "taiko_parser.hpp"
#include "taiko_common.hpp"

TaikoParser::TaikoParser()
{
}

TaikoParser::~TaikoParser()
{
}

std::vector<std::unique_ptr<Note>> TaikoParser::parse(std::vector<std::vector<double>>& hitObjects, std::vector<std::vector<double>>& timingPoints)
{
    beatmapHiSpeed_ = 1.f;
    notesInterval_ = timingPoints[0][1];

    std::vector<std::unique_ptr<Note>> notes(hitObjects.size());

    for (unsigned int i = 0; i < notes.size(); ++i)
    {
        notes[i] = std::make_unique<Note>();

        if (timingPoints.size() > 0 && hitObjects[i][2] >= timingPoints[0][0])
        {
            updateSettings(timingPoints[0]);
            timingPoints.erase(timingPoints.begin());
        }

        notes[i]->vx = (noteSmallSize * 4 * systemHiSpeed * beatmapHiSpeed_) / notesInterval_;
        notes[i]->type = getNoteType(hitObjects[i]);
        notes[i]->timing = (int) hitObjects[i][2];
    }

    return notes;
}

void TaikoParser::updateSettings(std::vector<double>& timingPoint)
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