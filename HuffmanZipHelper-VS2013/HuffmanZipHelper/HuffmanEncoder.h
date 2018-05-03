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

class HuffmanEncoder {
private:
	/*ԭ�ļ�����·��*/
	string originalFilePath;
	/*�����ѹ���ļ�����·��*/
	string zippedFilePath;
	/*ԭ�ļ���*/
	ifstream originalFile;
	/*���ѹ���ļ���*/
	ofstream zippedFile;
	/*ѹ���ļ�ͷ��Ϣ�ṹ��*/
	ZIPFileInfo zipFileHeadTag;
	/*���������Ŷ�����ָ��*/
	BinTree* hfmTree;
	/*�ַ�ͳ��Ƶ�ʱ�*/
	BinTree* freqList[256];
	/*ԭ�ļ���С*/
	unsigned __int32 oFileSize;
	/*����õ����ݴ�С*/
	unsigned __int32 cDataSize;
	/*encoder״̬��*/
	int status;
	/*�ַ�������ǰ׺������*/
	char* prefixCode[256];
	/*�������������Ϣ�ĺ���*/
	void errMsgDisplay(char*);
	/*��ʼ������*/
	void init();
	/*���ļ���غ���*/
	int openFile();
	/*�����ַ�Ƶ�ʱ���*/
	void generateFreqList();
	/*��ȡƵ�ʱ�����СȨ�ص�Ԫ�ؽ�㺯��*/
	int getMinFreqElemSeq();
	/*���ɹ��������Ŷ���������*/
	int generateHFMTree();
	/*д�������ǰ׺������ʵ�ֺ���*/
	void writePrefixCodeTable(BinTree*, BinTree*, char*);
	/*���ɸ��ַ���Ӧ�Ķ�����ǰ׺�����*/
	int generatePrefixCodeTable();
	/*��ȡ�����ַ�������Ч������ǰ׺��ĳ���*/
	int getPrefixCodeLen(unsigned char*);
	/*ǰ׺��ƴ�Ӻ���*/
	unsigned char* prefixCodeCat(unsigned char*, unsigned char*);
	/*����ǰ׺�����,������ת����ɺ�Ķ�����ǰ׺��������������,Ȼ��ʣ��δת���Ķ�����ǰ׺���ƶ�����ǰ��*/
	unsigned char* trimPrefixCode(unsigned char*, __int32, __int32);
	/*��ȡ����ԭ�ļ���������������ѹ���ļ���*/
	int writeByteStream();
	/*ʹ��new���������һ���µĽ��*/
	BinTree* copyNode(BinTree*);

public:
	/*��������*/
	void build(char*, char*);
	/*����Ĺ��캯��*/
	HuffmanEncoder();
	/*�����װ�õı��뺯��*/
	int encode();
};