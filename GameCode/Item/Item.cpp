#include "Item.hpp"

#include "GameCode\Map.hpp"
#include "GameCode\Agent.hpp"
#include "GameCode\GameCommon.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

Item::Item(std::string& name,Map* currentMap) : GameEntity(name,currentMap)
{
	m_owner = nullptr;
	m_beenPicked = false;
	m_isBroke = false;
}


Item::~Item()
{

}


bool Item::CanMoveTo(const Vec2i location)
{
	if(m_map->GetCellType(location.x,location.y) != AIR)
		return false;
	return true;
}


void Item::MoveTo(const Vec2i location)
{
	if(CanMoveTo(location))
	{
		m_map->SetItem(location.x,location.y,this);
		m_position = location;
	}
}


void Item::Use()
{
	if(m_owner->m_map)
	{
		Vec2i center = m_owner->m_position;
		for(int x = center.x-1; x <= center.x+1; ++x)
		{
			for(int y = center.y-1; y <= center.y+1; ++y)
			{
				CellType type = m_owner->m_map->GetCellType(x,y);
				if(type == WATER)
				{
					m_owner->m_map->SetCellType(x,y,ICE);
				}
			}
		}
	}
}


void Item::Throw()
{

}


void Item::Consume()
{
	if(m_category == ITEM_POTION)
	{
		int healingPoints = (int)(m_maxEffectValue * getRandomPercent());
		if(healingPoints < m_minEffectValue)
			healingPoints = m_minEffectValue;

		m_owner->m_currentHP += healingPoints;
		GameCommon::s_UI->PostMessageToLog("Affect Health by %d From The Potion.",RGBA(),healingPoints);
		m_isBroke = true;
	}
}


void Item::Drop()
{

}


void Item::SaveToXML(TiXmlElement* element)
{
	TiXmlElement* sub_element = new TiXmlElement("Item");
	GameEntity::SaveToXML(sub_element);
	element->LinkEndChild(sub_element);
}


};