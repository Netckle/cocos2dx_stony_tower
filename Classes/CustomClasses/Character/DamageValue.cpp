#include "CustomClasses/Character/DamageValue.h"

float DamageValue::HpDamage(float num) 
{
	if (num == 0) 
		return hpDamage;

	hpDamage = num;
	return hpDamage;
}
float DamageValue::NpDamage(float num) 
{
	if (num == 0)
		return npDamage;

	npDamage = num;
	return npDamage;
}