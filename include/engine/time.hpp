#ifndef ENGINE_TIME_HPP
#define ENGINE_TIME_HPP

#include <chrono>

namespace Engine
{
    // handle time in the game
    class Time final
    {
    public:
        static float getTimeScale();
        static double getDeltaTime();
        static double getFixedDeltaTime();
        static int getLastFPS();
        static void setTimeScale(float timeScale);
    private:
        static void frameStart();
        static void frameEnd();
        inline static float timeScale = 1.0f;
        inline static int currentFPS = 0, lastFPS = 0;
        inline static double deltaTime = 0.0f, diffTime = 0.0f;
        inline static std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    friend class GameLoop;
    };
}

#endif