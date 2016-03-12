#include "AIBehaviorRegistration.hpp"

namespace Henry
{

AIBehaviorRegistrationMap* AIBehaviorRegistration::s_behaviorRegistrationMap;


AIBehaviorRegistrationMap* AIBehaviorRegistration::GetBehaviorRegistrationMap()
{
	return s_behaviorRegistrationMap;
}


AIBehavior* AIBehaviorRegistration::CreateThisAIBehavior(TiXmlElement* xmlElem)
{
	return (*m_registrationFunc)(xmlElem); 
}


AIBehaviorRegistration::AIBehaviorRegistration(const std::string name,AICreateFunc* creationFunc) : m_name(name) , m_registrationFunc(creationFunc)
{
	if(!s_behaviorRegistrationMap)
		s_behaviorRegistrationMap = new AIBehaviorRegistrationMap();

	(*s_behaviorRegistrationMap)[m_name] = this;
}


AIBehaviorRegistration::~AIBehaviorRegistration(void)
{
}

};