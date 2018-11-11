#pragma once
#include <vector>
#include <memory>

enum NoteType
{
    None,
    Don,
    Katsu,
    DonBig,
    KatsuBig
};

typedef struct
{
    double velocity;
    NoteType type;
    int timing;
} Note;

typedef struct
{
    double velocity;
    int timing;
} BarLine;

class TaikoParser
{
public:
    TaikoParser();
    ~TaikoParser();

    std::vector<std::unique_ptr<Note>> makeNotes(std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&);
    std::vector<std::unique_ptr<BarLine>> makeBarLines(std::vector<std::vector<double>>&, const double);

private:
    double calcVelocity() const;
    void updateSettings(const std::vector<double>&);
    NoteType getNoteType(const std::vector<double>&) const;

    double notesInterval_;
    double beatmapHiSpeed_;
};