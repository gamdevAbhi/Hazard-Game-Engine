#include <engine/audio.hpp>

// for loading the audio
bool Engine::Audio::load(std::string relative_audio_path, AudioFile fileType)
{
    std::string full_path = Resources::get_current_dir();
    full_path += relative_audio_path;

    if(fileType == NO_TYPE) return false;

    if(Resources::is_file_exist(full_path) == false)
    {
        Handler::debug("Audio File not exist : " + full_path, "Audio");
        return false;
    }

    if(audioStatus != NONE) unload();

    audio_path = full_path;
    audioAliasID = aliasID++;

    std::string command = "open \"" + full_path + 
    ((fileType == MP3)? "\" type mpegvideo" : "\" type waveaudio") +
    " alias " +  std::to_string(audioAliasID);
    
    if(mciSendString(command.c_str(), NULL, 0, NULL) != 0)
    {
        Handler::debug("Audio File can't load : " + full_path, "Audio");
        return false;
    }

    audioStatus = READY;
    audioFile = fileType;
    looping = false;

    setVolume(volume);

    return true;
}

// for unloading the audio
void Engine::Audio::unload()
{
    if(audioAliasID == -1) return;

    if(audioStatus != READY) stop();

    std::string command = "close " + std::to_string(audioAliasID);

    mciSendString(command.c_str(), NULL, 0, NULL);

    audioAliasID = -1;
    audio_path = "";
    audioStatus = NONE;
    audioFile = NO_TYPE;
}

// for playing the audio
void Engine::Audio::play(bool shouldLoop)
{
    if(audioStatus != READY) return;

    std::string command = "play " + std::to_string(audioAliasID) + (shouldLoop? " repeat" : "");

    mciSendString(command.c_str(), NULL, 0, NULL);
    audioStatus = PLAYING;
    looping = shouldLoop;
}

// for pausing the audio
void Engine::Audio::pause()
{
    if(audioStatus != PLAYING) return;

    std::string command = "pause " + std::to_string(audioAliasID);

    mciSendString(command.c_str(), NULL, 0, NULL);
    audioStatus = PAUSE;
}

// for resuming the audio
void Engine::Audio::resume()
{
    if(audioStatus != PAUSE) return;

    std::string command = "resume " + std::to_string(audioAliasID);

    mciSendString(command.c_str(), NULL, 0, NULL);
    audioStatus = PLAYING;
}

// for stoping the audio
void Engine::Audio::stop()
{
    if(audioStatus != PLAYING) return;

    std::string command = "stop " + std::to_string(audioAliasID);

    mciSendString(command.c_str(), NULL, 0, NULL);
    audioStatus = READY;
}

// for changing the volume of the audio
void Engine::Audio::setVolume(int volume)
{
    if(audioStatus == NONE) return;

    if(volume < 0) volume = 0;
    else if(volume > 100) volume = 100;

    std::string command = "setaudio " + std::to_string(audioAliasID) + " volume to " +
    std::to_string(volume);
    mciSendStringA((LPCSTR)command.c_str(), NULL, 0, NULL);

    this->volume = volume;
}

// get the status of the audio
Engine::Audio::AudioStatus Engine::Audio::getStatus()
{
    return audioStatus;
}

// get the audio file path
std::string Engine::Audio::getAudioFilePath()
{
    return audio_path;
}

// get the audio file type
Engine::Audio::AudioFile Engine::Audio::getAudioFileType()
{
    return audioFile;
}

// get the audio volume
int Engine::Audio::getVolume()
{
    return volume;
}

// get the audio is looping
bool Engine::Audio::isLooping()
{
    return looping;
}

// update
void Engine::Audio::update()
{
    if(audioStatus != PLAYING) return;

    char msg[128] = {};

    std::string command = "status " + std::to_string(audioAliasID) + " mode";

    mciSendString(command.c_str(), msg, 128, NULL);

    if(strcmp(msg, "playing") != 0) audioStatus = READY;
}

// on destroy
void Engine::Audio::onDestroy()
{
    if(audioStatus != NONE) unload();
}