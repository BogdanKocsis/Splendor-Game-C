#include "UICard.h"
#include "GamePieces.h"
#include "UIColors.h"
#include "UISelectedCard.h"
#include "SoundSystem.h"

#include <thread>

UICard::Data::Data(Type dataType, uint16_t dataID, bool isDataNumb) :type(dataType), id(dataID), isNumb(isDataNumb) {}

UICard::UICard(uint16_t id, Type type, const sf::Vector2f& position, const sf::Vector2f& size) :
	RectCollider(position, size),
	RectangleShape(size),
	m_initialPosition(position),
	m_numb(false),
	m_state(State::None),
	m_id(id),
	m_type(type),
	m_warning(false)
{
	// Initialize
	setPosition(position);
	if (!s_texturesLoaded)
	{
		LoadTextures();
		s_texturesLoaded = true;
	}

	// Set texture
	const sf::Texture* texture = GetTexture(id, type);
	if (texture != nullptr)
	{
		setTexture(texture);
	}

	// Animation
	setOutlineThickness(0.05f * size.x);
	setOutlineColor(UIColors::Transparent);
}

uint16_t UICard::GetID() const
{
	return m_id;
}

UICard::Type UICard::GetType() const
{
	return m_type;
}

UICard::Data UICard::GetData() const
{
	return Data(m_type, m_id, m_numb);
}

void UICard::SetData(Data data)
{
	const sf::Texture* texture = GetTexture(data.id, data.type);
	if (texture != nullptr)
	{
		m_id = data.id;
		m_type = data.type;
		m_numb = data.type == Type::Unknown ? true : data.isNumb;
		setTexture(texture);
	}
}

bool UICard::GetNumb() const
{
	return m_numb;
}

void UICard::SetNumb(bool numb)
{
	m_numb = numb;
}

UICard::State UICard::GetState() const
{
	return m_state;
}

void UICard::SetState(State state)
{
	m_state = state;
}

void UICard::TriggerWarning()
{
	m_warning = true;
	setOutlineColor(UIColors::WarningRed);
	SoundSystem::PlaySFX(SoundSystem::SoundType::WrongSFX);
}

void UICard::Deactivate()
{
	setFillColor(UIColors::Transparent);
	setOutlineColor(UIColors::Transparent);
	UISelectedCard::DisplayText(UISelectedCard::TextType::None);
	UISelectedCard::Set(nullptr);
}

void UICard::Activate()
{
	setFillColor(UIColors::OpaqueWhite);
}

void UICard::SetInHand(bool inHand)
{
	m_inHand = inHand;
}

void UICard::OnMouseOver()
{
	if (m_inHand) return;
	setPosition(sf::Mouse::getPosition().x - getSize().x, sf::Mouse::getPosition().y - getSize().y);
	UISelectedCard::UpdateText(getPosition());
}

void UICard::OnMouseEnter()
{
	if (!m_numb)
	{
		m_state = State::Hover;
		setOutlineColor(UIColors::GoldYellow - UIColors::QuarterTransparent);
		if (!m_inHand)
		{
			m_type == Type::Background ? UISelectedCard::DisplayText(UISelectedCard::TextType::Half) : UISelectedCard::DisplayText(UISelectedCard::TextType::Full);
		}
		else
		{
			// inHand set-up
			UISelectedCard::DisplayText(UISelectedCard::TextType::InHand);
			UISelectedCard::UpdateText(sf::Vector2f(getPosition().x + getSize().x, getPosition().y));
			return;
		}
	}
	
	SoundSystem::PlaySFX(SoundSystem::SoundType::OverSFX);
	
	if (m_inHand) return;

	setScale(2, 2);
	UISelectedCard::Set(dynamic_cast<sf::Drawable*>(this));
}

void UICard::OnMouseLeave()
{
	if (!m_numb)
	{
		m_state = State::None;
		setOutlineColor(UIColors::Transparent);
		UISelectedCard::DisplayText(UISelectedCard::TextType::None);
	}

	if (m_warning)
	{
		m_warning = false;
		setOutlineColor(UIColors::Transparent);
	}

	if (m_inHand) return;

	setScale(1, 1);
	setPosition(m_initialPosition);
	UISelectedCard::Set(nullptr);
}

void UICard::OnMouseLeftClick()
{
	if (!m_numb && m_type != Type::Background)
	{
		if (m_inHand)
		{
			m_state = State::Press;
			setOutlineColor(UIColors::DarkGreen - UIColors::QuarterTransparent);
			return;
		}
		m_state = State::Press;
		setScale(1.8f, 1.8f);
		setOutlineColor(UIColors::DarkGreen - UIColors::QuarterTransparent);
	}
}

void UICard::OnMouseLeftRelease()
{
	if (!m_numb && m_type != Type::Background)
	{
		if (m_inHand)
		{
			m_state = State::LeftRelease;
			setOutlineColor(UIColors::GoldYellow - UIColors::QuarterTransparent);
			return;
		}
		m_state = State::LeftRelease;
		setScale(2, 2);
		setOutlineColor(UIColors::GoldYellow - UIColors::QuarterTransparent);
	}
}

void UICard::OnMouseRightClick()
{
	if (m_inHand) return;

	if (!m_numb)
	{
		m_state = State::Press;
		setScale(1.8f, 1.8f);
		setOutlineColor(UIColors::DarkYellow - UIColors::QuarterTransparent);
	}
}

void UICard::OnMouseRightRelease()
{
	if (m_inHand) return;

	if (!m_numb)
	{
		m_state = State::RightRelease;
		setScale(2, 2);
		setOutlineColor(UIColors::GoldYellow - UIColors::QuarterTransparent);
	}
}

void UICard::LoadTextures()
{
	// Null Texture
	s_nullTexture = new sf::Texture;
	s_nullTexture->loadFromFile(std::string(s_texturesFile + "NullTexture.png"));
	// Background Textures
	std::thread thread1(LoadSpecificTextures, "", "ExpansionCardBackground", 3, &s_backgroundTextures);
	// Expansion L1 Textures
	std::thread thread2(LoadSpecificTextures, "Level1ExpansionCards", "Level1ExpansionCard", GamePieces::s_L1ExpansionCardCount, &s_expansionL1Textures);
	// Expansion L2 Textures
	std::thread thread3(LoadSpecificTextures, "Level2ExpansionCards", "Level2ExpansionCard", GamePieces::s_L2ExpansionCardCount, &s_expansionL2Textures);
	// Expansion L3 Textures
	std::thread thread4(LoadSpecificTextures, "Level3ExpansionCards", "Level3ExpansionCard", GamePieces::s_L3ExpansionCardCount, &s_expansionL3Textures);
	// Noble Textures
	std::thread thread5(LoadSpecificTextures, "NobleCards", "NobleCard", GamePieces::s_NobleCardCount, &s_nobleTextures);

	// Join Threads
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
}

void UICard::LoadSpecificTextures(std::string&& fileName, const std::string&& textureName, uint16_t count, TextureMap* into)
{
	// construct general path
	if (!fileName.empty())
	{
		fileName += '/';
	}
	const std::string path = s_texturesFile + fileName + textureName + "-";
	const std::string fileType = ".png";

	// run threads and populate TextureMaps
	std::vector<std::thread> threads;
	for (uint16_t textureID = 1; textureID <= count; ++textureID)
	{
		// construct specific path
		std::string current_path = path;
		std::string textureNumber = std::to_string(textureID);
		if (textureID < 10)
		{
			textureNumber.insert(textureNumber.begin(), '0');
		}
		current_path += textureNumber + fileType;
		threads.emplace_back(LoadSpecificTexture, current_path, into, textureID);
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
}

void UICard::LoadSpecificTexture(const std::string& path, TextureMap* into, uint16_t atID)
{
	auto* cardTexture = new sf::Texture;
	cardTexture->loadFromFile(path);
	(*into)[atID] = cardTexture;
}

const sf::Texture* UICard::GetTexture(uint16_t id, Type type)
{
	if (id == 0)
	{
		return s_nullTexture;
	}

	switch (type)
	{
	case Type::Background:
		try
		{
			return s_backgroundTextures[id];
		}
		catch (std::exception)
		{
			throw std::invalid_argument("Wrong id or type");
		}
	case Type::ExpansionL1:
		try
		{
			return s_expansionL1Textures[id];
		}
		catch (std::exception)
		{
			throw std::invalid_argument("Wrong id or type");
		}
	case Type::ExpansionL2:
		try {
			return s_expansionL2Textures[id];
		}
		catch (std::exception)
		{
			throw std::invalid_argument("Wrong id or type");
		}
	case Type::ExpansionL3:
		try
		{
			return s_expansionL3Textures[id];
		}
		catch (std::exception)
		{
			throw std::invalid_argument("Wrong id or type");
		}
	case Type::Noble:
		try
		{
			return s_nobleTextures[id];
		}
		catch (std::exception)
		{
			throw std::invalid_argument("Wrong id or type");
		}
	case Type::Unknown:
		return nullptr;
	default:
		throw std::invalid_argument("Wrong id or type");
	}
}