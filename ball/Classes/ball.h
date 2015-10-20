#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include <vector>

class CrazyBall : public cocos2d::CCLayer
{
	
	
public:
	b2Body *body;
	b2World* world;
	cocos2d::CCMenu *pMenu;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    CREATE_FUNC(CrazyBall);
	void update(float dt);
	void initPhysics();
	void addALine(float dt);
	void addNewSpriteAtPosition(cocos2d::CCPoint p);
	void clickball(CCObject* Spender);
	void deleteBody(CCNode* Spender) ;
	bool judgeBody();
	cocos2d::CCArray *allball;
	cocos2d::CCArray *ballBeClicked;
	cocos2d::CCPoint pdebody;
	std::vector<b2Body *> bodyBeClicked;
	int a;
};

#endif  // __HELLOWORLD_SCENE_H__