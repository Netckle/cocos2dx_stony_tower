#pragma once

enum CharType
{ 
	NONE, PLAYER, ENEMY 
};
enum CharAnim
{
	PHYSICAL_ATTACK_ANIM,
	MAGIC_ATTACK_ANIM,
	GUARD_ANIM,
	DODGE_ANIM,
	DAMAGE_ANIM,
	IDLE_ANIM,
	DEAD_ANIM,
	MOVE_ANIM
};
enum Direction
{ 
	LEFT  = -1, 
	ZERO  =  0,
	RIGHT =  1
};
enum StoneType 
{
	PHYSICAL_ATTACK,
	MAGIC_ATTACK,
	GUARD,
	DODGE
};
enum StoneTier 
{
	NORMAL, RARE, UNIQUE, EPIC
};
enum UpdateType
{
	SELECTION, RESULT
};