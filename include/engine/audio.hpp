#ifndef ENGINE_AUDIO_HPP
#define ENGINE_AUDIO_HPP

#include <engine/component.hpp>
#include <engine/resources.hpp>
#include <string>
#include <windows.h>

namespace Engine
{
    class Audio : public Component
    {
    public:
        enum AudioStatus {NONE, READY, PLAYING, PAUSE};
        enum AudioFile {NO_TYPE, MP3, WAV};
        bool load(std::string relative_audio_path, AudioFile fileType);
        void unload();
        void play(bool shouldLoop);
        void pause();
        void resume();
        void stop();
        void setVolume(int volume);
        AudioStatus getStatus();
        std::string getAudioFilePath();
        AudioFile getAudioFileType();
        int getVolume();
        bool isLooping();
    protected:
        inline static int aliasID = 0;
        AudioStatus audioStatus = NONE;
        AudioFile audioFile = NO_TYPE;
        std::string audio_path = "";
        int audioAliasID = -1;
        int volume = 50;
        bool looping = false;
        void update();
        void onDestroy();
    };
}


#endif
