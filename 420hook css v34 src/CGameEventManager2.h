#ifndef _CGAMEEVENTMANAGER_H_
#define _CGAMEEVENTMANAGER_H_

#include "Include.h"

class cGameEventManager : public IGameEventListener2
{
public:
	void FireGameEvent( IGameEvent *event );
	void RegisterSelf( );
};

#endif