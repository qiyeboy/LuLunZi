#include "stdafx.h"
#include "ExportTable.h"


CExportTable::CExportTable()
{
}


CExportTable::~CExportTable()
{
}

void CExportTable::operator=(CPEFile & pefile)
{
	SetInfo(pefile.GetInfo());
}
//获取输出表
PIMAGE_EXPORT_DIRECTORY CExportTable::GetExportDir()
{
	PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)GetDirectoryEntry(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!exportDir)
		return NULL;

	return exportDir;
}

DWORD CExportTable::GetNumOfFuns()
{
	PIMAGE_EXPORT_DIRECTORY pExportDir = GetExportDir();
	if(!pExportDir)
		return 0;
	return pExportDir->NumberOfFunctions;
}

DWORD CExportTable::GetNumOfNames()
{
	PIMAGE_EXPORT_DIRECTORY pExportDir = GetExportDir();
	if (!pExportDir)
		return 0;
	return pExportDir->NumberOfNames;
}

DWORD CExportTable::GetFirstFun()
{

	PIMAGE_EXPORT_DIRECTORY pExportDir = GetExportDir();
	if (!pExportDir)
		return 0;
	return pExportDir->AddressOfFunctions;
}

DWORD CExportTable::GetFun(DWORD nIndex)
{
	DWORD firstFun = GetFirstFun();
	if (!firstFun || nIndex>GetNumOfFuns())
		return 0;
	return firstFun +nIndex*sizeof(DWORD);
}

DWORD CExportTable::GetRVAOfFun(DWORD nIndex)
{
	DWORD fun = GetFun(nIndex);
	if (!fun)
		return 0;

	return *(DWORD*)RVAToVA(fun);
}

DWORD CExportTable::GetFirstName()
{
	PIMAGE_EXPORT_DIRECTORY pExportDir = GetExportDir();
	if (!pExportDir)
		return 0;
	return pExportDir->AddressOfNames;
}

DWORD CExportTable::GetName(DWORD nIndex)
{
	DWORD firstName = GetFirstName();
	if (!firstName|| nIndex>GetNumOfNames())
		return 0;

	return firstName+nIndex*sizeof(DWORD);
}

PBYTE CExportTable::GetNameValue(DWORD nIndex)
{
	DWORD name = GetName(nIndex);
	if (!name)
		return 0;
	DWORD nameRVA = *(DWORD*)RVAToVA(name);
	return (PBYTE)RVAToVA(nameRVA);

}

DWORD CExportTable::GetFirstNameOrd()
{
	PIMAGE_EXPORT_DIRECTORY pExportDir = GetExportDir();
	if (!pExportDir)
		return 0;
	return pExportDir->AddressOfNameOrdinals;
}

DWORD CExportTable::GetNameOrd(DWORD nIndex)
{
	DWORD  firstNameOrd = GetFirstNameOrd();
	if (!firstNameOrd || nIndex>GetNumOfNames())
		return 0;

	return firstNameOrd+nIndex*sizeof(WORD);
}

WORD CExportTable::GetNameOrdValue(DWORD nIndex)
{
	DWORD  nameOrd = GetNameOrd(nIndex);
	if(!nameOrd)
		return 0;
	return *(WORD*)RVAToVA(nameOrd);
}
