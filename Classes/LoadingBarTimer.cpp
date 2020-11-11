/****************************************************************************
Copyright (C) 2010      Lam Pham
Copyright (c) 2010-2012 cocos2d-x.org
CopyRight (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "LoadingBarTimer.h"
#include "ui/UILoadingBar.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

NS_CC_BEGIN

#define kLoadingTimerCast LoadingBar*

// implementation of LoadingTo

LoadingTo* LoadingTo::create(float duration, float percent)
{
    LoadingTo *loadingTo = new LoadingTo();
    loadingTo->initWithDuration(duration, percent);
    loadingTo->autorelease();

    return loadingTo;
}

bool LoadingTo::initWithDuration(float duration, float percent)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = percent;

        return true;
    }

    return false;
}

LoadingTo* LoadingTo::clone() const
{
	// no copy constructor	
	auto a = new LoadingTo();
    a->initWithDuration(_duration, _to);
	a->autorelease();
	return a;
}

LoadingTo* LoadingTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in LoadingTo");
	return nullptr;
}

void LoadingTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _from = ((kLoadingTimerCast)(target))->getPercent();

    // XXX: Is this correct ?
    // Adding it to support Repeat
    if (_from == 100)
    {
        _from = 0;
    }
}

void LoadingTo::update(float time)
{
    ((kLoadingTimerCast)(_target))->setPercent(_from + (_to - _from) * time);
}

// implementation of LoadingFromTo

LoadingFromTo* LoadingFromTo::create(float duration, float fromPercentage, float toPercentage)
{
    LoadingFromTo *loadingFromTo = new LoadingFromTo();
    loadingFromTo->initWithDuration(duration, fromPercentage, toPercentage);
    loadingFromTo->autorelease();

    return loadingFromTo;
}

bool LoadingFromTo::initWithDuration(float duration, float fromPercentage, float toPercentage)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = toPercentage;
        _from = fromPercentage;

        return true;
    }

    return false;
}

LoadingFromTo* LoadingFromTo::clone() const
{
	// no copy constructor	
	auto a = new LoadingFromTo();
	a->initWithDuration(_duration, _from, _to);
	a->autorelease();
	return a;
}


LoadingFromTo* LoadingFromTo::reverse(void) const
{
    return LoadingFromTo::create(_duration, _to, _from);
}

void LoadingFromTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
}

void LoadingFromTo::update(float time)
{
    ((kLoadingTimerCast)(_target))->setPercent(_from + (_to - _from) * time);
}

NS_CC_END

