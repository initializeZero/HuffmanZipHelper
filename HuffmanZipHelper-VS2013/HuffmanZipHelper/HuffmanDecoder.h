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

class HuffmanDecoder {
/*私有成员变量及函数*/
private:
	/*压缩文件完整路径*/
	string zippedFilePath;
	/*待输出原文件完整路径*/
	string originalFilePath;
	/*压缩文件流*/
	ifstream zippedFile;
	/*输出原文件流*/
	ofstream originalFile;
	/*压缩文件头信息结构体*/
	ZIPFileInfo zipFileHeadTag;
	/*哈夫曼最优二叉树(动态)指针*/
	BinTree* hfmTree;
	/*哈夫曼最优二叉树(静态表)指针*/
	BinTreeTable* hfmTreeTable;
	/*压缩文件大小*/
	unsigned __int32 zFileSize;
	/*原文件大小*/
	unsigned __int32 oDataSize;
	/*decoder状态码*/
	int status;
	/*原文件流编码好的二进制流对8取余后剩余数量*/
	unsigned char eofTag;
	/*显示错误信息*/
	void errMsgDisplay(char*);
	/*初始化函数*/
	void init();
	/*打开文件相关函数*/
	int openFile();
	/*通过读入的压缩文件头结构体数据来判断文件是否有效*/
	int checkZippedFileValidity();
	/*检测读入的哈夫曼编码字典是否有效*/
	int checkBinTreeTableValidity();
	/*获取传入字符串中有效二进制前缀码的长度*/
	int getPrefixCodeLen(unsigned char*);
	/*前缀码拼接函数*/
	unsigned char* prefixCodeCat(unsigned char*, unsigned char*);
	/*整理前缀码表函数,用来将转换完成后的二进制前缀码清理出这个数组,然后将剩余未转换的二进制前缀码移动到最前方*/
	unsigned char* trimPrefixCode(unsigned char*, __int32, __int32);
	/*按照二进制流读取对应二叉树结点数据*/
	void getCharFromHfmTree(BinTree*, unsigned char*, unsigned char*, int&, int);
	/*从编码数据中解码得到原数据*/
	int readByteStream();
/*公有成员函数*/
public:
	/*构建函数*/
	void build(char*, char*);
	/*对象的构造函数*/
	HuffmanDecoder();
	/*对外封装好的解码函数*/
	int decode();
};