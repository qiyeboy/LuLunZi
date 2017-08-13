#include "stdafx.h"
#include "ImportTable.h"


CImportTable::CImportTable()
{
}


CImportTable::~CImportTable()
{
}

//ThunkRva为IMAGE_THUNK_DATA所在的RVA
PIMAGE_IMPORT_BY_NAME CImportTable::GetFuncByName(DWORD ThunkRva)
{
	PIMAGE_IMPORT_BY_NAME apiName = (PIMAGE_IMPORT_BY_NAME)RVAToVA(ThunkRva);
	if (!apiName->Name)
		return NULL;
	return apiName;
}
//num为导入表中的个数
PIMAGE_IMPORT_DESCRIPTOR CImportTable::GetImportDes(int nIndex, int num)
{
	PIMAGE_IMPORT_DESCRIPTOR firstImportDes = GetFirstImportDes();
	if (nIndex<0 ||num <= nIndex)
		return NULL;
		
	return &firstImportDes[nIndex];
}
//获取导入表中描述的个数
int CImportTable::GetImportDesCount()
{
	int i = 0;
	PIMAGE_IMPORT_DESCRIPTOR firstImportDes = GetFirstImportDes();
	while (firstImportDes&&firstImportDes->FirstThunk)
	{
		i++;
		firstImportDes++;
	}
		
	return i;
}
//获取导入表中的第一个描述
PIMAGE_IMPORT_DESCRIPTOR CImportTable::GetFirstImportDes()
{
	PIMAGE_IMPORT_DESCRIPTOR firstImportDes = (PIMAGE_IMPORT_DESCRIPTOR)GetDirectoryEntry(IMAGE_DIRECTORY_ENTRY_IMPORT);
	if (!firstImportDes)
		return NULL;

	return firstImportDes;
}

void CImportTable::operator=(CPEFile & pefile)
{
	SetInfo(pefile.GetInfo());
}
//获取第一个ThunkData
PIMAGE_THUNK_DATA CImportTable::GetFirstThunkData(DWORD ThunkDataRva)
{
	PIMAGE_THUNK_DATA pThunkData= (PIMAGE_THUNK_DATA)RVAToVA(ThunkDataRva);
	if (!pThunkData->u1.Ordinal)
		return NULL;
	return pThunkData;

}
//获取指定的ThunkData
PIMAGE_THUNK_DATA CImportTable::GetThunkData(int nIndex, DWORD ThunkDataRva, int num)
{
	PIMAGE_THUNK_DATA pThunkData = GetFirstThunkData(ThunkDataRva);
	if (nIndex < 0 || nIndex >= num)
		return NULL;
	return &pThunkData[nIndex];
}
//获取ThunkData个数
int CImportTable::GetThunkDataCount(DWORD ThunkDataRva)
{
	int i = 0;
	PIMAGE_THUNK_DATA pThunkData = GetFirstThunkData(ThunkDataRva);
	while (pThunkData&&pThunkData->u1.Ordinal)
	{
		i++;
		pThunkData++;
	}

	return i;
}

