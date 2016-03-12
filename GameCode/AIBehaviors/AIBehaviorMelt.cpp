#include "AIBehaviorMelt.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

AIBehaviorRegistration AIBehaviorMelt::s_meltRegistration("Melt",&AIBehaviorMelt::CreateAndGetBehavior);


AIBehavior* AIBehaviorMelt::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorMelt(elem) );
}


AIBehaviorMelt::AIBehaviorMelt(const TiXmlElement* xmlElem) : AIBehavior(xmlElem)
{
	m_affectRdius = atoi(xmlElem->Attribute("radius"));
}


AIBehaviorMelt::~AIBehaviorMelt(void)
{
}


bool AIBehaviorMelt::Think()
{
	return true;
}


AIBehavior* AIBehaviorMelt::Clone()
{
	return new AIBehaviorMelt(*this);
}


float AIBehaviorMelt::Evaluate()
{
	if(m_agent)
	{
		Vec2i position = m_agent->m_position;
		for(int x=position.x-m_affectRdius; x < position.x+m_affectRdius; ++x)
		{
			for(int y=position.y-m_affectRdius; y < position.y+m_affectRdius; ++y)
			{
				if(m_agent->m_map)
				{
					CellType type = m_agent->m_map->GetCellType(x,y);
					if(type == ICE)
						m_agent->m_map->SetCellType(x,y,WATER);
				}
			}
		}
	}
	return 1.0f;
}

};