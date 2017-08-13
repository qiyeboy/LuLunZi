#include "stdafx.h"
#include "PEFile.h"

#include <imagehlp.h>
#pragma comment(lib,"Dbghelp.lib")
CPEFile::CPEFile()
{
	this->bLoadStart = LFILE_FROM_NOMAL;
	this->pstMapFile = NULL;
}


CPEFile::~CPEFile()
{
	if (Free()) {

	
	}
}
//将RVA转化为VA(这指的是文件映射到内存的地址，而不是真实加载的VA)
LPVOID CPEFile::RVAToVA(DWORD dwRVA)
{
	PIMAGE_NT_HEADERS pNtH = GetNtHeader();
	if (!pNtH)
		return NULL;
	return ImageRvaToVa(pNtH, pstMapFile->ImageBase, dwRVA, NULL);
	
}
//获取指定数据目录表的基址，这次使用ImageRvaToVa函数来表示
LPVOID CPEFile::GetDirectoryEntry(int DirectoryEntry)
{
	PIMAGE_DATA_DIRECTORY pDataDirectory = GetDataDirectory(DirectoryEntry);
	if (!pDataDirectory)
		return NULL;
	
	return RVAToVA(pDataDirectory->VirtualAddress);
}

BOOL CPEFile::RVA2(DWORD dwRVA, DWORD & dwVA, DWORD & dwFileOffset)
{
	PIMAGE_OPTIONAL_HEADER OptionHeader = GetNtOptionalHeader();
	if (!OptionHeader || dwRVA>OptionHeader->SizeOfImage)
		return FALSE;
	dwVA = dwRVA + OptionHeader->ImageBase;
	DWORD dwSecHRVA = 0;
	DWORD dwFileHRVA = 0;
	if (!GetOffsetByMenRVA(dwRVA, dwSecHRVA, dwFileHRVA))//获取的是节头的偏移
		return FALSE;
	dwFileOffset = (dwRVA - dwSecHRVA) + dwFileHRVA;
	return TRUE;
}

BOOL CPEFile::VA2(DWORD dwVA, DWORD & dwRVA, DWORD & dwFileOffset)
{
	PIMAGE_OPTIONAL_HEADER OptionHeader = GetNtOptionalHeader();
	if (!OptionHeader||dwVA>OptionHeader->SizeOfImage+ OptionHeader->ImageBase)
		return FALSE;
	dwRVA = dwVA - OptionHeader->ImageBase;
	DWORD dwSecHRVA = 0;
	DWORD dwFileHRVA = 0;
	if (!GetOffsetByMenRVA(dwRVA, dwSecHRVA, dwFileHRVA))//获取的是节头的偏移
		return FALSE;
	dwFileOffset = (dwRVA - dwSecHRVA) + dwFileHRVA;
	return TRUE;
}

BOOL CPEFile::FileOffset2(DWORD dwFileOffset, DWORD & dwRVA, DWORD & dwVA)
{

	if (!GetImageSize()||dwFileOffset>GetImageSize())
		return FALSE;
	DWORD dwSecHRVA = 0;
	DWORD dwFileHRVA = 0;
	if (!GetOffsetByFileRVA(dwFileOffset, dwSecHRVA, dwFileHRVA))//获取的是节头的偏移
		return FALSE;
	dwRVA = (dwFileOffset - dwFileHRVA) + dwSecHRVA;
	PIMAGE_OPTIONAL_HEADER OptionHeader = GetNtOptionalHeader();
	dwVA = dwRVA + OptionHeader->ImageBase;
	return TRUE;
}

BOOL CPEFile::GetOffsetByMenRVA(DWORD dwRVA,DWORD& dwSecHRVA,DWORD& dwFileHRVA)
{
	PIMAGE_OPTIONAL_HEADER OptionHeader = GetNtOptionalHeader();
	if (!OptionHeader||dwRVA>OptionHeader->SizeOfImage)
		return FALSE;
	PIMAGE_SECTION_HEADER SectionHeader;
	DWORD preSecHRVA = 0;
	DWORD preFileHRVA = 0;
	int count = GetSectionCount();
	for (int i = 0; i < count; i++)
	{
		SectionHeader = GetSection(i);
		if (dwRVA < SectionHeader->VirtualAddress)
		{
			dwSecHRVA = preSecHRVA;
			dwFileHRVA = preFileHRVA;
			return TRUE;
		}
		else
		{
			preSecHRVA = SectionHeader->VirtualAddress;
			preFileHRVA = SectionHeader->PointerToRawData;
		}
		
	}
	dwSecHRVA = preSecHRVA;
	dwFileHRVA = preFileHRVA;
	return TRUE;
}

BOOL CPEFile::GetOffsetByFileRVA(DWORD dwFileRVA, DWORD & dwSecHRVA, DWORD & dwFileHRVA)
{
	PIMAGE_SECTION_HEADER SectionHeader;
	DWORD preSecHRVA = 0;
	DWORD preFileHRVA = 0;
	int count = GetSectionCount();
	if (count <= 0)
		return FALSE;
	for (int i = 0; i < count; i++)
	{
		SectionHeader = GetSection(i);
		if (dwFileRVA < SectionHeader->PointerToRawData)
		{
			dwSecHRVA = preSecHRVA;
			dwFileHRVA = preFileHRVA;
			return TRUE;
		}
		else
		{
			preSecHRVA = SectionHeader->VirtualAddress;
			preFileHRVA = SectionHeader->PointerToRawData;
		}


	}
	return FALSE;
}

//得到文件打开的数据 
MAP_FILE_STRUCT* CPEFile::GetInfo()
{
	return pstMapFile;
}
//用打开的信息载入一个PE

BOOL CPEFile::SetInfo(MAP_FILE_STRUCT* pMapFileMsg)
{
	this->pstMapFile = pMapFileMsg;

	return TRUE;
}
//这个很重要，不用改变状态，为的是不同对话框之前传参数
void CPEFile::operator =(CPEFile& PEFile)
{
	this->SetInfo(PEFile.GetInfo());

}

//用于将文件载入内存

BOOL CPEFile::LoadPEFile(char *lpFilename)
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
	this->pstMapFile = new MAP_FILE_STRUCT;
	memset(this->pstMapFile, 0, sizeof(MAP_FILE_STRUCT));//数据的初始化

	bLoadStart = LFILE_FROM_FILE;

	hFile = CreateFile(lpFilename,
		GENERIC_ALL, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//CString error;
	//error.Format("%d", GetLastError());
	//MessageBox(NULL, error, NULL, 0);
	
	if (hFile==NULL)
		return FALSE;
	this->pstMapFile->dwFileSize = GetFileSize(hFile,NULL);
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
		0, pstMapFile->dwFileSize, NULL);

	if (hMapping == NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	ImageBase = MapViewOfFile(
		hMapping, FILE_MAP_ALL_ACCESS,
		0, 0,
		pstMapFile->dwFileSize);
	if (ImageBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMapping);
		return FALSE;
	}
	pstMapFile->hFile = hFile;
	pstMapFile->hMapping = hMapping;
	pstMapFile->ImageBase = ImageBase;
	return TRUE;
}

//将文件从内存中释放

BOOL CPEFile::Free()
{
	if (bLoadStart == LFILE_FROM_FILE)
	{

		if (pstMapFile->hMapping != NULL)
		{
			UnmapViewOfFile(pstMapFile->hMapping);
			pstMapFile->hMapping = NULL;
			if (pstMapFile->hFile != NULL)
			{
				CloseHandle(pstMapFile->hFile);

				pstMapFile->hFile = NULL;
			}
			delete pstMapFile;
			pstMapFile = NULL;

			return TRUE;
		}
	}
	return FALSE;
}
//获取指定的节表
PIMAGE_SECTION_HEADER CPEFile::GetSection(int nIndex)
{
	int nCount = GetSectionCount();
	if (nIndex >= nCount || nIndex < 0)
		return NULL;

	PIMAGE_SECTION_HEADER pFirstSecHeader = GetFirstSecHeader();
	for (int i = 0; i < nCount; i++)
	{
		if (i == nIndex)
			return pFirstSecHeader;
		pFirstSecHeader++;
	}
	return NULL;
}
//判断PE文件从DOS头和PE头两方面
BOOL CPEFile::IsPEFile()
{
	PIMAGE_DOS_HEADER pDosHeader = GetDosHeader();
	if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE)
	{
		PIMAGE_NT_HEADERS pNtHeader = GetNtHeader();
		if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
			return TRUE;
	}

	return FALSE;
}

PIMAGE_SECTION_HEADER CPEFile::GetFirstSecHeader()
{
	PIMAGE_NT_HEADERS pNth = GetNtHeader();
	if (pNth != NULL)
	{
		return IMAGE_FIRST_SECTION(pNth);
	}
	return NULL;
}
//获取节表的数目
int CPEFile::GetSectionCount()
{
	PIMAGE_FILE_HEADER pNtFileHeader = GetNtFileHeader();
	if (pNtFileHeader != NULL)
	{
		return pNtFileHeader->NumberOfSections;
	}
	return 0;
}
//获取指定数据表
PIMAGE_DATA_DIRECTORY CPEFile::GetDataDirectory(int nIndex)
{
	if (nIndex >= 0 && nIndex <= 16)
	{
		PIMAGE_DATA_DIRECTORY pDataDir = GetDataDirectory();
		if (pDataDir != NULL)
		{
			return &pDataDir[nIndex];
		}
	}

	return NULL;
}
//获取数据表
PIMAGE_DATA_DIRECTORY CPEFile::GetDataDirectory()
{
	PIMAGE_OPTIONAL_HEADER pOptionHeader = GetNtOptionalHeader();
	if (pOptionHeader != NULL)
	{
		return (PIMAGE_DATA_DIRECTORY)pOptionHeader->DataDirectory;
	}
	return NULL;
}
//获取OptionalHeader
PIMAGE_OPTIONAL_HEADER CPEFile::GetNtOptionalHeader()
{
	PIMAGE_NT_HEADERS pNth = GetNtHeader();
	if (pNth != NULL)
	{
		return (PIMAGE_OPTIONAL_HEADER)&(pNth->OptionalHeader);
	}

	return NULL;
}
//获取File_Header
PIMAGE_FILE_HEADER CPEFile::GetNtFileHeader()
{
	PIMAGE_NT_HEADERS pNth = GetNtHeader();
	if (pNth != NULL)
	{
		return (PIMAGE_FILE_HEADER)&(pNth->FileHeader);
	}

	return NULL;
}
//获取NtHeader
PIMAGE_NT_HEADERS CPEFile::GetNtHeader()
{
	PIMAGE_DOS_HEADER pDH = GetDosHeader();
	if (pDH != NULL)
	{
		return (PIMAGE_NT_HEADERS32)(GetImage() + pDH->e_lfanew);
	}
	return NULL;
}
//获取DosHeader
PIMAGE_DOS_HEADER CPEFile::GetDosHeader()
{
	return (PIMAGE_DOS_HEADER)GetImage();
}
//获取文件的大小
DWORD CPEFile::GetImageSize()
{
	if (pstMapFile != NULL)
	{
		return pstMapFile->dwFileSize;
	}
	return 0;
}
//获取映射到内存中的基址
BYTE* CPEFile::GetImage()
{
	if (pstMapFile != NULL)
	{
		return (BYTE*)pstMapFile->ImageBase;
	}
	return NULL;
}
