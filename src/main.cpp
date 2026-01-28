#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "injector/injector.hpp"
#include "ini.h"
#include "cdmain.h"

#include "iostream"

ini::File trackcamconf = ini::open(std::filesystem::current_path().string() + "\\CDTrackCam.ini");

DWORD WINAPI MainTHREAD(LPVOID)
{
	int trackcam_keyactiv = std::stoi(trackcamconf["General"].get<std::string>("KeyActivation"), nullptr, 16);

	int trackcam_mode_move = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_MoveMode"), nullptr, 16);
	int trackcam_mode_zoom = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_ZoomMode"), nullptr, 16);
	int trackcam_movein_moveout = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_MoveInOut"), nullptr, 16);
	int trackcam_up = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_Up"), nullptr, 16);
	int trackcam_left = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_Left"), nullptr, 16);
	int trackcam_right = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_Right"), nullptr, 16);
	int trackcam_down = std::stoi(trackcamconf["General"].get<std::string>("TrackCam_Down"), nullptr, 16);

	injector::WriteMemory<int>(0x435F22, trackcam_mode_move, true);

	injector::WriteMemory<int>(0x436334, trackcam_mode_zoom, true);

	injector::WriteMemory<int>(0x4360CD, trackcam_movein_moveout, true);
	injector::WriteMemory<int>(0x4365D2, trackcam_movein_moveout, true);

	injector::WriteMemory<int>(0x435F93, trackcam_up, true);
	injector::WriteMemory<int>(0x43638E, trackcam_up, true);
	injector::WriteMemory<int>(0x436586, trackcam_up, true);

	injector::WriteMemory<int>(0x435FDF, trackcam_left, true);
	injector::WriteMemory<int>(0x43645E, trackcam_left, true);
	injector::WriteMemory<int>(0x43653A, trackcam_left, true);

	injector::WriteMemory<int>(0x436056, trackcam_right, true);
	injector::WriteMemory<int>(0x4364C6, trackcam_right, true);
	injector::WriteMemory<int>(0x436560, trackcam_right, true);

	injector::WriteMemory<int>(0x435FB9, trackcam_down, true);
	injector::WriteMemory<int>(0x4363F6, trackcam_down, true);
	injector::WriteMemory<int>(0x4365AC, trackcam_down, true);

	while (true)
	{
		if (GetRaceState() == (BYTE)CDRaceState::InProcess)
		{
			if (KeyPress(trackcam_keyactiv))
			{
				SetRaceCameraID(CDRaceCamera::FreeTrackCam);
			}
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (*(DWORD*)0x4000A8 == 0x29BB90)	// Checking the entry point of 1.2 executable
		{
			CreateThread(0, 0, MainTHREAD, 0, 0, 0);
		}
		else
			return FALSE;
	}

	return TRUE;
}
