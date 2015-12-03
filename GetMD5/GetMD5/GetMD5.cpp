// GetMD5.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

// 根据digest获取到MD5字符串
// 返回MD5字符串
char *MD5String(unsigned char *digest)
{
	char _MD5[36];
	char tmp[4];
	int i = 0;
	int k = 0;
	memset(_MD5, 0, sizeof(_MD5)); // 内存置0，为了字符串截断
	for (; i < 16; ++i)
	{
		sprintf(tmp, "%2X", digest[i]);
		if (tmp[0] == 0x20)// 如果十六进制是0x0A这种，需要将空格置换成0
		{
			tmp[0] = 0x30;
		}
		_MD5[k] = tmp[0];
		_MD5[k + 1] = tmp[1];
		k = k + 2;
	}

	return _MD5;
}

int EnmuDirectory(char *pszDestPath)
{
	unsigned char digest[16];  //存放结果
	char* _MD5 = (char *)malloc(sizeof(char)* 36);
	char* pNewDir = "E:\\2";	 //要保存的新目录
	FILE *f;
	//保存MD5信息
	f = fopen("info.txt","a+");
	//此结构说明参MSDN;
	WIN32_FIND_DATA FindFileData;
	//查找文件的句柄;
	HANDLE hListFile;
	//绝对路径
	char szFullPath[MAX_PATH];
	char szNewPath[MAX_PATH];
	//相对路径;
	char szFilePath[MAX_PATH];
	//构造相对路径;
	sprintf(szFilePath, "%s\\*.*", pszDestPath);

	memset(_MD5, 0, 36);

	//查找第一个文件，获得查找句柄，如果FindFirstFile返回INVALID_HANDLE_VALUE则返回;
	if ((hListFile = FindFirstFile(szFilePath, &FindFileData)) == INVALID_HANDLE_VALUE)
	{
		//查找文件错误;
		return 1;
	}
	else
	{
		do
		{
			//过滤.和..;
			//“.”代表本级目录“..”代表父级目录;
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			//构造全路径;
			sprintf(szFullPath, "%s\\%s", pszDestPath, FindFileData.cFileName);
			//读取文件属性，如果不是文件夹;
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char *pszFileType = NULL;
				// 获取文件的MD5
				MD5File(szFullPath, digest);
				strcpy(_MD5, MD5String(digest));
				// 拼接新的路径，注意新的主目录得提前建好
				printf("File Name:%s  MD5:%s\n", szFullPath, _MD5);
				sprintf(szNewPath, "%s\\%s", pNewDir, _MD5);
				fprintf(f, _MD5);
				fprintf(f, "\n");
				// 重命名移动到新的文件夹
				rename(szFullPath, szNewPath);
			}


			//如果是文件夹，则递归调用EnmuDirectory函数;
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				EnmuDirectory(szFullPath);
			}
			//循环，查找下一个文件;
		} while (FindNextFile(hListFile, &FindFileData));
	}
	//关闭句柄;
	FindClose(hListFile); \
	fclose(f);

	// 释放申请的指针，并置NULL
	free(_MD5);
	_MD5 = NULL;
	return 0;
}


void main(void)
{
	EnmuDirectory("E:\\1"); //替换自己要获取的目录	
	getchar();
}
