#ifndef _CMENU_H_
#define _CMENU_H_

#include "Include.h"

class cMouse
{
public:
	bool IsInBox(int x, int y, int w, int h);
	bool KeyActive(int val);
	void ClickHandler();

public:
	bool bMouse1pressed,
		bMouse2pressed,
		bMouse1released,
		bMouse2released;
};

class CMenu
{
public:
	void CloseAllComboBoxes();
	void DrawWindow(int x, int y, int w, int h, char* cName);
	void DrawGroupBox(int x, int y, int w, int h, char* Name);
	void DrawComboBox(int x, int y, char* Name, int &Selection, int num, int max, char** structs, char* Description);
	void DrawComboBox2(int x, int y, char* Name, int &Selection, int num, int max, char** structs, char* Description);
	void DrawCheckBox(int x, int y, const char* szString, bool &toggle, char* Description);
	void DrawTabs(const char* Name, int tab, int x, int y);
	void MenuDrawTabs(const char* Name, int tab, int x, int y);
	void MenuDrawTabs2(const char* Name, int tab, int x, int y);
	void DrawCloseButton(int x, int y);
	void DrawRadar(int x, int y);
	void Slider(int x, int y, bool bInt, float &iInput, float iMin, float iMax, char* Name, char* Description);
	void DrawPlayerList();
	void DrawMainFrame(int x, int y, int w, int h);
	void DrawMonitor(int x, int y);
	void DrawMenu();
};

#endif