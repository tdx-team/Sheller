#pragma once
#ifndef HASHOFFUNCTION_H_
#define HASHOFFUNCTION_H_
#include <windows.h>

/*
	�����ƣ�HashofFunction
	��;����д����GetProcAddress �Լ� HASH��ȡ��ַ
	ʱ�䣺2018/12/2
*/

class HashofFunction
{
public:
	HashofFunction();
	~HashofFunction();

	// �ӿ�
public:
	DWORD puGetModule(const DWORD Hash){ return this->GetModule(Hash); }
	DWORD puGetProcAddress(const DWORD dllvalues, const DWORD Hash){ return this->GetProcAddress(dllvalues, Hash); }

private:
	/*
			0x228C4218			KERNEL32.DLL
			0xec1c6278;			kernel32.dll
			0xc0d832c7;			LoadlibraryExa
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
	*/
	// ��ȡģ���ַ������kernel32.dll
	DWORD GetModule(const DWORD Hash);
	// ��ȡ����VA��ַ����дGetProcAddress��
	DWORD GetProcAddress(const DWORD dllvalues, const DWORD Hash);
};

#endif