#include "MapGeneratorRegistration.hpp"

namespace Henry
{

MapRegistrationMap* MapGeneratorRegistration::s_mapRegistrationMap = nullptr;


MapGeneratorRegistration::MapGeneratorRegistration(const std::string& name,RegistrationFunc* registrationFunc)
	 :	m_name(name) , 
		m_registrationFunc(registrationFunc)
{
	if( !s_mapRegistrationMap )
	{
		s_mapRegistrationMap = new MapRegistrationMap();
	}

	(*s_mapRegistrationMap)[m_name] = this;
}


MapGeneratorRegistration::~MapGeneratorRegistration(void)
{
}


MapRegistrationMap* MapGeneratorRegistration::GetMapRegistrationMap()
{
	return s_mapRegistrationMap;
}


MapGenerator* MapGeneratorRegistration::CreateThisGenerator()
{
	return (*m_registrationFunc)(m_name); 
}

};