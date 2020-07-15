#pragma once

#include <SFML/Audio.hpp>
#include "UIButton.h"

#include <string>
#include <unordered_map>
#include <memory>
#include "UISettingsSession.h"

class SoundSystem
{

public:
	enum class SoundType
	{
		ButtonSFX,
		TokenSFX,
		BuyCardSFX,
		HoldCardSFX,
		WinNobleSFX,
		OverSFX,
		CheckBoxSFX,
		WrongSFX
	};
	static inline const uint16_t s_soundTypeSize = 8;

	enum class MusicType
	{
		MenuMusic,
		GameMusic
	};
	static inline const uint16_t s_musicTypeSize = 2;

public:
	static void StopMusic(MusicType musicType);
	static void PauseMusic();
	static void PlayMusic();
	static void PlayMusic(MusicType musicType);
	static void PlaySFX(SoundType soundType);
	static void LoadFromFile();
	static uint16_t GetSFXVolume();
	static uint16_t GetMusicVolume();
	static void SetMusicVolume(uint16_t musicVolume);
	static void SetSFXVolume(uint16_t sfxVolume);

private:
	static void AddMusic(MusicType musicType, std::unique_ptr<sf::Music> value);
	static void AddSound(SoundType soundType, std::unique_ptr<sf::SoundBuffer> value);
	static sf::Music& GetMusic(MusicType musicType);
	static sf::SoundBuffer& GetSound(SoundType soundType);

private:
	const inline static std::string s_musicFile = "../external/Resources/sounds/";
	static inline std::unordered_map <MusicType, std::unique_ptr<sf::Music>> s_musics;
	static inline std::unordered_map <SoundType, std::unique_ptr<sf::SoundBuffer>> s_sounds;
	static inline bool alreadyLoaded = false;
	static inline MusicType s_currentMusicType;
	static inline std::unique_ptr<sf::SoundBuffer> s_buffer;
	static inline std::unique_ptr<sf::Sound> s_activeSound;
	static inline uint16_t s_musicVolume = 200;
	static inline uint16_t s_sfxVolume = 100;
};

