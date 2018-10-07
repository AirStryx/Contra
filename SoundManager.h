#pragma once
#include <memory>
#include "SoundEffect.h"
#include "SoundStream.h"
using namespace std;
class SoundManager
{
public:
	static SoundManager & GetInstance();

	void PlayJungleTheme();
	void PlayLanding();
	void PlayMachineGun();
	void PlayPlayerHit();
	void PlayShoot();
	void PlaySpreadGun();
	void PlayDamage();
	void PlayBridgeExplosion();
	void PlayEndBossDeath();
	void PlayEndBossShift();
	void PlayDeath();
	void PlayVictory();
	void StopSoundEffects();

	void LowerAudio();
	void HigherAudio();
private:
	//sounds
	shared_ptr<SoundStream> m_spJungleTheme;
	shared_ptr<SoundEffect> m_spLanding;
	shared_ptr<SoundEffect> m_spMachineGun;
	shared_ptr<SoundEffect> m_spPlayerHit;
	shared_ptr<SoundEffect> m_spShoot;
	shared_ptr<SoundEffect> m_spSpreadGun;
	shared_ptr<SoundEffect> m_spDamage;
	shared_ptr<SoundEffect> m_spBridgeExplosion;
	shared_ptr<SoundEffect> m_spEndBossDeath;
	shared_ptr<SoundEffect> m_spEndBossShift;
	shared_ptr<SoundEffect> m_spDeath;
	shared_ptr<SoundStream> m_spVictory;
	//constructor
	SoundManager();
};

