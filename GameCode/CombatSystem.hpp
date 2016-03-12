#pragma once

#ifndef COMBATSYSTEM_HPP
#define COMBATSYSTEM_HPP

#include "GameCode\Map.hpp"

namespace Henry
{

enum BufferType
{
	BUFFER_INCREASE_DAMAGE,
	BUFFER_INCREASE_DEFENCE,
	BUFFER_INCREASE_CRITICAL_CHANCE,
};

enum AttackType
{
	ATTACK_MELEE,
	ATTACK_WEAPON,
	ATTACK_RANGED,
	ATTACK_MAGIC,
};


enum AttackResult
{
	RESULT_HIT,
	RESULT_BLOCK,
	RESULT_DODGE,
	RESULT_DEFEND,
	RESULT_CRITICAL_HIT,
};


class Agent;
struct AttackData
{
	AttackData() : attacker(nullptr) , target(nullptr)	{};
	Agent*			attacker;
	GameEntity*		target;
	float			potentialDamageToHealth;
	float			actualDamageToHealth;
	float			potentialChangeToEnergy;
	float			actualChangeToEnergy;
	bool			knockBack;
	bool			friendlyFire;
	bool			attackwasSuccessful;
	bool			defenderDies;
	BufferType		buffer;
	AttackType		attackType;
	AttackResult	attackResult;
};


class CombatSystem
{
public:
	CombatSystem(void);
	~CombatSystem(void);
	void RequestCombat(AttackData& atkData);
};

};

#endif