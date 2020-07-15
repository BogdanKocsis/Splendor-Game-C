#pragma once

// UI
#include "UIText.h"
#include "UIButton.h"
#include "UIOptionsPanel.h"
#include "UISelectorBox.h"
#include "Collider.h"
#include "UIColors.h"


class UITutorialSession : public sf::Drawable
{

public:
	enum class Events
	{
		None,
		MainMenu,
		Next,
		Previous

	};

public:
	UITutorialSession(const sf::Vector2u& windowSize);
	~UITutorialSession();

	static inline std::vector<sf::Sprite> s_tutorialImages;
	static inline int s_currentSprite=0 ;

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

	void PassEvent(const sf::Event& event);
	Events GetEvent() const;


	static void LoadFromFile();
	static sf::Sprite& GetSprite();
	static void IncrementSprite();
	static void DecrementSprite();

private:
	const inline static std::string s_imageFile = "../external/Resources/Textures/Tutorial1.2/";
	static inline const uint16_t s_imagesSize = 6;
	UIText m_title;
	sf::RectangleShape m_titleBackground;
	UIOptionsPanel m_gameModePanel;
	UIButton* m_nextButton;
	UIButton* m_previousButton;
	UIButton* m_backToMenuButton;
	static inline sf::Texture* s_texture;
	static inline sf::Sprite* s_sprite;;

};

