#pragma once
#include "RectCollider.h"
#include "UISelector.h"
#include "UIColors.h"

class UISelectorBox :public UISelector, public RectCollider, public sf::Drawable
{
public:
	struct Design
	{
		sf::Color fillColor;
		sf::Color outlineColor;
	};
	enum class Type
	{
		Check,
		Radio
	};

public:
	UISelectorBox(const std::string& title, Type type = Type::Check, const sf::Vector2f& position = sf::Vector2f(0, 0), const sf::Vector2f& size = sf::Vector2f(100, 100), const Design& uncheckedDesign = Design{ UIColors::DarkBlue,UIColors::GoldYellow }, const Design& checkedDesign = Design{ UIColors::GoldYellow,sf::Color(255,255,255) });

	void SetState(bool state) override;
	void ChangeState() override;
	void OnMouseLeftClick() override;

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

private:
	void UpdateDesign();

private:
	const inline static std::string s_checkMarkFilePath = "../external/Resources/Textures/UI/checkmark.png";
	const inline static std::string s_radioMarkFilePath = "../external/Resources/Textures/UI/radiomark.png";

private:
	sf::RectangleShape m_box;
	Type m_type;
	Design m_uncheckedDesign, m_checkedDesign;
	Design* m_currentDesign;
	sf::Texture* m_checkTexture;
	sf::Sprite m_checkSprite;
};

