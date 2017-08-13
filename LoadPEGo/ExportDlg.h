#pragma once
#include "afxcmn.h"

#include "ExportTable.h"
// CExportDlg 对话框

class CExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// 导出表list
	CListCtrl m_list_export;
	// Characteristics
	CString m_edit_char;
	// TimeDateStamp
	CString m_edit_time;
	// Base
	CString m_edit_base;
	// NameRVA
	CString m_edit_nameRVA;
	// Name
	CString m_edit_name;
	// NumberOfFunctions
	CString m_edit_numFun;
	// NumberofNames
	CString m_edit_numName;
	// AddressOfFunctions
	CString m_edit_addrFun;
	// AddressOfNames
	CString m_edit_addrNames;
	// AddressOfNameOrdinals
	CString m_edit_AddrOrd;

	CExportTable exportTableFile;
};
