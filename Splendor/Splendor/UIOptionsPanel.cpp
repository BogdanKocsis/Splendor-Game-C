#include "UIOptionsPanel.h"

UIOptionsPanel::UIOptionsPanel(const std::string& title, Type type, sf::Vector2f position, sf::Vector2f size, bool isActive) :
	UIPanel(title, size, position, isActive),
	m_title({ position.x,position.y + size.y / 2.0f }, UIText::TextAlign::mid_center, UIText::AvailableFonts::LatoBlack, title, 0.4f * size.y),
	m_type(type),
	m_lastCheckedID(0)
{
	// Text
	setPosition(position);
	m_title.AlignText(UIText::TextAlign::mid_left);
	m_lastOption.first = nullptr;
	m_lastOption.second = nullptr;

	AddContent(dynamic_cast<sf::Drawable*>(&m_title));
}

void UIOptionsPanel::AddOption(const std::string& optionName)
{
	auto& [collider, drawable] = m_lastOption;
	const float yPosition = getPosition().y;
	if (collider == nullptr)
	{
		const auto titleBounds = m_title.getGlobalBounds();
		UISelectorBox* checkBox = new UISelectorBox(optionName, static_cast<UISelectorBox::Type>(m_type), { titleBounds.left + titleBounds.width,yPosition }, { 200,getSize().y });
		collider = checkBox;
		drawable = checkBox;
		if (m_type == Type::Radio)
		{
			checkBox->ChangeState();
			m_lastCheckedID = 0;
		}
	}
	else
	{
		const auto lastBounds = dynamic_cast<UISelectorBox*>(collider)->GetRect();
		UISelectorBox* checkBox = new UISelectorBox(optionName, static_cast<UISelectorBox::Type>(m_type), sf::Vector2f(lastBounds.left + lastBounds.width, yPosition), { 200,getSize().y });
		collider = checkBox;
		drawable = checkBox;
	}
	AddContent(m_lastOption);
}

void UIOptionsPanel::HandleEvent(const sf::Event& event)
{
	for (size_t id = 0; id < ContentColliderSize(); id++)
	{
		GetContentCollider(id)->HandleEvent(event);
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			UpdateOptions();
		}
	}
}

bool UIOptionsPanel::IsChecked(const std::string& optionName) const
{
	for (size_t id = 0; id < ContentColliderSize(); ++id)
	{
		const auto option = dynamic_cast<UISelectorBox*>(GetContentCollider(id));
		if (option->GetName() == optionName)
		{
			return option->IsChecked();
		}
	}
	throw std::invalid_argument("Invalid option name");
}

std::string UIOptionsPanel::FirstChecked() const
{
	for (size_t id = 0; id < ContentColliderSize(); ++id)
	{
		const auto option = dynamic_cast<UISelectorBox*>(GetContentCollider(id));
		if (option->IsChecked())
		{
			return option->GetName();
		}
	}
	return "";
}

void UIOptionsPanel::UpdateOptions()
{
	if (m_type == Type::Radio)
	{
		for (size_t id = 0; id < ContentColliderSize(); ++id)
		{
			if (dynamic_cast<UISelectorBox*>(GetContentCollider(id))->IsChecked())
			{
				if (id != m_lastCheckedID)
				{
					dynamic_cast<UISelectorBox*>(GetContentCollider(m_lastCheckedID))->ChangeState();
					m_lastCheckedID = id;
				}
			}
			else
			{
				if (id == m_lastCheckedID)
				{
					dynamic_cast<UISelectorBox*>(GetContentCollider(m_lastCheckedID))->SetState(true);
				}
			}
		}
	}
}

