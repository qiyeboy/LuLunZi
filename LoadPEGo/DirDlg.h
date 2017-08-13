#pragma once

#include "PEFile.h"
// CDirDlg 对话框

class CDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDirDlg)

public:
	CDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DIR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	//定义PE类成员变量
	CPEFile m_PeFile;
	// 输出表RVA
	CString m_Export_RVA;
	// 输出表Size
	CString m_Export_Size;
	// 导入表RVA
	CString m_Import_RVA;
	// 导入表Size
	CString m_Import_Size;
	// 资源RVA
	CString m_Res_RVA;
	// 资源Size
	CString m_Res_Size;
	// 异常RVA
	CString m_Exa_RVA;
	// 异常Size
	CString m_Exa_Size;
	// 安全RVA
	CString m_Sec_RVA;
	// 安全
	CString m_Sec_Size;
	// 重定位表RVA
	CString m_Loc_RVA;
	// 重定位表Size
	CString m_Loc_Size;
	// 调试RVA
	CString m_Dbg_RVA;
	// 调试Size
	CString m_Dbg_Size;
	// 版权RVA
	CString m_Rev_RVA;
	// 版权Size
	CString m_Rev_Size;
	// 全局区域RVA
	CString m_Global_RVA;
	// 全局区域Size
	CString m_Global_Size;
	// TLS_RVA
	CString m_TLS_RVA;
	// TLS Size
	CString m_TLS_Size;
	// 载入配置RVA
	CString m_Load_RVA;
	// 载入配置
	CString m_Load_Size;
	// 绑定输入
	CString m_Bound_RVA;
	// 绑定输入Size
	CString m_Bound_Size;
	// IAT RVA
	CString m_IAT_RVA;
	// IAT Size
	CString m_IAT_Size;
	// 延迟输入RVA
	CString m_Delay_RVA;
	// 延迟输入size
	CString m_Delay_Size;
	// COM_RVA
	CString m_COM_RVA;
	// COM_Size
	CString m_COM_Size;
	// 保留_RVA
	CString m_Other_RVA;
	// 保留 Size
	CString m_Other_Size;
};
