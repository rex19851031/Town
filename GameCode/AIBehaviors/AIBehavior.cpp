#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

//(?X?) pass in the xml node instead of string name
AIBehavior* AIBehavior::CreateAndGetBehavior(const std::string& name , TiXmlElement* xmlElem)	
{
	//AIBehavior* result = nullptr;

	AIBehaviorRegistrationMap* registrationBehaviorMap = AIBehaviorRegistration::s_behaviorRegistrationMap;
	
	AIBehaviorRegistrationMap::iterator iter = registrationBehaviorMap->begin();
	for( ; iter != registrationBehaviorMap->end(); ++iter)	//Find() in map class , return iterator
	{
		AIBehaviorRegistration* registration = iter->second;
		if(iter->first == name)
		{
			AIBehavior* thisBehavior = registration->CreateThisAIBehavior(xmlElem);
			return thisBehavior;
		}
	}
	// can't find , report error in log //  ASSERT // return nullptr
	return nullptr;	//return result; , one return point rather than multiple return;
}


AIBehavior::AIBehavior(const TiXmlElement* xmlElem) : m_xmlElem(xmlElem)
{
}


AIBehavior::~AIBehavior(void)
{
}


bool AIBehavior::Think()
{
	return true;
}


float AIBehavior::Evaluate()
{
	return -1;
}


AIBehavior* AIBehavior::Clone()
{
	return new AIBehavior(*this);
}


void AIBehavior::SetAgent(Agent* agent)
{
	m_agent = agent;
}

};