﻿#pragma once
#ifndef COMPRESSIONDATA_H_
#define COMPRESSIONDATA_H_
#include "stdafx.h"

/*
	类名称：CompressionData
	用途：压缩PE数据
	时间：2018/12/1

	new decode
	time：2020/9/1
*/

class CompressionData
{
public:
	CompressionData();
	~CompressionData();

public:
	// void puAddCompresData(){ this->AddCompreDataSection(); }
	BOOL puCompressSection() { return this->CompressSectionData(); }
	// void puCleanDirectData(char* NewAddress){ this->CleanDirectData(NewAddress); }
	HANDLE puGetStubBase() { return this->m_studBase; }

private:
	// 压缩PE区段数据
	BOOL CompressSectionData();
	// 加密PE区段数据
	BOOL EncryptionSectionData(char* src, int srclen, char enkey);
	// 清空数据目录表
	BOOL CleanDirectData(const char* NewAddress, const DWORD & CompresSize, const DWORD & Size);
	// 判断真正的区段数据大小（未对齐）
	DWORD IsSectionSize(DWORD MiscVirtualsize, DWORD sizeOfRawData);
	// 添加一个区段给压缩后的数据使用
	void AddCompreDataSection(const DWORD & size);

private:
	// 获取基址
	static void* m_lpBase;
	// 保存区段头
	void* m_SectionHeadre = nullptr;
	// 保存文件大小
	DWORD m_hFileSize = 0;
	// 保存区段个数
	DWORD m_SectionCount = 0;
	// 获取文件句柄
	void* m_hFile = nullptr;
	// 保存加载stub的地址
	static HANDLE m_studBase;
	// 保存mask壳区段地址
	PIMAGE_SECTION_HEADER m_maskAddress;
	// 保存文件名称更新
	CString FileName;
	// 文件句柄
	FILE* fpFile = nullptr;
};

#endif