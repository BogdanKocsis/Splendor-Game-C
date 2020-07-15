#include "UISettingsSession.h"

#define  soundTypeSize 8;
static inline const uint16_t musicTypeSize = 2;

UISettingsSession::UISettingsSession(const sf::Vector2u& windowSize) :
	// Game Mode Panel
	m_musicPanel("Music", UIOptionsPanel::Type::Check,
		sf::Vector2f(50.0f, 0.2f * windowSize.y + 30.0f),
		sf::Vector2f(windowSize.x - 50.0f, 0.15f * windowSize.y)),
	// Players Panel
	m_sfxPanel("SFX", UIOptionsPanel::Type::Check,
		sf::Vector2f(50.0f, 0.4f * windowSize.y + 50.0f),
		sf::Vector2f(windowSize.x - 50.0f, 0.15f * windowSize.y))
{
	// Title Background
	m_titleBackground.setPosition(0, 0);
	m_titleBackground.setSize(sf::Vector2f(windowSize.x, 0.2f * windowSize.y));
	m_titleBackground.setFillColor(UIColors::DarkYellow);

	// Title
	m_title.setString("Settings");
	m_title.setCharacterSize(80);
	m_title.setFillColor(UIColors::NeutralWhite);
	m_title.setPosition(windowSize.x / 2.0f, 0.2f * windowSize.y / 2.0f);
	m_title.AlignText(UIText::TextAlign::mid_center);


	//Music Panel
	m_musicPanel.AddOption("Sound On");
	m_musicPanel.AddOption("Mute");

	// SFX Panel

	m_sfxPanel.AddOption("Sound On");
	m_sfxPanel.AddOption("Mute");




	//Button
	sf::RectangleShape button({ 300,80 });

	// Main Menu Button
	button.setPosition(windowSize.x - 670.0f, windowSize.y - 130.0f);
	m_mainMenuButton = new UIButton(button);
	m_mainMenuButton->ChangeText("Main Menu");
}

UISettingsSession::~UISettingsSession()
{
	delete m_mainMenuButton;
}

void UISettingsSession::PassEvent(const sf::Event& event)
{
	m_musicPanel.HandleEvent(event);
	m_sfxPanel.HandleEvent(event);
	m_mainMenuButton->HandleEvent(event);
}

UISettingsSession::Events UISettingsSession::GetEvent() const
{

	if (m_mainMenuButton->GetState() == UIButton::State::Release)
	{
		return Events::MainMenu;
	}
	return Events::None;
}

void UISettingsSession::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_titleBackground);
	target.draw(m_title);
	target.draw(m_musicPanel);
	target.draw(m_sfxPanel);
	target.draw(*m_mainMenuButton);
}

void UISettingsSession::UpdateSound()
{

	std::string soundModeOptionString = m_musicPanel.FirstChecked();
	std::string sfxModeOptionString = m_sfxPanel.FirstChecked();

	if (soundModeOptionString == "Mute")
	{
		activeSound = false;
		SoundSystem::PauseMusic();
	}

	if (soundModeOptionString == "Sound On")
	{
		activeSound = true;
		SoundSystem::PlayMusic();
	}


	if (sfxModeOptionString == "Mute")
	{
		activeSFX = false;

	}

	if (sfxModeOptionString == "Sound On")
	{
		activeSFX = true;

	}

}

bool UISettingsSession::GetActiveSound()
{
	return activeSound;
}

bool UISettingsSession::GetActiveSFX()
{
	return activeSFX;
}