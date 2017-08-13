#pragma once
#include "afxcmn.h"
#include "ImportTable.h"

// CImportDlg 对话框

class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IMPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// 导入表
	CListCtrl m_list_import;
	// API函数列表
	CListCtrl m_list_apiName;
	//导入表对象
	CImportTable importTable;
	afx_msg void OnNMClickListImport(NMHDR *pNMHDR, LRESULT *pResult);
};
