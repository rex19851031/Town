#pragma once

#include "GameCode\Agent.hpp"
#include "GameCode\AIBehaviors\AIBehavior.hpp"

#include <vector>

namespace Henry
{

class NPC : public Agent
{
public:
	NPC(std::string& name,Map* currentMap);
	~NPC(void);
	void Update();
	void MoveTo(const Vec2i location);
	void AddBehavior(AIBehavior* behavior);
	void SaveToXML(TiXmlElement* element);

	bool m_random;

private:
	Vec2i m_characterSize;
	std::vector<AIBehavior*> m_behaviorList;
};

};