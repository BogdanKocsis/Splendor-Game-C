#include "UIText.h"

std::vector<sf::Font> UIText::s_availableFonts;

UIText::UIText(const sf::Vector2f& position, TextAlign align, AvailableFonts font, const std::string& text, uint16_t fontSize, sf::Color fontFill, sf::Color fontOutline, float outlineThickness)
{
	AlignText(align);
	setPosition(position);

	LoadFonts();
	SetFont(font);
	setString(text);
	setCharacterSize(fontSize);
	setFillColor(fontFill);
	setOutlineColor(fontOutline);
	setOutlineThickness(outlineThickness);
}

void UIText::SetFont(AvailableFonts font)
{
	setFont(s_availableFonts[static_cast<int>(font)]);
}

void UIText::LoadFonts()
{
	if (s_availableFonts.size() != s_fontsCount)
	{
		s_availableFonts.resize(s_fontsCount);
		size_t fontIndex = 0;
		for (auto& font : s_availableFonts)
		{
			switch (static_cast<AvailableFonts>(fontIndex++))
			{
			case AvailableFonts::DosisLight:
				font.loadFromFile(s_fontsFile + "Dosis-Light.ttf");
				break;
			case AvailableFonts::DosisBold:
				font.loadFromFile(s_fontsFile + "Dosis-Bold.ttf");
				break;
			case AvailableFonts::PixellettersFull:
				font.loadFromFile(s_fontsFile + "PixellettersFull.ttf");
				break;
			case AvailableFonts::Splendor:
				font.loadFromFile(s_fontsFile + "Splendor.ttf");
				break;
			case AvailableFonts::BerlinBold:
				font.loadFromFile(s_fontsFile + "Berlin-Bold.ttf");
				break;
			case AvailableFonts::LatoBlack:
				font.loadFromFile(s_fontsFile + "Lato-Black.ttf");
				break;
			case AvailableFonts::LatoLight:
				font.loadFromFile(s_fontsFile + "Lato-Light.ttf");
				break;
			case AvailableFonts::LatoMediumItalic:
				font.loadFromFile(s_fontsFile + "Lato-MediumItalic.ttf");
				break;
			default:
				font.loadFromFile(s_fontsFile + "Dosis-Light.ttf");
				break;
			}
		}
	}
}

void UIText::AlignText(TextAlign align)
{
	const sf::FloatRect alignRect = getLocalBounds();
	switch (align) {
	case TextAlign::classic:
		break;
	case TextAlign::mid_center:
		setOrigin(alignRect.left + alignRect.width / 2.0f, alignRect.top + alignRect.height / 2.0f);
		break;
	case TextAlign::mid_left:
		setOrigin(alignRect.left, alignRect.top + alignRect.height / 2.0f);
		break;
	case TextAlign::mid_right:
		setOrigin(alignRect.left + alignRect.width, alignRect.top + alignRect.height / 2.0f);
		break;
	default:
		break;
	}
}

void UIText::PositionText(const sf::RectangleShape& alignShape)
{
	const sf::Vector2f position = alignShape.getPosition();
	const sf::Vector2f size = alignShape.getSize();
	const sf::Vector2f midPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
	setPosition(midPosition);
}
