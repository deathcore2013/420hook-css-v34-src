#include "Include.h"

CreateInterfaceFn g_AppSysFactory = NULL;
CreateInterfaceFn ClientFactory = NULL;
CreateInterfaceFn EngineFactory = NULL;
CreateInterfaceFn PhysicFactory = NULL;
CreateInterfaceFn FileSystemFactory = NULL;
CreateInterfaceFn StudioFactory = NULL;
CreateInterfaceFn MaterialFactory = NULL;
CreateInterfaceFn VGUIFactory = NULL;
CreateInterfaceFn VGUI2Factory = NULL;
HANDLE hThread = NULL;
HANDLE hClient = NULL;
HANDLE hEngine = NULL;
IMaterial* mat = NULL;
IMaterial* mat2 = NULL;
IMaterial* mat3 = NULL;
DWORD dwCL_MOVE = NULL;
DWORD OrigCreateMove = NULL;

CL_Move_t CL_Move;

void GetInterfaces()
{
	DWORD* dwClientVMT = NULL;
	ClientFactory =			(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("client.dll"), "CreateInterface");
	EngineFactory =			(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("engine.dll"), "CreateInterface");
	PhysicFactory =			(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vphysics.dll"), "CreateInterface");
	FileSystemFactory =		(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("FileSystem_Steam.dll"), "CreateInterface");
	StudioFactory =			(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("StudioRender.dll"), "CreateInterface");
	MaterialFactory =		(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("MaterialSystem.dll"), "CreateInterface");
	VGUIFactory =			(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vguimatsurface.dll"), "CreateInterface");
	VGUI2Factory =			(CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vgui2.dll"), "CreateInterface");

	Ikaros.m_pClient = (IBaseClientDLL*)ClientFactory(CLIENT_DLL_INTERFACE_VERSION, NULL);
	dwClientVMT = (DWORD*)*(DWORD*)Ikaros.m_pClient;
	speak("Ikaros.m_pClient: [0x%.8X]", (DWORD)Ikaros.m_pClient);

	//Ikaros.m_pPrediction = (IPrediction*)ClientFactory("VClientPrediction001", NULL);
	Ikaros.m_pPrediction = (CPrediction*)ClientFactory("VClientPrediction001", NULL);
	speak("Ikaros.m_pPrediction: [0x%.8X]", (DWORD)Ikaros.m_pPrediction);

	Ikaros.m_pGameMovement = (CGameMovement*)ClientFactory("GameMovement001", NULL);
	speak("Ikaros.m_pGameMovement: [0x%.8X]", (DWORD)Ikaros.m_pGameMovement);

	Ikaros.m_pInput = (CInput*)*(PDWORD)*(PDWORD)(dwClientVMT[15] + 0x2);
	speak("Ikaros.m_pInput: [0x%.8X]", (DWORD)Ikaros.m_pInput);

	Ikaros.m_pEntList = (IClientEntityList*)ClientFactory("VClientEntityList003", NULL);
	speak("Ikaros.m_pEntList: [0x%.8X]", (DWORD)Ikaros.m_pEntList);

	Ikaros.m_pEngine = (IVEngineClient*)EngineFactory("VEngineClient012", NULL);
	speak("Ikaros.m_pEngine: [0x%.8X]", (DWORD)Ikaros.m_pEngine);

	Ikaros.m_pEnginetrace = (IEngineTrace*)EngineFactory("EngineTraceClient003", NULL);
	speak("Ikaros.m_pEnginetrace: [0x%.8X]", (DWORD)Ikaros.m_pEnginetrace);

	Ikaros.m_pStudioRender = (IStudioRender *)StudioFactory(STUDIO_RENDER_INTERFACE_VERSION, NULL);
	speak("Ikaros.m_pStudioRender: [0x%.8X]", (DWORD)Ikaros.m_pStudioRender);

	Ikaros.m_pRender = (IVRenderView*)EngineFactory("VEngineRenderView012", NULL);
	speak("Ikaros.m_pRender: [0x%.8X]", (DWORD)Ikaros.m_pRender);

	Ikaros.m_pPanel = (vgui::IPanel*)VGUI2Factory("VGUI_Panel009", NULL);
	speak("Ikaros.m_pPanel: [0x%.8X]", (DWORD)Ikaros.m_pPanel);

	Ikaros.m_pCvar = (ICvar*)EngineFactory(VENGINE_CVAR_INTERFACE_VERSION, NULL);
	speak("Ikaros.m_pCvar: [0x%.8X]", (DWORD)Ikaros.m_pCvar);

	Ikaros.m_pModelRender = (IVModelRender*)EngineFactory("VEngineModel012", NULL);
	speak("Ikaros.m_pModelRender: [0x%.8X]", (DWORD)Ikaros.m_pModelRender);

	Ikaros.m_pMaterialSystem = (IMaterialSystem*)MaterialFactory("VMaterialSystem076", NULL);
	speak("Ikaros.m_pMaterialSystem: [0x%.8X]", (DWORD)Ikaros.m_pMaterialSystem);

	Ikaros.m_pModelinfo = (IVModelInfoClient*)EngineFactory("VModelInfoClient003", NULL);
	speak("Ikaros.m_pModelinfo: [0x%.8X]", (DWORD)Ikaros.m_pModelinfo);

	Ikaros.m_pGameEventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);
	speak("Ikaros.m_pGameEventManager: [0x%.8X]", (DWORD)Ikaros.m_pGameEventManager);

	Ikaros.m_pPhysicAPI = (IPhysicsSurfaceProps*)PhysicFactory("VPhysicsSurfaceProps001", NULL);
	speak("Ikaros.m_pPhysicAPI: [0x%.8X]", (DWORD)Ikaros.m_pPhysicAPI);

	if (Ikaros.m_pMaterialSystem)
	{
		mat = Ikaros.m_pNeeded->CreateMaterial(false);//player visible
		mat2 = Ikaros.m_pNeeded->CreateMaterial(true);//weapons
		mat3 = Ikaros.m_pNeeded->CreateMaterial(true);//player nonvisible
	}
}

void* AppSysFactory(const char* pszName, int* piRetnCode)
{
	void* lpReturn = g_AppSysFactory(pszName, piRetnCode);

	DO_ONCE(
		vgui::VGui_InitInterfacesList("CLIENT", &g_AppSysFactory, 1);

		if (!vgui::VGui_InitInterfacesList("CLIENT", &g_AppSysFactory, 1))
			speak("[ERROR]: Failed to initialize VGUI");

		if (lpReturn)
		{
			if (strcmp(pszName, "VGUI_Surface030"))
			{
				Ikaros.m_pSurface = (vgui::ISurface*)lpReturn;
				speak("Ikaros.m_pSurface: [0x%.8X]", (DWORD)Ikaros.m_pSurface);

				Ikaros.m_pMatSurface = (IMatSystemSurface*)vgui::surface();
				speak("Ikaros.m_pMatSurface: [0x%.8X]", (DWORD)Ikaros.m_pMatSurface);
			}
		}
	);

	return (void*)lpReturn;
}

int	__stdcall new_Init(CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals)
{
	g_AppSysFactory = appSystemFactory;
	Ikaros.m_pGlobals = pGlobals;

	return pClientHook->Function<Init_t>(0)(AppSysFactory, physicsFactory, pGlobals);
}

void __stdcall new_RunCommand(C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
{
	pPredHook->Function<RunCommand_t>(17)(player, ucmd, moveHelper);
}

void new_CL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	CL_Move(accumulated_extra_samples, bFinalTick);

	if (Ikaros.m_pCvars->bSpeed)
	{
		if (Ikaros.m_pNeeded->IsKeyPressed(Ikaros.m_pCvars->iSpeedKey))
		{
			for (int i = 0; i <= Ikaros.m_pCvars->iSpeedVal; i++)
				CL_Move(accumulated_extra_samples, bFinalTick);
		}
	}
}

static int iSpeedTicks = 0;

void __declspec(naked) __fastcall hkCreateMove(void* thisptr, void* _EAX, int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		mov     bSendPacket, bl
		push    ebp
		mov     ebp, esp
		sub     esp, 8
		pusha
		push[ebp + 10h];
		push[ebp + 0Ch];
		push[ebp + 8h];
		call    OrigCreateMove
	}

	new_CreateMove(sequence_number, input_sample_frametime, active);

	__asm
	{
		popa
		mov     bl, bSendPacket
		mov     esp, ebp
		pop     ebp
		retn    0Ch
	}
}

void ApplySendPacket(void)
{
	auto CL_RunPrediction = (uintptr_t)(GetModuleHandleW(L"engine.dll")) + 0x464D4;

	MEMORY_BASIC_INFORMATION Memory = { 0 };
	VirtualQuery((LPCVOID)CL_RunPrediction, &Memory, sizeof(Memory));

	if (Memory.Protect != PAGE_NOACCESS)
	{
		DWORD Protect = 0;

		if (VirtualProtect((LPVOID)CL_RunPrediction, 5, PAGE_EXECUTE_READWRITE, &Protect) != FALSE)
		{
			memset((void*)CL_RunPrediction, 0x90, 5);

			VirtualProtect((LPVOID)CL_RunPrediction, 5, Protect, &Protect);
		}
	}
}

bool __fastcall hkdWriteUsercmdDeltaToBuffer(void* ecx, void*, bf_write* buf, int from, int to, bool isnewcommand)
{
    auto GetUserCmd = [](int sequence_number)
    {
        return &(*(CUserCmd**)((DWORD)Ikaros.m_pInput + 0xC4))[sequence_number % MULTIPLAYER_BACKUP];
    };
    static auto WriteUsercmd = (void(__cdecl*)(bf_write*, CUserCmd*, CUserCmd*))(Ikaros.m_pNeeded->FindPattern("client.dll", "51 8B 44 24 10 8B 48 04"));
 
    CUserCmd nullcmd, *f, *t;
 
    if (from == -1)
        f = &nullcmd;
    else
    {
        f = GetUserCmd(from);
 
        if (!f)
            f = &nullcmd;
    }
 
    t = GetUserCmd(to);
    if (!t)
        t = &nullcmd;
 
    WriteUsercmd(buf, t, f);
 
    return (!(*(BYTE*)((DWORD)buf + 0x10)));
}

bool __fastcall hkdIsPlayingTimeDemo(void* ecx, void*)
{
	static auto call = Ikaros.m_pNeeded->FindPattern("client.dll","84 C0 74 07 C6 05");

	if (Ikaros.m_pCvars->LagFix)
	{
		if (Ikaros.m_pEngine->IsInGame() || Ikaros.m_pEngine->IsConnected())
		{
			//if (_ReturnAddress() == (void*)call)
				return true;
		}
	}

	return pEngineHook->Function<IsPlayingTimeDemoFn>(78)(ecx);
}

typedef void(__stdcall* OverrideViewFn)(void*);
extern void __stdcall hkdOverrideView(void* pSetup);
void __stdcall hkdOverrideView(void* pSetup)
{
	if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
		*(float*)((DWORD)pSetup + 0x28) = Ikaros.m_pCvars->OverrideFOV;

	pClientModeHook->Function<OverrideViewFn>(16)(pSetup);
}

void StartHooking()
{
	for (; hClient == NULL; Sleep(100))
		hClient = (HMODULE)GetModuleHandleA("client.dll");

	for (; hEngine == NULL; Sleep(100))
		hEngine = (HMODULE)GetModuleHandleA("engine.dll");

	//DWORD dwCL_MOVE = ((DWORD)hEngine + 0x42510);
	dwCL_MOVE = Ikaros.m_pNeeded->dwFindPattern((DWORD)hEngine + 0x1000, 0x2df000, (PBYTE)"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x1C\x83\x3D\x00\x00\x00\x00\x00\x0F\x8C\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\x01\x53\xB3\x01", "xx????xxx????xxxx????xxxxx?????xx????x????xxxx????xx????xxxxx");
	speak("Found CL_Move: [0x%.8X]", dwCL_MOVE);

	if (hEngine != NULL && dwCL_MOVE != NULL)
	{
		CL_Move = (CL_Move_t)DetourFunction((PBYTE)dwCL_MOVE, (PBYTE)new_CL_Move);
	}

	GetInterfaces();

	Ikaros.m_pNetVar->GetOffsets();
	DO_ONCE(Ikaros.m_pCvars->OverrideFOV = 90.f; Ikaros.m_pCvars->ViewmodelFOV = 90.f;);
	Ikaros.m_pSettings->CreateCfgFile();
	Ikaros.m_pSettings->LoadSettings();
	Ikaros.m_pCvars->bShowGUI = true;
	Ikaros.m_pMenu->CloseAllComboBoxes();
	for (int i = 0; i < 65; i++)
	{
		Ikaros.m_pPlayerlist[i].index = 0;
		Ikaros.m_pPlayerlist[i].bFriend = false;
		Ikaros.m_pPlayerlist[i].iPitch = 1;
		Ikaros.m_pPlayerlist[i].iYaw = 0;
		Ikaros.m_pPlayerlist[i].bAdvancedYaw = false;
		Ikaros.m_pPlayerlist[i].iAdvancedYaw = 0.0f;
	}

	DWORD dwClient = (DWORD)Ikaros.m_pClient;
	DWORD dwInput = (DWORD)Ikaros.m_pInput;
	DWORD dwModel = (DWORD)Ikaros.m_pModelRender;
	DWORD dwPanel = (DWORD)Ikaros.m_pPanel;
	DWORD dwPrediction = (DWORD)Ikaros.m_pPrediction;
	DWORD dwEngine = (DWORD)Ikaros.m_pEngine;

	ApplySendPacket();

	pInputHook = new C_VMT(&dwInput);
	pInputHook->Hook((DWORD)new_GetUserCmd, 8);

	pModelHook = new C_VMT(&dwModel);
	pModelHook->Hook((DWORD)new_DrawModelEx, 19);

	pPanelHook = new C_VMT(&dwPanel);
	pPanelHook->Hook((DWORD)new_PaintTraverse, 40);

	pEngineHook = new C_VMT(&dwEngine);
	pEngineHook->Hook((DWORD)hkdIsPlayingTimeDemo, 78);

	//pPredHook = new C_VMT(&dwPrediction);
	//pPredHook->Hook((DWORD)new_RunCommand, 17);

	pClientHook = new C_VMT(&dwClient);
	pClientHook->Hook((DWORD)new_Init, 0);
	pClientHook->Hook((DWORD)new_HudUpdate, 8);
	OrigCreateMove = pClientHook->OldPointer(18);
	pClientHook->Hook((DWORD)hkCreateMove, 18);
	//pClientHook->Hook((DWORD)new_CreateMove, 18);
	pClientHook->Hook((DWORD)new_FrameStageNotify, 32);
	pClientHook->Hook((DWORD)hkdDispatchUserMesssage, 33);
	pClientHook->Hook((DWORD)hkdWriteUsercmdDeltaToBuffer, 20);

	if (dwCL_MOVE == NULL)
		Ikaros.m_pCvars->bSigFail = true;
	else
		Ikaros.m_pCvars->bSigFail = false;

	while (!Ikaros.m_pClientMode)
		Ikaros.m_pClientMode = **(IClientMode***)((*(uintptr_t**)g_pClient)[7] + 0x2);

	speak("Ikaros.m_pClientMode: [0x%.8X]", (DWORD)Ikaros.m_pClientMode);

	DWORD dwClientMode = (DWORD)Ikaros.m_pClientMode;
	pClientModeHook = new C_VMT(&dwClientMode);
	pClientModeHook->Hook((DWORD)hkdGetViewModelFOV, 32);
	pClientModeHook->Hook((DWORD)hkdOverrideView, 16);

	ExitThread(1);
	//FreeLibraryAndExitThread(hMod, 1);
}