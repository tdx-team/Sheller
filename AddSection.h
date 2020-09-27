#pragma once
#ifndef ADDSECTION_H_
#define ADDSECTION_H_
#include "stdafx.h"

/*
	�����ƣ�AddSection
	��;�����һ������
	ʱ�䣺2018/11/30
*/

class AddSection
{
public:
	AddSection();

	~AddSection();

public:
	void puModifySectioNumber(){ this->ModifySectionNumber(); }

	void puModifyProgramEntryPoint(){ this->ModifyProgramEntryPoint(); }

	void puModifySizeofImage(){ this->ModifySizeofImage(); }

	BOOL puModifySectionInfo(BYTE* Name, const DWORD & size){ return this->ModifySectionInfo(Name, size); }

	BOOL puAddNewSectionByData(const DWORD & size){ return this->AddNewSectionByteData(size); }

	void* puGetNewBaseAddress(){ return this->newlpBase; }

	DWORD puGetNewBaseSize(){ return this->FileSize + 0x1000; }


private:
	// 1. �޸���������
	BOOL ModifySectionNumber();
	// 2. �޸�������Ϣ(RVA\��С\����)
	BOOL ModifySectionInfo(const BYTE* Name, const DWORD & size);
	// 3. �޸ĳ�����ڵ�
	BOOL ModifyProgramEntryPoint();
	// 4. �޸�SizeImage
	BOOL ModifySizeofImage();
	// 5. �����µ��ֽ�(һ������ڵ���)
	BOOL AddNewSectionByteData(const DWORD & size);

private:
	// ����FileImage
	void* pFileBaseData = nullptr;
	// ����Ntͷ
	void* pNtHeadre = nullptr;
	// ��������ͷ
	void* pSectionHeadre = nullptr;
	// ��������һ�����ٸ��ֽ� (���θ���*ÿ�����δ�С--�����������ݶ������νṹ�壨����������Ϣ�Ĵ�С��)
	DWORD SectionSizeof = 0;
	// �����ļ���С
	DWORD FileSize = 0;
	// ���ڴ�ռ�
	static char* newlpBase;
	// �����ļ����
	HANDLE FileHandle = nullptr;
	// ������
	PIMAGE_SECTION_HEADER NewpSection = { 0 };
	// ����ԭʼOEP
	DWORD64 OldOep = 0;
};
#endif