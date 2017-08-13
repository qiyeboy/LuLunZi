#pragma once
#include "PEFile.h"
class CImportTable :
	public CPEFile
{
public:
	CImportTable();
	virtual ~CImportTable();
	PIMAGE_IMPORT_BY_NAME GetFuncByName(DWORD ThunkRva);
	PIMAGE_IMPORT_DESCRIPTOR GetImportDes(int nIndex,int num);
	int GetImportDesCount();
	PIMAGE_IMPORT_DESCRIPTOR GetFirstImportDes();
	void operator=(CPEFile& pefile);
	PIMAGE_THUNK_DATA GetFirstThunkData(DWORD ThunkDataRva);
	PIMAGE_THUNK_DATA GetThunkData(int nIndex, DWORD ThunkDataRva,int num);
	int GetThunkDataCount(DWORD ThunkDataRva);

	
};

