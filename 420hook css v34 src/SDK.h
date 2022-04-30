#ifndef _public_H_
#define _public_H_
#define CLIENT_DLL

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string.h>
#include <mmsystem.h>

using namespace std;

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4541 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4183 )
#pragma warning( disable : 4518 )
#pragma warning( disable : 4228 )
#pragma warning( disable : 4273 )
#pragma warning( disable : 4996 )

#define DO_ONCE( arg )				\
	static bool UnIqUe_ONC3 = false;\
	if( UnIqUe_ONC3 == false )\
{									\
	UnIqUe_ONC3 = true;				\
	arg								\
}

#define TEXTURE_GROUP_LIGHTMAP				"Lightmaps"
#define TEXTURE_GROUP_WORLD					"World textures"
#define TEXTURE_GROUP_MODEL					"Model textures"
#define TEXTURE_GROUP_VGUI					"VGUI textures"
#define TEXTURE_GROUP_PARTICLE				"Particle textures"
#define TEXTURE_GROUP_DECAL					"Decal textures"
#define TEXTURE_GROUP_SKYBOX				"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS		"ClientEffect textures"
#define TEXTURE_GROUP_OTHER					"Other textures"
#define TEXTURE_GROUP_PRECACHED				"Precached"
#define TEXTURE_GROUP_CUBE_MAP				"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET			"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED			"Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER	"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP	"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"

#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
//#define CHAR_TEX_UNUSED		'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
//#define CHAR_TEX_UNUSED		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
//#define CHAR_TEX_UNUSED		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

#define WEAPON_NULL				0
#define WEAPON_AK47				1
#define WEAPON_AUG				2
#define WEAPON_AWP				3
#define WEAPON_DEAGLE			4
#define WEAPON_ELITES			5
#define WEAPON_FAMAS			6
#define WEAPON_FIVESEVEN		7
#define WEAPON_G3SG1			8
#define WEAPON_GALIL			9
#define WEAPON_GLOCK18			10
#define WEAPON_M249				11
#define WEAPON_M3				12
#define WEAPON_M4A1				13
#define WEAPON_MAC10			14
#define WEAPON_MP5				15
#define WEAPON_P228				16
#define WEAPON_P90				17
#define WEAPON_SCOUT			18
#define WEAPON_SG550			19  
#define WEAPON_SG552			20 
#define WEAPON_TMP				21
#define WEAPON_UMP45			22
#define WEAPON_USP45			23
#define WEAPON_XM1014			24
#define WEAPON_KNIFE			25
#define WEAPON_FLASHBANG		26 
#define WEAPON_HE_GRENADE		27
#define WEAPON_SMOKE_GRENADE	28 
#define WEAPON_C4				29

#include "public/cdll_int.h"
#include "public/iprediction.h"
#include "public/mathlib.h"
#include "public/cl_dll/imessagechars.h"
#include "public/cl_dll/cdll_client_int.h"
#include "public/cl_dll/cbase.h"
#include "public/bone_setup.h"
#include "public/cl_dll/c_baseanimating.h"
#include "public/cl_dll/c_basecombatweapon.h"
#include "public/cl_dll/c_baseplayer.h"
#include "public/icliententitylist.h"
#include "public/engine/ivmodelrender.h"
#include "public/engine/ivdebugoverlay.h"
#include "public/engine/ivmodelinfo.h"
#include "public/ienginevgui.h"
#include "public/vgui/IClientPanel.h"
#include "public/vgui/IPanel.h"
#include "public/vgui/ISurface.h"
#include "public/vgui_controls/Panel.h"
#include "public/engine/IEngineTrace.h"
#include "public/VGuiMatSurface/IMatSystemSurface.h"
#include "public/input.h"
#include "public/game_shared/basecombatweapon_shared.h"
#include "public/igameevents.h"
#include "public/materialsystem/IMaterialVar.h"
#include "public/cl_dll/game_controls/commandmenu.h"
#include "public/istudiorender.h"
#include "public/icvar.h"
#include "public/game_shared/usercmd.h"
#include "public/inetchannel.h"
#include "public/dt_recv.h"
#include "public/tier1/convar.h"
#include "public/tier1/checksum_md5.h"
#include "public/game_shared/util_shared.h"
#include "public/engine/ivmodelrender.h"
#include "public/cl_dll/prediction.h"
#include "public/game_shared/gamemovement.h"
#include "public/cl_dll/iclientmode.h"
#include "public/GameUI/IGameConsole.h"

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)

#define	HIDEHUD_WEAPONSELECTION		( 1<<0 )	// Hide ammo count & weapon selection
#define	HIDEHUD_FLASHLIGHT			( 1<<1 )
#define	HIDEHUD_ALL					( 1<<2 )
#define HIDEHUD_HEALTH				( 1<<3 )	// Hide health & armor / suit battery
#define HIDEHUD_PLAYERDEAD			( 1<<4 )	// Hide when local player's dead
#define HIDEHUD_NEEDSUIT			( 1<<5 )	// Hide when the local player doesn't have the HEV suit
#define HIDEHUD_MISCSTATUS			( 1<<6 )	// Hide miscellaneous status elements (trains, pickup history, death notices, etc)
#define HIDEHUD_CHAT				( 1<<7 )	// Hide all communication elements (saytext, voice icon, etc)
#define	HIDEHUD_CROSSHAIR			( 1<<8 )	// Hide crosshairs
#define	HIDEHUD_VEHICLE_CROSSHAIR	( 1<<9 )	// Hide vehicle crosshair
#define HIDEHUD_INVEHICLE			( 1<<10 )
#define HIDEHUD_SCOPE				( 1<<11 )

void speak( char *pszText, ... );

#define M_RADPI 57.295779513082f
#define SQUARE( a ) a*a

#endif