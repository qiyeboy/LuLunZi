
// LoadPEGoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "PEFile.h"
// CLoadPEGoDlg 对话框
class CLoadPEGoDlg : public CDialogEx
{
// 构造
public:
	CLoadPEGoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADPEGO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	CListCtrl m_Section_list;
	// 入口点
	CString m_entry;
	// 内存对齐
	CString m_Mem;
	// 区块数量
	CString m_Section_Num;
	// 文件对齐
	CString m_File_Alie;
	// 镜像基址
	CString m_ImageBase;
	// 代码段起始
	CString m_CodeStart;
	// 镜像大小
	CString m_ImageSize;
	// 数据段起始
	CString m_DataStart;
	//定义PE类成员变量
	CPEFile m_PeFile;
	afx_msg void OnBnClickedBtnDir();
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnFlc();
	afx_msg void OnBnClickedBtnExport();
};
