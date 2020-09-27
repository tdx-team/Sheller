#include "stud.h"
#include "../lz4/include/lz4.h"
#include "../quick/quicklz.h"

#include <CommCtrl.h>
#pragma comment(linker, "/merge:.data=.text")
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")

extern "C" {
	__declspec(dllexport) Stud g_stud = { 0 };
}

static TCHAR szWindowClass[] = TEXT("PasswdWind");

HINSTANCE g_hInstance;

typedef void* (WINAPI*FnGetProcAddress)(HMODULE, const char*);
FnGetProcAddress MyGetProcAddress;

typedef HMODULE(WINAPI* FnLoadLibraryExA)(_In_ LPCSTR lpLibFileName, _Reserved_ HANDLE hFile, _In_ DWORD dwFlags);
FnLoadLibraryExA MyLoadLibraryExA;

typedef HMODULE(WINAPI* FnGetModuleHandleW)(_In_opt_ LPCWSTR lpModuleName);
FnGetModuleHandleW MyGetModuleHandleW;

typedef HBRUSH(WINAPI* FnCreateSolidBrush)(_In_ COLORREF color);
FnCreateSolidBrush MyCreateSolidBrush;
// 
typedef ATOM(WINAPI* FnRegisterClassW)(_In_ CONST WNDCLASSW *lpWndClass);
FnRegisterClassW MyRegisterClassW;

typedef WINUSERAPI ATOM (WINAPI* FnRegisterClassExW)(_In_ CONST WNDCLASSEXW *);
FnRegisterClassExW MyRegisterClassExW;

typedef VOID*(WINAPIV* Fnmalloc)(_In_ _CRT_GUARDOVERFLOW size_t _Size);
Fnmalloc Mymalloc;

typedef	WINUSERAPI HWND(WINAPI* FnCreateWindowExW)(
	_In_ DWORD dwExStyle,
	_In_opt_ LPCWSTR lpClassName,
	_In_opt_ LPCWSTR lpWindowName,
	_In_ DWORD dwStyle,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_opt_ HWND hWndParent,
	_In_opt_ HMENU hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID lpParam);
FnCreateWindowExW MyCreateWindowExW;

typedef BOOL(WINAPI* FnShowWindow)(_In_ HWND hWnd, _In_ int nCmdShow);
FnShowWindow MyShowWindow;

typedef BOOL(WINAPI* FnUpdateWindow)(_In_ HWND hWnd);
FnUpdateWindow MyUpdateWindow;

typedef BOOL(WINAPI* FnGetMessageW)(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax);
FnGetMessageW MyGetMessageW;

typedef BOOL(WINAPI* FnTranslateMessage)(_In_ CONST MSG *lpMsg);
FnTranslateMessage	MyTranslateMessage;

typedef LRESULT(WINAPI* FnDispatchMessageW)(_In_ CONST MSG *lpMsg);
FnDispatchMessageW MyDispatchMessageW;

typedef int (WINAPI* FnGetWindowTextW)(_In_ HWND hWnd, _Out_writes_(nMaxCount) LPWSTR lpString, _In_ int nMaxCount);
FnGetWindowTextW MyGetWindowTextW;

typedef int (WINAPI* FnlstrcmpW)(_In_ LPCWSTR lpString1, _In_ LPCWSTR lpString2);
FnlstrcmpW MylstrcmpW;

typedef int (WINAPI* FnMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);
FnMessageBoxA MyMessageBoxA;

typedef VOID(WINAPI* FnPostQuitMessage)(_In_ int nExitCode);
FnPostQuitMessage MyPostQuitMessage;

typedef LRESULT(WINAPI* FnDefWindowProcW)(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
FnDefWindowProcW MyDefWindowProcW;

typedef LRESULT(WINAPI* FnDefWindowProcA)(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
FnDefWindowProcW MyDefWindowProcA;

typedef HCURSOR(WINAPI* FnLoadCursorW)(_In_opt_ HINSTANCE hInstance, _In_ LPCWSTR lpCursorName);
FnLoadCursorW MyLoadCursorW;

typedef HICON(WINAPI* FnLoadIconW)(_In_opt_ HINSTANCE hInstance, _In_ LPCWSTR lpIconName);
FnLoadIconW MyLoadIconW;

typedef VOID(WINAPI* FnExitProcess)(_In_ UINT uExitCode);
FnExitProcess MyExitProcess;

typedef HWND(WINAPI* FnGetDlgItem)(_In_opt_ HWND hDlg, _In_ int nIDDlgItem);
FnGetDlgItem MyGetDlgItem;

typedef BOOL(WINAPI* FnVirtualProtect)(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flNewProtect, _Out_ PDWORD lpflOldProtect);
FnVirtualProtect MyVirtualProtect;

typedef HWND(WINAPI* FnFindWindowExW)(_In_opt_ HWND hWndParent, _In_opt_ HWND hWndChildAfter, _In_opt_ LPCWSTR lpszClass, _In_opt_ LPCWSTR lpszWindow);
FnFindWindowExW MyFindWindowExW;

typedef LRESULT(WINAPI* FnSendMessageW)(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_Pre_maybenull_ _Post_valid_ WPARAM wParam,
	_Pre_maybenull_ _Post_valid_ LPARAM lParam
	);
FnSendMessageW MySendMessageW;

typedef HANDLE(WINAPI* FnCreateThread)(
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ SIZE_T dwStackSize,
	_In_ LPTHREAD_START_ROUTINE lpStartAddress,
	_In_opt_ __drv_aliasesMem LPVOID lpParameter,
	_In_ DWORD dwCreationFlags,
	_Out_opt_ LPDWORD lpThreadId
	);
FnCreateThread MyCreateThread;

typedef VOID(WINAPI* FnSleep)(
	_In_ DWORD dwMilliseconds
	);
FnSleep MySleep;

typedef LPVOID
(WINAPI* FnVirtualAlloc)(
	_In_opt_ LPVOID lpAddress,
	_In_     SIZE_T dwSize,
	_In_     DWORD flAllocationType,
	_In_     DWORD flProtect
	);
FnVirtualAlloc MyVirtualAlloc;

typedef int (WINAPI* FnGetDlgCtrlID)(
	_In_ HWND hWnd
	);
FnGetDlgCtrlID MyGetDlgCtrlID;

typedef HWND(WINAPI* FnFindWindowW)(
	_In_opt_ LPCWSTR lpClassName,
	_In_opt_ LPCWSTR lpWindowName
	);
FnFindWindowW MyFindWindowW;

typedef BOOL(WINAPI* FnPostMessage) (
	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam
	);
FnPostMessage MyPostMessageW;

typedef WINBASEAPI
_Check_return_
_Post_equals_last_error_
DWORD
(WINAPI* FnGetLastError)(
	VOID
	);
FnGetLastError MyGetLastError;

extern "C" {
	void __stdcall puGetModule(const DWORD Hash, DWORD64* address);
	PVOID64 __stdcall puGetProcAddress(const DWORD64 modules, DWORD Hash);
	void __stdcall CodeExecEntry(DWORD64 oep);
}

void SetString(HWND hWnd)
{
	MyPostQuitMessage = (FnPostQuitMessage)puGetProcAddress(g_stud.s_User32, 0xCAA94781);
	MyDefWindowProcW = (FnDefWindowProcW)puGetProcAddress(g_stud.s_User32, 0x22E85CBA);
	MyCreateWindowExW(0L, WC_BUTTON, TEXT("Shell"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 98, 300, 200, hWnd, NULL, 0, NULL);
	MyCreateWindowExW(0L, WC_STATIC, TEXT("Account:"), WS_CHILD | WS_VISIBLE, 30, 145, 80, 20, hWnd, NULL, 0, NULL);
	MyCreateWindowExW(0L, WC_STATIC, TEXT("Passwd :"), WS_CHILD | WS_VISIBLE, 30, 175, 80, 20, hWnd, NULL, 0, NULL);
	MyCreateWindowExW(WS_EX_CLIENTEDGE, WC_EDIT, TEXT(""), WS_CHILD | WS_VISIBLE, 120, 145, 160, 20, hWnd, (HMENU)0x1001, 0, NULL);
	MyCreateWindowExW(WS_EX_CLIENTEDGE, WC_EDIT, TEXT(""), WS_CHILD | WS_VISIBLE, 120, 175, 160, 20, hWnd, (HMENU)0x1002, 0, NULL);
	MyCreateWindowExW(0L, WC_BUTTON, TEXT("login:"), WS_CHILD | WS_VISIBLE, 120, 220, 70, 25, hWnd, (HMENU)0x1003, 0, NULL);
}

void UnCompression()
{
	MyVirtualAlloc = (FnVirtualAlloc)puGetProcAddress(g_stud.s_Krenel32, 0x1EDE5967);
	DWORD64 m_lpbase = 0x140000000;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(((PIMAGE_DOS_HEADER)m_lpbase)->e_lfanew + (DWORD64)m_lpbase);
	PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)pNt->OptionalHeader.DataDirectory;
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	DWORD Att_old = 0;

	for (DWORD i = 0; i < 16; ++i)
	{
		MyVirtualProtect(pDataDirectory, 0x8, PAGE_READWRITE, &Att_old);
		if (0 != g_stud.s_DataDirectory[i][0])
			pDataDirectory->VirtualAddress = g_stud.s_DataDirectory[i][0];
		if (0 != g_stud.s_DataDirectory[i][1])
			pDataDirectory->Size = g_stud.s_DataDirectory[i][1];
		MyVirtualProtect(pDataDirectory, 0x8, Att_old, &Att_old);
		++pDataDirectory;
	}

	for (DWORD i = 0; i < g_stud.s_SectionCount - 2; ++i)
	{
		MyVirtualProtect(pSection, 0x8, PAGE_READWRITE, &Att_old);
		if (0 != g_stud.s_SectionOffsetAndSize[i][0])
			pSection->SizeOfRawData = g_stud.s_SectionOffsetAndSize[i][0];
		if (0 != g_stud.s_SectionOffsetAndSize[i][1])
			pSection->PointerToRawData = g_stud.s_SectionOffsetAndSize[i][1];
		MyVirtualProtect(pSection, 0x8, Att_old, &Att_old);
		++pSection;
	}

	PIMAGE_SECTION_HEADER pSections = IMAGE_FIRST_SECTION(pNt);

	DWORD Att_olds = 0;
	DWORD64 SectionAddress = g_stud.s_CompressionSectionRva;
	qlz_state_decompress *state_decompress = (qlz_state_decompress *)MyVirtualAlloc(NULL, sizeof(qlz_state_decompress), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	for (DWORD i = 0; i < g_stud.s_SectionCount - 2; ++i)
	{
		byte* Address = (byte *)(pSections->VirtualAddress + m_lpbase);

		MyVirtualProtect(Address, g_stud.s_SectionOffsetAndSize[i][0], PAGE_EXECUTE_READWRITE, &Att_old);
		MyVirtualProtect((void*)SectionAddress, g_stud.s_blen[i], PAGE_EXECUTE_READWRITE, &Att_olds);

		int nRet = qlz_decompress((char*)(SectionAddress + m_lpbase), (char*)(pSections->VirtualAddress + m_lpbase), state_decompress);
		MyVirtualProtect(Address, g_stud.s_SectionOffsetAndSize[i][0], Att_old, &Att_old);
		MyVirtualProtect((void*)SectionAddress, g_stud.s_blen[i], Att_olds, &Att_olds);

		++pSections;
		SectionAddress += g_stud.s_blen[i];
	}
}

void RepairTheIAT()
{
	DWORD64 dwMoudle = (DWORD64)MyGetModuleHandleW(NULL);
	DWORD64 ImportTabVA = g_stud.s_DataDirectory[1][0] + dwMoudle;
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)ImportTabVA;

	DWORD Att_old = 0;
	DWORD64 FunAddress = 0;

	while (pImport->Name)
	{
		char* Name = (char*)(pImport->Name + dwMoudle);
		HMODULE hModuledll = MyLoadLibraryExA(Name, NULL, NULL);
		PIMAGE_THUNK_DATA pThunkINT = (PIMAGE_THUNK_DATA)(pImport->OriginalFirstThunk + dwMoudle);
		PIMAGE_THUNK_DATA pThunkIAT = (PIMAGE_THUNK_DATA)(pImport->FirstThunk + dwMoudle);
		while (pThunkINT->u1.AddressOfData)
		{
			MyVirtualProtect((PVOID64)pThunkIAT, 0x16, PAGE_READWRITE, &Att_old);
			if (!IMAGE_SNAP_BY_ORDINAL(pThunkIAT->u1.Ordinal))
			{
				PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(pThunkINT->u1.AddressOfData + dwMoudle);
				FunAddress = (DWORD64)MyGetProcAddress(hModuledll, pName->Name);
			}
			else
			{
				DWORD64 dwFunOrdinal = IMAGE_ORDINAL((pThunkIAT->u1.Ordinal));
				FunAddress = (DWORD64)MyGetProcAddress(hModuledll, (char*)dwFunOrdinal);
			}

			pThunkIAT->u1.Function = (ULONGLONG)FunAddress;
			MyVirtualProtect((PVOID64)pThunkIAT, 0x16, Att_old, &Att_old);
			++pThunkINT;
			++pThunkIAT;
		}
		++pImport;
	}
}

extern "C" __declspec(dllexport) void main()
{
	puGetModule(0x228C4218, &g_stud.s_Krenel32);
	// GetLoadlibraryExA
	MyLoadLibraryExA = (FnLoadLibraryExA)puGetProcAddress(g_stud.s_Krenel32, 0xC0D83287);
	// Load User32.dll
	//g_stud.s_User32 = (DWORD64)MyLoadLibraryExA("user32.dll", NULL, NULL);
	// Load GDI32.lib
	g_stud.s_Gdi32 = (DWORD64)MyLoadLibraryExA("gdi32.dll", NULL, NULL);
	// Load 
	g_stud.s_MSVCRT = (DWORD64)MyLoadLibraryExA("msvcrt.dll", NULL, NULL);
	// GetExitProcW
	MyExitProcess = (FnExitProcess)puGetProcAddress(g_stud.s_Krenel32, 0x4FD18963);
	// GetGetModuleW
	MyGetModuleHandleW = (FnGetModuleHandleW)puGetProcAddress(g_stud.s_Krenel32, 0xF4E2F2C8);
	g_hInstance = (HINSTANCE)MyGetModuleHandleW(NULL);

	g_stud.s_User32 = (DWORD64)MyGetModuleHandleW(L"user32.dll");
	// GetCreateSolidBrush
	MyCreateSolidBrush = (FnCreateSolidBrush)puGetProcAddress(g_stud.s_Gdi32, 0xBB7420F9);
	// GetUpdateData
	MyUpdateWindow = (FnUpdateWindow)puGetProcAddress(g_stud.s_User32, 0x9BB5D8DC);
	// GetGetMessageW
	MyGetMessageW = (FnGetMessageW)puGetProcAddress(g_stud.s_User32, 0x61060461);
	// GetTranslateMessage
	MyTranslateMessage = (FnTranslateMessage)puGetProcAddress(g_stud.s_User32, 0xE09980A2);
	// GetDispatchMessageW
	MyDispatchMessageW = (FnDispatchMessageW)puGetProcAddress(g_stud.s_User32, 0x7A1506D8);
	// GetShowWindow
	MyShowWindow = (FnShowWindow)puGetProcAddress(g_stud.s_User32, 0xDD8B5FB8);
	// GetLoadCursorW
	MyLoadCursorW = (FnLoadCursorW)puGetProcAddress(g_stud.s_User32, 0xC6B20165);
	// GetLoadIconW
	MyLoadIconW = (FnLoadIconW)puGetProcAddress(g_stud.s_User32, 0x7636E8F4);
	// GetRegisterClassExW
	MyRegisterClassW = (FnRegisterClassW)puGetProcAddress(g_stud.s_User32, 0xBC05E48);
	MyRegisterClassExW = (FnRegisterClassExW)puGetProcAddress(g_stud.s_User32, 0x68D82F59);
	MyMessageBoxA = (FnMessageBoxA)puGetProcAddress(g_stud.s_User32, 0x1E380A6A);
	// GetCreateWindowExW
	MyCreateWindowExW = (FnCreateWindowExW)puGetProcAddress(g_stud.s_User32, 0x1FDAF571);
	// GetGetWindowTextW
	MyGetWindowTextW = (FnGetWindowTextW)puGetProcAddress(g_stud.s_User32, 0x457BF55A);
	// GetlstrcmpW
	MylstrcmpW = (FnlstrcmpW)puGetProcAddress(g_stud.s_Krenel32, 0x7EAD1F86);
	// GetDefWindowProcW
	MyDefWindowProcW = (FnDefWindowProcW)puGetProcAddress(g_stud.s_User32, 0x22E85CBA);
	// GetGetDlgItem
	MyGetDlgItem = (FnGetDlgItem)puGetProcAddress(g_stud.s_User32, 0x5D0CB479);
	// Mymemcpy = (Fnmemcpy)puGetProcAddress(g_stud.s_MSVCRT, 0x818F6ED7);
	MyVirtualProtect = (FnVirtualProtect)puGetProcAddress(g_stud.s_Krenel32, 0xEF64A41E);
	// GetMyGetProcessAddress
	MyGetProcAddress = (FnGetProcAddress)puGetProcAddress(g_stud.s_Krenel32, 0xBBAFDF85);
	UnCompression();
	RepairTheIAT();
	CodeExecEntry(g_stud.s_dwOepBase);
	return;
}