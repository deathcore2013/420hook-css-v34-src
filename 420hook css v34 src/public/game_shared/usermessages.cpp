//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "usermessages.h"
#include <bitbuf.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void RegisterUserMessages( void );

//-----------------------------------------------------------------------------
// Purpose: Force registration on .dll load
// FIXME:  Should this be a client/server system?
//-----------------------------------------------------------------------------
CUserMessages::CUserMessages()
{
	// Game specific registration function;
	//RegisterUserMessages();
}

CUserMessages::~CUserMessages()
{
	int c = m_UserMessages.Count();
	for ( int i = 0; i < c; ++i )
	{
		delete m_UserMessages[ i ];
	}
	m_UserMessages.RemoveAll();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *name - 
// Output : int
//-----------------------------------------------------------------------------
int CUserMessages::LookupUserMessage( const char *name )
{
	int idx = m_UserMessages.Find( name );
	if ( idx == m_UserMessages.InvalidIndex() )
	{
		return -1;
	}

	return idx;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : index - 
// Output : int
//-----------------------------------------------------------------------------
int CUserMessages::GetUserMessageSize( int index )
{
	if ( index < 0 || index >= (int)m_UserMessages.Count() )
	{
		Error( "CUserMessages::GetUserMessageSize( %i ) out of range!!!\n", index );
	}

	CUserMessage *e = m_UserMessages[ index ];
	return e->size;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : index - 
// Output : char const
//-----------------------------------------------------------------------------
const char *CUserMessages::GetUserMessageName( int index )
{
	if ( index < 0 || index >= (int)m_UserMessages.Count() )
	{
		return NULL;
	}

	return m_UserMessages.GetElementName( index );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : index - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CUserMessages::IsValidIndex( int index )
{
	return m_UserMessages.IsValidIndex( index );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *name - 
//			size - -1 for variable size
//-----------------------------------------------------------------------------
void CUserMessages::Register( const char *name, int size )
{
	Assert( name );
	int idx = m_UserMessages.Find( name );
	if ( idx != m_UserMessages.InvalidIndex() )
	{
		Error( "CUserMessages::Register '%' already registered\n", name );
	}

	CUserMessage * entry = new CUserMessage;
	entry->size = size;
	entry->name = name;

	m_UserMessages.Insert( name, entry );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *name - 
//			hook - 
//-----------------------------------------------------------------------------
void CUserMessages::HookMessage( const char *name, pfnUserMsgHook hook )
{
#if defined( CLIENT_DLL )
	Assert( name );
	Assert( hook );

	int idx = m_UserMessages.Find( name );
	if ( idx == m_UserMessages.InvalidIndex() )
	{
		DevMsg( "CUserMessages::HookMessage:  no such message %s\n", name );
		Assert( 0 );
		return;
	}

	int i = m_UserMessages[ idx ]->clienthooks.AddToTail();
	m_UserMessages[ idx ]->clienthooks[i] = hook;

#else
	Error( "CUserMessages::HookMessage called from server code!!!\n" );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pszName - 
//			iSize - 
//			*pbuf - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CUserMessages::DispatchUserMessage( int msg_type, bf_read &msg_data )
{
	return true;
}

// Singleton
static CUserMessages g_UserMessages;
// Expose to rest of .dll
CUserMessages *usermessages = &g_UserMessages;
