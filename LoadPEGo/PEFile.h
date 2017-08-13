#ifndef _PEFILE_H_
#define _PEFILE_H_

#if _MSC_VER > 1000
#pragma once
#endif

typedef struct _MAP_FILE_STRUCT
{
	HANDLE hFile;//打开的文件句柄
	HANDLE hMapping;//文件映射到内存的句柄
	LPVOID ImageBase;//文件的 数据
	DWORD dwFileSize;//文件的大小


} MAP_FILE_STRUCT,*PMAP_FILE_STRUCT;

enum {
	LFILE_FROM_NOMAL = 0,       //文件打开时的处理方式
	LFILE_FROM_FILE,
	LFILE_FROM_MEMORY
};

class CPEFile
{
public:
	PIMAGE_SECTION_HEADER GetSection(int nIndex);
	BOOL IsPEFile();
	PIMAGE_SECTION_HEADER GetFirstSecHeader();
	int GetSectionCount();
	PIMAGE_DATA_DIRECTORY GetDataDirectory(int nIndex);
	PIMAGE_DATA_DIRECTORY GetDataDirectory();
	PIMAGE_OPTIONAL_HEADER GetNtOptionalHeader();
	PIMAGE_FILE_HEADER GetNtFileHeader();
	PIMAGE_NT_HEADERS GetNtHeader();
	PIMAGE_DOS_HEADER GetDosHeader();
	DWORD GetImageSize();
	BYTE* GetImage();
	BOOL Free();
	BOOL LoadPEFile(char* lpFileName);
	BOOL SetInfo(MAP_FILE_STRUCT* pMapFileMsg);
	MAP_FILE_STRUCT* GetInfo();
	void operator=(CPEFile& lPeFile);
	CPEFile();
	virtual ~CPEFile();
	LPVOID RVAToVA(DWORD dwRVA);//将RVA转为VA
	LPVOID GetDirectoryEntry(int DirectoryEntry);//获取指定数据目录表的地址

	BOOL RVA2(DWORD dwRVA,DWORD& dwVa, DWORD& dwFileOffset);//通过文件头的基址
	BOOL VA2(DWORD dwVA, DWORD& dwRVA, DWORD& dwFileOffset);
	BOOL FileOffset2(DWORD dwFileOffset, DWORD& dwRVA, DWORD& dwVA);
	BOOL GetOffsetByMenRVA(DWORD dwRVA, DWORD& dwSecHRVA, DWORD& dwFileHRVA);//通过内存的RVA获取其所在节的RVA和文件偏移
	BOOL GetOffsetByFileRVA(DWORD dwRVA, DWORD& dwSecHRVA, DWORD& dwFileHRVA);//通过文件中的Offset获取其所在节的RVA和文件偏移
private:
	//用于标识文件的打开状态//用于标识文件的打开状态
	BYTE bLoadStart;
	//用于存储打开的文件句柄，文件数据，文件大小
	MAP_FILE_STRUCT *pstMapFile;

	
};

#endif // !_PEFILE_H_