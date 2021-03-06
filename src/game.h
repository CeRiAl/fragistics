/* game.h
 Fragistics  -  Game Statistics program for Quake 3 Arena
 Copyright (C) 2000   Brian Risinger

see game.cpp or main.cpp for license info
*/

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdio.h>
//#include <stdlib.h>
#include <string>
#include "defines.h"
#include "player.h"
#include "crosstable.h"
#include "gameevent.h"

char* toucase( char* ptr);


class Game {
	PList players;
	Player* pindex[GAME_MAX_SIMU_CLIENTS];

	CrossTable kills[CROSSTABLE_MAX_KILL];

//	int total_kills;
	int total_items;
	int total_time_sec;
	int msgs;
	int msgs_team;
	int all_kills[MAX_KILLTYPE];
	int all_items[ITEM_LAST];

	string mapname;

	int gametype;
	bool gameover;
	int gameoverreason;

	int fraglimit;
	int timelimit;
	int capturelimit;

	int teamscores[TEAM_MAX];

	bool cont1v1;
	int gamenumber;

	Stats *st;

public:
    Game( char* map, int type, int startsec, int frag_limit, int time_limit, int capture_limit, Stats *st_);
    ~Game();
    

    int GetType();
    int GetGameOverReason();
    
    void HandleEvent           (GameEvent *evt);
    
    //  void WriteResults      ( int num);
    
    int GetTotalTime           (); //only valid after endgame event;
    int GetTotalItems          ();
    
    PList* GetPlayers          ();
    int GetTeamScore           (int team);
    
    string GetMapname          ();
    int GetKills               (int type=0);
    void GetTimeStr            (char *buf);
    
    Player* GetPlayerFromIndex (int index);
    
    bool isGameOver            ();
    
    int GetMsgs                ();
    int GetMsgsTeam            ();
    int GetItem                (int type);
    
    int GetGameNumber          ();
    void SetGameNumber         (int gn);
    
    void GetVariableValue      (string *variable, string *value, Stats *stats);
    
    CrossTable* GetKillTable   (int num);
    
    void GameReset1v1          (int time);  //used for continueing 1V1 games
};





class Map {
	string mapname;

	int games;

	PList players;

	CrossTable kills[CROSSTABLE_MAX_KILL];

	int total_items;
	int total_time_sec;
	int msgs;
	int msgs_team;
	int all_kills[MAX_KILLTYPE];
	int all_items[ITEM_LAST];
	

	int gametype[GAMETYPE_MAX];
	int gameoverreason[LIMIT_MAX];

	int teamscores[TEAM_MAX];

	GameList gamelist;

	bool modified;

public:
	Map(string *name){
		mapname=(*name);

		games=0;
		total_items=0;
		total_time_sec=0;
		msgs=0;
		msgs_team=0;

		int i;
		for(i=0;i<TEAM_MAX;i++)
			teamscores[i]=0;
		for(i=0;i<MAX_KILLTYPE;i++)
			all_kills[i]=0;
		for(i=0;i<ITEM_LAST;i++)
			all_items[i]=0;
		for(i=0;i<GAMETYPE_MAX;i++)
			gametype[i]=0;
		for(i=0;i<LIMIT_MAX;i++)
			gameoverreason[i]=0;

		modified=false;
	};
	Map(ConfigFile *cf);
	~Map(){};

	void GetName(string *str){
		(*str)=mapname;
	};

	void AddGame(Game *game);
	void GetVariableValue(string *variable, string *value);

	void InitKilltables();

	void GetTimeStr(char *buf){
		int hour,min,sec;
		char temp[50];
		min=total_time_sec/60;
		sec=total_time_sec%60;
		temp[0]='\0';
		buf[0]='\0';
		hour=min/60;
		min%=60;
		if(hour>0){
			sprintf(buf,"%d:",hour);
		}
		if(min<10){
			sprintf(temp,"0%d:",min);
		}else{
			sprintf(temp,"%d:",min);
		}
		strcat(buf,temp);
	
		if(sec<10){
			sprintf(temp,"0%d",sec);
		}else{
			sprintf(temp,"%d",sec);
		}
		strcat(buf,temp);
	};

	GameList *GetGameList(){return &gamelist;};

	PList* GetPlayers(){return &players;};
	
	bool GetModified(){return modified;};

	void WriteToDb(ConfigFile *cf);
};



class MapList {
	MapList *head;
	MapList *current;
	MapList *next;

	Map *data;

public:
	MapList(){
		head=NULL;
		current=NULL;
		next=NULL;
	};
	~MapList(){
		if(head!=NULL)
			delete head;
		if(next!=NULL)
			delete next;
	};

	void AddGame(Game *game){
		MapList *tmp,*prev=NULL;
		tmp=head;
		string curname;
		while(tmp!=NULL){
			tmp->data->GetName(&curname);
			if(!strcmp(curname.c_str(),game->GetMapname().c_str())){
				tmp->data->AddGame(game);
				return;
			}
			prev=tmp;
			tmp=tmp->next;
		}
		if(prev!=NULL){
			//reached end of list
			prev->next=new MapList();
			prev=prev->next;
			prev->data=new Map(&game->GetMapname());
			prev->data->AddGame(game);
		}else{
			//first map
			head=new MapList();
			head->data=new Map(&game->GetMapname());
			head->data->AddGame(game);
		}
	};

	void AddMap(Map *map){
		MapList *ml=new MapList();
		ml->data=map;
		ml->next=head;
		head=ml;
	};

	bool GetFirst(){
		current=head;
		if(current!=NULL){
			return true;
		}
		return false;
	};

	MapList* GetNext(){
		if((current=current->next)!=NULL){
			return current;
		}
		current=NULL;
		return NULL;
	};
	Map* GetMap(){
		if(current!=NULL)
			return current->data;
		return data;
	};

	void ReSort(int type);
};

#endif //GAME_H_INCLUDED
