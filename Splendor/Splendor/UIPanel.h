#pragma once
#include "Collider.h"

#include <SFML/Graphics.hpp>
#include <vector>

class UIPanel :public sf::RectangleShape
{
public:
	// Constr.
	UIPanel(const std::string& title, const sf::Vector2f& size = sf::Vector2f(0, 0), const sf::Vector2f& position = sf::Vector2f(0, 0), bool isActive = true, bool isInteractable = true);

	// G&S
	std::string GetTitle() const;
	void SetTitle(const std::string& title);
	bool IsActive() const;
	void SetActive(bool active);
	bool IsInteractable() const;
	void SetInteractable(bool interactable);

	// Content
	Collider* GetContentCollider(size_t contentID) const;
	sf::Drawable* GetContentDrawable(size_t contentID) const;
	size_t ContentColliderSize()const;
	size_t ContentDrawableSize()const;

	// Implicit draw
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Implicit event-handler
	virtual void HandleEvent(const sf::Event& event);

protected:
	// Content
	void AddContent(std::pair<Collider*, sf::Drawable*> contentItem);
	void AddContent(Collider* contentCollider);
	void AddContent(sf::Drawable* contentDrawable);

protected:
	std::vector<Collider*> m_colliderContent;
	std::vector<sf::Drawable*> m_drawableContent;

private:
	std::string m_titleString;
	bool m_isActive;
	bool m_isInteractable;
};

