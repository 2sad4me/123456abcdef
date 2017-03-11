#include "Offsets.h"
#include "Structs.h"
#include <iostream>
#include <TlHelp32.h>
#include <thread>

using namespace std;
//Data
HWND GameWindow;
DWORD pid;
HANDLE hProc;
BYTE* client;
DWORD localPlayer;
DWORD entityList;
ePlayer enemy[1];
myPlayer me;
bool bTrigger;
DWORD glowobject;
int eCount;



//Functions
void getClient(void);
void trigger(void);
void bhop(void);
void glow(void);
void noFlash(void);

int main() {

	getClient();
	//std::thread inCross(trigger);
	//std::thread bunny(bhop);
	while (true) {
		glow();
		//trigger();
		bhop();
		noFlash();
	}
	cout << "External Trigger and bHop by Louis" << endl;
	cin.get();
	}



void getClient() {//try hinzufügen, um das automatische Injecten zu ermöglichen, test
	cout << "suche Fenster..." << endl;
	do {
		GameWindow = FindWindow(NULL, "Counter-Strike: Global Offensive");
		Sleep(1000);
	} while (!GameWindow);
	cout << "Fenster wurde gefunden!" << endl;
	GetWindowThreadProcessId(GameWindow, &pid);
	hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

	MODULEENTRY32 me32;

	me32.dwSize = sizeof(MODULEENTRY32);

	cout << "Looking for client.dll..." << endl;

	Module32First(hModuleSnap, &me32);

	string sClientdll = "client.dll";

	while (me32.szModule != sClientdll)
	{
		Module32Next(hModuleSnap, &me32);

	}

	cout << "client.dll found!" << endl;

	client = me32.modBaseAddr;
	CloseHandle(hModuleSnap);

}

void getInGameInfo() {


	ReadProcessMemory(hProc, LPCVOID(client + olocalPlayer), &localPlayer, sizeof(localPlayer), NULL);
	ReadProcessMemory(hProc, LPCVOID(client + oentityList), &entityList, sizeof(entityList), NULL);
	ReadProcessMemory(hProc, (LPCVOID)(localPlayer + oteamNum), &me.teamNum, sizeof(int), NULL);
	ReadProcessMemory(hProc, (LPCVOID)(localPlayer + ohealth), &me.health, sizeof(int), NULL);
	for (int i = 2; i < 64;i++) {
		ReadProcessMemory(hProc, (LPCVOID)(client + oentityList + (i * 0x10) - 0x10), &enemy[i].base, sizeof(DWORD), NULL);//get Player Base by Index
		if (enemy[i].base == 0)
			break;
		ReadProcessMemory(hProc, (LPCVOID)(enemy[i].base + oteamNum), &enemy[i].teamNum, sizeof(int), NULL);

	}
	ReadProcessMemory(hProc, (LPCVOID)(localPlayer + ocrosshairID), &me.cID, sizeof(int), NULL);
	ReadProcessMemory(hProc, (LPCVOID)(localPlayer + oflags), &me.flags, sizeof(int), NULL);

}
void trigger() {
	//while(true){
		 //getting Info about the enemy and crosshair
		/*if (me.cID > 0) {
			cout << "cID:" << me.cID << endl;
			cout << "teamnum:" << enemy[me.cID].teamNum << endl;
			Sleep(2000);
		}*/
		
		if (me.cID > 0 && me.cID < 64 && (GetKeyState(VK_SHIFT) & 0x8000)) {

			if (enemy[me.cID].teamNum != me.teamNum) {
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				Sleep(10);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

			}
		}
		//}
		


		
	}
void bhop() {
	
	int onetickJump = 6;
	//while (true) {
		if ((me.flags == 257 || me.flags == 263) && (GetKeyState(VK_SPACE) & 0x8000)) {
			WriteProcessMemory(hProc, (LPVOID)(client + oforceJump), &onetickJump, sizeof(int), NULL);
		}
	//}
}
	

void glow() {

	getInGameInfo();
	for (int i = 2; i < 64; i++) {
		if (enemy[i].teamNum != me.teamNum) {
			Glowstruct glow;
			ReadProcessMemory(hProc, LPCVOID(enemy[i].base + oGlowindex), &enemy[i].glowindex, sizeof(DWORD), NULL);
			ReadProcessMemory(hProc, LPCVOID(client + oGlowobject), &glowobject, sizeof(DWORD), NULL);
			int current = glowobject + (enemy[i].glowindex * 0x38 + 0x4);
			WriteProcessMemory(hProc, (LPVOID)(current), &glow.r, sizeof(float), NULL);
			current = glowobject + (enemy[i].glowindex * 0x38 + 0x8);
			WriteProcessMemory(hProc, (LPVOID)(current), &glow.g, sizeof(int), NULL);
			current = glowobject + (enemy[i].glowindex * 0x38 + 0xC);
			WriteProcessMemory(hProc, (LPVOID)(current), &glow.b, sizeof(int), NULL);
			current = glowobject + (enemy[i].glowindex * 0x38 + 0x10);
			WriteProcessMemory(hProc, (LPVOID)(current), &glow.a, sizeof(int), NULL);
			current = glowobject + (enemy[i].glowindex * 0x38 + 0x24);
			WriteProcessMemory(hProc, (LPVOID)(current), &glow.rwo, sizeof(bool), NULL);
			current = glowobject + (enemy[i].glowindex * 0x38 + 0x25);
			WriteProcessMemory(hProc, (LPVOID)(current), &glow.rwo, sizeof(bool), NULL);
			current = glowobject + (enemy[i].glowindex * 0x38 + 0x26);
			WriteProcessMemory(hProc, (LPVOID)current, &glow.bloom, sizeof(bool), NULL);
		}
	}
}

void noFlash() {
		ReadProcessMemory(hProc, (LPCVOID)(localPlayer + oFlashDuration), &me.flashduration, sizeof(float), NULL);
		if (me.flashduration > 0.0) {
			float reset = 0.0;
			WriteProcessMemory(hProc, (LPVOID)(localPlayer + oFlashDuration), &reset, sizeof(float), NULL);
		}
}

