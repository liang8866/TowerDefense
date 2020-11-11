#ifndef __SHAKE_H__
#define __SHAKE_H__
#include "cocos2d.h"
#include "GameConfig.h"
#include "CCActionInterval.h"

class Shake : public CCActionInterval
{
	// Code by Francois Guibert
	// Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax
public:
	Shake();

	static Shake* create();
	// Create the action with a time and a strength (same in x and y)
	static Shake* create(float d, float strength );
	// Create the action with a time and strengths (different in x and y)
	static Shake* createWithStrength(float d, float strength_x, float strength_y );
	bool initWithDuration(float d, float strength_x, float strength_y );

	/*Shake* reverse();
	Shake *clone();*/

	virtual Shake* clone() const override;
	virtual Shake* reverse(void) const override;

protected:

	void startWithTarget(cocos2d::CCNode *pTarget);
	void update(float time);
	void stop(void);


	// Initial position of the shaked node
	float m_initial_x, m_initial_y;
	// Strength of the action
	float m_strength_x, m_strength_y;

	float tempDt;
	CCNode* m_pTarget;
};

#endif //__SHAKE_H__
