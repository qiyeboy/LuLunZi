
// LoadPEGoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoadPEGo.h"
#include "LoadPEGoDlg.h"
#include "afxdialogex.h"
#include "DirDlg.h"
#include "ImportDlg.h"
#include "FLCDlg.h"
#include "ExportDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoadPEGoDlg 对话框



CLoadPEGoDlg::CLoadPEGoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOADPEGO_DIALOG, pParent)
	, m_entry(_T(""))
	, m_Mem(_T(""))
	, m_Section_Num(_T(""))
	, m_File_Alie(_T(""))
	, m_ImageBase(_T(""))
	, m_CodeStart(_T(""))
	, m_ImageSize(_T(""))
	, m_DataStart(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadPEGoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEC_LIST, m_Section_list);
	DDX_Text(pDX, IDC_EDIT_ENTRY, m_entry);
	DDX_Text(pDX, IDC_EDIT_MEM, m_Mem);
	DDX_Text(pDX, IDC_EDIT_SECTION, m_Section_Num);
	DDX_Text(pDX, IDC_EDIT_FILEALIE, m_File_Alie);
	DDX_Text(pDX, IDC_EDIT_IMAGEBASE, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT_CODE, m_CodeStart);
	DDX_Text(pDX, IDC_EDIT_IMAGESIZE, m_ImageSize);
	DDX_Text(pDX, IDC_EDIT_DATA, m_DataStart);
}

BEGIN_MESSAGE_MAP(CLoadPEGoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CLoadPEGoDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_DIR, &CLoadPEGoDlg::OnBnClickedBtnDir)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CLoadPEGoDlg::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_FLC, &CLoadPEGoDlg::OnBnClickedBtnFlc)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CLoadPEGoDlg::OnBnClickedBtnExport)
END_MESSAGE_MAP()


// CLoadPEGoDlg 消息处理程序

BOOL CLoadPEGoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化列表控件
	m_Section_list.ModifyStyle(0, LVS_REPORT);               // 报表模式  
	m_Section_list.SetExtendedStyle(m_Section_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);// 间隔线+行选中  
	//设置表头
	m_Section_list.InsertColumn(0, "名称", LVCFMT_LEFT, 100);
	m_Section_list.InsertColumn(1, "vOffset", LVCFMT_LEFT, 80);
	m_Section_list.InsertColumn(2, "vSize", LVCFMT_LEFT, 80);
	m_Section_list.InsertColumn(3, "rOffset", LVCFMT_LEFT, 80);
	m_Section_list.InsertColumn(4, "rSize", LVCFMT_LEFT, 80);
	m_Section_list.InsertColumn(5, "标记", LVCFMT_LEFT, 120);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoadPEGoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoadPEGoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoadPEGoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoadPEGoDlg::OnBnClickedBtnOpen()
{

	// 打开文件
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = _T("E:\\");   //默认打开的文件路径  
	CString fileName = _T("");         //默认打开的文件名  
	CString filter = _T("文件 (*.exe; *.dll)|*.exe;*.dll||");   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = _T("E:\\");
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = _T("");
	if (result != IDOK) {
		return;
	}
	if (m_Section_list.GetItemCount()>0)
		m_Section_list.DeleteAllItems();
	filePath = openFileDlg.GetPathName();
	
	m_PeFile.Free();
	if (!m_PeFile.LoadPEFile(filePath.GetBuffer(0))) {
		MessageBox(_T("文件加载失败!"), 0, 0);
		return;
	
	}
	if (!m_PeFile.IsPEFile())
	{
		MessageBox(_T("这不是有效的PE文件!"), 0, 0);
		return;
	}
	PIMAGE_FILE_HEADER FileHeader = m_PeFile.GetNtFileHeader();
	PIMAGE_OPTIONAL_HEADER OptionHeader = m_PeFile.GetNtOptionalHeader();
	//入口点
	m_entry.Format(_T("0x%x"), OptionHeader->AddressOfEntryPoint);
	//内存对齐
	m_Mem.Format(_T("0x%x"), OptionHeader->SectionAlignment);
	//区块数量
	m_Section_Num.Format(_T("0x%x"), FileHeader->NumberOfSections);
	//文件对齐
	m_File_Alie.Format(_T("0x%x"), OptionHeader->FileAlignment);
	//镜像基址
	m_ImageBase.Format(_T("0x%x"), OptionHeader->ImageBase);
	//代码段起始
	m_CodeStart.Format(_T("0x%x"), OptionHeader->BaseOfCode);
	//镜像大小
	m_ImageSize.Format(_T("0x%x"), OptionHeader->SizeOfImage);
	//数据段起始
	m_DataStart.Format(_T("0x%x"), OptionHeader->BaseOfData);
	//开始遍历区段
	PIMAGE_SECTION_HEADER SectionHeader;
	CString sectionData;
	for (int i = 0; i < m_PeFile.GetSectionCount(); i++)
	{
		SectionHeader = m_PeFile.GetSection(i);
		sectionData = SectionHeader->Name;
		m_Section_list.InsertItem(0, sectionData);
		//RVA
		sectionData.Format(_T("0x%x"), SectionHeader->VirtualAddress);
		m_Section_list.SetItemText(0, 1, sectionData);
		//VSize
		sectionData.Format(_T("0x%x"), SectionHeader->Misc.VirtualSize);
		m_Section_list.SetItemText(0, 2, sectionData);
		//rOffset
		sectionData.Format(_T("0x%x"), SectionHeader->PointerToRawData);
		m_Section_list.SetItemText(0, 3, sectionData);
		//rSize
		sectionData.Format(_T("0x%x"), SectionHeader->SizeOfRawData);
		m_Section_list.SetItemText(0, 4, sectionData);
		//标识
		sectionData.Format(_T("0x%x"), SectionHeader->Characteristics);
		m_Section_list.SetItemText(0, 5, sectionData);
	}
	UpdateData(FALSE);
			


	CWnd::SetDlgItemText(IDC_EDIT_FILE, filePath);
}


void CLoadPEGoDlg::OnBnClickedBtnDir()
{
	// 打开目录表
	CDirDlg dlg;

	dlg.m_PeFile = m_PeFile;
	dlg.DoModal();
	return;
}


void CLoadPEGoDlg::OnBnClickedBtnImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CImportDlg dlg;
	dlg.importTable = m_PeFile;
	dlg.DoModal();
}


void CLoadPEGoDlg::OnBnClickedBtnFlc()
{
	// TODO: 在此添加控件通知处理程序代码
	CFLCDlg flcDlg;
	flcDlg.peFile = m_PeFile;
	flcDlg.DoModal();
}


void CLoadPEGoDlg::OnBnClickedBtnExport()
{
	// TODO: 在此添加控件通知处理程序代码
	CExportDlg dlg;
	dlg.exportTableFile = m_PeFile;
	dlg.DoModal();
}
