#pragma once

#ifndef AIBEHAVIORREGISTRATION_HPP 
#define AIBEHAVIORREGISTRATION_HPP

#include <string>
#include <map>

#include "AIBehavior.hpp"

#include "Engine\Parsing\TinyXML\tinyxml.h"

namespace Henry
{

class AIBehaviorRegistration;
typedef std::map<std::string,AIBehaviorRegistration*> AIBehaviorRegistrationMap;
typedef AIBehavior* (AICreateFunc)(const TiXmlElement* elem);//(const std::string& name);			//pass in xml node instead of pass in string


class AIBehaviorRegistration
{
public:
	AIBehaviorRegistration(const std::string name,AICreateFunc* creationFunc);
	~AIBehaviorRegistration(void);
	AIBehavior* CreateThisAIBehavior(TiXmlElement* xmlElem);

	static AIBehaviorRegistrationMap* GetBehaviorRegistrationMap();
	static AIBehaviorRegistrationMap* s_behaviorRegistrationMap;

protected:
	AICreateFunc* m_registrationFunc;

private:
	std::string m_name;
};

};

#endif