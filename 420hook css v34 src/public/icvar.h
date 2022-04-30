//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef ICVAR_H
#define ICVAR_H
#ifdef _WIN32
#pragma once
#endif

#include "convar.h"
#include "interface.h"

//-----------------------------------------------------------------------------
// Purpose: DLL interface to ConVars/ConCommands
//-----------------------------------------------------------------------------

class ICvar
{
public:
	virtual void			RegisterConCommandBase ( ConCommandBase *variable ) = 0;
	virtual ConVar			*FindVar ( const char *var_name ) = 0; 
	virtual const ConVar	*FindVar ( const char *var_name ) const = 0;
	virtual ConCommandBase	*GetCommands( void ) = 0; 
};

class ICvar_new
{
public:
	virtual void*			Unknown0( void ) = 0; // C0 31 9A 0D
	virtual void*			Unknown1( void ) = 0; // D0 31 9A 0D
	virtual void*			Unknown2( void ) = 0; // E0 31 9A 0D
	virtual void*			passVACInterface( void ) = 0; // 30 3D 9A 0D
	virtual void			RegisterConCommandBase ( ConCommandBase *variable ) = 0; // 50 3D 9A 0D
	virtual char const*		GetCommandLineValue( char const *pVariableName ) = 0; // F0 34 9A 0D
	virtual void*			Unknown3( void ) = 0; //90 35 9A 0D
	virtual ConVar			*FindVar ( const char *var_name ) = 0; // 70 3F 9A 0D
	virtual const ConVar	*FindVar ( const char *var_name ) const = 0; // 20 40 9A 0D
	virtual ConCommandBase	*GetCommands( void ) = 0; // 20 3D 9A 0D
	/*
	A0 3B 9A 0D| B0 3B 9A 0D | C0 3B 9A 0D |  
	*/
};


#define VENGINE_CVAR_INTERFACE_VERSION		"VEngineCvar002"
#define VENGINE_CVAR_INTERFACE_VERSION_NEW	"VEngineCvar003"

extern ICvar *cvar;

#endif // ICVAR_H
