#pragma once

#ifndef FEATUREFACFORY_HPP
#define FEATUREFACFORY_HPP

#include <set>
#include <vector>

#include "Engine\Parsing\FileSystem\FileSystem.hpp"

#include "GameCode\AIBehaviors\AIBehavior.hpp"
#include "GameCode\Features\Feature.hpp"
#include "GameCode\Map.hpp"

namespace Henry
{

class FeatureFactory;
typedef std::set<FeatureFactory*> FeatureFactorySet;

class FeatureFactory
{
public:
	FeatureFactory();
	~FeatureFactory();
	Feature* SpawnFeature(Map* theMap , const Vec2i location);
	static FeatureFactory* FindFactory(const std::string& name);
	static void LoadAndParseAllFilesUnder(const char* path);
	static FeatureFactorySet s_featureFactorySet;
private:
	Feature* m_featureTemplate;
};

};

#endif