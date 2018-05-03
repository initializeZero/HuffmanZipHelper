/*
* Author: initializeZero CopyRight 2016~2020
* 本项目遵循GPL 2.0 共享协议
* 可以使用于非商业用途
* 请勿用于商业用途,保留一切追究法律责任的权利
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#pragma once
/*强制编译器使用2字节对齐*/
#pragma pack (2)
typedef struct ZIPFileInfoStruct {
	/*MagicalNum,默认为WT两个字符,即16进制0x0x5457*/
	unsigned __int16 zipTag;
	/*压缩软件版本,第一版为0x0001*/
	unsigned __int16 zipVer;
	/*原文件大小,无符号32位整数(即最大4GB)*/
	unsigned __int32 oFileSize;
	/*此压缩文件大小,无符号32位整数(即最大4GB)*/
	unsigned __int32 cFileSize;
	/*此压缩文件中编码好的数据大小,无符号32位整数(即最大4GB)*/
	unsigned __int32 cDataSize;
	/*此压缩文件中编码好的数据偏移量,无符号32位整数*/
	unsigned __int32 cDataPosi;
	/*此压缩文件中哈夫曼编码字典大小,无符号32位整数*/
	unsigned __int32 cDictSize;
	/*此压缩文件中哈夫曼编码字典偏移量,无符号32位整数*/
	unsigned __int32 cDictPosi;
	/*此压缩文件中哈夫曼编码字典单条记录大小,无符号16位整数*/
	unsigned __int16 cDictItemSize;
	/*此压缩文件中哈夫曼编码字典记录数目,无符号16位整数*/
	unsigned __int16 cDictItemNumb;
}ZIPFileInfo;
/*恢复编译器默认对齐设置*/
#pragma pack ()