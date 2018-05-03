/*
* Author: initializeZero CopyRight 2016~2020
* 本项目遵循GPL 2.0 共享协议
* 可以使用于非商业用途
* 请勿用于商业用途,保留一切追究法律责任的权利
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#pragma once
#include "commen.h"
#include "BinTreeStruct.h"
#include "BinTreeUtils.h"
#include "FileHeadStruct.h"

class HuffmanEncoder {
private:
	/*原文件完整路径*/
	string originalFilePath;
	/*待输出压缩文件完整路径*/
	string zippedFilePath;
	/*原文件流*/
	ifstream originalFile;
	/*输出压缩文件流*/
	ofstream zippedFile;
	/*压缩文件头信息结构体*/
	ZIPFileInfo zipFileHeadTag;
	/*哈夫曼最优二叉树指针*/
	BinTree* hfmTree;
	/*字符统计频率表*/
	BinTree* freqList[256];
	/*原文件大小*/
	unsigned __int32 oFileSize;
	/*编码好的数据大小*/
	unsigned __int32 cDataSize;
	/*encoder状态码*/
	int status;
	/*字符二进制前缀码数组*/
	char* prefixCode[256];
	/*用于输出错误信息的函数*/
	void errMsgDisplay(char*);
	/*初始化函数*/
	void init();
	/*打开文件相关函数*/
	int openFile();
	/*生成字符频率表函数*/
	void generateFreqList();
	/*获取频率表中最小权重的元素结点函数*/
	int getMinFreqElemSeq();
	/*生成哈夫曼最优二叉树函数*/
	int generateHFMTree();
	/*写入二进制前缀码表具体实现函数*/
	void writePrefixCodeTable(BinTree*, BinTree*, char*);
	/*生成各字符对应的二进制前缀码表函数*/
	int generatePrefixCodeTable();
	/*获取传入字符串中有效二进制前缀码的长度*/
	int getPrefixCodeLen(unsigned char*);
	/*前缀码拼接函数*/
	unsigned char* prefixCodeCat(unsigned char*, unsigned char*);
	/*整理前缀码表函数,用来将转换完成后的二进制前缀码清理出这个数组,然后将剩余未转换的二进制前缀码移动到最前方*/
	unsigned char* trimPrefixCode(unsigned char*, __int32, __int32);
	/*读取输入原文件并编码后将其输出到压缩文件中*/
	int writeByteStream();
	/*使用new运算符复制一个新的结点*/
	BinTree* copyNode(BinTree*);

public:
	/*构建函数*/
	void build(char*, char*);
	/*对象的构造函数*/
	HuffmanEncoder();
	/*对外封装好的编码函数*/
	int encode();
};