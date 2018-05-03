/*
* Author: initializeZero CopyRight 2016~2020
* ����Ŀ��ѭGPL 2.0 ����Э��
* ����ʹ���ڷ���ҵ��;
* ����������ҵ��;,����һ��׷���������ε�Ȩ��
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#pragma once
typedef unsigned char BinTreeDataType;
typedef struct BinTreeDynamicStruct {
	BinTreeDynamicStruct* par;
	BinTreeDynamicStruct* left;
	BinTreeDynamicStruct* right;
	unsigned __int32 weight;
	__int16 depth;
	BinTreeDataType data;
}BinTree;

#pragma pack(2)
typedef struct BinTreeStaticStruct {
	/*���ڵ�˫��λ��,������16λ����,Ϊ-1ʱ��ʾ�����*/
	__int16 par;
	/*���ڵ�������λ��,������16λ����,Ϊ-1ʱ��ʾ��������*/
	__int16 left;
	/*���ڵ�������λ��,������16λ����,Ϊ-1ʱ��ʾ��������*/
	__int16 right;
	/*���ڵ����,������16λ����,����Ϊ1,ÿ��һ��+1*/
	__int16 depth;
	/*�˽ڵ�Ȩ��,�޷���32λ����*/
	unsigned __int32 weight;
	/*��������*/
	BinTreeDataType data;
	/*�������ֽ�ΪԤ��λ��,��ʵ������,����ṹ���ֽڶ���Ϊ16�ֽ�,���CPU����Ч��*/
	char reserve1;
	char reserve2;
	char reserve3;
}BinTreeTable;
#pragma pack()