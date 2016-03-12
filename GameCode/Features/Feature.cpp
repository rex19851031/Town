#include "Feature.hpp"

#include "GameCode\TheApplication.hpp"
#include "GameCode\GameCommon.hpp"
#include "GameCode\Agent.hpp"

#include <string>

namespace Henry
{

Feature::Feature(std::string& name,Map* currentMap) : GameEntity(name,currentMap)
{
	m_drawSize = GameCommon::s_cellSize;
	m_isActivate = false;
	m_isWalkable = false;
	m_isBroke = false;
}


Feature::~Feature(void)
{
}


void Feature::Update()
{
	if(!m_isActivate)
		m_glyph = m_glyphOn;
	else
		m_glyph = m_glyphOff;

	if(m_type == "Fountain")
	{
		for(int x=m_position.x-m_radius; x<=m_position.x+m_radius; ++x)
		{
			for(int y=m_position.y-m_radius; y<=m_position.y+m_radius; ++y)
			{
				Agent* agent = m_map->GetAgent(x,y);
				if(agent)
				{
					agent->m_currentHP += m_affectValue;
					if(agent->m_currentHP > agent->m_maxHP)
						agent->m_currentHP = agent->m_maxHP;
				}
			}
		}
	}
}


bool Feature::CanMoveTo(const Vec2i location)
{
	if(m_map->GetCellType(location.x,location.y) != AIR || m_map->GetFeature(location) != nullptr)
		return false;
	return true;
}


void Feature::MoveTo(const Vec2i location)
{
	if(CanMoveTo(location))
	{
 		m_map->SetFeature(location,this);
 		m_position = location;
	}
}


void Feature::SaveToXML(TiXmlElement* element)
{
	TiXmlElement* sub_element = new TiXmlElement("Feature");
	GameEntity::SaveToXML(sub_element);

	sub_element->SetAttribute("status" , m_isActivate ? "On" : "Off" );
	element->LinkEndChild(sub_element);
}


};