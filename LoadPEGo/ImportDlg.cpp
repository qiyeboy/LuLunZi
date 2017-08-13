// ImportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoadPEGo.h"
#include "ImportDlg.h"
#include "afxdialogex.h"
#include "PEFile.h"
#include "ImportTable.h"
// CImportDlg 对话框

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)

CImportDlg::CImportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_IMPORT, pParent)
{

}

CImportDlg::~CImportDlg()
{
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMPORT, m_list_import);
	DDX_Control(pDX, IDC_LIST_API, m_list_apiName);
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMPORT, &CImportDlg::OnNMClickListImport)
END_MESSAGE_MAP()


// CImportDlg 消息处理程序
BOOL CImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化列表控件
	m_list_import.ModifyStyle(0, LVS_REPORT);               // 报表模式  
	m_list_import.SetExtendedStyle(m_list_import.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 间隔线+行选中  
																												 //设置表头
	m_list_import.InsertColumn(0, "DllName", LVCFMT_LEFT, 80);
	m_list_import.InsertColumn(1, "OriginalFirstThunk", LVCFMT_LEFT, 100);
	m_list_import.InsertColumn(2, "TimeDateStamp", LVCFMT_LEFT, 100);
	m_list_import.InsertColumn(3, "ForWarderChain", LVCFMT_LEFT, 100);
	m_list_import.InsertColumn(4, "名称", LVCFMT_LEFT, 80);
	m_list_import.InsertColumn(5, "FirstThunk", LVCFMT_LEFT, 120);

	// 初始化列表控件
	m_list_apiName.ModifyStyle(0, LVS_REPORT);               // 报表模式  
	m_list_apiName.SetExtendedStyle(m_list_import.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 间隔线+行选中  
																											   //设置表头
	m_list_apiName.InsertColumn(0, "ThunkRVA", LVCFMT_LEFT, 100);
	m_list_apiName.InsertColumn(1, "ThunkValue", LVCFMT_LEFT, 80);
	m_list_apiName.InsertColumn(2, "Hint", LVCFMT_LEFT, 80);
	m_list_apiName.InsertColumn(3, "Api Name", LVCFMT_LEFT, 100);

	//首先初始化导入表dll的信息

	int num = importTable.GetImportDesCount();
	CString importData;
	for (int i = 0; i < num; i++) 
	{
		PIMAGE_IMPORT_DESCRIPTOR pImport = importTable.GetImportDes(i, num);
		PBYTE dllName = (PBYTE)importTable.RVAToVA(pImport->Name);
		importData = dllName;
		m_list_import.InsertItem(0, importData);

		importData.Format(_T("0x%x"), pImport->OriginalFirstThunk);
		m_list_import.SetItemText(0, 1, importData);
	
		importData.Format(_T("0x%x"), pImport->TimeDateStamp);
		m_list_import.SetItemText(0, 2, importData);

		importData.Format(_T("0x%x"), pImport->ForwarderChain);
		m_list_import.SetItemText(0, 3, importData);

		importData.Format(_T("0x%x"), pImport->Name);
		m_list_import.SetItemText(0, 4, importData);

		importData.Format(_T("0x%x"), pImport->FirstThunk);
		m_list_import.SetItemText(0, 5, importData);
	}


	return TRUE;

}
//点击事件，当选中导入表中dll时，显示其中的函数
void CImportDlg::OnNMClickListImport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 点击某一行获取所有的apiName
	POSITION ps = m_list_import.GetFirstSelectedItemPosition();
	int nIndex = m_list_import.GetNextSelectedItem(ps);
	*pResult = 0;
	if (nIndex < 0)
		return;
	//CString importData;
	//importData.Format(_T("%d"), nIndex);
	//MessageBox(importData, 0, 0);
	int num = importTable.GetImportDesCount();
	PIMAGE_IMPORT_DESCRIPTOR pImport = importTable.GetImportDes(nIndex, num);
	int size = importTable.GetThunkDataCount(pImport->OriginalFirstThunk);
	m_list_apiName.DeleteAllItems();
	for (int i = 0; i < size; i++)
	{
		CString strHint, strName,strThunkRVA,strThunkValue;
		PIMAGE_THUNK_DATA pThunkData = importTable.GetThunkData(i, pImport->OriginalFirstThunk, size);
		if ( pThunkData->u1.Ordinal & 0x80000000)
		{
			strThunkRVA.Format(_T("0x%x"), pImport->OriginalFirstThunk + i * sizeof(DWORD));
			strThunkValue.Format(_T("0x%x"), pThunkData->u1.Ordinal);
			strHint = _T("--");
			strName = _T("--");

		}
		else 
		{

			strThunkRVA.Format(_T("0x%x"), pImport->OriginalFirstThunk + i * sizeof(DWORD));
			strThunkValue.Format(_T("0x%x"), pThunkData->u1.AddressOfData);
			PIMAGE_IMPORT_BY_NAME pName = importTable.GetFuncByName(pThunkData->u1.AddressOfData);
			strHint.Format(_T("0x%x"), pName->Hint);
			strName = pName->Name;

		
		}

		m_list_apiName.InsertItem(0, strThunkRVA);

	
		m_list_apiName.SetItemText(0, 1, strThunkValue);

		m_list_apiName.SetItemText(0, 2, strHint);

		m_list_apiName.SetItemText(0, 3, strName);



	}
	
	
}
