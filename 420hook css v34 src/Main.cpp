#include "Include.h"
#include "detours.h"

HMODULE hMod = NULL;
SetCursorPos_t OrigSetCursorPos;
PeekMessageA_t OrigPeekMessageA;
static int mouseOldX = 0;
static int mouseOldY = 0;
//=============================================================================
void vgui::Panel::OnKeyTyped(wchar_t)
{

}
//=============================================================================
vgui::Panel* vgui::Panel::HasHotkey(wchar_t)
{
	return 0;
}
//=============================================================================
void CreateConsole()
{
	if (AllocConsole())
	{
		//Beep(0x389u, 0x9Bu);
		////Beep(0x389u, 0x9Bu);
		//Beep(0x389u, 0x7Du);
		//Beep(0x389u, 0x7Du);
		//Beep(0x2A3u, 0xCDu);
		//freopen("CONOUT$", "wt", stdout);
		//SetConsoleTitle("Debug Console");
		HWND hwnd;
		char ConsoleSize[1024];
		GetConsoleTitle(ConsoleSize, 1024);
		//hwnd = FindWindow(NULL, ConsoleSize);
		//MoveWindow(hwnd, 25, 35, 900, 500, TRUE);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
}
//=============================================================================
void speak(char *pszText, ...)
{
	if (!pszText)
	{
		return;
	}

#ifdef DEBUG
	register char * In = pszText;
	register char *  Arg = (char *)&pszText + _INTSIZEOF(pszText);
	register __int32 len = 0;

	if (In == NULL || Arg == NULL)
	{
		return;
	}

	char Out[0x1000];
	len = vsprintf(Out, In, Arg);

	Out[len + 0] = '\r';
	Out[len + 1] = '\n';
	Out[len + 2] = '\0';
	len = len + 2;

	printf(Out);
#endif
}
//=============================================================================
bool peekMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!Ikaros.m_pCvars->bShowGUI)
		return false;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		return true;
	}
	return false;
}
//=============================================================================
BOOL WINAPI SetCursorPosHook(int x, int y)
{
	mouseOldX = x;
	mouseOldY = y;

	if (Ikaros.m_pCvars->bShowGUI && Ikaros.m_pEngine->IsInGame())
		return true;

	if (OrigSetCursorPos == 0)
		return false;

	return OrigSetCursorPos(x, y);
}
//=============================================================================
BOOL WINAPI PeekMessageAHook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	if (!OrigPeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg))
		return false;
	if (Ikaros.m_pEngine->IsInGame() && Ikaros.m_pEngine->IsConnected()){
		if (peekMessage(lpMsg->message, lpMsg->wParam, lpMsg->lParam))
			return false;
	}
	return true;
}
//=============================================================================
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hMod = hModule;
		DisableThreadLibraryCalls(hModule);
#ifdef DEBUG
		CreateConsole();
#endif
		speak("***         *****     **    *********     *********           *****");
		speak("***        ** ***     **    *********     *********          ******");
		speak("***       **  ***     **    **            **                **     **");
		speak("***      **   ***     **    **            **               **       **");
		speak("***     **    ***     **    *********     *********       *************");
		speak("***    **     ***     **    *********     *********      ***************");
		speak("***   **      ***     **    **     **     **     **     **            **");
		speak("***  **       ***     **    **     **     **     **    **              **");
		speak("*** **        ***     **    *********     *********   **                **");
		speak("*****         ***     **    *********     *********  **                  **");

		speak("Ikaros attached");
		OrigSetCursorPos = (SetCursorPos_t)DetourFunction((PBYTE)SetCursorPos, (PBYTE)SetCursorPosHook);
		OrigPeekMessageA = (PeekMessageA_t)DetourFunction((PBYTE)PeekMessageA, (PBYTE)PeekMessageAHook);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartHooking, 0, 0, 0);
		speak("Started Hooking");
	}
	return TRUE;
}