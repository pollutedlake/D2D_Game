#include "framework.h"
#include "GlobalValue.h"
#include <mmsystem.h>		// tiimeGetTime() 함수 사용을 위하여...
#pragma comment(lib, "winmm.lib")		// timeGetTime() 함수 사용을 위하여...

EWeaponSlots WeaponSlot = HEAVY_MACHINE_GUN;

//------ 랜덤값 구하기
int GlobalSeed = 0;

int RandInt(int min, int max)
{
	int SEED = timeGetTime() + GlobalSeed;
	srand((unsigned)SEED);
	GlobalSeed += SEED * SEED;

	int RAND = min + rand() % (max + 1 - min);
	return RAND;
}
//------ 랜덤값 구하기

float g_MvGunTick = 0.0f;
float g_MvGunDelta = 0.0f;
float g_MvRocTick = 0.0f;
float g_MvRocDelta = 0.0f;

void GDeltaUpdate(float a_DeltaTime)
{
	//------ HEAVY_MACHINE_GUN
	g_MvGunTick = g_MvGunTick + a_DeltaTime;
	g_MvGunDelta = 0.0f;

	if (0.05f <= g_MvGunTick) {
		g_MvGunDelta = g_MvGunTick;
		g_MvGunTick = 0.0f;
	}
	//------ HEAVY_MACHINE_GUN

	//------ ROCKET_LAUNCHER
	g_MvRocTick = g_MvRocTick + a_DeltaTime;
	g_MvRocDelta = 0.0f;

	if (0.008f <= g_MvRocTick) {
		g_MvRocDelta = g_MvRocTick;
		g_MvRocTick = 0.0f;
	}
	//------ ROCKET_LAUNCHER
}

float GetMvDelta(int WpType)
{
	if ((EWeaponSlots)WpType == EWeaponSlots::PISTOL) {
		return g_MvRocDelta;
	}
	else if ((EWeaponSlots)WpType == EWeaponSlots::HEAVY_MACHINE_GUN) {
		return g_MvGunDelta;
	}
	else if ((EWeaponSlots)WpType == EWeaponSlots::ROCKET_LAUNCHER) {
		return g_MvRocDelta;
	}

	return g_MvRocDelta;
}

int ChangeWeapon()
{
	if (WeaponSlot == EWeaponSlots::HEAVY_MACHINE_GUN) {
		WeaponSlot = EWeaponSlots::ROCKET_LAUNCHER;
	}
	else if (WeaponSlot == EWeaponSlots::ROCKET_LAUNCHER) {
		WeaponSlot = EWeaponSlots::PISTOL;
	}
	else if (WeaponSlot == EWeaponSlots::PISTOL) {
		WeaponSlot = EWeaponSlots::HEAVY_MACHINE_GUN;
	}

	return (int)WeaponSlot;
}

//------ 카메라 흔들기...
CameraShaker CameraShake;
DWORD g_vLastTime = 0.0;

void AdjustRenderPosForShake(float& Pos)
{
	if (CameraShake.Enabled) {
		float PER = (CameraShake.EndTime - g_vLastTime) / CameraShake.LifeTime;

		if (PER <= 0.0f) {
			CameraShake.Enabled = false;
			return;
		}

		if (1.0f < PER) {
			PER = 1.0f;
		}

		float Power = CameraShake.Power;
		Power *= PER;
		Power *= (RandInt(-1000, 1000) / 1000.0f);

		Pos += Power;
	}
}

void ShakeScreen(float Power, float Duration)
{
	Duration = Duration * 1000.0f;

	CameraShake.Power = Power;
	CameraShake.EndTime = g_vLastTime + (double)Duration;
	CameraShake.LifeTime = (double)Duration;
	CameraShake.Enabled = true;
}
//------ 카메라 흔들기...
