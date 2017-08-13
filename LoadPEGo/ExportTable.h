#pragma once
#include "PEFile.h"
class CExportTable :
	public CPEFile
{
public:
	CExportTable();
	virtual ~CExportTable();
	void operator=(CPEFile& pefile);
	//获取输出表中的
	PIMAGE_EXPORT_DIRECTORY GetExportDir();
	DWORD GetNumOfFuns();
	DWORD GetNumOfNames();
	DWORD GetFirstFun();//获取函数数组的首地址
	DWORD GetFun(DWORD nIndex);//获取指定函数
	DWORD GetRVAOfFun(DWORD nIndex);//获取函数所在的RVA,即提取出函数数组中的值(里面存储着RVA);

	DWORD GetFirstName();//获取带有函数的名称数组地址
	DWORD GetName(DWORD nIndex);//获取指定数组位置的名称
	PBYTE GetNameValue(DWORD nIndex);//获取名称

	DWORD GetFirstNameOrd();//获取index数组地址
	DWORD GetNameOrd(DWORD nIndex);//获取指定数组位
	WORD GetNameOrdValue(DWORD nIndex);//获取ord index
};

