/*
* Author: initializeZero CopyRight 2016~2020
* ����Ŀ��ѭGPL 2.0 ����Э��
* ����ʹ���ڷ���ҵ��;
* ����������ҵ��;,����һ��׷���������ε�Ȩ��
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#pragma once
/*ǿ�Ʊ�����ʹ��2�ֽڶ���*/
#pragma pack (2)
typedef struct ZIPFileInfoStruct {
	/*MagicalNum,Ĭ��ΪWT�����ַ�,��16����0x0x5457*/
	unsigned __int16 zipTag;
	/*ѹ������汾,��һ��Ϊ0x0001*/
	unsigned __int16 zipVer;
	/*ԭ�ļ���С,�޷���32λ����(�����4GB)*/
	unsigned __int32 oFileSize;
	/*��ѹ���ļ���С,�޷���32λ����(�����4GB)*/
	unsigned __int32 cFileSize;
	/*��ѹ���ļ��б���õ����ݴ�С,�޷���32λ����(�����4GB)*/
	unsigned __int32 cDataSize;
	/*��ѹ���ļ��б���õ�����ƫ����,�޷���32λ����*/
	unsigned __int32 cDataPosi;
	/*��ѹ���ļ��й����������ֵ��С,�޷���32λ����*/
	unsigned __int32 cDictSize;
	/*��ѹ���ļ��й����������ֵ�ƫ����,�޷���32λ����*/
	unsigned __int32 cDictPosi;
	/*��ѹ���ļ��й����������ֵ䵥����¼��С,�޷���16λ����*/
	unsigned __int16 cDictItemSize;
	/*��ѹ���ļ��й����������ֵ��¼��Ŀ,�޷���16λ����*/
	unsigned __int16 cDictItemNumb;
}ZIPFileInfo;
/*�ָ�������Ĭ�϶�������*/
#pragma pack ()