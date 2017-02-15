

#ifndef __starcrash__SoundManager__
#define __starcrash__SoundManager__

#include <iostream>
#include "cocos2d.h"
#include <map>


USING_NS_CC;

class CDSoundEngine;

class SoundManager : public Ref
{

    typedef std::map<int, std::string> SoundIdMapType;
    
public:
	enum SoundId
	{
		SOUND_ID_BG,
		SOUND_ID_BUTTON,
		SOUND_ID_READYGO,
		SOUND_ID_CLICKBLOCK,
		SOUND_ID_CLEAR,
		SOUND_ID_TIMEWARNING,
		SOUND_ID_WIN,
		SOUND_ID_LOSS,
		SOUND_ID_TIMEUP,
		SOUND_ID_USEHP,
		SOUND_ID_NEXTSTAGE,
		SOUND_ID_UNLOCKCHAPTER,
		SOUND_ID_PAUSEBUTTON,
		SOUND_ID_BARRIERBOMB,
		SOUND_ID_ICEBOMB,
		SOUDN_ID_LOCKBOMB,
		SOUND_ID_NEXTSMALLSTAGE,
		SOUND_ID_ROLECRASH,
		SOUND_ID_SUCC,
		SOUND_ID_FAIL,
		SOUND_ID_SUCCSTAR,
		SOUND_ID_FIREWORKS,
		SOUND_ID_BLOCKCHANGE,
		SOUND_ID_PAUSESOUNDBUTTON,
		SOUND_ID_SKILLSUCC,
		SOUND_ID_THEMEMOVE,
		SOUND_ID_LOTTERY,
		SOUND_ID_10TIMESLOTTERY,
		SOUND_ID_STARADD,
		SOUND_ID_USEPROP,
		SOUND_ID_ADDCOIN,
		SOUND_ID_ADDGEM,
		SOUND_ID_ADDHP,
		SOUND_ID_HITBOSS,
		SOUND_ID_COMBO2,
		SOUND_ID_COMBO3,
		SOUND_ID_COMBO4,
		SOUND_ID_COMBO5,
		SOUND_ID_COMBO6,
		SOUND_ID_BOSSSKILL
	};
	
    static SoundManager* sharedManager();
    
    Ref* init();
    
    void loadSounds();
    void unloadSounds();

	void playBackMusic(const char * filename);
	void stopBackMusic();

	void pauseBackMusic();
	void resumeBackMusic();

    int playSound(int soundId);
    int playSound(int soundId, bool isloop);
    
	void stopSound(int soundId);
	void stopAllEffectSound();
	void pauseAllEffectSound();
	void resumeAllEffectSound();

    void setMute(bool newMuteValue);
    
    void setIsSoundOn(bool isSoundOn);
	void setIsMusicOn(bool isMusicOn);
    
    float getVolume();
    void setVolume(float volume);
	bool getSoundIsOn();
	bool getMusicPlayed();
private:
	bool                _isSoundOn;
	bool				_isMusicOn;
	bool				_isMusicPlayed;
    SoundIdMapType      _soundIds;
};
//*/

#endif /* defined(__starcrash__SoundManager__) */
