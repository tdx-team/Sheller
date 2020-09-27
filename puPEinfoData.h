#pragma once
#include "stdafx.h"
#ifndef PUPEINFODATA_H_
#define PUPEINFODATA_H_

/*
	�����ƣ�PuPeInfo
	����;��	�����ӿ��࣬�ṩ��ִ���ļ���PE��Ϣ
	ʱ�䣺	2018-11-29
*/
class PuPEInfo
{
public:
	PuPEInfo();

	~PuPEInfo();
	
	/*�����ӿ�*/
public:
	void* puGetImageBase(){ return m_pFileBase; }

	void* puGetNtHeadre(){ return m_pNtHeader; }

	void* puGetSection(){ return m_SectionHeader; }

	DWORD puFileSize(){ return m_FileSize; }

	BOOL puOpenFileLoad(const CString & PathName){ return prOpenFile(PathName); }

	BOOL puIsPEFile(){ return IsPEFile(); }

	DWORD puRVAofFOA(const DWORD Rva){ return RVAofFOA(Rva); }

	CString puFilePath(){ return m_strNamePath; }

	HANDLE puFileHandle() { return m_hFileHandle; }

	DWORD64 puOldOep(){ return this->m_OldOEP; }

	int puGetSectionCount() { return this->m_SectionCount; }

	PIMAGE_SECTION_HEADER puGetSectionAddress(const char* Base, const BYTE* Name){ return this->GetSectionAddress(Base, Name); }

	// �����ļ�ƫ���Լ��ļ���С
	BOOL puSetFileoffsetAndFileSize(const void* Base, const DWORD & offset, const DWORD size, const BYTE* Name)
	{
		return this->SetFileoffsetAndFileSize(Base, offset, size, Name);
	}


	/*˽�з���������*/
private:
	// �ļ���ȡ
	BOOL prOpenFile(const CString & PathName);

	// PE�ļ��ж�
	BOOL IsPEFile();

	// RVAofFOA
	DWORD RVAofFOA(const DWORD Rva);

	// �����������ƻ�ȡ�����׵�ַ
	PIMAGE_SECTION_HEADER GetSectionAddress(const char* Base, const BYTE* SectionName);

	// �����ļ�ƫ���Լ��ļ���С
	BOOL SetFileoffsetAndFileSize(const void* Base, const DWORD & offset, const DWORD size, const BYTE* Name);

	// ����ImageBase
	static void* m_pFileBase;

	// ����PEͷ
	static void* m_pNtHeader;

	// ����Section
	static void* m_SectionHeader;

	// �����ļ���С
	static DWORD m_FileSize;

	// �����ļ�·��
	static CString m_strNamePath;

	// �����ļ����
	static HANDLE m_hFileHandle;

	// ����ԭʼOEP
	static DWORD m_OldOEP;

	// �������θ���
	static int	m_SectionCount;

	// ���ԭʼOEP
	static BOOL OepFlag;

};

#endif
