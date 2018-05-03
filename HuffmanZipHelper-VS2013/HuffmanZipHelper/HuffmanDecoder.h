/*
* Author: initializeZero CopyRight 2016~2020
* ����Ŀ��ѭGPL 2.0 ����Э��
* ����ʹ���ڷ���ҵ��;
* ����������ҵ��;,����һ��׷���������ε�Ȩ��
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#pragma once
#include "commen.h"
#include "BinTreeStruct.h"
#include "BinTreeUtils.h"
#include "FileHeadStruct.h"

class HuffmanDecoder {
/*˽�г�Ա����������*/
private:
	/*ѹ���ļ�����·��*/
	string zippedFilePath;
	/*�����ԭ�ļ�����·��*/
	string originalFilePath;
	/*ѹ���ļ���*/
	ifstream zippedFile;
	/*���ԭ�ļ���*/
	ofstream originalFile;
	/*ѹ���ļ�ͷ��Ϣ�ṹ��*/
	ZIPFileInfo zipFileHeadTag;
	/*���������Ŷ�����(��̬)ָ��*/
	BinTree* hfmTree;
	/*���������Ŷ�����(��̬��)ָ��*/
	BinTreeTable* hfmTreeTable;
	/*ѹ���ļ���С*/
	unsigned __int32 zFileSize;
	/*ԭ�ļ���С*/
	unsigned __int32 oDataSize;
	/*decoder״̬��*/
	int status;
	/*ԭ�ļ�������õĶ���������8ȡ���ʣ������*/
	unsigned char eofTag;
	/*��ʾ������Ϣ*/
	void errMsgDisplay(char*);
	/*��ʼ������*/
	void init();
	/*���ļ���غ���*/
	int openFile();
	/*ͨ�������ѹ���ļ�ͷ�ṹ���������ж��ļ��Ƿ���Ч*/
	int checkZippedFileValidity();
	/*������Ĺ����������ֵ��Ƿ���Ч*/
	int checkBinTreeTableValidity();
	/*��ȡ�����ַ�������Ч������ǰ׺��ĳ���*/
	int getPrefixCodeLen(unsigned char*);
	/*ǰ׺��ƴ�Ӻ���*/
	unsigned char* prefixCodeCat(unsigned char*, unsigned char*);
	/*����ǰ׺�����,������ת����ɺ�Ķ�����ǰ׺��������������,Ȼ��ʣ��δת���Ķ�����ǰ׺���ƶ�����ǰ��*/
	unsigned char* trimPrefixCode(unsigned char*, __int32, __int32);
	/*���ն���������ȡ��Ӧ�������������*/
	void getCharFromHfmTree(BinTree*, unsigned char*, unsigned char*, int&, int);
	/*�ӱ��������н���õ�ԭ����*/
	int readByteStream();
/*���г�Ա����*/
public:
	/*��������*/
	void build(char*, char*);
	/*����Ĺ��캯��*/
	HuffmanDecoder();
	/*�����װ�õĽ��뺯��*/
	int decode();
};