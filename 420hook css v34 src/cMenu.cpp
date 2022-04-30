#include "cMenu.h"
#include "CHitmarker.h"

tagPOINT Mouse;
int SavedX, SavedY;
bool bCfg = true;
bool bOnze = true;
bool bSave = false;
bool bPanic = false;
bool bReset = false;
bool bSaveMonitorPos = false;
bool bSavePos = true;
bool bLock = false;
bool bClick = true;
bool bLoad = true;
bool bMouseOnce = true;
int Selection = 1, iSel = 0;

//char* Keys[] = { "Press Any Key", "L Mouse", "R Mouse", "Cancel", "M Mouse", "", "", "", "Backspace", "Tab", "", "", "Clear", "Return", "", "", "Shift", "Ctrl", "Alt", "Pause", "Caps Lock", "", "", "", "", "", "", "Escape", "", "", "", "", "Spacebar", "Page up", "Page down", "End", "Home", "L Arrow", "Up Arrow", "R Arrow", "D Arrow", "Select", "Print", "Execute", "P.Screen", "Insert", "Delete", "Help", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "L.Menu", "R.Menu", "App", "", "Sleep", "Num 0", "Num 1", "Num 2", "Num 3", "Num 4", "Num 5", "Num 6", "Num 7", "Num 8", "Num 9", "Multiply", "Add", "Separator", "Subtract", "Decimal", "Divide", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "", "", "", "", "", "", "", "", "NumLock", "ScrLock", "Equals", "", "", "", "", "", "", "", "", "", "", "", "", "", "LShift", "RShift", "LCtrl", "RCtrl", "LMenu", "RMenu" };
char* TrigBox[7] = { "All", "Head", "Chest", "Pelvis", "Arms", "Legs", "Multi Hitbox" };
char* AimSelect[3] = { "backtrack", "Field of View", "Next Shot" };
char* Removals[3] = { "Off", "Only Aimbot", "Always" };
char* AntiAimX[7] = { "SMAC/KAC Safe", "Normal", "Configurable", "Static", "Static Up", "Off", "Random" };
char* AntiAimY[11] = { "Normal", "Jitter", "Configurable", "Spinhack", "Fake Sideways", "Fake Static", "Static", "StaticJitter", "CustomFake", "FakeBackwards", "Fake Forwards" };
char* Keys[11] = { "MouseW", "LMouse", "RMouse", "Mouse 4", "Mouse 5", "Shift", "Control", "Alt", "Q", "F", "E" };
char* AimMode[2] = { "Legit", "Rage" };
char* TrigMode[3] = { "Normal", "No Spread", "Seed"};
char* ESPStyle[3] = { "No", "Dynamic", "InterwebZ" };
char* ESPHealth[4] = { "Off", "Text", "Healthbar", "InterwebZ" };
char* Crosshair[7] = { "Off", "Dot", "Small Cross", "Rainbow", "Box", "Box2", "Swastika" };
char* RecoilCrosshair[7] = { "Off", "Dot", "Small Cross", "Rainbow", "Box", "Box2", "Swastika" };
char* Roundsay[6] = { "Off", "420hook", "999hook", "game bandit 1.1", "bennyhook.pw", "polenware.pw"};
char* ChatSpam[4] = { "Off", "coverme", "sectorclear", "enemydown" };
char* ChatSpam2[4] = { "Off", "ez", "noob", "shit server" };
char* CloseButtons[2] = { "X", "Swastika" };
char* OpenTabsMenu[2] = { "Page 1", "Page 2" };
char* OpenTabsMenu2[2] = { "Colors1", "Colors2" };
char* OpenTabsMenu3[1] = { "Other" };
char* PlayerChamsMaterial[2] = { "UnlitGeneric", "Wireframe" };
char* WeaponChamsMaterial[2] = { "UnlitGeneric", "Wireframe" };
char* KillSay[6] = { "Off", "ez win", "1", "1tap", "noob", "sucker" };
char* AutoBuy[4] = { "Off", "HvH", "HvH2", "HvH3" };
char* HandsPosition[2] = { "Left", "Right" };
char* WatermarkTextColor[2] = {"None", "Rainbow" };
char* DisconnectReason[4] = { "Disconnect by user", "VAC BAN!!!", "420hook", "Gay Shit" };
char* SpinMode[3] = { "Slow", "Normal", "Fast" };
char* FakelagMode[2] = { "Normal", "Adaptive" };
char* ThirdpersonAA[2] = { "Real", "Fake" };
char* AimMethod[3] = { "Hitbox", "Bone", "Hack vs Hack" };
char* Strafe[3] = { "Off", "Assist", "Full" };
char* AAAPitch[4] = { "Off", "Auto", "Up", "Down" };
char* AAAYaw[7] = { "Off", "Inverse", "Half Left", "Half Right", "Spin", "FakeBackwards", "Static Jitter Spin" };

enum Tabs
{
	tab0,
	tab1,
	tab2,
	tab3,
	tab4,
	tab5,
	tab6,
	tab7,
	tab8,
	tab9,
	tab10,
	tab11,
	tab12,
	tab13,
	tab14,
	tab15,
	tab16,
	tab17,
	tab18
};

struct OpenTab
{
	Tabs ActiveTab;
} openTab;

struct sComboBox
{
	bool Open, Active;
	int iSelect;
	char* Name;
} cComboBox[21];

void ESPBox(int x, int y, int x1, int y1, int r, int g, int b, int a)
{
	Ikaros.m_pDraw->Border(x, y, x1, y1, 1, r, g, b, a);
	Ikaros.m_pDraw->Border(x - 1, y - 1, x1 + 2, y1 + 2, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->Border(x + 1, y + 1, x1 - 2, y1 - 2, 1, 0, 0, 0, 255);
}

void DrawMouse(int x, int y)
{
	Ikaros.m_pDraw->FillRGBA(x, y, 1, 17, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x, y + 16, 2, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 2, y + 15, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 3, y + 14, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 4, y + 13, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 5, y + 14, 1, 2, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 6, y + 16, 1, 2, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 7, y + 18, 2, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 8, y + 14, 1, 2, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 9, y + 16, 1, 2, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 7, y + 12, 1, 2, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 7, y + 12, 5, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 11, y + 11, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 10, y + 10, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 9, y + 9, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 8, y + 8, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 7, y + 7, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 6, y + 6, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 5, y + 5, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 4, y + 4, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 3, y + 3, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 2, y + 2, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 1, y + 1, 1, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->FillRGBA(x + 1, y + 2, 1, 14, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 2, y + 3, 1, 12, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 3, y + 4, 1, 10, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 4, y + 5, 1, 8, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 5, y + 6, 1, 8, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 6, y + 7, 1, 9, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 7, y + 8, 1, 4, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 8, y + 9, 1, 3, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 9, y + 10, 1, 2, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 10, y + 11, 1, 1, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 7, y + 14, 1, 4, 255, 255, 255, 255);
	Ikaros.m_pDraw->FillRGBA(x + 8, y + 16, 1, 2, 255, 255, 255, 255);
}

bool cMouse::IsInBox(int x, int y, int w, int h)
{
	if (Ikaros.m_pCvars->iMouseX >= x &&
		Ikaros.m_pCvars->iMouseX <= x + w &&
		Ikaros.m_pCvars->iMouseY >= y &&
		Ikaros.m_pCvars->iMouseY <= y + h)
		return true;
	else
		return false;
}

void cMouse::ClickHandler()
{
	static bool click[1];
	if ((GetAsyncKeyState(VK_LBUTTON)))
	{
		bMouse1pressed = true;
		if (!click[1])
			click[0] = true;
	}
	else if (!(GetAsyncKeyState(VK_LBUTTON)))
	{
		if (bMouse1pressed)
			bMouse1released = true;
		else
		{
			bMouse1released = false;
		}
		bMouse1pressed = false;
		click[0] = false;
		click[1] = false;
	}

	if (!click[1] && click[0])
	{
		//PlaySoundA(clicksound, 0, SND_NOWAIT | SND_ASYNC | SND_MEMORY);
		click[1] = true;
	}

	if ((GetAsyncKeyState(VK_RBUTTON)))
	{
		bMouse2pressed = true;
	}
	else if (!(GetAsyncKeyState(VK_RBUTTON)))
	{
		if (bMouse2pressed)
			bMouse2released = true;
		else
		{
			bMouse2released = false;
		}
		bMouse2pressed = false;
	}
}

void CfgSelectButton(int x, int y)
{
	if (Ikaros.m_pMouse->IsInBox(x, y, 45, 15))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor21, Ikaros.m_pCvars->DrawColor22, Ikaros.m_pCvars->DrawColor23, Ikaros.m_pCvars->DrawColor24);
		if (Ikaros.m_pMouse->bMouse1released)
		{
			bIsRage = !bIsRage;
			Ikaros.m_pSettings->LoadSettings();
		}	
	}
	else
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
	}

	if (bIsRage)
	{
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Rage");
	}
	else
	{
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Legit");
	}
}

void SaveButton(int x, int y)
{
	if (bSave)
	{
		Ikaros.m_pSettings->SaveSettings();
		bSave = false;
	}
	if (Ikaros.m_pMouse->IsInBox(x, y, 45, 15))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor21, Ikaros.m_pCvars->DrawColor22, Ikaros.m_pCvars->DrawColor23, Ikaros.m_pCvars->DrawColor24);
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Save");
		if (Ikaros.m_pMouse->bMouse1released)
			bSave = true;
	}
	else
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Save");
	}
}

void PanicButton(int x, int y)
{
	if (bPanic)
	{
		system("taskkill /im hl2.exe /f");
		bPanic = false;
	}
	if (Ikaros.m_pMouse->IsInBox(x, y, 45, 15))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor21, Ikaros.m_pCvars->DrawColor22, Ikaros.m_pCvars->DrawColor23, Ikaros.m_pCvars->DrawColor24);
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Panic");
		if (Ikaros.m_pMouse->bMouse1released)
			bPanic = true;
	}
	else
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Panic");
	}
}

void ResetColors(int x, int y)
{
	if (bReset)
	{
		Ikaros.m_pSettings->ResetColors();
		bReset = false;
	}
	if (Ikaros.m_pMouse->IsInBox(x, y, 45, 15))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor21, Ikaros.m_pCvars->DrawColor22, Ikaros.m_pCvars->DrawColor23, Ikaros.m_pCvars->DrawColor24);
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Reset");
		if (Ikaros.m_pMouse->bMouse1released)
			bReset = true;
	}
	else
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->CenterText(x + 22, y - 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Reset");
	}
}

void DrawInfo(char* Text)
{
	if (openTab.ActiveTab == 3)
	{
		Ikaros.m_pDraw->Text(Ikaros.m_pCvars->iMenuX + 165, Ikaros.m_pCvars->iMenuY + 465, 255, 255, 255, 255, Text);
	}
	else
	{
		Ikaros.m_pDraw->Text(Ikaros.m_pCvars->iMenuX + 165, Ikaros.m_pCvars->iMenuY + 445, 255, 255, 255, 255, Text);
	}
}

void CMenu::DrawWindow(int x, int y, int w, int h, char* cName)
{
	if (openTab.ActiveTab == tab9 || openTab.ActiveTab == tab3)
	{
		Ikaros.m_pDraw->FillRGBA(x + 5, y + 25, w - 145, h - 5, Ikaros.m_pCvars->DrawColor5, Ikaros.m_pCvars->DrawColor6, Ikaros.m_pCvars->DrawColor7, Ikaros.m_pCvars->DrawColor8);//Background
		Ikaros.m_pDraw->Border(x, y, w - 140, h + 20, 6, Ikaros.m_pCvars->DrawColor, Ikaros.m_pCvars->DrawColor2, Ikaros.m_pCvars->DrawColor3, Ikaros.m_pCvars->DrawColor4);//Border
		Ikaros.m_pDraw->FillRGBA(x, y, w - 140, 25, Ikaros.m_pCvars->DrawColor, Ikaros.m_pCvars->DrawColor2, Ikaros.m_pCvars->DrawColor3, Ikaros.m_pCvars->DrawColor4);//Topbar
		Ikaros.m_pDraw->Text(x + 6, y + 6, Ikaros.m_pCvars->WindowsTextColor1, Ikaros.m_pCvars->WindowsTextColor2, Ikaros.m_pCvars->WindowsTextColor3, Ikaros.m_pCvars->WindowsTextColor4, cName);//Name
		static float rainbow;
		rainbow += 0.005f;
		if (rainbow > 1.f)
			rainbow = 0.f;
		Color hsb = Color::FromHSB(rainbow, 1.f, 1.f);
		if (Ikaros.m_pCvars->bRainbowOutline)
		{
			Ikaros.m_pDraw->Border(x, y, w - 135, h + 25, 1, hsb.r(), hsb.g(), hsb.b(), 255);//Black 1px Outline
		}
		else
		{
			Ikaros.m_pDraw->Border(x, y, w - 135, h + 25, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));//Black 1px Outline
		}
		if (Ikaros.m_pCvars->bRainbowInline)
		{
			Ikaros.m_pDraw->Border(x + 5, y + 25, w - 145, h - 5, 1, hsb.r(), hsb.g(), hsb.b(), 255);//Black 1px Inline
		}
		else
		{
			Ikaros.m_pDraw->Border(x + 5, y + 25, w - 145, h - 5, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));//Black 1px Inline
		}
	}
	else
	{
		Ikaros.m_pDraw->FillRGBA(x + 5, y + 25, w - 145, h - 25, Ikaros.m_pCvars->DrawColor5, Ikaros.m_pCvars->DrawColor6, Ikaros.m_pCvars->DrawColor7, Ikaros.m_pCvars->DrawColor8);//Background
		Ikaros.m_pDraw->Border(x, y, w - 140, h, 6, Ikaros.m_pCvars->DrawColor, Ikaros.m_pCvars->DrawColor2, Ikaros.m_pCvars->DrawColor3, Ikaros.m_pCvars->DrawColor4);//Border
		Ikaros.m_pDraw->FillRGBA(x, y, w - 140, 25, Ikaros.m_pCvars->DrawColor, Ikaros.m_pCvars->DrawColor2, Ikaros.m_pCvars->DrawColor3, Ikaros.m_pCvars->DrawColor4);//Topbar
		Ikaros.m_pDraw->Text(x + 6, y + 6, Ikaros.m_pCvars->WindowsTextColor1, Ikaros.m_pCvars->WindowsTextColor2, Ikaros.m_pCvars->WindowsTextColor3, Ikaros.m_pCvars->WindowsTextColor4, cName);//Name
		static float rainbow;
		rainbow += 0.005f;
		if (rainbow > 1.f)
			rainbow = 0.f;
		Color hsb = Color::FromHSB(rainbow, 1.f, 1.f);
		if (Ikaros.m_pCvars->bRainbowOutline)
		{
			Ikaros.m_pDraw->Border(x, y, w - 135, h + 5, 1, hsb.r(), hsb.g(), hsb.b(), 255);//Black 1px Outline
		}
		else
		{
			Ikaros.m_pDraw->Border(x, y, w - 135, h + 5, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));//Black 1px Outline
		}
		if (Ikaros.m_pCvars->bRainbowInline)
		{
			Ikaros.m_pDraw->Border(x + 5, y + 25, w - 145, h - 25, 1, hsb.r(), hsb.g(), hsb.b(), 255);//Black 1px Inline
		}
		else
		{
			Ikaros.m_pDraw->Border(x + 5, y + 25, w - 145, h - 25, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));//Black 1px Inline
		}
	}
}

void CMenu::DrawGroupBox(int x, int y, int w, int h, char* Name)
{
	Ikaros.m_pDraw->FillRGBA(x, y, w, h, RED(Grey1), GREEN(Grey1), BLUE(Grey1), ALPHA(Grey1));//Background
	Ikaros.m_pDraw->FillRGBA(x, y, w, 20, Ikaros.m_pCvars->DrawColor9, Ikaros.m_pCvars->DrawColor10, Ikaros.m_pCvars->DrawColor11, Ikaros.m_pCvars->DrawColor12);//Topbar
	Ikaros.m_pDraw->Text(x + 3, y + 4, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), Name);
}

void CMenu::DrawComboBox(int x, int y, char* Name, int &Selection, int num, int max, char** structs, char* Description)
{
	cComboBox[num].Name = structs[Selection];
	Ikaros.m_pDraw->FillRGBA(x + 2, y - 3, 85, 2, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Top
	Ikaros.m_pDraw->FillRGBA(x + 2, y - 3, 2, 22, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Left
	Ikaros.m_pDraw->FillRGBA(x + 87, y - 3, 2, 22, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Right
	if (cComboBox[num].Active)
	{
		cComboBox[num].iSelect = Selection;
		if (Ikaros.m_pMouse->IsInBox(x + 25, y - 1, 69, 23) && Ikaros.m_pMouse->bMouse1released && bClick)
		{
			cComboBox[num].Open = true;
			cComboBox[num].Active = false;
		}
		Ikaros.m_pDraw->FillRGBA(x + 2, y + 17, 85, 2, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Bottom
		Ikaros.m_pDraw->Text(x + 8, y + 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), cComboBox[num].Name);
		if (Ikaros.m_pMouse->IsInBox(x + 2, y - 3, 85, 22) && !bLock)
		{
			DrawInfo(Description);
		}
	}
	else
	{
		if (cComboBox[num].Open)
		{
			bLock = true;
			DrawInfo(Description);
			for (int i = 0; i < max; i++)
			{
				Ikaros.m_pDraw->FillRGBA(x + 2, y + (i * 20), 87, 20, RED(BackgroundColor), GREEN(BackgroundColor), BLUE(BackgroundColor), ALPHA(BackgroundColor));//Back 1
				Ikaros.m_pDraw->FillRGBA(x + 2, y - 1 + (i * 20), 2, 24, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Left * count
				Ikaros.m_pDraw->FillRGBA(x + 87, y - 1 + (i * 20), 2, 24, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Right * count
				if (Ikaros.m_pMouse->IsInBox(x + 6, y + 1 + (i * 20), 79, 20))
				{
					Ikaros.m_pDraw->FillRGBA(x + 6, y + 1 + (i * 20), 79, 20, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//highlight selection
					if (Ikaros.m_pMouse->bMouse1released)
					{
						cComboBox[num].iSelect = i;
						cComboBox[num].Name = structs[cComboBox[num].iSelect];
						Selection = cComboBox[num].iSelect;
						cComboBox[num].Open = false;
						cComboBox[num].Active = true;
						bLock = false;
					}
				}
				Ikaros.m_pDraw->Text(x + 8, y + 3 + (i * 20), RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), structs[i]);//List struct
			}
			Ikaros.m_pDraw->FillRGBA(x + 2, y + 3 + (max * 20), 87, 2, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Bottom
		}
	}

	Ikaros.m_pDraw->Text(x + 92, y + 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), Name);
}

void CMenu::DrawComboBox2(int x, int y, char* Name, int &Selection, int num, int max, char** structs, char* Description)
{
	cComboBox[num].Name = structs[Selection];
	Ikaros.m_pDraw->FillRGBA(x + 2, y - 3, 135, 2, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Top
	Ikaros.m_pDraw->FillRGBA(x + 2, y - 3, 2, 22, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Left
	Ikaros.m_pDraw->FillRGBA(x + 135, y - 3, 2, 22, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Right
	if (cComboBox[num].Active)
	{
		cComboBox[num].iSelect = Selection;
		if (Ikaros.m_pMouse->IsInBox(x + 25, y - 1, 135, 23) && Ikaros.m_pMouse->bMouse1released && bClick)
		{
			cComboBox[num].Open = true;
			cComboBox[num].Active = false;
		}
		Ikaros.m_pDraw->FillRGBA(x + 2, y + 17, 135, 2, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Bottom
		Ikaros.m_pDraw->Text(x + 8, y + 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), cComboBox[num].Name);
		if (Ikaros.m_pMouse->IsInBox(x + 2, y - 3, 135, 22) && !bLock)
		{
			DrawInfo(Description);
		}
	}
	else
	{
		if (cComboBox[num].Open)
		{
			bLock = true;
			DrawInfo(Description);
			for (int i = 0; i < max; i++)
			{
				Ikaros.m_pDraw->FillRGBA(x + 2, y + (i * 20), 135, 20, RED(BackgroundColor), GREEN(BackgroundColor), BLUE(BackgroundColor), ALPHA(BackgroundColor));//Back 1
				Ikaros.m_pDraw->FillRGBA(x + 2, y - 1 + (i * 20), 2, 24, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Left * count
				Ikaros.m_pDraw->FillRGBA(x + 135, y - 1 + (i * 20), 2, 24, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Right * count
				if (Ikaros.m_pMouse->IsInBox(x + 6, y + 1 + (i * 20), 135, 20))
				{
					Ikaros.m_pDraw->FillRGBA(x + 6, y + 1 + (i * 20), 127, 20, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//highlight selection
					if (Ikaros.m_pMouse->bMouse1released)
					{
						cComboBox[num].iSelect = i;
						cComboBox[num].Name = structs[cComboBox[num].iSelect];
						Selection = cComboBox[num].iSelect;
						cComboBox[num].Open = false;
						cComboBox[num].Active = true;
						bLock = false;
					}
				}
				Ikaros.m_pDraw->Text(x + 8, y + 3 + (i * 20), RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), structs[i]);//List struct
			}
			Ikaros.m_pDraw->FillRGBA(x + 2, y + 3 + (max * 20), 135, 2, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);//Bottom
		}
	}

	Ikaros.m_pDraw->Text(x + 142, y + 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), Name);
}

void CMenu::CloseAllComboBoxes()
{
	for (int n = 0; n <= 21; n++)
	{
		cComboBox[n].Open = false;
		cComboBox[n].Active = true;
	}
	bLock = false;
}

void CMenu::DrawCheckBox(int x, int y, const char* szString, bool &toggle, char* Description)
{
	Ikaros.m_pDraw->FillRGBA(x + 2, y - 3, 14, 14, RED(Grey0), GREEN(Grey0), BLUE(Grey0), ALPHA(Grey0));
	if (Ikaros.m_pMouse->IsInBox(x + 2, y - 3, 14, 14))
	{
		Ikaros.m_pDraw->Border(x + 2, y - 3, 13, 13, 1, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		DrawInfo(Description);
	}
	if (Ikaros.m_pMouse->IsInBox(x + 4, y - 1, 10, 10) && Ikaros.m_pMouse->bMouse1released)
	{
		toggle =! toggle;
	}
	if (toggle)
	{
		Ikaros.m_pDraw->FillRGBA(x + 4, y - 1, 10, 10, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
	}
	Ikaros.m_pDraw->Text(x + 20, y - 2, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), szString);
}

void CMenu::DrawTabs(const char* Name, int tab, int x, int y)
{
	Ikaros.m_pDraw->FillRGBA(x, y, 90, 25, RED(Grey1), GREEN(Grey1), BLUE(Grey1), ALPHA(Grey1));

	static bool click[1];

	if (Ikaros.m_pMouse->IsInBox(x, y, 90, 25))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 90, 25, RED(Grey4), GREEN(Grey4), BLUE(Grey4), ALPHA(Grey4));
		if (Ikaros.m_pMouse->bMouse1released)
		{
			click[1] = true;
			if (Ikaros.m_pCvars->bButtonClickSound)
			{
				PlaySoundA(clicksound, 0, SND_NOWAIT | SND_ASYNC | SND_MEMORY);
			}
			CloseAllComboBoxes();

			if (tab == 1)
				openTab.ActiveTab = tab1;
			else if (tab == 2)
				openTab.ActiveTab = tab2;
			else if (tab == 3)
				openTab.ActiveTab = tab3;
			else if (tab == 4)
				openTab.ActiveTab = tab4;
			else if (tab == 5)
				openTab.ActiveTab = tab5;
			else if (tab == 6)
				openTab.ActiveTab = tab6;
			else if (tab == 7)
				openTab.ActiveTab = tab7;
			else if (tab == 8)
				openTab.ActiveTab = tab8;
			else if (tab == 9)
				openTab.ActiveTab = tab9;
			else if (tab == 10)
				openTab.ActiveTab = tab10;
			else if (tab == 11)
				openTab.ActiveTab = tab11;
			else if (tab == 12)
				openTab.ActiveTab = tab12;
			else if (tab == 13)
				openTab.ActiveTab = tab13;
			else if (tab == 14)
				openTab.ActiveTab = tab14;
			else if (tab == 15)
				openTab.ActiveTab = tab15;
			else if (tab == 16)
				openTab.ActiveTab = tab16;
		}
	}

	Ikaros.m_pDraw->Text(x + 17, y + 9, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), Name);
}

void CMenu::MenuDrawTabs(const char* Name, int tab, int x, int y)
{
	Ikaros.m_pDraw->FillRGBA(x, y, 70, 50, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(150));

	static bool click[1];

	if (Ikaros.m_pMouse->IsInBox(x, y, 70, 50))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 70, 50, RED(Grey4), GREEN(Grey4), BLUE(Grey4), ALPHA(Grey4));
		if (Ikaros.m_pMouse->bMouse1released)
		{
			click[1] = true;
			if (Ikaros.m_pCvars->bButtonClickSound)
			{
				PlaySoundA(clicksound, 0, SND_NOWAIT | SND_ASYNC | SND_MEMORY);
			}
			CloseAllComboBoxes();
			if (tab == 1)
				openTab.ActiveTab = tab1;
			else if (tab == 2)
				openTab.ActiveTab = tab2;
			else if (tab == 3)
				openTab.ActiveTab = tab3;
			else if (tab == 4)
				openTab.ActiveTab = tab4;
			else if (tab == 5)
				openTab.ActiveTab = tab5;
			else if (tab == 6)
				openTab.ActiveTab = tab6;
			else if (tab == 7)
				openTab.ActiveTab = tab7;
			else if (tab == 9)
				openTab.ActiveTab = tab9;
			else if (tab == 12)
				openTab.ActiveTab = tab12;
			else if (tab == 13)
				openTab.ActiveTab = tab13;
			else if (tab == 14)
				openTab.ActiveTab = tab14;
			else if (tab == 15)
				openTab.ActiveTab = tab15;
			else if (tab == 16)
				openTab.ActiveTab = tab16;
			else if (tab == 17)
				openTab.ActiveTab = tab17;
			else if (tab == 18)
				openTab.ActiveTab = tab18;
		}
	}

	Ikaros.m_pDraw->Text(x + 20, y + 15, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), Name);
}

void CMenu::MenuDrawTabs2(const char* Name, int tab, int x, int y)
{
	Ikaros.m_pDraw->FillRGBA(x, y, 85, 50, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(150));

	static bool click[1];

	if (Ikaros.m_pMouse->IsInBox(x, y, 85, 50))
	{
		Ikaros.m_pDraw->FillRGBA(x, y, 85, 50, RED(Grey4), GREEN(Grey4), BLUE(Grey4), ALPHA(Grey4));
		if (Ikaros.m_pMouse->bMouse1released)
		{
			click[1] = true;
			if (Ikaros.m_pCvars->bButtonClickSound)
			{
				PlaySoundA(clicksound, 0, SND_NOWAIT | SND_ASYNC | SND_MEMORY);
			}
			CloseAllComboBoxes();
			if (tab == 1)
				openTab.ActiveTab = tab1;
			else if (tab == 2)
				openTab.ActiveTab = tab2;
			else if (tab == 3)
				openTab.ActiveTab = tab3;
			else if (tab == 4)
				openTab.ActiveTab = tab4;
			else if (tab == 5)
				openTab.ActiveTab = tab5;
			else if (tab == 6)
				openTab.ActiveTab = tab6;
			else if (tab == 7)
				openTab.ActiveTab = tab7;
			else if (tab == 9)
				openTab.ActiveTab = tab9;
			else if (tab == 12)
				openTab.ActiveTab = tab12;
			else if (tab == 13)
				openTab.ActiveTab = tab13;
			else if (tab == 14)
				openTab.ActiveTab = tab14;
			else if (tab == 15)
				openTab.ActiveTab = tab15;
			else if (tab == 16)
				openTab.ActiveTab = tab16;
			else if (tab == 17)
				openTab.ActiveTab = tab17;
		}
	}

	Ikaros.m_pDraw->Text(x + 18, y + 15, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), Name);
}

void CMenu::DrawCloseButton(int x, int y)
{
	if (Ikaros.m_pCvars->iDrawCloseButton == 0)
	{
		if (Ikaros.m_pMouse->IsInBox(x, y, 45, 15))
		{
			Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, RED(Red1), GREEN(Red1), BLUE(Red1), ALPHA(Red1));
			if (Ikaros.m_pMouse->bMouse1released)
				Ikaros.m_pCvars->bShowGUI = false;
		}
		else
		{
			Ikaros.m_pDraw->FillRGBA(x, y, 45, 15, RED(Red0), GREEN(Red0), BLUE(Red0), ALPHA(Red0));
		}
		Ikaros.m_pDraw->Text(x + 19, y + 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "X");
	}
	else if (Ikaros.m_pCvars->iDrawCloseButton == 1)
	{
		if (Ikaros.m_pMouse->IsInBox(x - 7, y - 7, 15, 15))
		{
			Ikaros.m_pDraw->FillRGBA(x - 7, y, 15, 1, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
			Ikaros.m_pDraw->FillRGBA(x, y - 7, 1, 15, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));

			Ikaros.m_pDraw->FillRGBA(x, y - 7, 8, 1, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
			Ikaros.m_pDraw->FillRGBA(x - 7, y - 7, 1, 7, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
			Ikaros.m_pDraw->FillRGBA(x + 7, y, 1, 8, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
			Ikaros.m_pDraw->FillRGBA(x - 7, y + 7, 7, 1, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
			if (Ikaros.m_pMouse->bMouse1released)
				Ikaros.m_pCvars->bShowGUI = false;
		}
		else
		{
			Ikaros.m_pDraw->FillRGBA(x - 7, y, 15, 1, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
			Ikaros.m_pDraw->FillRGBA(x, y - 7, 1, 15, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));

			Ikaros.m_pDraw->FillRGBA(x, y - 7, 8, 1, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
			Ikaros.m_pDraw->FillRGBA(x - 7, y - 7, 1, 7, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
			Ikaros.m_pDraw->FillRGBA(x + 7, y, 1, 8, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
			Ikaros.m_pDraw->FillRGBA(x - 7, y + 7, 7, 1, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
		}
		//Ikaros.m_pDraw->Text(x + 19, y + 1, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "X");
	}
}

void CMenu::Slider(int x, int y, bool bInt, float &iInput, float iMin, float iMax, char* Name, char* Description)
{
	float sliderpos, barpos;

	if (Ikaros.m_pMouse->IsInBox(x, y + 3, 200, 9))
	{
		DrawInfo(Description);
	}

	if (Ikaros.m_pMouse->IsInBox(x, y + 3, 200, 9) && Ikaros.m_pMouse->bMouse1pressed)
	{
		sliderpos = (Ikaros.m_pCvars->iMouseX - x);
	}
	else
	{
		sliderpos = (((iInput * 100) * 2) / iMax);
	}

	iInput = (iMax * (sliderpos / 2) / 100);
	barpos = (sliderpos / 200 * 100) * 2;

	if (barpos > 200)
		barpos = 200;

	if (bInt)
	{
		double integral;
		float fraction = (float)modf(iInput, &integral);
		if (fraction >= 0.5)
			iInput += 1;
		iInput = (int)integral;
	}
		
	iInput = clamp(iInput, iMin, iMax);

	Ikaros.m_pDraw->FillRGBA(x, y + 5, 202, 5, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));

	Ikaros.m_pDraw->FillRGBA(x + 1, y + 6, barpos, 3, Ikaros.m_pCvars->DrawColor13, Ikaros.m_pCvars->DrawColor14, Ikaros.m_pCvars->DrawColor15, Ikaros.m_pCvars->DrawColor16);

	Ikaros.m_pDraw->FillRGBA(x + barpos - 2, y + 3, 5, 9, Ikaros.m_pCvars->DrawColor13, Ikaros.m_pCvars->DrawColor14, Ikaros.m_pCvars->DrawColor15, Ikaros.m_pCvars->DrawColor16);

	Ikaros.m_pDraw->Text(x + 208, y, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "%s: %0.1f", Name, iInput);
}

void DrawMonitorBar(int x, int y, int cur, const char* title)
{
	cur = clamp(cur, 0, 100);

	Ikaros.m_pDraw->FillRGBA(x, y, 30, 160, (int)(cur*2.55), (255 - (int)(cur*2.55)), 0, 255);
	Ikaros.m_pDraw->FillRGBA(x, y, 30, 160 - (cur * 1.6), RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
	Ikaros.m_pDraw->Text(x, y + 169, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), title);
}

void CMenu::DrawMonitor(int x, int y)
{
	if (Ikaros.m_pMouse->IsInBox(x, y, 160, 240) && Ikaros.m_pMouse->bMouse1pressed)
	{
		if (!bSaveMonitorPos)
		{
			Ikaros.m_pCvars->iMonitorSavePosX = Ikaros.m_pCvars->iMouseX - x;
			Ikaros.m_pCvars->iMonitorSavePosY = Ikaros.m_pCvars->iMouseY - y;
			bSaveMonitorPos = true;
		}
		Ikaros.m_pCvars->iMonitorPosX = Ikaros.m_pCvars->iMouseX;
		Ikaros.m_pCvars->iMonitorPosY = Ikaros.m_pCvars->iMouseY;
		Ikaros.m_pCvars->iMonitorPosX = Ikaros.m_pCvars->iMonitorPosX - Ikaros.m_pCvars->iMonitorSavePosX;
		Ikaros.m_pCvars->iMonitorPosY = Ikaros.m_pCvars->iMonitorPosY - Ikaros.m_pCvars->iMonitorSavePosY;
	}
	else
	{
		bSaveMonitorPos = false;
	}

	DrawWindow(x, y, 295, 240, g_pInput->m_fCameraInterceptingMouse ? "Monitor [thirdperson]" : "Monitor");

	//Speed
	int iSpeed = 0;
	float flPunch = 0.0f;
	float flAccuracy = 0.0f;
	float gabenplz = 0.0f;
	if (Ikaros.m_pNeeded->CrashCheck())
	{
		Vector vSpeed = Ikaros.m_pMyPlayer->BaseEnt()->m_vecVelocity;
		iSpeed = (int)vSpeed.Length2D();

		QAngle pAngle = Ikaros.m_pEntity->GetPunchAngle(Ikaros.m_pMyPlayer->BaseEnt());
		float flModifier = VectorNormalize( pAngle.Base( ) );
 
		flModifier -= ( 10.0f + flModifier * 0.5f ) * Ikaros.m_pGlobals->interval_per_tick;
		pAngle *= flModifier;
		flPunch = ((pAngle.x * 17.77777777777778) * 1.25);

		float flAccuracy = Ikaros.m_pNoSpread->GetVecSpread(gabenplz);
		gabenplz = (int)(flAccuracy * 66.67);
	}

	DrawMonitorBar( x + 12, y + 42, (-flPunch), "Recoil");
	DrawMonitorBar( x + 65, y + 42, (iSpeed * 0.2), "Speed");
	DrawMonitorBar( x + 118, y + 42, (gabenplz), "Spread");
}

void CMenu::DrawPlayerList()
{
	int in = 1;
	int x1 = Ikaros.m_pCvars->iMenuX + 10;
	int y1 = Ikaros.m_pCvars->iMenuY + 65;

	if (Ikaros.m_pEngine->IsConnected()
		&& Ikaros.m_pEngine->IsInGame()
		&& Ikaros.m_pMyPlayer->BaseEnt()
		&& !Ikaros.m_pEngine->IsTakingScreenshot()
		&& !Ikaros.m_pEngine->IsLevelMainMenuBackground())
	{
		player_info_t pInfo;
		//----------------------------------//
		for (int index = 0; index < Ikaros.m_pGlobals->maxClients; index++)
		{
			if (index == Ikaros.m_pEngine->GetLocalPlayer())
				continue;

			IClientEntity* ClientEntity = Ikaros.m_pEntList->GetClientEntity(index);

			if (ClientEntity == NULL)
				continue;

			C_BaseEntity* pBaseEntity = ClientEntity->GetBaseEntity();

			if (pBaseEntity == NULL)
				continue;

			if (Ikaros.m_pEntity->GetTeamID(pBaseEntity) == Ikaros.m_pEntity->GetTeamID(Ikaros.m_pMyPlayer->BaseEnt()))
				continue;

			if (Ikaros.m_pEngine->GetPlayerInfo(index, &pInfo))
			{
				Ikaros.m_pPlayerlist[index].index = index;

				if (in >= 24)
					in = 1;

				if (Ikaros.m_pCvars->bShowGUI && Ikaros.m_pCvars->bPlayerList)
				{
					if (Ikaros.m_pMouse->IsInBox(x1, y1 + 16 + ((in)* 13), 625, 12))
					{
						Ikaros.m_pDraw->FillRGBA(x1, y1 + 16 + ((in)* 13), 625, 12, RED(MenuColor), GREEN(MenuColor), BLUE(MenuColor) + 5, ALPHA(MenuColor));//highlight selection
						if ((Ikaros.m_pMouse->bMouse1pressed))
						{
							iSel = in;
							Selection = Ikaros.m_pPlayerlist[index].index;
						}
					}

					Ikaros.m_pDraw->Text(x1 + 10, y1 + 15 + ((in)* 13), RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "%s: %s", pInfo.name, pInfo.guid);
				}
				in++;
			}
		}
	}
}

void CMenu::DrawMainFrame(int x, int y, int w, int h)
{
	//Main Window
	DrawWindow(x, y, 800, 475, "420hook | October 25 2018 22:47");
	if (Ikaros.m_pCvars->iDrawCloseButton == 0)
	{
		DrawCloseButton(x + 615, y + 1);
	}
	else if (Ikaros.m_pCvars->iDrawCloseButton == 1)
	{
		DrawCloseButton(x + 650, y + 11);
	}

	//Tabs
	DrawTabs("Aimbot", 1, x + 6, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 6, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	//DrawTabs("Triggerbot", 2, x + 135, y + 26);
	DrawTabs("ESP", 3, x + 100, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 100, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	DrawTabs("Radar", 4, x + 194, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 194, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	DrawTabs("Anti Aim", 5, x + 288, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 288, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	//DrawTabs("Render", 6, x + 522, y + 26);
	DrawTabs("Misc", 7, x + 382, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 382, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	DrawTabs("Player List", 8, x + 476, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 476, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	DrawTabs("Colors", 9, x + 570, y + 26);
	Ikaros.m_pDraw->FillRGBA(x + 570, y + 51, 90, 4, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));
	//Config Zone
	if (openTab.ActiveTab == tab9 || openTab.ActiveTab == tab3)
	{
		Ikaros.m_pDraw->FillRGBA(x + 6, y + 449, 150, 45, RED(Grey1), GREEN(Grey1), BLUE(Grey1), ALPHA(Grey1));
		CfgSelectButton(x + 20, y + 454);
		SaveButton(x + 90, y + 454);
		PanicButton(x + 55, y + 474);

		Ikaros.m_pDraw->FillRGBA(x + 160, y + 449, 497, 45, RED(Grey1), GREEN(Grey1), BLUE(Grey1), ALPHA(Grey1));
	}
	else
	{
		Ikaros.m_pDraw->FillRGBA(x + 6, y + 430, 150, 45, RED(Grey1), GREEN(Grey1), BLUE(Grey1), ALPHA(Grey1));
		CfgSelectButton(x + 20, y + 435);
		SaveButton(x + 90, y + 435);
		PanicButton(x + 55, y + 455);

		Ikaros.m_pDraw->FillRGBA(x + 160, y + 430, 497, 45, RED(Grey1), GREEN(Grey1), BLUE(Grey1), ALPHA(Grey1));
	}

	if (openTab.ActiveTab == tab1)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "Aimbot:");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 6, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 454, y + 354, w + 86, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs2("Triggerbot", 2, x + 455, y + 355);
		DrawCheckBox(x + 10, y + 90, "Active", Ikaros.m_pCvars->bAimActive, "Enable Aimbot(Aim Help).");
		DrawCheckBox(x + 10, y + 120, "Auto Aim", Ikaros.m_pCvars->bAutoAim, "Aim automatically on the Target.");
		DrawCheckBox(x + 10, y + 150, "Auto Shoot", Ikaros.m_pCvars->bAutoShoot, "Shoot automatically when aimbot is on the target.");
		Slider(x + 320, y + 90, 0, Ikaros.m_pCvars->backtrack_tick, 1, 12, "Backtrack Ticks", "Adjusts the aimbot height.");
		Slider(x + 320, y + 120, 1, Ikaros.m_pCvars->iAimSpot, 1, 12, "Aim Spot", "Adjusts the aim spot.");
		//Slider(x + 320, y + 150, 1, Ikaros.m_pCvars->bttiks, 1, 100, "backtrack tiks2", "Adjusts the aim spot.");
		if (Ikaros.m_pCvars->iAim == 1)
		{
			DrawCheckBox(x + 185, y + 90, "Silent Aim", Ikaros.m_pCvars->bSilent, "Hides Aimbot movement for you.");
			DrawCheckBox(x + 185, y + 120, "Perfect Silent Aim", Ikaros.m_pCvars->bPSilent, "Hides Aimbot movement for you and spectators.");
			DrawCheckBox(x + 185, y + 150, "Aim at Team", Ikaros.m_pCvars->bTeamAim, "Aimbot aims at your teammates.");
			DrawCheckBox(x + 185, y + 180, "Autowall", Ikaros.m_pCvars->bAutoWall, "Aimbot shoots through penetratable walls.");
			DrawCheckBox(x + 10, y + 180, "Body AWP", Ikaros.m_pCvars->bBodyAWP, "Aimbot aims with the awp on the body(even when you set another aim spot).");
			DrawCheckBox(x + 10, y + 210, "Hitscan", Ikaros.m_pCvars->bHitscan, "Aimbot scans after visible parts of the body.");
			DrawCheckBox(x + 10, y + 240, "Backktrack", Ikaros.m_pCvars->backtrack, "Shows you the shake of norecoil and nospread while aimboting.");
			//DrawCheckBox(x + 185, y + 210, "Backtrack2", Ikaros.m_pCvars->btt, "Aimbot scans after visible corners of the body.");
			if (Ikaros.m_pCvars->iAim_Mode == 1)
			{
				Slider(x + 320, y + 150, 1, Ikaros.m_pCvars->flFOV, 2.0f, 360.0f, "Field of View", "Adjusts how near at your crosshar the enemy should be that the aimbots aims.");
				DrawCheckBox(x + 185, y + 240, "Deathmatch Rage", Ikaros.m_pCvars->bDM, "Aimbot ignores targets that have more than 100 HP.");
			}
			else
			{
				DrawCheckBox(x + 185, y + 240, "Deathmatch Rage", Ikaros.m_pCvars->bDM, "Aimbot ignores targets that have more than 100 HP.");
			}
		}
		else
		{
			DrawCheckBox(x + 10, y + 210, "Aim at Team", Ikaros.m_pCvars->bTeamAim, "Aimbot aims at your teammates.");
			if (Ikaros.m_pCvars->iAim_Mode == 1)
			{
				DrawCheckBox(x + 10, y + 180, "Advanced Smooth", Ikaros.m_pCvars->bAdvancedSmooth, "Enable advanced smooth aim.");
				//Slider(x + 320, y + 150, 1, Ikaros.m_pCvars->flFOV, 1.0f, 360.0f, "Field of View", "Adjusts how near at your crosshar the enemy should be that the aimbots aims.");
				Slider(x + 320, y + 150, 1, Ikaros.m_pCvars->backtrack_tick, 1.0f,12.0f, "BacktrackTick", "Adjusts how near at your crosshar the enemy should be that the aimbots aims.");
				if (Ikaros.m_pCvars->bAdvancedSmooth)
				{
					Slider(x + 320, y + 180, 1, Ikaros.m_pCvars->flSmoothX, 1.0f, 100.0f, "Smoothness X", "Adjusts how smooth the aimbot aims with the x(Pitch) axis.");
					Slider(x + 320, y + 210, 1, Ikaros.m_pCvars->flSmoothY, 1.0f, 100.0f, "Smoothness Y", "Adjusts how smooth the aimbot aims with the y(Yaw) axis.");
				}
				else
				{
					Slider(x + 320, y + 180, 1, Ikaros.m_pCvars->flSmoothX, 1.0f, 100.0f, "Smoothness", "Adjusts the smoothness of the aimbot.");
				}
			}
			else
			{
				DrawCheckBox(x + 10, y + 180, "Advanced Smooth", Ikaros.m_pCvars->bAdvancedSmooth, "Enable advanced smooth aim.");
				if (Ikaros.m_pCvars->bAdvancedSmooth)
				{
					Slider(x + 320, y + 150, 1, Ikaros.m_pCvars->flSmoothX, 1.0f, 100.0f, "Smoothness X", "Adjusts how smooth the aimbot aims with the x(Pitch) axis.");
					Slider(x + 320, y + 180, 1, Ikaros.m_pCvars->flSmoothY, 1.0f, 100.0f, "Smoothness Y", "Adjusts how smooth the aimbot aims with the y(Yaw) axis.");
				}
				else
				{
					Slider(x + 320, y + 150, 1, Ikaros.m_pCvars->flSmoothX, 1.0f, 100.0f, "Smoothness", "Adjusts the smoothness of the aimbot.");
				}
			}
		}
		DrawComboBox(x + 10, y + 330, "Aim on Key", Ikaros.m_pCvars->iAimKey, 3, 11, Keys, "Choose on which key the aimbot should activate.");
		DrawComboBox(x + 10, y + 300, "Target Selection", Ikaros.m_pCvars->iAim_Mode, 2, 3, AimSelect, "Select how the aimbot selects targets.");
		DrawComboBox(x + 10, y + 270, "Aimbot", Ikaros.m_pCvars->iAim, 1, 2, AimMode, "Choose the aimbot style.");
		DrawComboBox(x + 185, y + 330, "No Shake No Spread", Ikaros.m_pCvars->iPNoSpread, 14, 3, Removals, "Removes spread without any visual shake.");
		DrawComboBox(x + 185, y + 300, "No Spread", Ikaros.m_pCvars->iNoSpread, 13, 3, Removals, "Removes spread with visual shakes for spectators.");
		DrawComboBox(x + 185, y + 270, "No Recoil", Ikaros.m_pCvars->iNoRecoil, 11, 3, Removals, "Removes recoil with visual shakes for spectators.");
	}
	else if (openTab.ActiveTab == tab2)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "Triggerbot:");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 6, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 454, y + 354, w + 86, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs2("Aimbot", 1, x + 455, y + 355);
		DrawCheckBox(x + 10, y + 90, "Active", Ikaros.m_pCvars->bTriggerActive, "Enables Triggerbot(Shoots for you).");
		DrawCheckBox(x + 10, y + 120, "Auto Trigger", Ikaros.m_pCvars->bAutoTrigger, "Shoots automatically if the triggerbot should hit a target");
		DrawCheckBox(x + 10, y + 150, "Trigger at Team", Ikaros.m_pCvars->bTrigTeam, "Triggerbot also shoots on teammates.");
		DrawCheckBox(x + 225, y + 90, "Head", Ikaros.m_pCvars->bTrigHead, "");
		DrawCheckBox(x + 225, y + 120, "Chest", Ikaros.m_pCvars->bTrigChest, "");
		DrawCheckBox(x + 225, y + 150, "Pelvis", Ikaros.m_pCvars->bTrigPelvis, "");
		DrawCheckBox(x + 225, y + 180, "Arms", Ikaros.m_pCvars->bTrigArms, "");
		DrawCheckBox(x + 225, y + 210, "Legs", Ikaros.m_pCvars->bTrigLegs, "");
		Slider(x + 10, y + 270, 1, Ikaros.m_pCvars->iTrigDelay, 0, 1000, "Triggerdelay", "How delayed the triggerbot should shoot.");
		DrawComboBox(x + 10, y + 240, "Key", Ikaros.m_pCvars->iTrigKey, 6, 11, Keys, "Choose on which key the triggerbot should activate.");
		DrawComboBox(x + 10, y + 210, "Hitbox", Ikaros.m_pCvars->iTrigBone, 5, 7, TrigBox, "Choose on which hitbox the triggerbot should shoot.");
		DrawComboBox(x + 10, y + 180, "Mode", Ikaros.m_pCvars->iTriggerbot, 4, 3, TrigMode, "Select the triggerbot mode.");
	}
	else if (openTab.ActiveTab == tab3)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 383, "Esp:");
		DrawGroupBox(x + 265, y + 60, 395, 383, "");
		Ikaros.m_pDraw->FillRGBA(x + 100, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page1", 3, x + 450, y + 355);
		MenuDrawTabs("Page2", 18, x + 525, y + 355);
		DrawCheckBox(x + 10, y + 90, "Active ", Ikaros.m_pCvars->bESP, "Enables ESP(Extrasensory perception).");
		DrawCheckBox(x + 10, y + 120, "Name ESP ", Ikaros.m_pCvars->bNameESP, "Shows name esp on target.");
		DrawCheckBox(x + 10, y + 150, "Hitbox ESP ", Ikaros.m_pCvars->bBoneESP, "Shows all hitboxes on target.");
		DrawCheckBox(x + 10, y + 180, "Backtrack ESP ", Ikaros.m_pCvars->bAimspotESP, "Shows the aimbot aim spot.");
		DrawCheckBox(x + 10, y + 210, "Enemy Only ", Ikaros.m_pCvars->bEnemyOnlyESP, "Draws ESP only on enemy.");
		DrawCheckBox(x + 165, y + 90, "Chams", Ikaros.m_pCvars->bChams, "Enables Chams(Colored Models).");
		DrawCheckBox(x + 165, y + 120, "Weapon Chams", Ikaros.m_pCvars->bWeaponChams, "Enables Weapon Chams(Colored Weapons).");
		DrawCheckBox(x + 165, y + 150, "HandsWireframe", Ikaros.m_pCvars->bHandsWireframe, "HandsWireframe");
		DrawCheckBox(x + 165, y + 180, "XQZ Wallhack", Ikaros.m_pCvars->bXQZWalls, "Makes all players visible through walls.");
		DrawCheckBox(x + 165, y + 210, "Glass Chams", Ikaros.m_pCvars->bGlassChams, "Makes the Chams more transparent.");
		DrawCheckBox(x + 320, y + 90, "No Flash", Ikaros.m_pCvars->bNoFlash, "Disables the Flashbang Effect.");
		DrawCheckBox(x + 320, y + 120, "No Smoke", Ikaros.m_pCvars->bNoSmoke, "Disables the Smoke Effect from smoke granades.");
		DrawCheckBox(x + 320, y + 150, "No Hands", Ikaros.m_pCvars->bNoHands, "Removes Hands.");
		DrawCheckBox(x + 320, y + 180, "No Sky", Ikaros.m_pCvars->bNoSky, "Makes the sky black(to boost fps).");
		DrawCheckBox(x + 320, y + 210, "Fullbright", Ikaros.m_pCvars->bFullbright, "Enables full brightness.");
		Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->OverrideFOV, 1.f, 360.f, "Override FOV", "");
		Slider(x + 10, y + 360, 0, Ikaros.m_pCvars->ViewmodelFOV, 1.f, 150.f, "Viewmodel FOV", "");
		Slider(x + 10, y + 420, 0, Ikaros.m_pCvars->flAsusAlpha, 0.f, 255.f, "Asus Walls Alpha", "");
		Slider(x + 10, y + 390, 0, Ikaros.m_pCvars->flNightMode, 1.f, 255.f, "Night Mode Alpha", "");
		DrawCheckBox(x + 10, y + 300, "Asus Walls", Ikaros.m_pCvars->bAsusWalls, "");
		DrawCheckBox(x + 475, y + 90, "Monitor", Ikaros.m_pCvars->bMonitorActive, "Enables a info monitor for recoil, speed and spread.");
		DrawCheckBox(x + 475, y + 120, "Watermark", Ikaros.m_pCvars->bWatermark, "Enables Watermark.");
		DrawCheckBox(x + 475, y + 150, "Night Mode", Ikaros.m_pCvars->bNightMode, "");
		DrawCheckBox(x + 475, y + 180, "RecoilCrosshair", Ikaros.m_pCvars->bRecoilCrosshair, "RecoilCrosshair");
		DrawCheckBox(x + 475, y + 210, "ShowVelocity", Ikaros.m_pCvars->bShowVelocity, "ShowVelocity");
		DrawCheckBox(x + 475, y + 300, "Preview", Ikaros.m_pCvars->bPreview, "Preview");
		DrawComboBox(x + 475, y + 270, "WatermarkColor", Ikaros.m_pCvars->iWatermark, 15, 2, WatermarkTextColor, "WatermarkColor");
		DrawComboBox(x + 475, y + 240, "RecoilCrosshair", Ikaros.m_pCvars->iRecoilCrosshair, 19, 7, RecoilCrosshair, "RecoilCrosshair");
		DrawCheckBox(x + 165, y + 300, "ChamsThroughWalls", Ikaros.m_pCvars->bChamsThroughWalls, "ChamsThroughWalls");
		DrawComboBox(x + 165, y + 270, "WeaponChamsMaterial", Ikaros.m_pCvars->iWeaponChamsMaterial, 10, 2, WeaponChamsMaterial, "WeaponChamsMaterial");
		DrawComboBox(x + 165, y + 240, "PlayerChamsMaterial", Ikaros.m_pCvars->iPlayerChamsMaterial, 13, 2, PlayerChamsMaterial, "PlayerChamsMaterial");
		DrawComboBox(x + 10, y + 270, "Health", Ikaros.m_pCvars->iHealthESP, 8, 4, ESPHealth, "Select the Health ESP style.");
		DrawComboBox(x + 10, y + 240, "Box", Ikaros.m_pCvars->i2DBox, 7, 3, ESPStyle, "Select the ESP Box style.");
		if (Ikaros.m_pCvars->bPreview)
		{
			if (Ikaros.m_pCvars->bESP)
			{
				DrawGroupBox(x + 665, y + 60, 235, 383, "");
				int Health = 78;
				int g = Health * 2.55;
				int r = 255 - g;
				/*if (Ikaros.m_pCvars->i2DBox == 1)
				{
					ESPBox(x + 745 - 20, y + 188 - 20, 40, 40, 255, 255, 0, 255);
					if (Ikaros.m_pCvars->bNameESP)
					{
						Ikaros.m_pDraw->Text(x + 745 + 28, y + 182, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Gabe Newell");
					}
					if (Ikaros.m_pCvars->bAimspotESP)
					{
						Ikaros.m_pDraw->FillRGBA(x + 745 - 2, y + 122 - 2, 5, 5, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA(x + 745 - 1, y + 122 - 1, 3, 3, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_BLACK));
					}
					if (Ikaros.m_pCvars->iHealthESP == 1)
					{
						Ikaros.m_pDraw->Text(x + 745 + 28, y + 194, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Health : %i", Health);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 2)
					{
						Ikaros.m_pDraw->FillRGBA(x + 745 - 28, y + 188 - 20, 5, 40, r, g, 0, 255);
						Ikaros.m_pDraw->FillRGBA(x + 745 - 28, y + 188 - 20, 5, 40 - (Health / 2.5), 0, 0, 0, 255);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 3)
					{
						Ikaros.m_pESP->DrawHeader(x + 745 - 19, y + 188 + 23, 39 + 2, 255, 255, 0, (Health / 2.5) - 2);
					}
				}*/
				if (Ikaros.m_pCvars->i2DBox == 1)
				{
					ESPBox(x + 745 - 40, y + 188 - 90, 80, 180, 255, 255, 0, 255);
					if (Ikaros.m_pCvars->bNameESP)
					{
						Ikaros.m_pDraw->Text(x + 748 + 40, y + 188 - 15, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Gabe Newell");
					}
					if (Ikaros.m_pCvars->bAimspotESP)
					{
						Ikaros.m_pDraw->FillRGBA(x + 745 - 2, y + 122 - 2, 5, 5, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA(x + 745 - 1, y + 122 - 1, 3, 3, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_BLACK));
					}
					if (Ikaros.m_pCvars->iHealthESP == 1)
					{
						Ikaros.m_pDraw->Text(x + 748 + 40, y + 190 - 5, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Health : %i", Health);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 2)
					{
						float multi = 180 / 100;
						Ikaros.m_pDraw->FillRGBA(x + 695 - 8, y + 98, 5, 180, r, g, 0, 255);
						Ikaros.m_pDraw->FillRGBA(x + 695 - 8, y + 98, 5, 180 - (180 * ((float)Health / 100.0f)), 0, 0, 0, 255);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 3)
					{
						Ikaros.m_pESP->DrawHeader(x + 745 - 40 + 2, y + 278 + 3, 80, 255, 255, 0, (Health / (100 / 80)) - 2);
					}
				}
				else if (Ikaros.m_pCvars->i2DBox == 2)
				{
					int h = 180;
					int w = 12;

					//Left Up
					Ikaros.m_pDraw->FillRGBA(x + 706 - 1, y + 98 - 1, w, 3, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 706 - 1, y + 98 - 1, 3, w, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 706, y + 98, w - 2, 1, 255, 255, 0, 255);
					Ikaros.m_pDraw->FillRGBA(x + 706, y + 98, 1, w - 2, 255, 255, 0, 255);

					//Left Down
					Ikaros.m_pDraw->FillRGBA(x + 706 - 1, y + 280 - 4, w, 3, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 706 - 1, y + 280 - w - 2, 3, w, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 706, y + 280 - 3, w - 2, 1, 255, 255, 0, 255);
					Ikaros.m_pDraw->FillRGBA(x + 706, y + 280 - w - 1, 1, w - 2, 255, 255, 0, 255);

					//Right Up
					Ikaros.m_pDraw->FillRGBA(x + 785 - w, y + 98 - 1, w, 3, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 785 - 3, y + 98 - 1, 3, w, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 785 - w + 1, y + 98, w - 2, 1, 255, 255, 0, 255);
					Ikaros.m_pDraw->FillRGBA(x + 785 - 2, y + 98, 1, w - 2, 255, 255, 0, 255);

					//Right Down
					Ikaros.m_pDraw->FillRGBA(x + 785 - w, y + 280 - 4, w, 3, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 785 - 3, y + 280 - w - 2, 3, w, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					Ikaros.m_pDraw->FillRGBA(x + 785 - w + 1, y + 280 - 3, w - 2, 1, 255, 255, 0, 255);
					Ikaros.m_pDraw->FillRGBA(x + 785 - 2, y + 280 - w - 1, 1, w - 2, 255, 255, 0, 255);

					if (Ikaros.m_pCvars->bNameESP)
					{
						Ikaros.m_pDraw->Text(x + 748 + 40, y + 188 - 15, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Gabe Newell");
					}
					if (Ikaros.m_pCvars->bAimspotESP)
					{
						Ikaros.m_pDraw->FillRGBA(x + 748 - 2, y + 122 - 2, 5, 5, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA(x + 748 - 1, y + 122 - 1, 3, 3, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_BLACK));
					}
					if (Ikaros.m_pCvars->iHealthESP == 1)
					{
						Ikaros.m_pDraw->Text(x + 748 + 40, y + 190 - 5, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Health : %i", Health);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 2)
					{
						float multi = 180 / 100;
						Ikaros.m_pDraw->FillRGBA(x + 695 - 8, y + 98, 5, 180, r, g, 0, 255);
						Ikaros.m_pDraw->FillRGBA(x + 695 - 8, y + 98, 5, 180 - (180 * (float)((float)Health / 100.0f)), 0, 0, 0, 255);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 3)
					{
						Ikaros.m_pESP->DrawHeader(x + 745 - 40 + 2, y + 278 + 3, 80, 255, 255, 0, (Health / (100 / 80)) - 2);
					}
				}
				else
				{
					if (Ikaros.m_pCvars->bNameESP)
					{
						Ikaros.m_pDraw->Text(x + 748, y + 182, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Gabe Newell");
					}
					if (Ikaros.m_pCvars->bAimspotESP)
					{
						Ikaros.m_pDraw->FillRGBA(x + 745 - 2, y + 122 - 2, 5, 5, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA(x + 745 - 1, y + 122 - 1, 3, 3, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_BLACK));
					}
					if (Ikaros.m_pCvars->iHealthESP == 1)
					{
						Ikaros.m_pDraw->Text(x + 748, y + 194, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), "Health : %i", Health);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 2)
					{
						Ikaros.m_pDraw->FillRGBA(x + 745 - 28, y + 188 - 20, 5, 40, r, g, 0, 255);
						Ikaros.m_pDraw->FillRGBA(x + 745 - 28, y + 188 - 20, 5, 40 - (Health / 2.5), 0, 0, 0, 255);
					}
					else if (Ikaros.m_pCvars->iHealthESP == 3)
					{
						Ikaros.m_pESP->DrawHeader(x + 745 - 19, y + 188 + 23, 39 + 2, 255, 255, 0, (Health / 2.5) - 2);
					}
				}
			}
		}
	}
	else if (openTab.ActiveTab == tab4)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "Radar:");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 194, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		DrawCheckBox(x + 10, y + 90, "Active ", Ikaros.m_pCvars->bRadar2D, "Enables Radar.");
		DrawCheckBox(x + 10, y + 120, "Hide Radar Background ", Ikaros.m_pCvars->bRadarBG, "Draws only a crosshair as radar.");
		DrawCheckBox(x + 10, y + 150, "Enemy Only ", Ikaros.m_pCvars->bEnemyOnlyRadar, "Shows only enemy on the radar.");
		DrawCheckBox(x + 10, y + 180, "Show Names ", Ikaros.m_pCvars->bRadarName, "Shows names on the radar.");
		Slider(x + 10, y + 210, 0, Ikaros.m_pCvars->bRadarSize, 50.f, 250.f, "Radar Size", "");
		Slider(x + 10, y + 240, 0, Ikaros.m_pCvars->bRadarRange, 5.f, 250.f, "Radar Range", "");
	}
	else if (openTab.ActiveTab == tab5)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "AntiAim:");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		DrawGroupBox(x + 345, y + 60, 315, 225, "Fakelag:");
		Ikaros.m_pDraw->FillRGBA(x + 288, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		DrawCheckBox(x + 10, y + 90, "Active", Ikaros.m_pCvars->bAntiAim, "Enables Anti Aim(makes it harder to hit you with an aimbot).");
		DrawCheckBox(x + 10, y + 210, "Edge Anti Aim", Ikaros.m_pCvars->bEdgeAntiAim, "Rotates your model to look at a wall if you stand at a wall.");
		DrawCheckBox(x + 10, y + 240, "Aim At Targets", Ikaros.m_pCvars->bAimAtTargets, "Aims at the target to make it harder to hit you with an aimbot.");
		DrawCheckBox(x + 10, y + 270, "DisableKnifeAA", Ikaros.m_pCvars->KnifeAA, "knifeAA");
		DrawComboBox(x + 345, y + 90, "FakelagMode", Ikaros.m_pCvars->iFakelagMode, 10, 2, FakelagMode, "FakelagMode");
		Slider(x + 345, y + 125, 1, Ikaros.m_pCvars->FakelagAmount, 2, 16, "fakelag", "");
		if (Ikaros.m_pCvars->iAntiAimTypeX == 2)
		{
			if (Ikaros.m_pCvars->iAntiAimTypeY == 2)
			{
				Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->flAA_X, 0.0f, 360.0f, "X Axis", "Define the x(Pitch) axis of the anti aim.");
				Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->flAA_Y, 0.0f, 360.0f, "Y Axis", "Define the y(Yaw) axis of the anti aim.");
			}
			else
			{
				Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->flAA_X, 0.0f, 360.0f, "X Axis", "Define the x(Pitch) axis of the anti aim.");
			}
		}
		if (Ikaros.m_pCvars->iAntiAimTypeY == 2)
		{
			if (Ikaros.m_pCvars->iAntiAimTypeX == 2)
			{
				Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->flAA_X, 0.0f, 360.0f, "X Axis", "Define the x(Pitch) axis of the anti aim.");
				Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->flAA_Y, 0.0f, 360.0f, "Y Axis", "Define the y(Yaw) axis of the anti aim.");
			}
			else
			{
				Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->flAA_Y, 0.0f, 360.0f, "Y Axis", "Define the y(Yaw) axis of the anti aim.");
			}
		}
		/*if (Ikaros.m_pCvars->iAntiAimTypeY == 3)
		{
			Slider(x + 125, y + 150, 0, Ikaros.m_pCvars->SpinSpeed, 0.1f, 2.0f, "SpinSpeed", "");
		}*/
		if (Ikaros.m_pCvars->iAntiAimTypeY == 8)
		{
			if (Ikaros.m_pCvars->iAntiAimTypeX == 2)
			{
				Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->flAA_Y, 0.0f, 360.0f, "Fake Yaw", "Define the y(Yaw) axis of the anti aim.");
				Slider(x + 10, y + 360, 0, Ikaros.m_pCvars->flA_Y, 0.0f, 360.0f, "Real Yaw", "Define the y(Yaw) axis of the anti aim.");
			}
			else
			{
				Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->flAA_Y, 0.0f, 360.0f, "Fake Yaw", "Define the y(Yaw) axis of the anti aim.");
				Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->flA_Y, 0.0f, 360.0f, "Real Yaw", "Define the y(Yaw) axis of the anti aim.");
			}
		}
		DrawComboBox2(x + 10, y + 180, "Spin Mode", Ikaros.m_pCvars->iSpinMode, 11, 3, SpinMode, "Spin Mode");
		DrawComboBox2(x + 10, y + 150, "Y", Ikaros.m_pCvars->iAntiAimTypeY, 17, 11, AntiAimY, "Select the anti aim y(Yaw) style.");
		DrawComboBox2(x + 10, y + 120, "X", Ikaros.m_pCvars->iAntiAimTypeX, 9, 7, AntiAimX, "Select the anti aim x(Pitch) style.");
	}
	else if (openTab.ActiveTab == tab6)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 160, y + 30, 635, 400, "");
		//DrawFloatSlider("Asus Walls Alpha", x + 20, y + 560, 0.1, 1, Ikaros.m_pCvars->flAsusAlpha, 0.1);
	}
	else if (openTab.ActiveTab == tab7)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "Misc:");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 382, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page1", 7, x + 450, y + 355);
		MenuDrawTabs("Page2", 15, x + 525, y + 355);
		DrawCheckBox(x + 10, y + 90, "Bunny Hop", Ikaros.m_pCvars->bBunnyHop, "Makes you jump continuously if you hold space.");
		DrawCheckBox(x + 10, y + 120, "Auto Pistol", Ikaros.m_pCvars->bAutoPistol, "Fires pistols like automatic rifles.");
		DrawCheckBox(x + 10, y + 150, "Speedhack", Ikaros.m_pCvars->bSpeed, "Enables Speedhack.");
		DrawCheckBox(x + 10, y + 180, "No Visual Recoil", Ikaros.m_pCvars->bNoVisRecoil, "Removes any visual recoil.");
		Slider(x + 10, y + 210, 1, Ikaros.m_pCvars->iSpeedVal, 1.0f, 10.0f, "Speed Value", "Choose how fast the speedhack is.");

		if (!Ikaros.m_pCvars->iAim)
		{
			DrawCheckBox(x + 10, y + 300, "Namestealer", Ikaros.m_pCvars->bNameStealer, "Steals the other players name.");
			//DrawCheckBox(x + 10, y + 330, "Roundsay", Ikaros.m_pCvars->roundsay, "Advises the cheat.");
			DrawCheckBox(x + 485, y + 120, "Hit Sound", Ikaros.m_pCvars->bHitSound, "Plays a sound if you hit the enemy.");
			DrawCheckBox(x + 485, y + 150, "Flashlight Spam", Ikaros.m_pCvars->bFlashSpam, "Spams the flashlight command to make a annoying noise.");
			DrawCheckBox(x + 10, y + 360, "Chat Repeater", Ikaros.m_pCvars->bChatRepeat, "Repeats what other people say in chat.");
			DrawComboBox(x + 485, y + 90, "Recoil Control System", Ikaros.m_pCvars->iRCS, 12, 2, Removals, "Removes recoil in a controlled way(spraydown).");
		}
		else
		{
			DrawCheckBox(x + 10, y + 300, "Namestealer", Ikaros.m_pCvars->bNameStealer, "Steals the other players name.");
			//DrawCheckBox(x + 10, y + 330, "Roundsay", Ikaros.m_pCvars->roundsay, "Advises the cheat.");
			DrawCheckBox(x + 175, y + 300, "Flashlight Spam", Ikaros.m_pCvars->bFlashSpam, "Spams the flashlight command to make a annoying noise.");
			DrawCheckBox(x + 330, y + 120, "Radio Kill Informer", Ikaros.m_pCvars->RadioKillInformer, "RadioKillInformer");
			DrawCheckBox(x + 330, y + 150, "Sv_cheats", Ikaros.m_pCvars->Cheats, "bypass");
			DrawCheckBox(x + 330, y + 180, "Airstuck (VK_SHIFT)", Ikaros.m_pCvars->Airstuck, "Airstuck");
			Slider(x + 330, y + 210, 0, Ikaros.m_pCvars->ChatClearTime, 0.1f, 2.0f, "ChatClearTime", "");
			DrawCheckBox(x + 175, y + 90, "CircleStrafe(Alt)", Ikaros.m_pCvars->CircleStrafe, "CircleStrafe");
			DrawCheckBox(x + 175, y + 120, "Enemy Check", Ikaros.m_pCvars->bEnemyCheck, "Disable Anti Aim when there's no enemies.");
			DrawCheckBox(x + 175, y + 180, "Knifebot", Ikaros.m_pCvars->Knifebot, "Knifebot");
			DrawCheckBox(x + 175, y + 330, "FakeDuck", Ikaros.m_pCvars->FakeDuck, "FakeDuck");
			DrawCheckBox(x + 175, y + 360, "LagExploit", Ikaros.m_pCvars->LagExploit, "LagExploit");
			DrawCheckBox(x + 320, y + 300, "ChatSpam", Ikaros.m_pCvars->ChatSpam, "ChatSpam");
			DrawCheckBox(x + 320, y + 330, "ChatClear", Ikaros.m_pCvars->ChatClear, "ChatSpam");
			DrawCheckBox(x + 175, y + 150, "FastWalk", Ikaros.m_pCvars->FastWalk, "+28 vel");
			DrawCheckBox(x + 10, y + 360, "FakeWalk", Ikaros.m_pCvars->FakeWalk, "FakeWalk");
			DrawCheckBox(x + 175, y + 390, "Chat Repeater", Ikaros.m_pCvars->bChatRepeat, "Repeats what other people say in chat.");
			DrawComboBox(x + 320, y + 270, "ChatSpam(Radio)", Ikaros.m_pCvars->iChatSpam, 15, 4, ChatSpam, "ChatSpam");
			DrawComboBox(x + 320, y + 240, "ChatSpam(Text)", Ikaros.m_pCvars->iChatSpam2, 17, 4, ChatSpam2, "ChatSpam");
			DrawCheckBox(x + 330, y + 90, "Hit Sound", Ikaros.m_pCvars->bHitSound, "Plays a sound if you hit the enemy.");
		}
		DrawCheckBox(x + 10, y + 390, "LagFix", Ikaros.m_pCvars->LagFix, "LagFix");
		DrawComboBox(x + 10, y + 270, "Auto Strafe", Ikaros.m_pCvars->iAutoStrafe, 18, 3, Strafe, "Enables strafe helper for gaining speed while bunnyhopping.");
		DrawComboBox(x + 10, y + 240, "Speedhack Key", Ikaros.m_pCvars->iSpeedKey, 10, 11, Keys, "Choose on which key the speedhack enables.");
		DrawComboBox(x + 175, y + 270, "Roundsay", Ikaros.m_pCvars->iRoundsay, 3, 6, Roundsay, "change roundsay.");
		DrawComboBox(x + 175, y + 240, "Crosshair", Ikaros.m_pCvars->iCrosshair, 16, 7, Crosshair, "Choose a custom crosshair which is drawn over your normal crosshair.");
	}
	else if (openTab.ActiveTab == tab8)
	{
		int x1 = x + 10;
		int y1 = y + 35;
		Ikaros.m_pCvars->bPlayerList = true;
		Ikaros.m_pDraw->FillRGBA(x + 476, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		DrawGroupBox(x + 265, y + 330, 395, 35, "");
		DrawGroupBox(x + 6, y + 60, 395, 365, "Players");
		DrawGroupBox(x + 6, y + 330, 395, 35, "Options");

		DrawCheckBox(x1 + 435, y1 + 325, "Manually Yaw", Ikaros.m_pPlayerlist[Selection].bAdvancedYaw, "Enables manually Yaw(y axis) Correction.");

		if (Ikaros.m_pPlayerlist[Selection].bAdvancedYaw)
		{
			Slider(x1 + 315, y1 + 355, 1, Ikaros.m_pPlayerlist[Selection].iAdvancedYaw, 0, 360, "Yaw Correction", "Manually correct Yaw(y axis) for anti aim users with fake yaw.");
		}
		else
		{
			DrawComboBox2(x1 + 315, y1 + 355, "Yaw Correction", Ikaros.m_pPlayerlist[Selection].iYaw, 20, 7, AAAYaw, "Correct Yaw(y axis) for anti aim users with fake yaw.");
			if (Ikaros.m_pPlayerlist[Selection].iYaw == 4)
			{
				Slider(x1 + 315, y1 + 385, 1, Ikaros.m_pPlayerlist[Selection].iAddYaw, 0, 360, "To add", "");
			}
		}
		DrawComboBox(x1, y1 + 355, "Pitch Correction", Ikaros.m_pPlayerlist[Selection].iPitch, 19, 4, AAAPitch, "Correct Pitch(x axis) for anti aim users with fake pitch or 'magic' pitch.");
		DrawCheckBox(x1, y1 + 325, "Friend", Ikaros.m_pPlayerlist[Selection].bFriend, "Makes the esp on the player white and aimbot, triggerbot won't kill him.");

		DrawCheckBox(x1 + 315, y1 + 325, "Jitter", Ikaros.m_pPlayerlist[Selection].bJitterYaw, "Jitter");
		DrawCheckBox(x1 + 375, y1 + 325, "Static", Ikaros.m_pPlayerlist[Selection].bStaticYaw, "Static");

		if (Ikaros.m_pPlayerlist[Selection].bAdvancedYaw)
		{
			if (Ikaros.m_pPlayerlist[Selection].bJitterYaw)
			{
				Slider(x1 + 315, y1 + 375, 1, Ikaros.m_pPlayerlist[Selection].iAddJitterYaw, 0, 360, "JitterYaw", "");
			}
		}

		if (iSel > 0)
		{
			Ikaros.m_pDraw->FillRGBA(x1, y1 + 46 + (iSel * 13), 625, 12, RED(MenuColor), GREEN(MenuColor), BLUE(MenuColor), ALPHA(MenuColor));//highlight selection
		}
	}
	else if (openTab.ActiveTab == tab9)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 383, "");
		DrawGroupBox(x + 265, y + 60, 395, 383, "");
		Ikaros.m_pDraw->FillRGBA(x + 570, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 383, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 383, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 374, y + 383, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page2", 12, x + 450, y + 385);
		MenuDrawTabs("Credits", 14, x + 525, y + 385);
		MenuDrawTabs("Other", 17, x + 375, y + 385);
		DrawCheckBox(x + 10, y + 395, "Colors(Preview)", Ikaros.m_pCvars->PreviewColors, "");
		ResetColors(x + 10, y + 425);
		Ikaros.m_pDraw->Text(x + 15, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " BorderColor");
		Slider(x + 10, y + 120, 0, Ikaros.m_pCvars->DrawColor, 0.f, 255.f, "Red", "");
		Slider(x + 10, y + 150, 0, Ikaros.m_pCvars->DrawColor2, 0.f, 255.f, "Green", "");
		Slider(x + 10, y + 180, 0, Ikaros.m_pCvars->DrawColor3, 0.f, 255.f, "Blue", "");
		Slider(x + 10, y + 210, 0, Ikaros.m_pCvars->DrawColor4, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 15, y + 240, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "BackGroundColor");
		Slider(x + 10, y + 270, 0, Ikaros.m_pCvars->DrawColor5, 0.f, 255.f, "Red", "");
		Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->DrawColor6, 0.f, 255.f, "Green", "");
		Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->DrawColor7, 0.f, 255.f, "Blue", "");
		Slider(x + 10, y + 360, 0, Ikaros.m_pCvars->DrawColor8, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 335, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "DrawGroupBoxColor");
		Slider(x + 320, y + 120, 0, Ikaros.m_pCvars->DrawColor9, 0.f, 255.f, "Red", "");
		Slider(x + 320, y + 150, 0, Ikaros.m_pCvars->DrawColor10, 0.f, 255.f, "Green", "");
		Slider(x + 320, y + 180, 0, Ikaros.m_pCvars->DrawColor11, 0.f, 255.f, "Blue", ".");
		Slider(x + 320, y + 210, 0, Ikaros.m_pCvars->DrawColor12, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 355, y + 240, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "SlidersColor");
		Slider(x + 320, y + 270, 0, Ikaros.m_pCvars->DrawColor13, 0.f, 255.f, "Red", "");
		Slider(x + 320, y + 300, 0, Ikaros.m_pCvars->DrawColor14, 0.f, 255.f, "Green", "");
		Slider(x + 320, y + 330, 0, Ikaros.m_pCvars->DrawColor15, 0.f, 255.f, "Blue", "");
		Slider(x + 320, y + 360, 0, Ikaros.m_pCvars->DrawColor16, 0.f, 255.f, "Alpha", "");
		/*Ikaros.m_pDraw->Text(x + 160, y + 30, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Credits(i haz dem):");
		Ikaros.m_pDraw->Text(x + 160, y + 50, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " Kamay");
		Ikaros.m_pDraw->Text(x + 160, y + 70, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " KeyBode");
		Ikaros.m_pDraw->Text(x + 160, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " MattDog");
		Ikaros.m_pDraw->Text(x + 160, y + 110, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " me(n0xius)");
		Ikaros.m_pDraw->Text(x + 160, y + 130, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " InUrFace");
		Ikaros.m_pDraw->Text(x + 160, y + 150, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " royalhack.ru");
		Ikaros.m_pDraw->Text(x + 160, y + 170, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " game-deception.com(P47R!CK, Tetsuo, Darawk, Dom1n1k, PizzaPan, Panzer, s0beit, [myg0t]wav, etc..)");
		Ikaros.m_pDraw->Text(x + 160, y + 190, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Greets:");
		Ikaros.m_pDraw->Text(x + 160, y + 210, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " BH4X0R");
		Ikaros.m_pDraw->Text(x + 160, y + 230, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " Kamay");
		Ikaros.m_pDraw->Text(x + 160, y + 250, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " KeyBode");
		Ikaros.m_pDraw->Text(x + 160, y + 270, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " MattDog");
		Ikaros.m_pDraw->Text(x + 160, y + 290, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " gopnix");
		Ikaros.m_pDraw->Text(x + 160, y + 310, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " maximum1441");*/
		if (Ikaros.m_pCvars->PreviewColors)
		{
			DrawGroupBox(x + 665, y + 60, 235, 383, "");
			Ikaros.m_pDraw->FillRGBA(x + 705, y + 120, 70, 50, Ikaros.m_pCvars->DrawColor, Ikaros.m_pCvars->DrawColor2, Ikaros.m_pCvars->DrawColor3, Ikaros.m_pCvars->DrawColor4);
			Ikaros.m_pDraw->FillRGBA(x + 785, y + 120, 70, 50, Ikaros.m_pCvars->DrawColor5, Ikaros.m_pCvars->DrawColor6, Ikaros.m_pCvars->DrawColor7, Ikaros.m_pCvars->DrawColor8);
			Ikaros.m_pDraw->FillRGBA(x + 705, y + 270, 70, 50, Ikaros.m_pCvars->DrawColor9, Ikaros.m_pCvars->DrawColor10, Ikaros.m_pCvars->DrawColor11, Ikaros.m_pCvars->DrawColor12);
			Ikaros.m_pDraw->FillRGBA(x + 785, y + 270, 70, 50, Ikaros.m_pCvars->DrawColor13, Ikaros.m_pCvars->DrawColor14, Ikaros.m_pCvars->DrawColor15, Ikaros.m_pCvars->DrawColor16);
			Ikaros.m_pDraw->Border(x + 704, y + 119, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
			Ikaros.m_pDraw->Border(x + 784, y + 119, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
			Ikaros.m_pDraw->Border(x + 704, y + 269, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
			Ikaros.m_pDraw->Border(x + 784, y + 269, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		}
	}
	else if (openTab.ActiveTab == tab12)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 570, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page1", 9, x + 450, y + 355);
		MenuDrawTabs("Page3", 13, x + 525, y + 355);
		DrawCheckBox(x + 10, y + 395, "Colors(Preview)", Ikaros.m_pCvars->PreviewColors, "");
		Ikaros.m_pDraw->Text(x + 15, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "ButtonsColor");
		Slider(x + 10, y + 120, 0, Ikaros.m_pCvars->DrawColor17, 0.f, 255.f, "Red", "");
		Slider(x + 10, y + 150, 0, Ikaros.m_pCvars->DrawColor18, 0.f, 255.f, "Green", "");
		Slider(x + 10, y + 180, 0, Ikaros.m_pCvars->DrawColor19, 0.f, 255.f, "Blue", "");
		Slider(x + 10, y + 210, 0, Ikaros.m_pCvars->DrawColor20, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 15, y + 240, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "ClickedButtonsColor");
		Slider(x + 10, y + 270, 0, Ikaros.m_pCvars->DrawColor21, 0.f, 255.f, "Red", "");
		Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->DrawColor22, 0.f, 255.f, "Green", "");
		Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->DrawColor23, 0.f, 255.f, "Blue", "");
		Slider(x + 10, y + 360, 0, Ikaros.m_pCvars->DrawColor24, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 335, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "ChamsT Color");
		Slider(x + 320, y + 120, 0, Ikaros.m_pCvars->DrawColor25, 0.0f, 1.0f, "Red", "");
		Slider(x + 320, y + 150, 0, Ikaros.m_pCvars->DrawColor26, 0.0f, 1.0f, "Green", "");
		Slider(x + 320, y + 180, 0, Ikaros.m_pCvars->DrawColor27, 0.0f, 1.0f, "Blue", "");
		Ikaros.m_pDraw->Text(x + 335, y + 240, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "ChamsCT Color");
		Slider(x + 320, y + 270, 0, Ikaros.m_pCvars->DrawColor28, 0.0f, 1.0f, "Red", "");
		Slider(x + 320, y + 300, 0, Ikaros.m_pCvars->DrawColor29, 0.0f, 1.0f, "Green", "");
		Slider(x + 320, y + 330, 0, Ikaros.m_pCvars->DrawColor30, 0.0f, 1.0f, "Blue", "");
		if (Ikaros.m_pCvars->PreviewColors)
		{
			DrawGroupBox(x + 665, y + 60, 235, 383, "");
			Ikaros.m_pDraw->FillRGBA(x + 705, y + 120, 70, 50, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
			Ikaros.m_pDraw->FillRGBA(x + 785, y + 120, 70, 50, Ikaros.m_pCvars->DrawColor21, Ikaros.m_pCvars->DrawColor22, Ikaros.m_pCvars->DrawColor23, Ikaros.m_pCvars->DrawColor24);
			Ikaros.m_pDraw->Border(x + 704, y + 119, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
			Ikaros.m_pDraw->Border(x + 784, y + 119, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		}
	}
	else if (openTab.ActiveTab == tab13)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		DrawCheckBox(x + 10, y + 395, "Colors(Preview)", Ikaros.m_pCvars->PreviewColors, "");
		Ikaros.m_pDraw->FillRGBA(x + 570, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Text(x + 15, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "WindowsTextColor");
		Slider(x + 10, y + 120, 0, Ikaros.m_pCvars->WindowsTextColor1, 0.f, 255.f, "Red", "");
		Slider(x + 10, y + 150, 0, Ikaros.m_pCvars->WindowsTextColor2, 0.f, 255.f, "Green", "");
		Slider(x + 10, y + 180, 0, Ikaros.m_pCvars->WindowsTextColor3, 0.f, 255.f, "Blue", "");
		Slider(x + 10, y + 210, 0, Ikaros.m_pCvars->WindowsTextColor4, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 15, y + 240, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "TabsLineColor");
		Slider(x + 10, y + 270, 0, Ikaros.m_pCvars->DrawColor31, 0.f, 255.f, "Red", "");
		Slider(x + 10, y + 300, 0, Ikaros.m_pCvars->DrawColor32, 0.f, 255.f, "Green", "");
		Slider(x + 10, y + 330, 0, Ikaros.m_pCvars->DrawColor33, 0.f, 255.f, "Blue", "");
		Slider(x + 10, y + 360, 0, Ikaros.m_pCvars->DrawColor34, 0.f, 255.f, "Alpha", "");
		Ikaros.m_pDraw->Text(x + 335, y + 90, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "GlassChamsAlpha");
		Slider(x + 330, y + 120, 0, Ikaros.m_pCvars->GlassChamsColor, 0.f, 1.f, "Alpha", "");
		MenuDrawTabs("Page1", 9, x + 450, y + 355);
		MenuDrawTabs("Page2", 12, x + 525, y + 355);
		if (Ikaros.m_pCvars->PreviewColors)
		{
			DrawGroupBox(x + 665, y + 60, 235, 383, "");
			Ikaros.m_pDraw->FillRGBA(x + 705, y + 120, 70, 50, Ikaros.m_pCvars->WindowsTextColor1, Ikaros.m_pCvars->WindowsTextColor2, Ikaros.m_pCvars->WindowsTextColor3, Ikaros.m_pCvars->WindowsTextColor4);
			Ikaros.m_pDraw->FillRGBA(x + 785, y + 120, 70, 50, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);
			Ikaros.m_pDraw->Border(x + 704, y + 119, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
			Ikaros.m_pDraw->Border(x + 784, y + 119, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		}
	}

	else if (openTab.ActiveTab == tab14)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 570, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Text(x + 6, y + 80, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), " kek");
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page1", 9, x + 450, y + 355);
	}

	else if (openTab.ActiveTab == tab15)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 382, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page1", 7, x + 450, y + 355);
		MenuDrawTabs("Page2", 15, x + 525, y + 355);
		DrawComboBox(x + 10, y + 300, "ThirdpersonAA", Ikaros.m_pCvars->iThirdpersonAA, 11, 2, ThirdpersonAA, "ThirdpersonAA");
		Slider(x + 10, y + 270, 2, Ikaros.m_pCvars->CircleSpeed, 0, 250, "CircleSpeed", "");
		Slider(x + 10, y + 240, 0, Ikaros.m_pCvars->iNetFakelag, 0.0f, 100.0f, "NetFakelagValue", "");
		DrawCheckBox(x + 10, y + 210, "NetFakelag", Ikaros.m_pCvars->bNetFakelag, "NetFakelag");
		DrawComboBox2(x + 10, y + 180, "Disconnect Reason", Ikaros.m_pCvars->iDisconnectReason, 9, 4, DisconnectReason, "Disconnect Reason");
		DrawComboBox(x + 10, y + 150, "HandsPosition", Ikaros.m_pCvars->iHandsPosition, 3, 2, HandsPosition, "HandsPosition");
		DrawComboBox(x + 10, y + 120, "Autobuy", Ikaros.m_pCvars->iAutoBuy, 2, 4, AutoBuy, "Autobuy");
		DrawComboBox(x + 10, y + 90, "KillSay", Ikaros.m_pCvars->iKillSay, 1, 6, KillSay, "KillSay");

	}
	else if (openTab.ActiveTab == tab17)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 570, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 374, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Colors", 9, x + 375, y + 355);
		DrawCheckBox(x + 10, y + 90, "RainbowOutline", Ikaros.m_pCvars->bRainbowOutline, "RainbowOutline");
		DrawCheckBox(x + 10, y + 120, "RainbowInline", Ikaros.m_pCvars->bRainbowInline, "RainbowInline");
		DrawCheckBox(x + 10, y + 150, "ButtonClickSound", Ikaros.m_pCvars->bButtonClickSound, "ButtonClickSound");
		Slider(x + 10, y + 210, 0, Ikaros.m_pCvars->BlackOut, 0.0f, 100.0f, "BlackOut", "");
		DrawComboBox(x + 10, y + 180, "CloseButtons", Ikaros.m_pCvars->iDrawCloseButton, 1, 2, CloseButtons, "CloseButtons");
	}
	else if (openTab.ActiveTab == tab18)
	{
		Ikaros.m_pCvars->bPlayerList = false;
		DrawGroupBox(x + 6, y + 60, 315, 365, "");
		DrawGroupBox(x + 265, y + 60, 395, 365, "");
		Ikaros.m_pDraw->FillRGBA(x + 100, y + 51, 90, 4, Ikaros.m_pCvars->DrawColor17, Ikaros.m_pCvars->DrawColor18, Ikaros.m_pCvars->DrawColor19, Ikaros.m_pCvars->DrawColor20);
		Ikaros.m_pDraw->Border(x + 449, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		Ikaros.m_pDraw->Border(x + 524, y + 353, w + 72, h + 52, 1, Ikaros.m_pCvars->DrawColor31, Ikaros.m_pCvars->DrawColor32, Ikaros.m_pCvars->DrawColor33, Ikaros.m_pCvars->DrawColor34);//Black 1px Inline
		MenuDrawTabs("Page1", 3, x + 450, y + 355);
		MenuDrawTabs("Page2", 18, x + 525, y + 355);
		DrawCheckBox(x + 10, y + 90, "Local Player ESP", Ikaros.m_pCvars->bEntityOnlyEsp, "Local Player Esp");
		DrawCheckBox(x + 10, y + 120, "Line To Player", Ikaros.m_pCvars->LineToPlayer, "Line To Player");
		DrawCheckBox(x + 10, y + 150, "ArmorBarESP", Ikaros.m_pCvars->ArmorBarESP, "ArmorBarESP");
	}
}
	

bool bWarning = true;

void DrawWarning(int x, int y, int w, int h)
{
	Ikaros.m_pMenu->DrawWindow(x - (w / 2), y - (h / 2), w, h, "Warning");
	Ikaros.m_pDraw->Text(x - (w / 2) + 10, y - (h / 2) + 30, 255, 255, 255, 255, "CL_Move Signature Scan failed.");
	Ikaros.m_pDraw->Text(x - (w / 2) + 10, y - (h / 2) + 43, 255, 255, 255, 255, "Disabled Features: Perfect Silent Aim, Fake Pitch/Yaw, Speedhack.");
	Ikaros.m_pDraw->FillRGBA(x - (w / 2) + ((w / 2) - 22), y - (h / 2) + ((h / 2) + 25), 45, 20, RED(Grey2), GREEN(Grey2), BLUE(Grey2), ALPHA(Grey2));

	if (Ikaros.m_pMouse->IsInBox(x - (w / 2) + ((w / 2) - 22), y - (h / 2) + ((h / 2) + 25), 45, 20))
	{
		Ikaros.m_pDraw->FillRGBA(x - (w / 2) + ((w / 2) - 22), y - (h / 2) + ((h / 2) + 25), 45, 20, RED(Grey3), GREEN(Grey3), BLUE(Grey3), ALPHA(Grey3));
		if (Ikaros.m_pMouse->bMouse1pressed)
			bWarning = false;
	}

	Ikaros.m_pDraw->Border(x - (w / 2) + ((w / 2) - 22), y - (h / 2) + ((h / 2) + 25), 45, 20, 1, 0, 0, 0, 255);
	Ikaros.m_pDraw->Text(x - (w / 2) + ((w / 2) - 10), y - (h / 2) + ((h / 2) + 28), 255, 255, 255, 255, "OK");
}

void CMenu::DrawMenu()
{
	DO_ONCE(openTab.ActiveTab = tab9;);

	if (bLock)
	{
		bClick = false;
	}
	else
	{
		bClick = true;
	}

	ConVar* mouseenable = Ikaros.m_pCvar->FindVar("cl_mouseenable");

	if (GetAsyncKeyState(VK_F12) & 1)
	{
		Ikaros.m_pCvars->bPanicKey = !Ikaros.m_pCvars->bPanicKey;
	}

	if (!Ikaros.m_pCvars->bPanicKey)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			Ikaros.m_pCvars->bShowGUI = !Ikaros.m_pCvars->bShowGUI;
			//if (!Ikaros.m_pCvars->bShowGUI && !Ikaros.m_pCvars->bPanicKey)
			//Save();
			if (Ikaros.m_pCvars->bShowGUI)
				CloseAllComboBoxes();
		}

		Ikaros.m_pRadar->DrawRadar();

		Ikaros.m_pESP->AsusWalls();

		if (Ikaros.m_pEngine->IsConnected()
			&& Ikaros.m_pEngine->IsInGame()
			&& Ikaros.m_pMyPlayer->BaseEnt()
			&& !Ikaros.m_pEngine->IsTakingScreenshot()
			&& !Ikaros.m_pEngine->IsLevelMainMenuBackground())
		{
			Ikaros.m_pESP->DrawEsp();
			C_BaseEntity* pBaseEntity = Ikaros.m_pMyPlayer->BaseEnt();
			Vector Velocity = Ikaros.m_pEntity->GetVelocity(pBaseEntity);
			if (Ikaros.m_pCvars->bShowVelocity)
			{
				if (Ikaros.m_pCvars->bWatermark)
				{
					Ikaros.m_pDraw->Text(2, 20, 255, 255, 255, 255, "Velocity: %0.f", Velocity.Length2D());
				}
				else
				{
					Ikaros.m_pDraw->Text(2, 2, 255, 255, 255, 255, "Velocity: %0.f", Velocity.Length2D());
				}
			}
			//Ikaros.m_pDraw->Text(2, 30, 255, 255, 255, 255, "flRangeModifier: %f", flRangeModifier);
			//Ikaros.m_pDraw->Text(2, 40, 255, 255, 255, 255, "iBulletType: %i", iBulletType);
			//Ikaros.m_pDraw->Text(2, 50, 255, 255, 255, 255, "iPenetration: %i", iPenetration);
		}

		Ikaros.m_pESP->CrossHair();

		if (Ikaros.m_pCvars->bMonitorActive)
			DrawMonitor(Ikaros.m_pCvars->iMonitorPosX, Ikaros.m_pCvars->iMonitorPosY);

		if (Ikaros.m_pCvars->bWatermark)
		{
			if (Ikaros.m_pCvars->iWatermark == 1)
			{
			static float rainbow;
			rainbow += 0.005f;
			if (rainbow > 1.f)
				rainbow = 0.f;
			Color hsb = Color::FromHSB(rainbow, 1.f, 1.f);
			Ikaros.m_pDraw->Text(2, 2, hsb.r(), hsb.g(), hsb.b(), 255, "420hook");
			}
			else if (Ikaros.m_pCvars->iWatermark == 0)
			{
				Ikaros.m_pDraw->Text(2, 2, 255, 255, 255, 255, "420hook");
			}

			if (Ikaros.m_pEngine->IsInGame() && Ikaros.m_pEngine->IsConnected() && breaking_lag_compensation)
				Ikaros.m_pDraw->CenterText(sScreenSize.m_iWidth / 2, (sScreenSize.m_iHeight / 2) - 70, 0, 255, 0, 255, "!!! BREAKING LAG COMPENSATION !!!");
		}
		static int alpha = 0;
		Ikaros.m_pDraw->FillRGBA(0, 0, sScreenSize.m_iWidth, sScreenSize.m_iHeight, 0, 0, 0, alpha);
		if (Ikaros.m_pCvars->bShowGUI)
		{
			if (!bMouseOnce)
			{
				mouseenable->SetValue(0);
				bMouseOnce = true;
			}

			GetCursorPos(&Mouse);
			LPPOINT penis = &Mouse;
			ScreenToClient(GetForegroundWindow(), penis);

			Mouse.x = penis->x;
			Mouse.y = penis->y;
			Ikaros.m_pCvars->iMouseX = Mouse.x;
			Ikaros.m_pCvars->iMouseY = Mouse.y;
			
			DrawMainFrame(Ikaros.m_pCvars->iMenuX, Ikaros.m_pCvars->iMenuY, Ikaros.m_pCvars->iMenuW, Ikaros.m_pCvars->iMenuH);
			if (alpha < Ikaros.m_pCvars->BlackOut)
				alpha++;/*+= 140 / 0.5f * Ikaros.m_pGlobals->frametime;*/
		}
		else
		{
			if (alpha > 0)
				alpha--;
		}

		DrawPlayerList();

		if (Ikaros.m_pCvars->bShowGUI)
		{
			if (Ikaros.m_pCvars->bSigFail)
				if (bWarning)
					DrawWarning(sScreenSize.m_iWidth / 2, sScreenSize.m_iHeight / 2, 350, 100);

			if (Ikaros.m_pMouse->IsInBox(Ikaros.m_pCvars->iMenuX, Ikaros.m_pCvars->iMenuY, 802, 28) && Ikaros.m_pMouse->bMouse1pressed)
			{
				if (!bSavePos)
				{
					SavedX = Ikaros.m_pCvars->iMouseX - Ikaros.m_pCvars->iMenuX;
					SavedY = Ikaros.m_pCvars->iMouseY - Ikaros.m_pCvars->iMenuY;
					bSavePos = true;
				}
				Ikaros.m_pCvars->iMenuX = Ikaros.m_pCvars->iMouseX;
				Ikaros.m_pCvars->iMenuY = Ikaros.m_pCvars->iMouseY;
				Ikaros.m_pCvars->iMenuX = Ikaros.m_pCvars->iMenuX - SavedX;
				Ikaros.m_pCvars->iMenuY = Ikaros.m_pCvars->iMenuY - SavedY;
			}
			else
			{
				bSavePos = false;
			}

			Ikaros.m_pMouse->ClickHandler();
			DrawMouse(Ikaros.m_pCvars->iMouseX, Ikaros.m_pCvars->iMouseY);
		}
		else
		{
			if (bMouseOnce)
			{
				mouseenable->SetValue(1);
				bMouseOnce = false;
			}
		}
	}
	else
	{
		Ikaros.m_pCvars->bShowGUI = false;
	}
}