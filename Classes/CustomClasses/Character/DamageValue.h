#pragma once

class DamageValue 
{
public:
	float HpDamage(float num = 0);
	float NpDamage(float num = 0);
	DamageValue() noexcept { hpDamage = npDamage = 0; }
private:
	float hpDamage;
	float npDamage;
};