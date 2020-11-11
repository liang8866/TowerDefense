// Code by Francois Guibert
// Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax


#include "Shake.h"
#include "cocos2d.h"
USING_NS_CC;

// not really useful, but I like clean default constructors
Shake::Shake() : m_strength_x(0), m_strength_y(0), m_initial_x(0), m_initial_y(0), m_pTarget(nullptr)
{
}

Shake* Shake::create( float d, float strength )
{
	// call other construction method with twice the same strength
	return createWithStrength( d, strength, strength );
}

Shake* Shake::createWithStrength(float duration, float strength_x, float strength_y)
{
	Shake *pRet = new Shake();

	if (pRet && pRet->initWithDuration(duration, strength_x, strength_y))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}


	return pRet;
}

bool Shake::initWithDuration(float duration, float strength_x, float strength_y)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_strength_x = strength_x;
		m_strength_y = strength_y;
		return true;
	}

	return false;
}

// Helper function. I included it here so that you can compile the whole file
// it returns a random value between min and max included
static float fgRangeRand( float min, float max )
{
	float rnd = ((float)rand()/(float)RAND_MAX);
	return rnd*(max-min)+min;
}

void Shake::update(float dt)
{

// 	float randx = fgRangeRand( -m_strength_x, m_strength_x )*dt;
// 	float randy = fgRangeRand( -m_strength_y, m_strength_y )*dt;

	float randx = fgRangeRand( -m_strength_x, m_strength_x )*tempDt;
 	float randy = fgRangeRand( -m_strength_y, m_strength_y )*tempDt;

	if (tempDt>0.0)
	{
			tempDt-=0.006666;
	}
	
	// move the target to a shaked position
	m_pTarget->setPosition(ccpAdd(ccp(m_initial_x, m_initial_y),ccp( randx, randy)));
	
	
}

void Shake::startWithTarget(CCNode *pTarget)
{
	if (!pTarget)
		return;

	m_pTarget = pTarget;
	
	CCActionInterval::startWithTarget( pTarget );

	// save the initial position
	m_initial_x = pTarget->getPosition().x;
	m_initial_y = pTarget->getPosition().y;
	tempDt=1.0;
}

void Shake::stop(void)
{
	// Action is done, reset clip position
	this->getTarget()->setPosition( ccp( m_initial_x, m_initial_y ) );

	CCActionInterval::stop();
}

Shake* Shake::reverse() const
{
	CCASSERT(false, "Shake doesn't support the 'reverse' method");
	return nullptr;
}
Shake* Shake::clone(void) const
{
	return nullptr;
}