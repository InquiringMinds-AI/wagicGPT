//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
//
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
//
//-------------------------------------------------------------------------------------
#include "DebugRoutines.h"

#ifdef VITA
#include <SDL2/SDL_mixer.h>
#elif defined WITH_FMOD
#include "../../Dependencies/include/fmod.h"
#else
#define FSOUND_FREE 0
#endif

#include "../../include/JSoundSystem.h"
#include "../../include/JFileSystem.h"

//////////////////////////////////////////////////////////////////////////
JMusic::JMusic()
#ifdef VITA
  : mTrack(NULL)
#elif defined USE_PHONON
  : mOutput(0), mMediaObject(0)
#elif defined QT_CONFIG
    : playlist(0), player(0)
#endif
{
}

void JMusic::Update(){

}

int JMusic::getPlayTime(){
#ifdef VITA
  return 0; // SDL_mixer doesn't provide precise position
#elif defined WITH_FMOD
  return static_cast<int>(FSOUND_GetCurrentPosition(JSoundSystem::GetInstance()->mChannel) / 44.1); //todo more generic, here it's only 44kHz
#else
  return 0;
#endif
}

JMusic::~JMusic()
{
#if defined VITA
  JSoundSystem::GetInstance()->StopMusic(this);
  if (mTrack) Mix_FreeMusic(mTrack);
#elif defined USE_PHONON
  if(mOutput)
    delete mOutput;
  if(mMediaObject)
    delete mMediaObject;
#elif defined QT_CONFIG
    if(player)
        delete player;
    if(playlist)
        delete playlist;
#elif defined WITH_FMOD
  JSoundSystem::GetInstance()->StopMusic(this);
  if (mTrack) FSOUND_Sample_Free(mTrack);
#endif
}

#ifdef USE_PHONON
void JMusic::seekAtTheBegining()
{
  mMediaObject->seek(0);
}
#endif

//////////////////////////////////////////////////////////////////////////
JSample::JSample()
#ifdef USE_PHONON
  : mOutput(0), mMediaObject(0)
#endif
{

}

JSample::~JSample()
{
#if defined VITA
  if (mSample) Mix_FreeChunk(mSample);
#elif (defined QT_CONFIG) && (!defined USE_PHONON)
    if(effect)
        delete effect;
#elif USE_PHONON
  if(mOutput)
    delete mOutput;
  if(mMediaObject)
    delete mMediaObject;
#elif (defined WITH_FMOD)
  if (mSample) FSOUND_Sample_Free(mSample);
#endif
}

unsigned long JSample::fileSize()
{
#ifdef VITA
  return mSample ? mSample->alen : 0;
#elif defined WITH_FMOD
  return FSOUND_Sample_GetLength(mSample);
#else
  return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
JSoundSystem* JSoundSystem::mInstance = NULL;

JSoundSystem* JSoundSystem::GetInstance()
{
  if (mInstance == NULL)
    {
      mInstance = new JSoundSystem();
      mInstance->InitSoundSystem();
    }
  return mInstance;
}


void JSoundSystem::Destroy()
{
  if (mInstance)
    {
      mInstance->DestroySoundSystem();
      delete mInstance;
      mInstance = NULL;
    }
}


JSoundSystem::JSoundSystem()
{
  mVolume = 0;
  mSampleVolume = 0;
#ifdef VITA
  mCurrentMusic = NULL;
  mCurrentSample = NULL;
#elif defined WITH_FMOD
  mChannel = FSOUND_FREE;
#endif
}

JSoundSystem::~JSoundSystem()
{
}

void JSoundSystem::InitSoundSystem()
{
#ifdef VITA
  // Mix_OpenAudio without a prior Mix_Init leaves the codec dispatchers
  // unloaded, so Mix_LoadMUS silently fails on every MP3 (no music) while
  // WAV samples still work because that decoder is built into SDL2_mixer.
  // Load every format we have a vdpm library for: mpg123 (MP3), vorbis+ogg
  // (OGG), libmikmod (MOD), libFLAC (FLAC).
  int wanted = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_FLAC;
  int got = Mix_Init(wanted);
  if ((got & wanted) != wanted)
  {
      DebugTrace("Mix_Init partial: wanted=" << wanted << " got=" << got
                 << " err=" << Mix_GetError());
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
  {
      DebugTrace("SDL_mixer init failed: " << Mix_GetError());
  }
#elif defined WITH_FMOD
  FSOUND_Init(44100, 32, 0);
#endif
}


void JSoundSystem::DestroySoundSystem()
{
#ifdef VITA
  Mix_CloseAudio();
  Mix_Quit();
#elif defined WITH_FMOD
  FSOUND_Close();
#endif
}


JMusic *JSoundSystem::LoadMusic(const char *fileName)
{
    JMusic* music = NULL;
#if defined VITA
    music = new JMusic();
    if (music)
    {
        string fullpath = JFileSystem::GetInstance()->GetResourceFile(fileName);
        if (!fullpath.empty())
        {
            music->mTrack = Mix_LoadMUS(fullpath.c_str());
            if (!music->mTrack)
            {
                DebugTrace("Mix_LoadMUS failed: " << Mix_GetError());
            }
        }
    }
#elif (defined QT_CONFIG)  && (!defined USE_PHONON)
    music = new JMusic();
    if (music)
    {
        music->player = new QMediaPlayer;
        music->player->setVolume(100);
        music->playlist = new QMediaPlaylist;
        music->fullpath = JFileSystem::GetInstance()->GetResourceFile(fileName);
        music->playlist->addMedia(QUrl::fromLocalFile(music->fullpath.c_str()));
        music->playlist->setCurrentIndex(0);
    }
#elif defined USE_PHONON
    music = new JMusic();
    if (music)
    {
        music->mOutput = new Phonon::AudioOutput(Phonon::GameCategory, 0);
        music->mMediaObject = new Phonon::MediaObject(0);
        string fullpath = JFileSystem::GetInstance()->GetResourceFile(fileName);
        music->mMediaObject->setCurrentSource(QString(fullpath.c_str()));
        Phonon::Path mediapath = Phonon::createPath(music->mMediaObject, music->mOutput);
        Q_ASSERT(mediapath.isValid());
    }
#elif (defined WITH_FMOD)
    music = new JMusic();
    if (music)
    {
        JFileSystem* fileSystem = JFileSystem::GetInstance();
        if (fileSystem->OpenFile(fileName))
        {
            int size = fileSystem->GetFileSize();
            char *buffer = new char[size];
            fileSystem->ReadFile(buffer, size);
            music->mTrack = FSOUND_Sample_Load(FSOUND_UNMANAGED, buffer, FSOUND_LOADMEMORY, 0, size);

            delete[] buffer;
            fileSystem->CloseFile();
        }
    }
#else
    cerr << fileName << endl;
#endif
    return music;
}


void JSoundSystem::PlayMusic(JMusic *music, bool looping)
{
#if defined VITA
    if (music && music->mTrack)
    {
        Mix_PlayMusic(music->mTrack, looping ? -1 : 0);
        Mix_VolumeMusic((mVolume * 128 / 100)); // 0-100 -> 0-128
        mCurrentMusic = music;
    }
#elif (defined QT_CONFIG)  && (!defined USE_PHONON)
    if(music && music->player && music->playlist)
    {
        if(looping)
            music->playlist->setPlaybackMode(QMediaPlaylist::Loop);

        music->player->setPlaylist(music->playlist);
        music->player->play();
    }
#elif USE_PHONON
    if (music && music->mMediaObject && music->mOutput)
    {
        if(looping)
        {
            music->mMediaObject->connect(music->mMediaObject, SIGNAL(aboutToFinish()), music, SLOT(seekAtTheBegining()));
        }
        music->mOutput->setVolume((qreal)mVolume*0.01);
        music->mMediaObject->play();

    }
#elif (defined WITH_FMOD)
    if (music && music->mTrack)
    {
        mChannel = FSOUND_PlaySound(mChannel, music->mTrack);
        SetMusicVolume(mVolume);

        if (looping)
            FSOUND_SetLoopMode(mChannel, FSOUND_LOOP_NORMAL);
        else
            FSOUND_SetLoopMode(mChannel, FSOUND_LOOP_OFF);
    }
#else
    music = 0;
    looping = false;
#endif
}


void JSoundSystem::StopMusic(JMusic *music)
{
#if defined VITA
    if (music && music->mTrack)
    {
        Mix_HaltMusic();
    }
#elif (defined QT_CONFIG) && (!defined USE_PHONON)
    if (music && music->player && music->playlist)
    {
        music->player->stop();
    }
#elif defined USE_PHONON
  if (music && music->mMediaObject && music->mOutput)
  {
    music->mMediaObject->stop();
  }
#elif (defined WITH_FMOD)
  FSOUND_StopSound(mChannel);
#else
    music = 0;
#endif
}


void JSoundSystem::PauseMusic(JMusic *music)
{
#if defined VITA
    if (music && music->mTrack)
        Mix_PauseMusic();
#else
    StopMusic(music);
#endif
}


void JSoundSystem::ResumeMusic(JMusic *music)
{
#if defined VITA
    if (music && music->mTrack)
        Mix_ResumeMusic();
#else
    PlayMusic(music);
#endif
}


void JSoundSystem::SetVolume(int volume)
{
  SetMusicVolume(volume);
  SetSfxVolume(volume);
}

void JSoundSystem::SetMusicVolume(int volume)
{
#ifdef VITA
  Mix_VolumeMusic((volume * 128 / 100)); // 0-100 -> 0-128
#elif defined WITH_FMOD
  if (mChannel != FSOUND_FREE) FSOUND_SetVolumeAbsolute(mChannel, static_cast<int>(volume * 2.55));
#endif
  mVolume = volume;
}

void JSoundSystem::SetSfxVolume(int volume){
#ifdef VITA
  // SDL_mixer channel volume applies to next PlaySample call
#elif defined WITH_FMOD
  FSOUND_SetVolumeAbsolute(FSOUND_ALL, static_cast<int>(volume * 2.55));
#endif
  mSampleVolume = volume;
  SetMusicVolume(mVolume);
}

JSample *JSoundSystem::LoadSample(const char *fileName)
{
    JSample* sample = NULL;
#if defined VITA
    sample = new JSample();
    if (sample)
    {
        string fullpath = JFileSystem::GetInstance()->GetResourceFile(fileName);
        if (!fullpath.empty())
        {
            sample->mSample = Mix_LoadWAV(fullpath.c_str());
            if (!sample->mSample)
            {
                DebugTrace("Mix_LoadWAV failed: " << Mix_GetError());
            }
        }
        else
            sample->mSample = NULL;
    }
#elif (defined QT_CONFIG) && (!defined USE_PHONON)
    sample = new JSample();
    if (sample)
    {
        string fullpath = JFileSystem::GetInstance()->GetResourceFile(fileName);
        sample->effect = new QMediaPlayer;
        sample->effect->setMedia(QUrl::fromLocalFile(fullpath.c_str()));
        sample->effect->setVolume(100);
        sample->mSample = &(sample->effect);
    }
#elif (defined USE_PHONON)
    sample = new JSample();
    if (sample)
    {
        sample->mOutput = new Phonon::AudioOutput(Phonon::GameCategory, 0);
        sample->mMediaObject = new Phonon::MediaObject(0);
        string fullpath = JFileSystem::GetInstance()->GetResourceFile(fileName);
        sample->mMediaObject->setCurrentSource(QString(fullpath.c_str()));
        Phonon::Path mediapath = Phonon::createPath(sample->mMediaObject, sample->mOutput);
        Q_ASSERT(mediapath.isValid());
    }
#elif (defined WITH_FMOD)
    sample = new JSample();
    if (sample)
    {
        JFileSystem* fileSystem = JFileSystem::GetInstance();
        if (fileSystem->OpenFile(fileName))
        {
            int size = fileSystem->GetFileSize();
            char *buffer = new char[size];
            fileSystem->ReadFile(buffer, size);
            sample->mSample = FSOUND_Sample_Load(FSOUND_UNMANAGED, buffer, FSOUND_LOADMEMORY, 0, size);

            delete[] buffer;
            fileSystem->CloseFile();
        }else
            sample->mSample = NULL;

    }
#else
    cerr << fileName << endl;
#endif
    return sample;
}


void JSoundSystem::PlaySample(JSample *sample)
{
#if defined VITA
    if (sample && sample->mSample)
    {
        int channel = Mix_PlayChannel(-1, sample->mSample, 0);
        if (channel >= 0)
            Mix_Volume(channel, (mSampleVolume * 128 / 100));
    }
#elif (defined QT_CONFIG) && (!defined USE_PHONON)
    if(sample)
    {
        sample->effect->play();
    }
#elif defined USE_PHONON
  if (sample && sample->mMediaObject && sample->mOutput)
  {
    sample->mOutput->setVolume((qreal)mSampleVolume*0.01);
    sample->mMediaObject->play();
  }
#elif (defined WITH_FMOD)
  if (sample && sample->mSample){
    int channel = FSOUND_PlaySound(FSOUND_FREE, sample->mSample);
    FSOUND_SetVolumeAbsolute(channel, static_cast<int>(mSampleVolume * 2.55));
  }
#else
    sample = 0;
#endif
}

