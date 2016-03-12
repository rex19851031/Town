#pragma once

#ifndef UI_HPP 
#define UI_HPP

#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Renderer\BitmapFont.hpp"

#include "GameCode\Agent.hpp"

#include <string>
#include <vector>

namespace Henry
{

struct MessageLogString
{
	MessageLogString(std::string _msg , RGBA _color) : msg(_msg) , color(_color) {};
	MessageLogString(const char* _msg , RGBA _color) : msg(_msg) , color(_color) {};
	std::string msg;
	RGBA color;
};

class UI
{
public:
	UI(void);
	~UI(void);
	void SetDisplayAgent(Agent* agent);
	void Update();
	void Draw();
	void PostMessageToLog( const std::string& msg , RGBA color = RGBA() );
	void PostMessageToLog( const char* msg , RGBA color = RGBA() , ...);
	void ReadNextMessage();
	bool AllMessageRead();

private:
	std::string PrintToConsoleVariable(const char*message , va_list args);
	void DrawMessage();
	void DrawHealthBar();
	void DrawEquipmentInfo();
	AABB2 m_healthBar;
	std::vector<MessageLogString> m_nextMsgBar;
	std::vector<MessageLogString> m_msgBar;
	BitmapFont* m_font;
	int m_currentStartDrawingLineNum;
	Agent* m_displayAgent;
};

};

#endif