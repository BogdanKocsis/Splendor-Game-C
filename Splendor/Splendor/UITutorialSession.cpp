#include "UITutorialSession.h"


UITutorialSession::UITutorialSession(const sf::Vector2u& windowSize) :
	m_gameModePanel("How To Play:", UIOptionsPanel::Type::Radio,
		sf::Vector2f(50.0f, 0.2f * windowSize.y + 30.0f),
		sf::Vector2f(windowSize.x - 50.0f, 0.15f * windowSize.y))
{
	// Title Background
	m_titleBackground.setPosition(0, 0);
	m_titleBackground.setSize(sf::Vector2f(windowSize.x, 0.2f * windowSize.y));
	m_titleBackground.setFillColor(UIColors::DarkYellow);

	// Title
	m_title.setString("Tutorial");
	m_title.setCharacterSize(80);
	m_title.setFillColor(UIColors::NeutralWhite);
	m_title.setPosition(windowSize.x / 2.0f, 0.2f * windowSize.y / 2.0f);
	m_title.AlignText(UIText::TextAlign::mid_center);

	//Button
	sf::RectangleShape button({ 300,80 });

	// Main Menu Button
	button.setPosition(windowSize.x - 1250.0f, windowSize.y - 130.0f);
	m_backToMenuButton = new UIButton(button);
	m_backToMenuButton->ChangeText("Main Menu");

	//Next Button
	button.setPosition(windowSize.x - 350.0f, windowSize.y - 130.0f);
	m_nextButton = new UIButton(button);
	m_nextButton->ChangeText("Next");

	// Previous Button
	button.setPosition(windowSize.x - 740.0f, windowSize.y - 130.0f);
	m_previousButton = new UIButton(button);
	m_previousButton->ChangeText("Previous");
}

UITutorialSession::~UITutorialSession()
{
	delete m_nextButton;
	delete m_backToMenuButton;
	delete m_previousButton;
}

void UITutorialSession::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_titleBackground);
	target.draw(m_title);
	target.draw(m_gameModePanel);
	target.draw(*m_nextButton);
	target.draw(*m_backToMenuButton);
	target.draw(*m_previousButton);
}

void UITutorialSession::PassEvent(const sf::Event& event)
{

	m_backToMenuButton->HandleEvent(event);
	m_nextButton->HandleEvent(event);
	m_previousButton->HandleEvent(event);
}

UITutorialSession::Events UITutorialSession::GetEvent() const
{
	if (m_backToMenuButton->GetState() == UIButton::State::Release)
	{
		return Events::MainMenu;
	}
	if (m_previousButton->GetState() == UIButton::State::Press)
	{

		return Events::Previous;
	}
	if (m_nextButton->GetState() == UIButton::State::Press)
	{

		return Events::Next;
	}
	return Events::None;
}

void UITutorialSession::LoadFromFile()
{

	for (uint16_t imageIt = 0; imageIt < s_imagesSize; ++imageIt)
	{
		auto path = s_imageFile + "Screenshot" + std::to_string(imageIt) + ".png";

		s_texture = new sf::Texture;
		if (!s_texture->loadFromFile(path))
		{
			throw "Screenshot file could not be loaded";
		}

		s_sprite = new sf::Sprite(*s_texture);
		s_sprite->setScale(sf::Vector2f(0.7, 0.7));
		s_sprite->setPosition(sf::Vector2f(890, 310));
		s_tutorialImages.push_back(*s_sprite);
	}

}

sf::Sprite& UITutorialSession::GetSprite()
{
	return s_tutorialImages.at(std::clamp<int>(s_currentSprite, 0, s_imagesSize - 1));
}

void UITutorialSession::IncrementSprite()
{

	++s_currentSprite;
	s_currentSprite = std::clamp<int>(s_currentSprite, 0, s_imagesSize);
}

void UITutorialSession::DecrementSprite()
{

	--s_currentSprite;
	s_currentSprite = std::clamp<int>(s_currentSprite, 0, s_imagesSize);
}
