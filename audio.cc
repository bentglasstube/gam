#include "audio.h"

Audio::Audio(const Config& config) : config_(config) {
  Mix_OpenAudio(config_.frequency, config_.format, config_.channels,
      config_.chunksize);
}

Audio::~Audio() {
  Mix_HaltChannel(-1);
  Mix_HaltMusic();

  for (ChunkMap::iterator i = chunks_.begin(); i != chunks_.end(); ++i) {
    Mix_FreeChunk(i->second);
  }

  for (MusicMap::iterator i = musics_.begin(); i != musics_.end(); ++i) {
    Mix_FreeMusic(i->second);
  }

  Mix_CloseAudio();
}

void Audio::stop_samples() {
  Mix_HaltChannel(-1);
}

void Audio::play_sample(const std::string& name) {
  Mix_Chunk* chunk = load_chunk(name);
  Mix_PlayChannel(-1, chunk, 0);
}

void Audio::play_music(const std::string& name, bool loop) {
  if (name != current_track_) {
    Mix_Music* music = load_music(name);
    Mix_FadeInMusic(music, loop ? -1 : 0, config_.fade_time);
    current_track_ = name;
  }
}

void Audio::stop_music() {
  Mix_FadeOutMusic(config_.fade_time);
  current_track_ = "";
}

void Audio::music_volume(int volume) {
  Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 10);
}

bool Audio::music_playing() const {
  return Mix_PlayingMusic() == 1;
}

Mix_Chunk* Audio::load_chunk(const std::string& file) {
  const std::string path("content/" + file);
  if (chunks_.count(path) == 0) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

    if (chunk == NULL) {
      fprintf(stderr, "Couldn't load sample %s: %s\n", path.c_str(), Mix_GetError());
    }

    chunks_[path] = chunk;
  }

  return chunks_[path];
}

Mix_Music* Audio::load_music(const std::string& file) {
  const std::string path("content/" + file);
  if (musics_.count(path) == 0) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());

    if (music == NULL) {
      fprintf(stderr, "Couldn't load music %s: %s\n", path.c_str(), Mix_GetError());
    }

    musics_[path] = music;
  }

  return musics_[path];
}
