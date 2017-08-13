// DirDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoadPEGo.h"
#include "DirDlg.h"
#include "afxdialogex.h"


// CDirDlg 对话框

IMPLEMENT_DYNAMIC(CDirDlg, CDialogEx)

CDirDlg::CDirDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DIR, pParent)
	, m_Export_RVA(_T(""))
	, m_Export_Size(_T(""))
	, m_Import_RVA(_T(""))
	, m_Import_Size(_T(""))
	, m_Res_RVA(_T(""))
	, m_Res_Size(_T(""))
	, m_Exa_RVA(_T(""))
	, m_Exa_Size(_T(""))
	, m_Sec_RVA(_T(""))
	, m_Sec_Size(_T(""))
	, m_Loc_RVA(_T(""))
	, m_Loc_Size(_T(""))
	, m_Dbg_RVA(_T(""))
	, m_Dbg_Size(_T(""))
	, m_Rev_RVA(_T(""))
	, m_Rev_Size(_T(""))
	, m_Global_RVA(_T(""))
	, m_Global_Size(_T(""))
	, m_TLS_RVA(_T(""))
	, m_TLS_Size(_T(""))
	, m_Load_RVA(_T(""))
	, m_Load_Size(_T(""))
	, m_Bound_RVA(_T(""))
	, m_Bound_Size(_T(""))
	, m_IAT_RVA(_T(""))
	, m_IAT_Size(_T(""))
	, m_Delay_RVA(_T(""))
	, m_Delay_Size(_T(""))
	, m_COM_RVA(_T(""))
	, m_COM_Size(_T(""))
	, m_Other_RVA(_T(""))
	, m_Other_Size(_T(""))
{

}

CDirDlg::~CDirDlg()
{
}

void CDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXPORT_RVA, m_Export_RVA);
	DDX_Text(pDX, IDC_EDIT_EXPORT_Size, m_Export_Size);
	DDX_Text(pDX, IDC_EDIT_IMPORT_RVA, m_Import_RVA);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SIZE, m_Import_Size);
	DDX_Text(pDX, IDC_EDIT_RES_RVA, m_Res_RVA);
	DDX_Text(pDX, IDC_EDIT_RES_SIZE, m_Res_Size);
	DDX_Text(pDX, IDC_EDIT_EXA_RVA, m_Exa_RVA);
	DDX_Text(pDX, IDC_EDIT_EXA_SIZE, m_Exa_Size);
	DDX_Text(pDX, IDC_EDIT_SEC_RVA, m_Sec_RVA);
	DDX_Text(pDX, IDC_EDIT_SEC_SIZE, m_Sec_Size);
	DDX_Text(pDX, IDC_EDIT_LOC_RVA, m_Loc_RVA);
	DDX_Text(pDX, IDC_EDIT_LOC_SIZE, m_Loc_Size);
	DDX_Text(pDX, IDC_EDIT_DBG_RVA, m_Dbg_RVA);
	DDX_Text(pDX, IDC_EDIT_DBG_SIZE, m_Dbg_Size);
	DDX_Text(pDX, IDC_EDIT_REV_RVA, m_Rev_RVA);
	DDX_Text(pDX, IDC_EDIT_REV_SIZE, m_Rev_Size);
	DDX_Text(pDX, IDC_EDIT_GLOBAL_RVA, m_Global_RVA);
	DDX_Text(pDX, IDC_EDIT_GLOBAL_SIZE, m_Global_Size);
	DDX_Text(pDX, IDC_EDIT_TLS_RVA, m_TLS_RVA);
	DDX_Text(pDX, IDC_EDIT_TLS_SIZE, m_TLS_Size);
	DDX_Text(pDX, IDC_EDIT_LOAD_RVA, m_Load_RVA);
	DDX_Text(pDX, IDC_EDIT_LOAD_SIZE, m_Load_Size);
	DDX_Text(pDX, IDC_EDIT_BOUND_RVA, m_Bound_RVA);
	DDX_Text(pDX, IDC_EDIT_BOUND_SIZE, m_Bound_Size);
	DDX_Text(pDX, IDC_EDIT_IAT_RVA, m_IAT_RVA);
	DDX_Text(pDX, IDC_EDIT_IAT_SIZE, m_IAT_Size);
	DDX_Text(pDX, IDC_EDIT_DELAY_RVA, m_Delay_RVA);
	DDX_Text(pDX, IDC_EDIT_DELAY_SIZE, m_Delay_Size);
	DDX_Text(pDX, IDC_EDIT_COM_RVA, m_COM_RVA);
	DDX_Text(pDX, IDC_EDIT_COM_SIZE, m_COM_Size);
	DDX_Text(pDX, IDC_EDIT_OTHER_RVA, m_Other_RVA);
	DDX_Text(pDX, IDC_EDIT_OTHER_SIZE, m_Other_Size);
}


BEGIN_MESSAGE_MAP(CDirDlg, CDialogEx)
END_MESSAGE_MAP()


// CDirDlg 消息处理程序
//初始化Dialog
BOOL CDirDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	PIMAGE_DATA_DIRECTORY pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (pTemp)
	{
		m_Export_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Export_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);
	if (pTemp)
	{
		m_Import_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Import_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_RESOURCE);
	if (pTemp)
	{
		m_Res_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Res_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXCEPTION);
	if (pTemp)
	{
		m_Exa_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Exa_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_SECURITY);
	if (pTemp)
	{
		m_Sec_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Sec_Size.Format(_T("0x%x"), pTemp->Size);
	}


	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if (pTemp)
	{
		m_Loc_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Loc_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_DEBUG);
	if (pTemp)
	{
		m_Dbg_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Dbg_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_ARCHITECTURE);
	if (pTemp)
	{
		m_Rev_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Rev_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_GLOBALPTR);
	if (pTemp)
	{
		m_Global_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Global_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_TLS);
	if (pTemp)
	{
		m_TLS_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_TLS_Size.Format(_T("0x%x"), pTemp->Size);
	}


	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
	if (pTemp)
	{
		m_Load_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Load_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
	if (pTemp)
	{
		m_Bound_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Bound_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_IAT);
	if (pTemp)
	{
		m_IAT_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_IAT_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
	if (pTemp)
	{
		m_Delay_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_Delay_Size.Format(_T("0x%x"), pTemp->Size);
	}

	pTemp = m_PeFile.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR);
	if (pTemp)
	{
		m_COM_RVA.Format(_T("0x%x"), pTemp->VirtualAddress);
		m_COM_Size.Format(_T("0x%x"), pTemp->Size);
	}

	UpdateData(FALSE);

	return TRUE;
}