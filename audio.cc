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

void Audio::play_sample(const std::string& name) {
  Mix_Chunk* chunk = load_chunk(name);
  Mix_PlayChannel(-1, chunk, 0);
}

void Audio::play_music(const std::string& name) {
  Mix_Music* music = load_music(name);
  Mix_FadeInMusic(music, 1, config_.fade_time);
}

Mix_Chunk* Audio::load_chunk(const std::string& file) {
  const std::string path("content/" + file);
  if (chunks_.count(path) == 0) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    chunks_[path] = chunk;
  }

  return chunks_[path];
}

Mix_Music* Audio::load_music(const std::string& file) {
  const std::string path("content/" + file);
  if (musics_.count(path) == 0) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    musics_[path] = music;
  }

  return musics_[path];
}
