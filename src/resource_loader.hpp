#pragma once

class ResourceLoader
{
public:
    ResourceLoader(const ResourceLoader&) = delete;
    ResourceLoader& operator=(const ResourceLoader&) = delete;

    static ResourceLoader *getInstance();
    void load();

    int getDonImage() const;
    int getKatsuImage() const;
    int getJudgeImage() const;

private:
    ResourceLoader() {};
    ~ResourceLoader() {};

    int donImage_;
    int katsuImage_;
    int judgeImage_;
};