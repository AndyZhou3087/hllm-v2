#include "SoundManager.h"
#include "SimpleAudioEngine.h"
//#include "CommonFuncs.h"

using namespace CocosDenshion;

static SoundManager* g_sharedManager = NULL;

SoundManager* SoundManager::sharedManager()
{
    if(!g_sharedManager)
    {
        g_sharedManager = new SoundManager();
        g_sharedManager->init();
    }
    
    return g_sharedManager;
}

Ref* SoundManager::init()
{
    // TODO : if ((self = [super init]))
    _isSoundOn = true;
	_isMusicOn = true;
	_isMusicPlayed = false;
    return this;
}

void SoundManager::loadSounds()
{
    // TODO :
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_BUTTON, "sound/button1.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_READYGO, "sound/readygo.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_CLICKBLOCK, "sound/button2.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_CLEAR, "sound/eliminate.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_TIMEWARNING, "sound/timewarning.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_WIN, "sound/win.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_LOSS, "sound/loss.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_TIMEUP, "sound/timeup.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_USEHP, "sound/usehp.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_NEXTSTAGE, "sound/nextstage.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_UNLOCKCHAPTER, "sound/unlockchapter.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_PAUSEBUTTON, "sound/pausebutton.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_BARRIERBOMB, "sound/barrierbomb.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_ICEBOMB, "sound/icebomb.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUDN_ID_LOCKBOMB, "sound/lockbomb.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_NEXTSMALLSTAGE, "sound/nextsmallstage.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_ROLECRASH, "sound/rolecrash.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_SUCC, "sound/succ.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_FAIL, "sound/fail.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_SUCCSTAR, "sound/succstar.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_FIREWORKS, "sound/fireworks.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_BLOCKCHANGE, "sound/blockchange.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_PAUSESOUNDBUTTON, "sound/pausesoundbutton.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_SKILLSUCC, "sound/skillsucc.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_THEMEMOVE, "sound/thememove.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_LOTTERY, "sound/lottery.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_10TIMESLOTTERY, "sound/10timeslottery.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_STARADD, "sound/staradd.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_USEPROP, "sound/useprop.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_ADDCOIN, "sound/addcoin.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_ADDGEM, "sound/addgem.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_ADDHP, "sound/addhp.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_HITBOSS, "sound/hitboss.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_COMBO2, "sound/combo2.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_COMBO3, "sound/combo3.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_COMBO4, "sound/combo4.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_COMBO5, "sound/combo5.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_COMBO6, "sound/combo6.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_BOSSSKILL, "sound/bossskill.mp3"));
	
	SimpleAudioEngine::getInstance()->preloadEffect("sound/button1.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/readygo.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/button2.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/eliminate.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/timewarning.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/win.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/loss.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/timeup.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/usehp.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/nextstage.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/unlockchapter.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/pausebutton.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/barrierbomb.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/icebomb.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/lockbomb.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/nextsmallstage.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/rolecrash.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/succ.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fail.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/succstar.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fireworks.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/blockchange.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/pausesoundbutton.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/skillsucc.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/thememove.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/lottery.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/10timeslottery.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/staradd.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/useprop.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/addcoin.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/addgem.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/addhp.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/hitboss.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/combo2.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/combo3.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/combo4.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/combo5.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/combo6.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/bossskill.mp3");
}

void SoundManager::unloadSounds()
{
    //CDSoundEngine* soundEngine = [CDAudioManager sharedManager].soundEngine;
    
	//for( int i = 0; i < SOUND_ID_MAX; ++i )
	//{
	//	[soundEngine unloadBuffer:i];
	//}
}

int SoundManager::playSound(int soundId)
{
   return playSound(soundId, false);
}

int SoundManager::playSound(int soundId, bool isloop)
{
    // TODO :
    //CDSoundEngine* soundEngine = [CDAudioManager sharedManager].soundEngine;
	//[soundEngine playSound:soundID sourceGroupId:0 pitch:pitch pan:0.f gain:1.f loop:NO];

	if(!_isSoundOn)
	{
	    return -1;
	}
    
    SoundIdMapType::iterator it = _soundIds.find(soundId);
    if (it == _soundIds.end()) {
        return -1;
    }
    
    std::string soundName = it->second;
    SimpleAudioEngine* soundEngine = SimpleAudioEngine::getInstance();
	return soundEngine->playEffect(soundName.c_str(), isloop);
}

void SoundManager::setMute(bool newMuteValue)
{
    // TODO :
    //CDSoundEngine* soundEngine = [CDAudioManager sharedManager].soundEngine;
	//[soundEngine setMute:newMuteValue];
}

bool SoundManager::getSoundIsOn()
{
	_isSoundOn = UserDefault::getInstance()->getBoolForKey("isSoundOn", true);
	return _isSoundOn;
}

bool SoundManager::getMusicPlayed()
{
	return _isMusicPlayed;
}
void SoundManager::playBackMusic(const char * filename)
{
	if (_isMusicOn)
    {
		_isMusicPlayed = true;
        SimpleAudioEngine::getInstance()->playBackgroundMusic(filename,true);
    }
}

void SoundManager::stopBackMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void SoundManager::pauseBackMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundManager::resumeBackMusic()
{
	if (_isMusicOn)
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void SoundManager::setIsMusicOn(bool isMusicOn)
{
	_isMusicOn = isMusicOn;
}
void SoundManager::setIsSoundOn(bool isSoundOn)
{
    _isSoundOn = isSoundOn;
    //setMute(_isSoundOn);
}

void SoundManager::stopSound(int soundId)
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->stopEffect(soundId);

}
void SoundManager::stopAllEffectSound()
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->stopAllEffects();
}
void SoundManager::pauseAllEffectSound()
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void SoundManager::resumeAllEffectSound()
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->resumeAllEffects();
}

float SoundManager::getVolume()
{
    return SimpleAudioEngine::getInstance()->getEffectsVolume();
}

void SoundManager::setVolume(float volume)
{
    SimpleAudioEngine* soundEngine = SimpleAudioEngine::getInstance();
    soundEngine->setEffectsVolume(volume);
    soundEngine->setBackgroundMusicVolume(volume);
}
