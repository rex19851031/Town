#include "NPC.hpp"

#include "GameCode\TheApplication.hpp"
#include "GameCode\GameCommon.hpp"

#include <string>

namespace Henry
{

NPC::NPC(std::string& name,Map* currentMap) : Agent(name,currentMap)
{
	m_characterSize = GameCommon::s_cellSize;
	m_random = false;
}


NPC::~NPC(void)
{
}


void NPC::Update()
{
	float evaluateValue = -1;
	int nextBehaviorID = -1;
	
	GameCommon::PoorMansFOV(this,20,false,20);

	for(size_t index=0; index < m_behaviorList.size(); index++)
	{
		if(evaluateValue < m_behaviorList[index]->Evaluate())
		{
			evaluateValue = m_behaviorList[index]->Evaluate();
			nextBehaviorID = index;
		}
	}

	if(nextBehaviorID != -1)
	{
		m_behaviorList[nextBehaviorID]->Think();
	}

	if(m_prevCombat)
	{
		delete m_prevCombat;
		m_prevCombat = nullptr;
	}
}


void NPC::MoveTo(const Vec2i location)
{
	if(CanMoveTo(location))
	{
		m_map->SetAgent(m_position.x,m_position.y,nullptr);
		m_map->SetAgent(location.x,location.y,this);
		m_position = location;
	}
}


void NPC::AddBehavior(AIBehavior* behavior)
{
	behavior->SetAgent(this);
	m_behaviorList.push_back(behavior);
}


void NPC::SaveToXML(TiXmlElement* element)
{
	//element->SetAttribute()
	TiXmlElement * sub_elem = new TiXmlElement("NPC");
	element->LinkEndChild(sub_elem);

	Agent::SaveToXML(sub_elem);
}


};