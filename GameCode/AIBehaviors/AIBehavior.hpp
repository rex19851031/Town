#pragma once

#ifndef AIBEHAVIOR_HPP
#define AIBEHAVIOR_HPP

#include <string>
#include <map>

#include "GameCode\Agent.hpp"

#include "Engine\Parsing\TinyXML\tinyxml.h"

namespace Henry
{

class AIBehavior
{
public:
	AIBehavior(const TiXmlElement* xmlElem);
	virtual ~AIBehavior(void);
	virtual bool Think();
	virtual float Evaluate();
	virtual AIBehavior* Clone();
	void SetAgent(Agent* agent);
	
	static AIBehavior* CreateAndGetBehavior(const std::string& name , TiXmlElement* xmlElem);
protected:
	Agent* m_agent;
private:
	const TiXmlElement* m_xmlElem;
};

};

#endif