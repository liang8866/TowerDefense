#ifndef _CACHEMANAGER_H__
#define _CACHEMANAGER_H__

#include "GameConfig.h"

class CachManager:public Ref
{
public:
	CachManager(){};
	~CachManager(){};

	static CachManager* getInstance();

	void freshCach();

	void cleanCach();

private:

};



#endif // !_CACHEMANAGER_H__
