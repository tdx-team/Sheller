#pragma once
#ifndef STUD_H_
#define STUD_H_
#include <Windows.h>

#ifdef _WIN64
typedef struct _Stud
{
	DWORD64 s_dwOepBase;
	DWORD64 s_Krenel32;
	DWORD64 s_User32;
	DWORD64 s_Gdi32;
	DWORD64 s_MSVCRT;
	HMODULE s_ModuleUser32;
	DWORD64 s_DirectoryCount;
	DWORD64 s_SectionCount;
	DWORD64 s_DataDirectory[16][2];
	DWORD s_SectionOffsetAndSize[20][2];
	DWORD s_blen[20];
	BOOL s_OneSectionSizeofData;
	DWORD64 s_CompressionSectionRva;
	DWORD64 s_SaveExportTabRVA;
}Stud;
#else
//  /NODEFAULTLIB:LIBCMT.lib 
typedef struct _Stud
{
	DWORD s_dwOepBase;
	DWORD s_Krenel32;
	DWORD s_User32;
	DWORD s_Gdi32;
	DWORD s_MSVCRT;
	DWORD s_DirectoryCount;
	DWORD s_SectionCount;
	DWORD s_DataDirectory[16][2];
	DWORD s_SectionOffsetAndSize[20][2];
	DWORD s_blen[20];
	BOOL s_OneSectionSizeofData;
	DWORD s_CompressionSectionRva;
	DWORD s_SaveExportTabRVA;
}Stud;
#endif
#endif
/*
		Function Hash Values:
			0x228C4218			KERNEL32.DLL - 64bit
			0xEC1C6278;			kernel32.dll
			0xC0D83287;			LoadlibraryExa
			0x4FD18963;			ExitPorcess
			0x5644673D			User32.dll
			0x1E380A6A			MessageBoxA
			0x9EBC86B			RtlExitUserProcess
			0xF4E2F2C8			GetModuleHandleW
			0xBB7420F9			CreateSolidBrush
			0xBC05E48			RegisterClassW
			0x1FDAF571			CreateWindowExW
			0xDD8B5FB8			ShowWindow
			0x9BB5D8DC			UpdateWindow
			0x61060461			GetMessageW
			0xE09980A2			TranslateMessage
			0x7A1506D8			DispatchMessageW
			0x457BF55A			GetWindowTextW
			0x7EAD1F86			lstrcmpW
			0x1E380A6A			MessageBoxA
			0xCAA94781			PostQuitMessage
			0x22E85CBA			DefWindowProcW
			0xC6B20165			LoadCursorW
			0x7636E8F4			LoadIconW
			0x1FDAF55B			CreateWindowA
			0x68D82F59			RegisterClassExW
			0x5D0CB479			GetDlgItem
			0x818F6ED7			Mymemcpy
			0x328CEB95			msvcrt.dll
			0x1EDE5967			VirtualAlloc
			0x2729F8BB			CreateThread		
*/