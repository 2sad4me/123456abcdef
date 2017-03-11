#pragma once


struct myPlayer {
	int cID;
	int teamNum;
	int health;
	int flags;
	float flashduration;
	float pos_x;
	float pos_y;
	float pos_z;
};
struct ePlayer {
	int teamNum;
	int health;
	DWORD base;
	DWORD glowindex;
	bool isEnemy;
	float pos_x;
	float pos_y;
	float pos_z;

};

struct Glowstruct {
	float r = 1.0;
	float g = 1.0;
	float b = 1.0;
	float a = .5;
	bool rwo = true;
	bool rwuo = false;
	bool bloom = false;
};
