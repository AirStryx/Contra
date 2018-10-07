#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
	: m_spJungleTheme{make_shared<SoundStream>("resources/Sounds/JungleTheme.mp3")}
	, m_spLanding{make_shared<SoundEffect>("resources/Sounds/landing.wav") }
	, m_spMachineGun{ make_shared<SoundEffect>("resources/Sounds/MachineGun.wav") }
	, m_spPlayerHit{ make_shared<SoundEffect>("resources/Sounds/PlayerHit.wav") }
	, m_spShoot{ make_shared<SoundEffect>("resources/Sounds/shoot.wav") }
	, m_spSpreadGun{ make_shared<SoundEffect>("resources/Sounds/SpreadGun.wav") }
	, m_spDamage{ make_shared<SoundEffect>("resources/Sounds/Damage.wav") }
	, m_spBridgeExplosion{ make_shared<SoundEffect>("resources/Sounds/bridgeExplosion.wav") }
	, m_spEndBossDeath{ make_shared<SoundEffect>("resources/Sounds/EndBossDeath.wav") }
	, m_spEndBossShift{ make_shared<SoundEffect>("resources/Sounds/EndBossShift.mp3") }
	, m_spDeath{ make_shared<SoundEffect>("resources/Sounds/DeathSound.wav") }
	, m_spVictory{ make_shared<SoundStream>("resources/Sounds/Victory.wav") }
{
	Mix_AllocateChannels(32);

	m_spBridgeExplosion->SetVolume(60);
	m_spJungleTheme->SetVolume(60);
	m_spLanding->SetVolume(60);
	m_spMachineGun->SetVolume(60);
	m_spPlayerHit->SetVolume(60);
	m_spShoot->SetVolume(60);
	m_spSpreadGun->SetVolume(60);
	m_spDamage->SetVolume(60);
	m_spEndBossDeath->SetVolume(60);
	m_spEndBossShift->SetVolume(60);
}

SoundManager & SoundManager::GetInstance()
{
	static SoundManager* pInstance{ new SoundManager() };
	return *pInstance;
}

void SoundManager::PlayJungleTheme()
{
	m_spJungleTheme->Play(true);
}
void SoundManager::PlayLanding()
{
	m_spLanding->Play(0);
}
void SoundManager::PlayMachineGun()
{
	m_spMachineGun->Play(0);
}
void SoundManager::PlayPlayerHit()
{
	m_spPlayerHit->Play(0);
}
void SoundManager::PlayShoot()
{
	m_spShoot->Play(0);	
}
void SoundManager::PlaySpreadGun()
{
	m_spSpreadGun->Play(0);
}
void SoundManager::PlayDamage()
{
	m_spDamage->Play(0);
}
void SoundManager::PlayBridgeExplosion()
{
	m_spBridgeExplosion->Play(0);
}
void SoundManager::PlayEndBossDeath()
{
	m_spEndBossDeath->Play(0);
}
void SoundManager::PlayEndBossShift()
{
	m_spEndBossShift->Play(0);
}
void SoundManager::PlayVictory()
{
	m_spVictory->Play(false);
}
void SoundManager::PlayDeath()
{
	m_spDeath->Play(0);
}
void SoundManager::StopSoundEffects()
{
	m_spBridgeExplosion->StopAll();
	m_spLanding->StopAll();
	m_spMachineGun->StopAll();
	m_spPlayerHit->StopAll();
	m_spShoot->StopAll();
	m_spSpreadGun->StopAll();
	m_spDamage->StopAll();
	m_spEndBossDeath->StopAll();
	m_spEndBossShift->StopAll();
}
void SoundManager::LowerAudio()
{
	m_spBridgeExplosion->SetVolume(m_spBridgeExplosion->GetVolume() - 1);
	m_spJungleTheme->SetVolume(m_spJungleTheme->GetVolume() - 1);
	m_spLanding->SetVolume(m_spLanding->GetVolume() - 1);
	m_spMachineGun->SetVolume(m_spMachineGun->GetVolume() - 1);
	m_spPlayerHit->SetVolume(m_spPlayerHit->GetVolume() - 1);
	m_spShoot->SetVolume(m_spShoot->GetVolume() - 1);
	m_spSpreadGun->SetVolume(m_spSpreadGun->GetVolume() - 1);
	m_spDamage->SetVolume(m_spDamage->GetVolume() - 1);
}
void SoundManager::HigherAudio()
{
	m_spBridgeExplosion->SetVolume(m_spBridgeExplosion->GetVolume() + 1);
	m_spJungleTheme->SetVolume(m_spJungleTheme->GetVolume() + 1);
	m_spLanding->SetVolume(m_spLanding->GetVolume() + 1);
	m_spMachineGun->SetVolume(m_spMachineGun->GetVolume() + 1);
	m_spPlayerHit->SetVolume(m_spPlayerHit->GetVolume() + 1);
	m_spShoot->SetVolume(m_spShoot->GetVolume() + 1);
	m_spSpreadGun->SetVolume(m_spSpreadGun->GetVolume() + 1);
	m_spDamage->SetVolume(m_spDamage->GetVolume() + 1);
}
