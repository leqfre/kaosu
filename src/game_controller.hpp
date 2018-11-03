#pragma once
#include <array>

namespace
{
    constexpr int keysSize = 256;
}

class GameController
{
public:
    GameController(const GameController&) = delete;
    GameController& operator=(const GameController&) = delete;

    static GameController *getInstance();
    void update();
    int getKey(int);

private:
    GameController() {};
    ~GameController() {};

    void updateKeys();

    std::array<int, keysSize> keys;
};