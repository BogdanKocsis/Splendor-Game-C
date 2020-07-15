#include "UIPanel.h"

UIPanel::UIPanel(const std::string& title, const sf::Vector2f& size, const sf::Vector2f& position, bool isActive, bool isInteractable) :
	RectangleShape(size), m_titleString(title), m_isActive(isActive), m_isInteractable(isInteractable)
{
	setPosition(position);
}

std::string UIPanel::GetTitle() const
{
	return m_titleString;
}

void UIPanel::SetTitle(const std::string& title)
{
	m_titleString = title;
}

bool UIPanel::IsActive() const
{
	return m_isActive;
}

void UIPanel::SetActive(bool active)
{
	m_isActive = active;
}

bool UIPanel::IsInteractable() const
{
	return m_isInteractable;
}

void UIPanel::SetInteractable(bool interactable)
{
	m_isInteractable = interactable;
}

Collider* UIPanel::GetContentCollider(unsigned contentID) const
{
	if (contentID < m_colliderContent.size())
		return m_colliderContent[contentID];
	return nullptr;
}

sf::Drawable* UIPanel::GetContentDrawable(unsigned contentID) const
{
	if (contentID < m_drawableContent.size())
		return m_drawableContent[contentID];
	return nullptr;
}

size_t UIPanel::ContentColliderSize() const
{
	return m_colliderContent.size();
}

size_t UIPanel::ContentDrawableSize() const
{
	return m_drawableContent.size();
}

void UIPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (IsActive())
	{
		for (size_t id = 0; id < ContentDrawableSize(); ++id)
		{
			target.draw(*m_drawableContent[id]);
		}
	}
}

void UIPanel::HandleEvent(const sf::Event& event)
{
	if (IsActive() && IsInteractable())
	{
		for (const auto& collider : m_colliderContent)
		{
			collider->HandleEvent(event);
		}
	}
}

void UIPanel::AddContent(std::pair<Collider*, sf::Drawable*> contentItem)
{
	m_colliderContent.push_back(contentItem.first);
	m_drawableContent.push_back(contentItem.second);
}

void UIPanel::AddContent(Collider* contentCollider)
{
	m_colliderContent.push_back(contentCollider);
}

void UIPanel::AddContent(sf::Drawable* contentDrawable)
{
	m_drawableContent.push_back(contentDrawable);
}
