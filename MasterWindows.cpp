// MasterWindows.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "MasterWindows.h"
#include "afxdialogex.h"
#include "puPEinfoData.h"
#include "SectionInfo.h"
#include "AddSection.h"
#include "studData.h"
#include "CompressionData.h"
#include "UnShell.h"

// MasterWindows
IMPLEMENT_DYNAMIC(MasterWindows, CDialogEx)
#define NEWSECITONNAME ".UPX"

MasterWindows::MasterWindows(CWnd* pParent /*=NULL*/)
	: CDialogEx(MasterWindows::IDD, pParent)
	, m_MasterStaticTextStr(_T(""))
{

}

MasterWindows::~MasterWindows()
{
}

void MasterWindows::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_MasterStaticText);
	DDX_Text(pDX, IDC_STATIC1, m_MasterStaticTextStr);
}

BEGIN_MESSAGE_MAP(MasterWindows, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MasterWindows::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON4, &MasterWindows::OnBnClickedButton4)
	// ON_BN_CLICKED(IDC_BUTTON3, &MasterWindows::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON9, &MasterWindows::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON3, &MasterWindows::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &MasterWindows::OnBnClickedButton2)
END_MESSAGE_MAP()

// MasterWindows ÏûÏ¢´¦Àí³ÌÐò

BOOL MasterWindows::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	/*ÖØÐÂÉèÖÃÍ¼±ê*/
	SetIcon(LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON1)), TRUE);

	return TRUE; 
}

// Ò»¼üÅÌ¼Ó¿Ç
void MasterWindows::OnBnClickedButton1()
{
	// ☆ 先增加新区~段后压缩
	PuPEInfo obj_Peinfo;

	// 1. 新增区段
	if (NewSection())
		AfxMessageBox(L"添加新区段成功");
	else
		AfxMessageBox(L"添加新区段失败");

	CloseHandle(obj_Peinfo.puFileHandle()); UpdateData(TRUE);

	obj_Peinfo.puOpenFileLoad(m_MasterStaticTextStr);


	// 2. 压缩全部区段 压缩的时候不清空数据目录表以及区段大小（不压缩新增区段）
	CompressionData obj_ComperData;

	CloseHandle(obj_Peinfo.puFileHandle()); UpdateData(TRUE);

	obj_Peinfo.puOpenFileLoad(m_MasterStaticTextStr);

	if (!obj_ComperData.puCompressSection())
		AfxMessageBox(L"CompressSection failuer!");
	else
		AfxMessageBox(L"CompressSection Seucess!");

	CloseHandle(obj_Peinfo.puFileHandle());

	m_MasterStaticTextStr = "C:\\Users\\Administrator\\Desktop\\CompressionMask.exe";

	obj_Peinfo.puOpenFileLoad(m_MasterStaticTextStr);

	// 3. Stud数据操作...
	studData obj_stuData;

	obj_stuData.puLoadLibraryStud();

	obj_stuData.puRepairReloCationStud();

	if (obj_stuData.puCopyStud())
	{
		CloseHandle(obj_Peinfo.puFileHandle());
		UpdateData(TRUE);
		obj_Peinfo.puOpenFileLoad(m_MasterStaticTextStr);
	}
	else
		AfxMessageBox(L"StudWrite failure!");

	// 4、收尾工作
	CloseHandle(obj_Peinfo.puFileHandle());
}


void MasterWindows::OnDropFiles(HDROP hDropInfo)
{
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0); 
	char wcStr[MAX_PATH] = {};
	for (int i = 0; i < DropCount; ++i)
	{
		wcStr[0] = 0;
		DragQueryFileA(hDropInfo, i, wcStr, MAX_PATH);
		m_MasterStaticTextStr = wcStr;
	}
	UpdateData(FALSE);
	ShowPEInfoData(m_MasterStaticTextStr);
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}

// 
void MasterWindows::ShowPEInfoData(const CString & FileName)
{
	PuPEInfo obj_puPe; CString Tempstr;	DWORD TempdwCode = 0;

	if (!obj_puPe.puOpenFileLoad(FileName))
		return;

	PIMAGE_NT_HEADERS pNtHeadre = (PIMAGE_NT_HEADERS)obj_puPe.puGetNtHeadre();

	PIMAGE_FILE_HEADER pFileHeadre = (PIMAGE_FILE_HEADER)&pNtHeadre->FileHeader;

	PIMAGE_OPTIONAL_HEADER pOption = (PIMAGE_OPTIONAL_HEADER)&pNtHeadre->OptionalHeader;

	// Çø¶ÎÊýÁ¿
	TempdwCode = pFileHeadre->NumberOfSections;
	Tempstr.Format(L"%d", TempdwCode);
	SetDlgItemText(IDC_EDIT9, Tempstr);
	
	// OEP
	TempdwCode = pOption->AddressOfEntryPoint;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT1, Tempstr);

	// Ä¬ÈÏ¼ÓÔØ»ùÖ·
	TempdwCode = pOption->ImageBase;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT3, Tempstr);
	
	// ±êÖ¾×Ö
	TempdwCode = pOption->Magic;
	Tempstr.Format(L"%04X", TempdwCode);
	SetDlgItemText(IDC_EDIT2, Tempstr);

	// Êý¾ÝÄ¿Â¼¸öÊý
	TempdwCode = pOption->NumberOfRvaAndSizes;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT7, Tempstr);

	// ÆðÊ¼´úÂëÏà¶ÔÐéÄâµØÖ·
	TempdwCode = pOption->BaseOfCode;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT4, Tempstr);

#ifdef _WIN64

#else
	TempdwCode = pOption->BaseOfData;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT5, Tempstr);
#endif

	// ¿é¶ÔÆëÁ¦¶È
	TempdwCode = pOption->SectionAlignment;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT6, Tempstr);

	// ÎÄ¼þ¶ÔÆëÁ¦¶È
	TempdwCode = pOption->FileAlignment;
	Tempstr.Format(L"%08X", TempdwCode);
	SetDlgItemText(IDC_EDIT8, Tempstr);

}

// ¼ÓÔØÇø¶ÎÐÅÏ¢
void MasterWindows::OnBnClickedButton4()
{
	SectionInfo obj_section;
	obj_section.DoModal();
	return;
}

// ÐÂÇø¶ÎÌí¼Ó£¨¿Ç£©
void MasterWindows::OnBnClickedButton9()
{
	if (NewSection())
		AfxMessageBox(L"Ìí¼ÓÐÂÇø¶Î³É¹¦");
	else
		AfxMessageBox(L"Ìí¼ÓÐÂÇø¶ÎÊ§°Ü");
}

// Çø¶ÎÌí¼ÓÄÚ²¿µ÷ÓÃº¯Êý
BOOL MasterWindows::NewSection()
{
	// ¿ÇÇø¶Î
	AddSection obj_addsection; BOOL nRet = TRUE;

	BYTE Name[] = ".UPX";
	
	// 遗留问题，SectionSize是Stud-DLL大小
	const DWORD SectionSize = 0x17000;
	
	obj_addsection.puModifySectioNumber();

	nRet = obj_addsection.puModifySectionInfo(Name, SectionSize);

	obj_addsection.puModifyProgramEntryPoint();

	obj_addsection.puModifySizeofImage();

	nRet = obj_addsection.puAddNewSectionByData(SectionSize);

	return nRet;
}

void MasterWindows::OnBnClickedButton3()
{
	CompressionData obj_ComperData;
	
	if (!obj_ComperData.puCompressSection())
		AfxMessageBox(L"CompressSection failuer!");
	else
		AfxMessageBox(L"CompressSection Seucess!");
}

void MasterWindows::OnBnClickedButton2()
{
	UnShell obj_Unshell;

	obj_Unshell.puRepCompressionData();

	obj_Unshell.puDeleteSectionInfo();

	if (obj_Unshell.puSaveUnShell())
		AfxMessageBox(L"Ò»¼üÍÑ¿Ç³É¹¦");

}
