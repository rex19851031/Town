#pragma once

#ifndef NPCFACFORY_HPP
#define NPCFACFORY_HPP

#include <set>
#include <vector>

#include "Engine\Parsing\FileSystem\FileSystem.hpp"

#include "GameCode\AIBehaviors\AIBehavior.hpp"
#include "GameCode\NPC\NPC.hpp"
#include "GameCode\Map.hpp"

namespace Henry
{

class NPCFactory;
typedef std::set<NPCFactory*> NPCFactorySet;

class NPCFactory
{
public:
	NPCFactory();
	~NPCFactory();
	NPC* SpawnNPC(Map* theMap , const Vec2i location);
	static NPCFactory* FindFactory(const std::string& name);
	static void LoadAndParseAllFilesUnder(const char* path);
	static NPCFactorySet s_npcFactorySet;

private:
	NPC* m_npcTemplate;
	std::vector<AIBehavior*> m_behaviors;
};

};

#endif