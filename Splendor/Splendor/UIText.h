#pragma once
#include <SFML/Graphics.hpp>

class UIText :public sf::Text
{
public:
	enum class AvailableFonts
	{
		DosisLight,
		DosisBold,
		PixellettersFull,
		Splendor,
		BerlinBold,
		LatoBlack,
		LatoLight,
		LatoMediumItalic
	};
	enum class TextAlign
	{
		classic,
		mid_center,
		mid_left,
		mid_right
	};

private:
	static std::vector<sf::Font> s_availableFonts;
	const inline static std::string s_fontsFile = "../external/Resources/Fonts/";
	const inline static size_t s_fontsCount = 8;

public:
	// Constr.
	UIText(const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f), TextAlign align = TextAlign::classic, AvailableFonts font = AvailableFonts::DosisLight, const std::string& text = "UIText Instance", uint16_t fontSize = 30, sf::Color fontFill = sf::Color(255, 255, 255), sf::Color fontOutline = sf::Color(0, 0, 0), float outlineThickness = 1.0f);

	// G&S
	void SetFont(AvailableFonts font);

	// Manipulation
	void AlignText(TextAlign align);
	void PositionText(const sf::RectangleShape& alignShape);

private:
	// Preparation
	static void LoadFonts();
};
