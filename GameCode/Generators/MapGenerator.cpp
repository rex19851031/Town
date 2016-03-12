#include "MapGenerator.hpp"
#include "MapGeneratorRegistration.hpp"

namespace Henry
{

#define UNUSED(x) (void)(x);

mapGeneratorMap MapGenerator::s_generators;

MapGenerator* MapGenerator::CreateAllGenerators(const std::string& name)
{	
	MapRegistrationMap* registeredGenerators = MapGeneratorRegistration::GetMapRegistrationMap();
	MapRegistrationMap::iterator iter = registeredGenerators->begin();
	for( ; iter != registeredGenerators->end(); ++iter)
	{
		MapGeneratorRegistration* registration = iter->second;
		MapGenerator* thisGenerator = registration->CreateThisGenerator();
		if(thisGenerator)
			s_generators.insert(std::pair<std::string,MapGenerator*>(iter->first,thisGenerator));
	}
	return ( new MapGenerator(name) );
}


MapGenerator::MapGenerator()
{

}


MapGenerator::MapGenerator(std::string name)
{

}


MapGenerator::~MapGenerator()
{

}


void MapGenerator::GenerateMap(Map* theMap)
{
	UNUSED(theMap);
}


void MapGenerator::ProcessOneStep(Map* theMap)
{
	UNUSED(theMap);
}


void MapGenerator::GetNameOfGenerators(std::vector<std::string>* names)
{
	mapGeneratorMap::iterator iter = s_generators.begin();
	for( ; iter != s_generators.end(); ++iter)
	{
		std::string generatorName = iter->first;
		names->push_back(generatorName);
	}
}


MapGenerator* MapGenerator::GetGenerator(const std::string& name)
{
	mapGeneratorMap::iterator iter = s_generators.begin();
	while(iter != s_generators.end())
	{
		if(name == iter->first)
			return iter->second;
		iter++;
	}
	return nullptr;
}

};