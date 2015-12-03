// GetMD5.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

// ����digest��ȡ��MD5�ַ���
// ����MD5�ַ���
char *MD5String(unsigned char *digest)
{
	char _MD5[36];
	char tmp[4];
	int i = 0;
	int k = 0;
	memset(_MD5, 0, sizeof(_MD5)); // �ڴ���0��Ϊ���ַ����ض�
	for (; i < 16; ++i)
	{
		sprintf(tmp, "%2X", digest[i]);
		if (tmp[0] == 0x20)// ���ʮ��������0x0A���֣���Ҫ���ո��û���0
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
	unsigned char digest[16];  //��Ž��
	char* _MD5 = (char *)malloc(sizeof(char)* 36);
	char* pNewDir = "E:\\2";	 //Ҫ�������Ŀ¼
	FILE *f;
	//����MD5��Ϣ
	f = fopen("info.txt","a+");
	//�˽ṹ˵����MSDN;
	WIN32_FIND_DATA FindFileData;
	//�����ļ��ľ��;
	HANDLE hListFile;
	//����·��
	char szFullPath[MAX_PATH];
	char szNewPath[MAX_PATH];
	//���·��;
	char szFilePath[MAX_PATH];
	//�������·��;
	sprintf(szFilePath, "%s\\*.*", pszDestPath);

	memset(_MD5, 0, 36);

	//���ҵ�һ���ļ�����ò��Ҿ�������FindFirstFile����INVALID_HANDLE_VALUE�򷵻�;
	if ((hListFile = FindFirstFile(szFilePath, &FindFileData)) == INVALID_HANDLE_VALUE)
	{
		//�����ļ�����;
		return 1;
	}
	else
	{
		do
		{
			//����.��..;
			//��.��������Ŀ¼��..��������Ŀ¼;
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			//����ȫ·��;
			sprintf(szFullPath, "%s\\%s", pszDestPath, FindFileData.cFileName);
			//��ȡ�ļ����ԣ���������ļ���;
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char *pszFileType = NULL;
				// ��ȡ�ļ���MD5
				MD5File(szFullPath, digest);
				strcpy(_MD5, MD5String(digest));
				// ƴ���µ�·����ע���µ���Ŀ¼����ǰ����
				printf("File Name:%s  MD5:%s\n", szFullPath, _MD5);
				sprintf(szNewPath, "%s\\%s", pNewDir, _MD5);
				fprintf(f, _MD5);
				fprintf(f, "\n");
				// �������ƶ����µ��ļ���
				rename(szFullPath, szNewPath);
			}


			//������ļ��У���ݹ����EnmuDirectory����;
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				EnmuDirectory(szFullPath);
			}
			//ѭ����������һ���ļ�;
		} while (FindNextFile(hListFile, &FindFileData));
	}
	//�رվ��;
	FindClose(hListFile); \
	fclose(f);

	// �ͷ������ָ�룬����NULL
	free(_MD5);
	_MD5 = NULL;
	return 0;
}


void main(void)
{
	EnmuDirectory("E:\\1"); //�滻�Լ�Ҫ��ȡ��Ŀ¼	
	getchar();
}
