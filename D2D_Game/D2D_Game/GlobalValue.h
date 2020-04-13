#pragma once

enum CT_Type {		// CharicType
	CT_None = 0,
	CT_Nanami,
	CT_Zombie1,
	CTT_Length,
};

enum EWeaponSlots {
	PISTOL = 0,
	HEAVY_MACHINE_GUN = 1,
	ROCKET_LAUNCHER = 2,
	ENEMY_CHASER = 3,
};

extern EWeaponSlots WeaponSlot;

extern int GlobalSeed;
int RandInt(int min, int max);

void GDeltaUpdate(float a_DeltaTime);
float GetMvDelta(int WpType);