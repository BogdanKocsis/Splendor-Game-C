#include "UIMainMenuSession.h"

UIMainMenuSession::UIMainMenuSession(const sf::Vector2u& windowSize) :
	m_mainMenu(sf::Vector2f(400, windowSize.y)),
	m_exitMenu(sf::Vector2f(800, 200)),
	m_exitMenuTriggered(false)
{
	// Background
	m_backgroundTexture.loadFromFile(s_backgroundTexture);
	m_background.setTexture(m_backgroundTexture);
	m_background.setScale(windowSize.x / m_background.getLocalBounds().width, windowSize.y / m_background.getLocalBounds().height);

	// Main Menu
	m_mainMenu.setPosition(windowSize.x - m_mainMenu.getSize().x, windowSize.y - m_mainMenu.getSize().y);
	const auto mainMenuPosition = m_mainMenu.getPosition();
	const auto mainMenuSize = m_mainMenu.getSize();
	m_mainMenu.setFillColor(UIColors::Transparent);

	// Exit Menu
	auto exitMenuPosition = m_exitMenu.getPosition();
	const auto exitMenuSize = m_exitMenu.getSize();
	m_exitMenu.setOrigin(exitMenuSize.x / 2.0f, exitMenuSize.y / 2.0f);
	m_exitMenu.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	exitMenuPosition = m_exitMenu.getPosition();
	m_exitMenu.setFillColor(UIColors::DarkBlue);
	m_exitMenu.setOutlineColor(UIColors::GoldYellow);
	m_exitMenu.setOutlineThickness(5.0f);

	// Main Menu Text
	m_mainMenuText = new UIText(sf::Vector2f(mainMenuPosition.x + mainMenuSize.x / 2, mainMenuPosition.y + 75), UIText::TextAlign::mid_center, UIText::AvailableFonts::DosisLight, "Main Menu", 60);
	m_mainMenuText->setFillColor(UIColors::GoldYellow);
	m_mainMenuText->setOutlineThickness(3.0f);
	m_mainMenuText->AlignText(UIText::TextAlign::mid_center);

	// Exit Menu Text
	m_exitMenuText = new UIText(sf::Vector2f(exitMenuPosition.x, exitMenuPosition.y - 50), UIText::TextAlign::mid_center, UIText::AvailableFonts::DosisBold, "Are you sure you want to exit Splendor Game?", 35);
	m_exitMenuText->setFillColor(UIColors::GoldYellow);
	m_exitMenuText->setOutlineThickness(0.0f);
	m_exitMenuText->AlignText(UIText::TextAlign::mid_center);

	// Button Shapes
	sf::RectangleShape mediumButtonShape(sf::Vector2f(300, 80));
	sf::RectangleShape smallButtonShape(sf::Vector2f(100, 50));

	// Main Menu Buttons
	mediumButtonShape.setPosition(sf::Vector2f(mainMenuPosition.x + 50, mainMenuPosition.y + 130));
	m_buttons[Buttons::NewGame] = new UIButton(mediumButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::NewGame]->ChangeText("New Game");
	
	mediumButtonShape.setPosition(sf::Vector2f(mainMenuPosition.x + 50, mainMenuPosition.y + 230));
	m_buttons[Buttons::Tutorial] = new UIButton(mediumButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::Tutorial]->ChangeText("Tutorial");
	
	mediumButtonShape.setPosition(sf::Vector2f(mainMenuPosition.x + 50, mainMenuPosition.y + 330));
	m_buttons[Buttons::Settings] = new UIButton(mediumButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::Settings]->ChangeText("Settings");
	
	mediumButtonShape.setPosition(sf::Vector2f(mainMenuPosition.x + 50, mainMenuPosition.y + 430));
	m_buttons[Buttons::Leaderboard] = new UIButton(mediumButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::Leaderboard]->ChangeText("Leaderboard");
	
	mediumButtonShape.setPosition(sf::Vector2f(mainMenuPosition.x + 50, mainMenuPosition.y + 530));
	m_buttons[Buttons::Exit] = new UIButton(mediumButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::Exit]->ChangeText("Exit Game");
	
	mediumButtonShape.setPosition(sf::Vector2f(mainMenuPosition.x + 50, mainMenuPosition.y + 630));
	m_buttons[Buttons::Test] = new UIButton(mediumButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::Test]->ChangeText("Test Session");

	// Exit Menu Buttons
	smallButtonShape.setPosition(sf::Vector2f(exitMenuPosition.x - smallButtonShape.getSize().x - 50, exitMenuPosition.y + 20));
	m_buttons[Buttons::ExitYes] = new UIButton(smallButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::ExitYes]->ChangeText("Yes");
	smallButtonShape.setPosition(sf::Vector2f(exitMenuPosition.x + 50, exitMenuPosition.y + 20));
	m_buttons[Buttons::ExitNo] = new UIButton(smallButtonShape, s_buttonNoneDesign, s_buttonHoverDesign, s_buttonPressDesign);
	m_buttons[Buttons::ExitNo]->ChangeText("No");

	// Drawable Main Menu Vector
	m_mainMenuDrawableVector.push_back(&m_background);
	m_mainMenuDrawableVector.push_back(&m_mainMenu);
	m_mainMenuDrawableVector.push_back(m_mainMenuText);
	m_mainMenuDrawableVector.push_back(m_buttons[Buttons::NewGame]);
	m_mainMenuDrawableVector.push_back(m_buttons[Buttons::Tutorial]);
	m_mainMenuDrawableVector.push_back(m_buttons[Buttons::Settings]);
	m_mainMenuDrawableVector.push_back(m_buttons[Buttons::Exit]);
	//m_mainMenuDrawableVector.push_back(m_buttons[Buttons::Test]);
	m_mainMenuDrawableVector.push_back(m_buttons[Buttons::Leaderboard]);
	// Drawable Exit Menu Vector
	m_exitMenuDrawableVector.push_back(&m_exitMenu);
	m_exitMenuDrawableVector.push_back(m_exitMenuText);
	m_exitMenuDrawableVector.push_back(m_buttons[Buttons::ExitNo]);
	m_exitMenuDrawableVector.push_back(m_buttons[Buttons::ExitYes]);

	// Collider Main Menu Vector
	m_mainMenuColliderVector.push_back(m_buttons[Buttons::NewGame]);
	m_mainMenuColliderVector.push_back(m_buttons[Buttons::Tutorial]);
	m_mainMenuColliderVector.push_back(m_buttons[Buttons::Settings]);
	m_mainMenuColliderVector.push_back(m_buttons[Buttons::Exit]);
	m_mainMenuColliderVector.push_back(m_buttons[Buttons::Test]);
	m_mainMenuColliderVector.push_back(m_buttons[Buttons::Leaderboard]);
	// Collider Exit Menu Vector
	m_exitMenuColliderVector.push_back(m_buttons[Buttons::ExitNo]);
	m_exitMenuColliderVector.push_back(m_buttons[Buttons::ExitYes]);
}

UIMainMenuSession::~UIMainMenuSession()
{
	delete m_mainMenuText;
	delete m_exitMenuText;
}

void UIMainMenuSession::PassEvent(const sf::Event& event)
{
	if (m_exitMenuTriggered)
	{
		for (const auto& collider : m_exitMenuColliderVector)
			collider->HandleEvent(event);
	}
	else
	{
		for (const auto& collider : m_mainMenuColliderVector)
			collider->HandleEvent(event);
	}
}

UIMainMenuSession::Events UIMainMenuSession::GetEvent()
{
	if (m_buttons[Buttons::NewGame]->GetState() == UIButton::State::Release)
	{
		m_buttons[Buttons::NewGame]->SwitchState(UIButton::State::None);
		return Events::NewGame;
	}
	if (m_buttons[Buttons::Tutorial]->GetState() == UIButton::State::Release)
	{
		m_buttons[Buttons::Tutorial]->SwitchState(UIButton::State::None);
		return Events::Tutorial;
	}
	if (m_buttons[Buttons::Settings]->GetState() == UIButton::State::Release)
	{
		m_buttons[Buttons::Settings]->SwitchState(UIButton::State::None);
		return Events::Settings;
	}
	if (m_buttons[Buttons::Exit]->GetState() == UIButton::State::Release)
	{
		m_exitMenuTriggered = true;
	}
	if (m_buttons[Buttons::Test]->GetState() == UIButton::State::Release)
	{
		m_buttons[Buttons::Test]->SwitchState(UIButton::State::None);
		return Events::Test;
	}
	if (m_buttons[Buttons::ExitNo]->GetState() == UIButton::State::Release)
	{
		m_exitMenuTriggered = false;
		m_buttons[Buttons::Exit]->SwitchState(UIButton::State::None);
		m_buttons[Buttons::ExitNo]->SwitchState(UIButton::State::None);
	}
	if (m_buttons[Buttons::ExitYes]->GetState() == UIButton::State::Release)
	{
		return Events::Exit;
	}
	if (m_buttons[Buttons::Leaderboard]->GetState() == UIButton::State::Release)
	{
		m_buttons[Buttons::Leaderboard]->SwitchState(UIButton::State::None);
		return Events::Leaderboard;
	}
	return Events::None;
}

void UIMainMenuSession::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& drawable : m_mainMenuDrawableVector)
	{
		target.draw(*drawable);
	}
	if (m_exitMenuTriggered)
	{
		for (const auto& drawable : m_exitMenuDrawableVector)
		{
			target.draw(*drawable);
		}
	}
}
