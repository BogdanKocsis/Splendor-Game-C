#pragma once
#include "RectCollider.h"
#include "ICard.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>

class UICard :public RectCollider, public sf::RectangleShape
{
public:
	using TextureMap = std::unordered_map<uint16_t, sf::Texture*>;

	enum class Type
	{
		Unknown = -1,
		Background = 4,
		Noble = 0,
		ExpansionL1 = 1,
		ExpansionL2 = 2,
		ExpansionL3 = 3
	};

	struct Data
	{
		Data(Type dataType = UICard::Type::Unknown, uint16_t dataID = 0, bool isDataNumb = false);

		Type type;
		uint16_t id;
		bool isNumb;
	};

	enum class State
	{
		None,
		Hover,
		Press,
		LeftRelease,
		RightRelease
	};

public:
	// Constr.
	UICard(uint16_t id, Type type, const sf::Vector2f& position = sf::Vector2f(0, 0), const sf::Vector2f& size = sf::Vector2f(0, 0));

	// Manipulators + G&S
	uint16_t GetID() const;
	Type GetType() const;
	Data GetData() const;
	void SetData(Data data);
	bool GetNumb() const;
	void SetNumb(bool numb);
	State GetState() const;
	void SetState(State state);
	void TriggerWarning();
	void Deactivate();
	void Activate();
	void SetInHand(bool inHand);

	// Collider
	void OnMouseOver() override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void OnMouseLeftClick() override;
	void OnMouseLeftRelease() override;
	void OnMouseRightClick() override;
	void OnMouseRightRelease() override;

private:
	// Textures
	static void LoadTextures();
	static void LoadSpecificTextures(std::string&& fileName, const std::string&& textureName, uint16_t count, TextureMap* into);
	static void LoadSpecificTexture(const std::string& path, TextureMap* into, uint16_t atID);
	static const sf::Texture* GetTexture(uint16_t id, Type type);

private:
	// Textures
	const inline static std::string s_texturesFile = "../external/Resources/Textures/Cards/";
	static inline sf::Texture* s_nullTexture;
	static inline TextureMap s_backgroundTextures;
	static inline TextureMap s_expansionL1Textures;
	static inline TextureMap s_expansionL2Textures;
	static inline TextureMap s_expansionL3Textures;
	static inline TextureMap s_nobleTextures;
	static inline bool s_texturesLoaded = false;

private:
	// Graphics
	sf::Vector2f m_initialPosition;
	bool m_warning;

	// Logic
	bool m_numb;
	bool m_inHand;
	State m_state;
	uint16_t m_id;
	Type m_type;
};

