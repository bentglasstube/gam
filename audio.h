#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

class Audio {

  public:

    struct Config {
      Config() :
        frequency(MIX_DEFAULT_FREQUENCY), channels(2), chunksize(1024),
        format(MIX_DEFAULT_FORMAT), fade_time(500) {}

      int frequency, channels, chunksize;
      Uint16 format;
      int fade_time;
     };

    Audio(const Config& config);
    ~Audio();

    void stop_samples();
    void play_sample(const std::string& name);
    void sfx_volume(int volume) { Mix_Volume(-1, MIX_MAX_VOLUME * volume / 10); }

    void pause() { Mix_Pause(-1); }
    void resume() { Mix_Resume(-1); }

    void play_music(const std::string& name);
    void stop_music() { Mix_FadeOutMusic(config_.fade_time); }
    void music_volume(int volume) { Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 10); }
    bool music_playing() const { return Mix_PlayingMusic() == 1; }

  private:

    Config config_;

    Mix_Chunk* load_chunk(const std::string& file);
    Mix_Music* load_music(const std::string& file);

    typedef std::map<std::string, Mix_Chunk*> ChunkMap;
    typedef std::map<std::string, Mix_Music*> MusicMap;

    ChunkMap chunks_;
    MusicMap musics_;

};
