#ifndef __MACRO_H__
#define __MACRO_H__

#include "sdk.h"


enum net_lib
{
	RECV_BUFFER = 4096,
	NETLIB_REASON_INITIALIZE = 0,
	NETLIB_REASON_CONNECT = 1,
	NETLIB_REASON_CONNECTION_FAILED = 2,
	NETLIB_REASON_DISCONNECT = 3,
	NETLIB_REASON_DATA = 4,
	TCP_FLAG_TEXT = (1 << 0),
};

enum irc_text_strip
{
	IRC_EMPTY = 0x0,
	IRC_COLOR = 0x03,
	IRC_COLOR1 = 0x030,
	IRC_COLOR2 = 0x034,
	IRC_BOLD = 0x02,
	IRC_UNDERLINE = 0x1F,
	IRC_REVERSE = 0x16,
	IRC_PLAIN = 0x0F,
	HIGHLIGHT_BOLD = 0x01,
	HIGHLIGHT_UNDERLINE = 0x02,
	HIGHLIGHT_INVERSE = 0x04,
};






enum gui_windows_number
{
	HACKCONSOLE = 0,
	COLORTOOL = 1,
	OVMAP = 2,
	WINAMP = 3,
	STATSBOX = 4,
	RADAR = 5,
	IRCCONSOLE = 6,
	HLSS = 7,
	MENU = 8,
	SNAKE = 9,
	TETRIS = 10,
	CROSSHAIRSELECTOR = 11,
	FONTSELECTOR = 12,
	CREDITS_BOX = 13,
	ZOOMVIEWMIRROR = 14,
	SPYVIEWMIRROR = 15,
	REARVIEWMIRROR = 16,
	TICTACTOE = 17,
	PONG = 18,
};

enum gui_windows_drag
{ 
	DRAGGING_NONE = -1, 
	DRAGGING_XY, 
	DRAGGING_W, 
	DRAGGING_H, 
	DRAGGING_WH, 
	DRAGGING_SLIDERH, 
	DRAGGING_SLIDERV 
}; 

// adds to the font
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC			= 0x001,
	FONTFLAG_UNDERLINE		= 0x002,
	FONTFLAG_STRIKEOUT		= 0x004,
	FONTFLAG_SYMBOL			= 0x008,
	FONTFLAG_ANTIALIAS		= 0x010,
	FONTFLAG_GAUSSIANBLUR	= 0x020,
	FONTFLAG_ROTARY			= 0x040,
	FONTFLAG_DROPSHADOW		= 0x080,
	FONTFLAG_ADDITIVE		= 0x100,
	FONTFLAG_OUTLINE		= 0x200,
	FONTFLAG_CUSTOM			= 0x400,		// custom generated font - never fall back to asian compatibility mode
};

//a shit buffer size
#define MSL	1024

//Fake Name (Mattdog Paranoid) :P
#define _Sleep Sleep
#define _CreateThread CreateThread
#define _GetModuleHandle GetModuleHandle
#define _GetProcAddress GetProcAddress
#define _CreateInterfaceFn CreateInterfaceFn
#define _GetCurrentProcessId GetCurrentProcessId
#define _OpenFileMapping OpenFileMapping
#define _CreateFileMapping CreateFileMapping
#define _MapViewOfFile MapViewOfFile
#define _CreateThread CreateThread
#define _VirtualLock VirtualLock
#define _ExitProcess ExitProcess
#define _MessageBox MessageBox
#define _RtlInitializeCriticalSection InitializeCriticalSection
#define _RtlEnterCriticalSection EnterCriticalSection
#define _RtlLeaveCriticalSection LeaveCriticalSection
#define _VirtualFreeEx VirtualFreeEx
#define _GetTickCount GetTickCount
#define _lstrlen strlen

//Patrick Macro
#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))
#define RED(COLORCODE)	((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)	((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)((int) COLORCODE & 0xFF )
#define RGBA(COLORCODE) RED( COLORCODE ), GREEN( COLORCODE ), BLUE( COLORCODE ), ALPHA( COLORCODE )

//Static shortcut for the most used color
#define CHEAT_ORANGE COLORCODE( 255, 100, 000, 255 )
#define CHEAT_YELLOW COLORCODE( 255, 255, 0, 255 )
#define CHEAT_PURPLE COLORCODE( 55, 25, 128, 255 )
#define CHEAT_RED COLORCODE( 255, 0, 0, 255 )
#define CHEAT_GREEN COLORCODE( 0, 255, 0, 255 )
#define CHEAT_BLUE COLORCODE( 0, 0, 255, 255 )
#define CHEAT_CYAN COLORCODE( 0, 255, 255, 255 )
#define CHEAT_BLACK COLORCODE( 0, 0, 0, 255 )
#define CHEAT_WHITE COLORCODE( 255, 255, 255, 255 )
#define CHEAT_RANDOM COLORCODE( valverand::RandomInt(1,255),valverand::RandomInt(1,255),valverand::RandomInt(1,255), 255 )

//IN_keys
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
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2


#define	HIDEHUD_ALL					( 1<<2 )
#define HIDEHUD_CHAT				( 1<<7 )	// Hide all communication elements (saytext, voice icon, etc)

//console scroll (irc too)
#define WM_MOUSEWHEEL					WM_MOUSELAST+1
#define WHEEL_DELTA                     120
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))

//Aimbot Shit
#define M_RADPI 57.295779f
#define SQUARE( a ) a*a

//ShortCut Shit
#define GameConsoleRandomColorPrint Main.m_pHl2GameConsole->RandomColorPrint
#define GameConsoleErrorPrint		Main.m_pHl2GameConsole->ErrorPrintf
#define CheatConsolePrint			Main.m_pCheatConsole->Printf
#define HUDPrint					Main.m_pDraw->SetHudMessage
#define FILLRGBA					Main.m_pDraw->FillRGBA
#define GUIWINDOW					Ikaros.m_pDraw->vWindow

//GUI Stuff
#define V_A( x ) (x < 0 ? -x : x)//Absolute Value : -x : x
#define V_0( x ) (x < 0 ? -x : 0)//Value zero : -x : 0

#endif