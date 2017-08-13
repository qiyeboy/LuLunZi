// ExportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoadPEGo.h"
#include "ExportDlg.h"
#include "afxdialogex.h"


// CExportDlg 对话框

IMPLEMENT_DYNAMIC(CExportDlg, CDialogEx)

CExportDlg::CExportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_EXPORT, pParent)
	, m_edit_char(_T(""))
	, m_edit_time(_T(""))
	, m_edit_base(_T(""))
	, m_edit_nameRVA(_T(""))
	, m_edit_name(_T(""))
	, m_edit_numFun(_T(""))
	, m_edit_numName(_T(""))
	, m_edit_addrFun(_T(""))
	, m_edit_addrNames(_T(""))
	, m_edit_AddrOrd(_T(""))
{

}

CExportDlg::~CExportDlg()
{
}

void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXPORT, m_list_export);
	DDX_Text(pDX, IDC_EDIT_Char, m_edit_char);
	DDX_Text(pDX, IDC_EDIT_Time, m_edit_time);
	DDX_Text(pDX, IDC_EDIT_Base, m_edit_base);
	DDX_Text(pDX, IDC_EDIT_NameRVA, m_edit_nameRVA);
	DDX_Text(pDX, IDC_EDIT_Name, m_edit_name);
	DDX_Text(pDX, IDC_EDIT_NUMFun, m_edit_numFun);
	DDX_Text(pDX, IDC_EDIT_NUMName, m_edit_numName);
	DDX_Text(pDX, IDC_EDIT_ADDRFun, m_edit_addrFun);
	DDX_Text(pDX, IDC_EDIT_ADDRName, m_edit_addrNames);
	DDX_Text(pDX, IDC_EDIT_ADDROrdinal, m_edit_AddrOrd);
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportDlg 消息处理程序
BOOL CExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	PIMAGE_EXPORT_DIRECTORY pExportDir = exportTableFile.GetExportDir();
	if (!pExportDir)
		return TRUE;
	m_edit_char.Format(_T("0x%x"), pExportDir->Characteristics);
	m_edit_time.Format(_T("0x%x"), pExportDir->TimeDateStamp);
	m_edit_base.Format(_T("0x%x"), pExportDir->Base);
	m_edit_nameRVA.Format(_T("0x%x"), pExportDir->Name);

	m_edit_name = (PBYTE)exportTableFile.RVAToVA(pExportDir->Name);
	m_edit_numFun.Format(_T("0x%x"), pExportDir->NumberOfFunctions);
	m_edit_numName.Format(_T("0x%x"), pExportDir->NumberOfNames);
	m_edit_addrFun.Format(_T("0x%x"), pExportDir->AddressOfFunctions);

	m_edit_addrNames.Format(_T("0x%x"), pExportDir->AddressOfNames);
	m_edit_AddrOrd.Format(_T("0x%x"), pExportDir->AddressOfNameOrdinals);
	UpdateData(FALSE);

	// 初始化列表控件
	m_list_export.ModifyStyle(0, LVS_REPORT);               // 报表模式  
	m_list_export.SetExtendedStyle(m_list_export.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 间隔线+行选中  
																											   //设置表头
	m_list_export.InsertColumn(0, "Ordinal", LVCFMT_LEFT, 80);
	m_list_export.InsertColumn(1, "RVA", LVCFMT_LEFT, 100);
	m_list_export.InsertColumn(2, "Function Name", LVCFMT_LEFT, 100);

	//提取其中的函数名称
	CString strTemp;
	for (DWORD i = 0; i < exportTableFile.GetNumOfNames(); i++)
	{
		strTemp.Format(_T("%d"), pExportDir->Base + i);
		m_list_export.InsertItem(i, strTemp);

		DWORD funIndex = exportTableFile.GetNameOrdValue(i);
		DWORD funRVA = exportTableFile.GetRVAOfFun(funIndex);
		strTemp.Format(_T("0x%x"), funRVA);
		m_list_export.SetItemText(i, 1,strTemp);

		strTemp = exportTableFile.GetNameValue(i);

		m_list_export.SetItemText(i, 2, strTemp);
	}

	return 0;
}