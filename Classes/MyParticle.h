#ifndef _MYPARTICLE_H_
#define _MYPARTICLE_H_
#include "cocos2d.h"
#include "Const.h"
USING_NS_CC;

class MyParticle : public ParticleSystemQuad
{
public:

	static MyParticle* Show(Node* parent, const std::string& plistFile, int x, int y, bool isbleand, float duration);
	static MyParticle* create(const std::string& plistFile, bool isbleand, float duration);
	bool initwithdata(const std::string& plistFile, bool isbleand, float duration);

	MyParticle();
	
	~MyParticle();

private:

};

#endif