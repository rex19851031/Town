#include "FeatureFactory.hpp"

#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"

#include "GameCode\GameCommon.hpp"

#include <string>

namespace Henry
{

FeatureFactorySet FeatureFactory::s_featureFactorySet;


FeatureFactory::FeatureFactory()
{

}


FeatureFactory::~FeatureFactory()
{
	delete m_featureTemplate;
}


void FeatureFactory::LoadAndParseAllFilesUnder(const char* path)
{
	std::vector<std::string> allFiles;
	FileSystem files;
	std::string fullPath(path);
	
	fullPath.append("\\*.xml");
	files.EnumerateFileInFolder(fullPath.c_str(),allFiles);
	for(size_t index=0; index < allFiles.size(); index++)
	{
		std::string xmlPath(path);
		xmlPath.append(allFiles[index]);
		TinyXMLHelper xmlHelper;
		xmlHelper.LoadFile(xmlPath.c_str());
		for(TiXmlElement* elem = xmlHelper.m_root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "Feature")
			{
				std::string name(elem->Attribute("name"));
				std::string glyphOn(elem->Attribute("glyphOn"));
				std::string glyphOff(elem->Attribute("glyphOff"));
				std::string color(elem->Attribute("color"));
				std::string maxHP(elem->Attribute("maxHP"));
				std::string type(elem->Attribute("type"));
				std::string value(elem->Attribute("affectValue"));
				std::string radius(elem->Attribute("radius"));

				Feature* feature = new Feature(name , nullptr); //GameCommon::s_world->GetMap());
				feature->m_glyphOn = glyphOn[0];
				feature->m_glyphOff = glyphOff[0];
				feature->m_glyph = glyphOn[0];
				feature->m_type = type;
				feature->m_affectValue = atoi(value.c_str());
				feature->m_radius = atoi(radius.c_str());
				if(feature->m_type == "Fountain")
					feature->m_isWalkable = true;

				sscanf_s(color.c_str(),"%d,%d,%d",&feature->m_color.r,&feature->m_color.g,&feature->m_color.b);
				feature->m_color.a = 255;
				feature->m_maxHP = (float)atof(maxHP.c_str());
				feature->m_currentHP = feature->m_maxHP;

				FeatureFactory* factory = new FeatureFactory(); //pass root to create NPCFactory() instead of get the value here
				factory->m_featureTemplate = feature;
				s_featureFactorySet.insert(factory);
			}
		}
	}
}


Feature* FeatureFactory::SpawnFeature(Map* theMap , const Vec2i location)
{
	Feature* feature = nullptr;
	if(theMap->GetCellType(location.x,location.y) == AIR)
	{
		feature = new Feature(*m_featureTemplate);
		feature->m_map = theMap;
		feature->MoveTo(location);
	}
	return feature;
}


FeatureFactory* FeatureFactory::FindFactory(const std::string& name)
{
	FeatureFactory* target_factory = nullptr;
	FeatureFactorySet::iterator iter = s_featureFactorySet.begin();
	while(iter != s_featureFactorySet.end())
	{
		FeatureFactory* factory = *iter;
		if(factory->m_featureTemplate->m_name == name)
		{
			target_factory = factory;
			break;
		}
		iter++;
	}

	return target_factory;
}

};