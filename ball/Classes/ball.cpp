#include "ball.h"
#define PTM_RATIO 32 
#define STOPHEIGHT 200
using namespace cocos2d;
using namespace std;

CCScene* CrazyBall::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CrazyBall *layer = CrazyBall::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool CrazyBall::init()
{
    bool bRet = false;
    do 
    {	
		allball = CCArray::create();
		allball->retain();
		ballBeClicked = CCArray::create();
		ballBeClicked->retain();
	    // set background.
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite* pBg = CCSprite::create("bg.png");
        CC_BREAK_IF(! pBg);
		pBg->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pBg, 0);
		
		//initialize physical world
		this->initPhysics();
		
		// add a line of ball every0.3s->multi add a ball
		this->schedule(schedule_selector(CrazyBall::addALine),3.0f);
		scheduleUpdate();

		pMenu = CCMenu::create();
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,1);
        bRet = true;
    } while (0);

    return bRet;
}
void CrazyBall::update(float dt)
{

	int velocityIterations = 8;
    int positionIterations = 1;
  
    world->Step(dt, velocityIterations, positionIterations);
	

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
			if (b->GetUserData() != NULL) { 
				CCMenuItemSprite *myActor = (CCMenuItemSprite *)b->GetUserData();
				myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
				myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
			}
		}

    //-------------------------------------------------------------//
	//添加点击的刚体到刚体容器bodyBeClick
	b2Body *node = world->GetBodyList();
	while(node)
	{
		b2Body *b = node;
		node = node->GetNext();
		if(b->GetType() == 2)
		{
			if((pdebody.x == b->GetPosition().x*PTM_RATIO) && (pdebody.y == b->GetPosition().y*PTM_RATIO))
				bodyBeClicked.push_back(b);
				//world->DestroyBody(b);
		}
	}

	//deleteBody();
	
	
}


void CrazyBall::initPhysics()
{

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

}

void CrazyBall::addALine(float dt)
{	
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	for(int n=1;n<11;n++)
	addNewSpriteAtPosition(ccp(s.width*(2*n-1)/20, s.height*9/10));
}

void CrazyBall::addNewSpriteAtPosition(CCPoint p)
{
	char buf[20];
	a = rand()%5 * 10;sprintf(buf,"ball%d.png",a);
	CCMenuItemSprite *pItem1 = CCMenuItemImage::create(
            buf,
            buf,
            this,
            menu_selector(CrazyBall::clickball));
	pItem1->setScale(0.32);
	pMenu->addChild(pItem1,3,a);
	allball->addObject(pItem1);
    //CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    //pItem1->setPosition( CCPointMake( p.x, p.y) );
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
	bodyDef.userData=pItem1;
	b2CircleShape circle;
	circle.m_radius = 24.0/PTM_RATIO;
	body = world->CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;    
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

}

void CrazyBall::clickball(CCObject* Spender)
{
	CCMenuItem * cball = (CCMenuItem *)Spender; 
	//pMenu->removeChild(cball,true);
	cball->runAction(CCSequence::create(
	            CCDelayTime::create(0.1f), 
				CCCallFuncN::create(this, callfuncN_selector(CrazyBall::deleteBody)), NULL));
	ballBeClicked->addObject(cball);
	pdebody = cball->getPosition();
}

bool CrazyBall::judgeBody()
{
	
return true;
}

void CrazyBall::deleteBody(CCNode* Spender)
{	
	CCMenuItem * cball = (CCMenuItem *)Spender; 
	pMenu->removeChild(cball,true);CCLOG("Delete body");
	std::vector<b2Body*>::iterator pr;
	for(pr=bodyBeClicked.begin();pr!=bodyBeClicked.end();){
		int j=judgeBody() ;
		if(j){
			b2Body *b = *pr;
			pr=bodyBeClicked.erase(pr);
			//b->SetUserData(NULL);
			world->DestroyBody(b);
			
			
		}
	}
}