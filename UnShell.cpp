#include "UnShell.h"
#include "./Stud/Stud.h"
#include <malloc.h>
#include "lz4/include/lz4.h"
#include "puPEinfoData.h"

extern _Stud* g_stu;

#define NEWSECITONNAME ".UPX"

UnShell::UnShell()
{
	PuPEInfo obj_puPe;

	HANDLE tempHandle = obj_puPe.puFileHandle();

	CloseHandle(tempHandle);

	hFile = CreateFile(L"C:\\Users\\Administrator\\Desktop\\CompressionMask.exe", GENERIC_READ | GENERIC_WRITE, FALSE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwSize = GetFileSize(hFile, NULL);

	m_Base = (void *)malloc(dwSize);

	memset(m_Base, 0, dwSize);

	DWORD dwRead = 0;

	OVERLAPPED OverLapped = { 0 };

	int nRetCode = ReadFile(hFile, m_Base, dwSize, &dwRead, &OverLapped);

	pDosHander = (PIMAGE_DOS_HEADER)m_Base;

	pHeadres = (PIMAGE_NT_HEADERS)(pDosHander->e_lfanew + (LONG)m_Base);

	pSection = IMAGE_FIRST_SECTION(pHeadres);

	m_NtAddress = (void*)pHeadres;

	if ((fpFile = fopen("FileData.txt", "r+")) == NULL)
	{
		AfxMessageBox(L"�ļ���ʧ��");
	}
}

UnShell::~UnShell()
{
	fclose(fpFile);
}

// �ָ�ѹ��������
BOOL UnShell::RepCompressionData()
{
	m_studBase = LoadLibraryEx(L"E:\\Sheller-master\\x64\\Release\\Stud.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);

	g_stu = (_Stud*)GetProcAddress((HMODULE)m_studBase, "g_stud");

	DWORD SectionCount = pHeadres->FileHeader.NumberOfSections;

	/*===================�ļ��ж�ȡ��Ҫ������================================*/
	for (DWORD i = 0; i < SectionCount - 3; ++i)
	{
		fread(&g_stu->s_blen[i], sizeof(DWORD), 1, fpFile);
	}


	for (DWORD i = 0; i < 16; ++i)
	{
		fread(&g_stu->s_DataDirectory[i][0], sizeof(DWORD), 1, fpFile);
		fread(&g_stu->s_DataDirectory[i][1], sizeof(DWORD), 1, fpFile);
		// fscanf(fpFile, "%04x %04x", &g_stu->s_DataDirectory[i][0], &g_stu->s_DataDirectory[i][1]);
	}

	for (DWORD i = 0; i < SectionCount - 2; ++i)
	{
		fread(&g_stu->s_SectionOffsetAndSize[i][0], sizeof(DWORD), 1, fpFile);
		fread(&g_stu->s_SectionOffsetAndSize[i][1], sizeof(DWORD), 1, fpFile);
		// fscanf(fpFile, "%04x %04x", &g_stu->s_SectionOffsetAndSize[i][0], &g_stu->s_SectionOffsetAndSize[i][1]);
	}

	fread(&g_stu->s_dwOepBase, sizeof(DWORD), 1, fpFile);

	/*========================================================================*/

	TotaldwSize = 0;

	for (DWORD i = 0; i < SectionCount - 2; ++i)
	{
		// ��ȡ�ļ�ƫ�Ƽ��ļ���С0��ƫ�� 1�Ǵ�С
		TotaldwSize += g_stu->s_SectionOffsetAndSize[i][0];
	}

	// ����
	Sectionbuf = (char*)malloc(TotaldwSize);

	// �ļ��м���������ʵλ��	
	DWORD DataStart = 0x400;
	// �����α�
	DWORD Flag = 0;

	PuPEInfo obj_pePE;

	BYTE Name[] = ".com";

	// ����ѹ����������ʼλ��
	PIMAGE_SECTION_HEADER address = (PIMAGE_SECTION_HEADER)obj_pePE.puGetSectionAddress((char*)m_Base, Name);

	int nFlag = 0;

	DWORD Address = address->PointerToRawData;

	for (DWORD i = 0; i < SectionCount - 2; ++i)
	{
		if (g_stu->s_blen[nFlag] == 0)
		{
			nFlag += 1;
			continue;
		}
		// ������  RVA+���ػ�ַ  ��������С  ѹ����ȥ�Ĵ�С
		int nRet = LZ4_decompress_safe((char*)(Address + (DWORD)m_Base), &Sectionbuf[Flag], g_stu->s_blen[nFlag], g_stu->s_SectionOffsetAndSize[i][0]);
		Address += g_stu->s_blen[i];
		Flag += g_stu->s_SectionOffsetAndSize[i][0];
		nFlag++;
	}

	return TRUE;
}

// ɾ����������
BOOL UnShell::DeleteSectionInfo()
{
	DWORD dwSectionCount = pHeadres->FileHeader.NumberOfSections;

	PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)pHeadres->OptionalHeader.DataDirectory;

	// �޸�����Ŀ¼��
	for (DWORD i = 0; i < 16; ++i)
	{
		// �ȶ�ȡ����
		if (0 != g_stu->s_DataDirectory[i][0])
			pDataDirectory->VirtualAddress = g_stu->s_DataDirectory[i][0];
		if (0 != g_stu->s_DataDirectory[i][1])
			pDataDirectory->Size = g_stu->s_DataDirectory[i][1];
		++pDataDirectory;
	}

	// �޸��������ε�����
	for (DWORD i = 0; i < dwSectionCount - 2; ++i)
	{
		if (0 != g_stu->s_SectionOffsetAndSize[i][0])
			pSection->SizeOfRawData = g_stu->s_SectionOffsetAndSize[i][0];
		if (0 != g_stu->s_SectionOffsetAndSize[i][1])
			pSection->PointerToRawData = g_stu->s_SectionOffsetAndSize[i][1];
		++pSection;
	}

	// ���θ��� - 2;
	pHeadres->FileHeader.NumberOfSections -= 2;

	// ����������ε�PE�ṹ�����ݣ�
	PIMAGE_SECTION_HEADER pSection_s = IMAGE_FIRST_SECTION(pHeadres);

	DWORD NewdwSectionOfSize = (dwSectionCount - 2) * 0x28;

	char* temp = (char*)malloc(80);

	memset(temp, 0, 80);

	// ��ռ��ص��������� �޸Ĳ�����������
	DWORD old = 0;
	BYTE Name[] = ".UPX";
	BYTE Name1[] = ".com";
	PuPEInfo pePu;

	DWORD masAdd = (DWORD)pePu.puGetSectionAddress((char*)m_Base, Name);
	VirtualProtect((char*)masAdd, 40, PAGE_READWRITE, &old);
	memcpy((char*)masAdd, temp, 40);
	VirtualProtect((char*)masAdd, 40, old, &old);

	DWORD comAdd = (DWORD)pePu.puGetSectionAddress((char*)m_Base, Name1);
	VirtualProtect((char*)comAdd, 40, PAGE_READWRITE, &old);
	memcpy((char*)comAdd, temp, 40);
	VirtualProtect((char*)comAdd, 40, old, &old);

	free(temp);

	temp = nullptr;

	--pSection;

	// �޸�SizeofImage = ���һ������.VirtualAddress + ���һ������.SizeOfRawData���ڴ�������ȶ���Ĵ�С
	pHeadres->OptionalHeader.SizeOfImage = pSection->VirtualAddress + pSection->SizeOfRawData;

	// �ָ�OEP
	pHeadres->OptionalHeader.AddressOfEntryPoint = g_stu->s_dwOepBase;
	
	return TRUE;
}

// ����\��β����
BOOL UnShell::SaveUnShell()
{
	DWORD Size = 0x400 + TotaldwSize;
	// PEͷ + ��ѹ�������
	UnShellNewFile = (char*)malloc(Size);

	memcpy(UnShellNewFile, m_Base, 0x400);

	memcpy(&UnShellNewFile[0x400], Sectionbuf, TotaldwSize);

	// д��exe�������ѹ��
	DWORD dwWrite = 0; 
	OVERLAPPED OverLapped = { 0, };

	// �����ļ�
	HANDLE Handle = CreateFile(L"C:\\Users\\Administrator\\Desktop\\UnShellNewPro.exe", GENERIC_READ | GENERIC_WRITE, FALSE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	// д���ļ�
	int nRet = WriteFile(Handle, UnShellNewFile, Size, &dwWrite, NULL);

	// �رվ��
	CloseHandle(Handle);

	if (!nRet)
		return FALSE;
	return TRUE;
}