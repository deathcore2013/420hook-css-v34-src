#include "CGameEventManager2.h"
#include "CHitmarker.h"

int iKills = 0;
char hitdir[255];
char headshotdir[255];
char firstblooddir[255];
char doubledir[255];
char tripledir[255];
char megadir[255];
char ultradir[255];
char ludicrousdir[255];
char monsterdir[255];

void cGameEventManager::FireGameEvent( IGameEvent *event )
{
	char str[128];
	const char* szEventName = event->GetName( );

	if( !szEventName )
		return;

	if(szEventName)
	{
		if(!strcmp(szEventName,"round_start"))
		{
		if (Ikaros.m_pCvars->roundsay < 1)
			return;

		if (Ikaros.m_pCvars->iRoundsay == 1)
			{
				Ikaros.m_pEngine->ClientCmd("say 420hook"); 
			}
		else if (Ikaros.m_pCvars->iRoundsay == 2)
			{
				Ikaros.m_pEngine->ClientCmd("say 999hook");
			}
		else if (Ikaros.m_pCvars->iRoundsay == 3)
			{
			   	Ikaros.m_pEngine->ClientCmd("say game bandit 1.1");
			}
		else if (Ikaros.m_pCvars->iRoundsay == 4)
			{
			   	Ikaros.m_pEngine->ClientCmd("say bennyhook.pw");
			}
		else if (Ikaros.m_pCvars->iRoundsay == 5)
			{
				Ikaros.m_pEngine->ClientCmd("say polenware.pw");
			}
		}

		if (Ikaros.m_pCvars->bChatRepeat)
		{
			if (!strcmp(szEventName, "player_say"))
			{
				char tosay[512];
				auto text = event->GetString("text");
				sprintf_s(tosay, "say %s", text);
				Ikaros.m_pEngine->ClientCmd(tosay);
			}
		}

		if(!strcmp(szEventName,"round_freeze_end"))
		{
			if (Ikaros.m_pCvars->iAutoBuy == 1)
			{
				Ikaros.m_pEngine->ClientCmd("say !akhelmdeagle"); 
			}
			else if (Ikaros.m_pCvars->iAutoBuy == 2)
			{
				Ikaros.m_pEngine->ClientCmd("say !m249helmdeagle");
			}
			else if (Ikaros.m_pCvars->iAutoBuy == 3)
			{
				Ikaros.m_pEngine->ClientCmd("say !awphelmdeagle");
			}
		}

		if (Ikaros.m_pCvars->bHitSound)
		{
			if (!strcmp(szEventName, "player_hurt"))
			{
				C_BaseEntity* pAttacker = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetPlayerForUserID(event->GetInt("attacker")))->GetBaseEntity();
				C_BaseEntity* pVictim = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetPlayerForUserID(event->GetInt("userid")))->GetBaseEntity();

				if (pAttacker == NULL || pVictim == NULL || Ikaros.m_pMyPlayer->BaseEnt() == NULL)
					return;

				if (Ikaros.m_pMyPlayer->BaseEnt()->index == pAttacker->index && Ikaros.m_pMyPlayer->BaseEnt()->index != pVictim->index)
				{
					if (!(Ikaros.m_pEntity->GetTeamID(Ikaros.m_pMyPlayer->BaseEnt()) == Ikaros.m_pEntity->GetTeamID(pVictim)))
					{
						sndPlaySoundA(hitmarkerdata, SND_NOWAIT | SND_ASYNC | SND_MEMORY);
						sndPlaySoundA(hitdir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
					}
				}
			}
		}

		if (!strcmp(szEventName, "player_death"))
		{
			C_BaseEntity* pAttacker = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetPlayerForUserID(event->GetInt("attacker")))->GetBaseEntity();
			C_BaseEntity* pVictim = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetPlayerForUserID(event->GetInt("userid")))->GetBaseEntity();

			if (pAttacker == NULL || pVictim == NULL || Ikaros.m_pMyPlayer->BaseEnt() == NULL)
				return;

			if (Ikaros.m_pMyPlayer->BaseEnt()->index == pAttacker->index && Ikaros.m_pMyPlayer->BaseEnt()->index != pVictim->index)
			{
				if (!(Ikaros.m_pEntity->GetTeamID(Ikaros.m_pMyPlayer->BaseEnt()) == Ikaros.m_pEntity->GetTeamID(pVictim)))

					if (Ikaros.m_pCvars->iKillSay == 1)
					{
						Ikaros.m_pEngine->ClientCmd("say ez win");
					}
					else if (Ikaros.m_pCvars->iKillSay == 2)
					{
						Ikaros.m_pEngine->ClientCmd("say 1");
					}
					else if (Ikaros.m_pCvars->iKillSay == 3)
					{
						Ikaros.m_pEngine->ClientCmd("say 1tap");
					}
					else if (Ikaros.m_pCvars->iKillSay == 4)
					{
						Ikaros.m_pEngine->ClientCmd("say noob");
					}
					else if (Ikaros.m_pCvars->iKillSay == 5)
					{
						Ikaros.m_pEngine->ClientCmd("say sucker");
					}
				}
		}

			if (Ikaros.m_pCvars->RadioKillInformer)
			{
			if (!strcmp(szEventName, "player_death"))
			{
				C_BaseEntity* pAttacker = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetPlayerForUserID(event->GetInt("attacker")))->GetBaseEntity();
				C_BaseEntity* pVictim = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetPlayerForUserID(event->GetInt("userid")))->GetBaseEntity();

				if (pAttacker == NULL || pVictim == NULL || Ikaros.m_pMyPlayer->BaseEnt() == NULL)
					return;

				if (Ikaros.m_pMyPlayer->BaseEnt()->index == pAttacker->index && Ikaros.m_pMyPlayer->BaseEnt()->index != pVictim->index)
				{
					if (!(Ikaros.m_pEntity->GetTeamID(Ikaros.m_pMyPlayer->BaseEnt()) == Ikaros.m_pEntity->GetTeamID(pVictim)))
					Ikaros.m_pEngine->ClientCmd("enemydown");
					{
						++iKills;
					
						if (iKills == 1)
						{
							sndPlaySoundA(firstblooddir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (iKills == 2)
						{
							sndPlaySoundA(doubledir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (iKills == 4)
						{
							sndPlaySoundA(tripledir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (iKills == 13)
						{
							sndPlaySoundA(megadir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (iKills == 15)
						{
							sndPlaySoundA(ultradir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (iKills == 18)
						{
							sndPlaySoundA(ludicrousdir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (iKills == 21)
						{
							sndPlaySoundA(monsterdir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
						else if (event->GetBool("headshot"))
						{
							sndPlaySoundA(headshotdir, SND_FILENAME | SND_NOWAIT | SND_NODEFAULT | SND_ASYNC);
						}
					}
				}
				else if (Ikaros.m_pMyPlayer->BaseEnt()->index == pVictim->index)
				{
					iKills = 0;
				}
			}
		}
	}
}


void cGameEventManager::RegisterSelf( )
{
	strcat(hitdir, soundsdir);
	strcat(hitdir, "\\Hit.wav");
	strcat(headshotdir, soundsdir);
	strcat(headshotdir, "\\Headshot.wav");
	strcat(firstblooddir, soundsdir);
	strcat(firstblooddir, "\\Firstblood.wav");
	strcat(doubledir, soundsdir);
	strcat(doubledir, "\\Doublekill.wav");
	strcat(tripledir, soundsdir);
	strcat(tripledir, "\\Multikill.wav");
	strcat(megadir, soundsdir);
	strcat(megadir, "\\Megakill.wav");
	strcat(ultradir, soundsdir);
	strcat(ultradir, "\\Ultrakill.wav");
	strcat(ludicrousdir, soundsdir);
	strcat(ludicrousdir, "\\Ludicrouskill.wav");
	strcat(monsterdir, soundsdir);
	strcat(monsterdir, "\\Monsterkill.wav");
	//Ikaros.m_pGameEventManager->AddListener( this, "game_newmap", false );
	//Ikaros.m_pGameEventManager->AddListener( this, "game_start", false );
	//Ikaros.m_pGameEventManager->AddListener( this, "game_end", false );
	//Ikaros.m_pGameEventManager->AddListener( this, "game_message", false );
	Ikaros.m_pGameEventManager->AddListener( this, "round_start", false );
	Ikaros.m_pGameEventManager->AddListener( this, "round_freeze_end", false );
	/*Ikaros.m_pGameEventManager->AddListener( this, "round_end", false );
	Ikaros.m_pGameEventManager->AddListener( this, "break_breakable", false );
	Ikaros.m_pGameEventManager->AddListener( this, "break_prop", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_connect", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_info", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_team", false );*/
	Ikaros.m_pGameEventManager->AddListener(this, "player_say", false);
	Ikaros.m_pGameEventManager->AddListener( this, "player_hurt", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_death", false );
	/*Ikaros.m_pGameEventManager->AddListener( this, "player_disconnect", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_beginplant", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_abortplant", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_planted", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_defused", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_exploded", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_dropped", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_pickup", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_begindefuse", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_abortdefuse", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hostage_follows", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hostage_hurt", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hostage_killed", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hostage_rescued", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hostage_stops_following", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hostage_rescued_all", false );
	Ikaros.m_pGameEventManager->AddListener( this, "vip_escaped", false );
	Ikaros.m_pGameEventManager->AddListener( this, "vip_killed", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_radio", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bomb_beep", false );
	Ikaros.m_pGameEventManager->AddListener( this, "weapon_fire", false );
	Ikaros.m_pGameEventManager->AddListener( this, "weapon_fire_on_empty", false );
	Ikaros.m_pGameEventManager->AddListener( this, "weapon_reload", false );
	Ikaros.m_pGameEventManager->AddListener( this, "weapon_zoom", false );
	Ikaros.m_pGameEventManager->AddListener( this, "item_pickup", false );
	Ikaros.m_pGameEventManager->AddListener( this, "grenade_bounce", false );
	Ikaros.m_pGameEventManager->AddListener( this, "hegrenade_detonate", false );
	Ikaros.m_pGameEventManager->AddListener( this, "flashbang_detonate", false );
	Ikaros.m_pGameEventManager->AddListener( this, "smokegrenade_detonate", false );
	Ikaros.m_pGameEventManager->AddListener( this, "bullet_impact", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_footstep", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_jump", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_blind", false );
	Ikaros.m_pGameEventManager->AddListener( this, "player_falldamage", false );
	Ikaros.m_pGameEventManager->AddListener( this, "door_moving", false );
	Ikaros.m_pGameEventManager->AddListener( this, "round_freeze_end", false );
	Ikaros.m_pGameEventManager->AddListener( this, "nav_blocked", false );*/
}