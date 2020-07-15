#include "SoundSystem.h"

void SoundSystem::AddMusic(MusicType musicType, std::unique_ptr<sf::Music> value)
{
	s_musics.insert(std::make_pair(musicType, std::move(value)));
}

void SoundSystem::AddSound(SoundType soundType, std::unique_ptr<sf::SoundBuffer> value)
{
	s_sounds.insert(std::make_pair(soundType, std::move(value)));
}

sf::Music& SoundSystem::GetMusic(MusicType musicType)
{
	return *s_musics.at(musicType);
}

sf::SoundBuffer& SoundSystem::GetSound(SoundType soundType)
{
	return *s_sounds.at(soundType);
}


void SoundSystem::StopMusic(MusicType musicType)
{
	GetMusic(musicType).pause();
}

void SoundSystem::PauseMusic()
{
	for (uint16_t musicIt = 0; musicIt < s_musicTypeSize; ++musicIt)
	{
		GetMusic(static_cast<MusicType>(musicIt)).pause();
	}

}

void SoundSystem::PlayMusic()
{

	GetMusic(MusicType::MenuMusic).play();

}


void SoundSystem::PlayMusic(MusicType musicType)
{

	if (UISettingsSession::activeSound)
	{
		s_currentMusicType = musicType;

		GetMusic(musicType).setVolume(std::clamp<uint16_t>(s_musicVolume, 0, 100));
		GetMusic(musicType).setLoop(true);
		GetMusic(musicType).play();
	}

}

void SoundSystem::PlaySFX(SoundType soundType)
{
	if (UISettingsSession::activeSFX)
	{
		s_activeSound = std::make_unique<sf::Sound>();
		s_buffer = std::make_unique<sf::SoundBuffer>(GetSound(soundType));
		s_activeSound->setBuffer(*s_buffer);
		s_activeSound->setVolume(std::clamp<uint16_t>(s_sfxVolume, 0, 100));
		s_activeSound->play();
	}
}

void SoundSystem::LoadFromFile()
{
	if (alreadyLoaded)
		return;

	for (uint16_t musicIt = 0; musicIt < s_musicTypeSize; ++musicIt)
	{
		auto music = std::unique_ptr<sf::Music>(new sf::Music());
		auto path = s_musicFile + "m_" + std::to_string(static_cast<int>(static_cast<MusicType>(musicIt))) + ".ogg";
		if (!music->openFromFile(path))
			throw "Music file could not be opened";
		AddMusic(static_cast<MusicType>(musicIt), std::move(music));
	}

	for (uint16_t soundIt = 0; soundIt < s_soundTypeSize; ++soundIt)
	{
		auto sound = std::make_unique<sf::SoundBuffer>(sf::SoundBuffer());
		auto path = s_musicFile + "s_" + std::to_string(static_cast<int>(static_cast<SoundType>(soundIt))) + ".wav";
		if (!sound->loadFromFile(path))
			throw "Sound effect file could not be opened";
		AddSound(static_cast<SoundType>(soundIt), std::move(sound));
	}

	alreadyLoaded = true;
}

uint16_t SoundSystem::GetSFXVolume()
{
	return s_sfxVolume;
}

uint16_t SoundSystem::GetMusicVolume()
{
	return s_musicVolume;
}

void SoundSystem::SetMusicVolume(uint16_t musicVolume)
{
	s_musicVolume = musicVolume;

	GetMusic(s_currentMusicType).setVolume(s_musicVolume);
	//GetMusic(s_currentMusicType).play();
}

void SoundSystem::SetSFXVolume(uint16_t sfxVolume)
{
	s_sfxVolume = sfxVolume;
}
