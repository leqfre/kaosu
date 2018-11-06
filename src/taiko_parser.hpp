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
    double vx;
    NoteType type;
    int timing;
} Note;

class TaikoParser
{
public:
    TaikoParser();
    ~TaikoParser();

    std::vector<std::unique_ptr<Note>> parse(std::vector<std::vector<double>>&, std::vector<std::vector<double>>&);

private:
    void updateSettings(std::vector<double>&);
    NoteType getNoteType(const std::vector<double>&) const;

    double notesInterval_;
    double beatmapHiSpeed_;
};