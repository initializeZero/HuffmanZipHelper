/*
* Author: initializeZero CopyRight 2016~2020
* 本项目遵循GPL 2.0 共享协议
* 可以使用于非商业用途
* 请勿用于商业用途,保留一切追究法律责任的权利
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
	/*本节点双亲位置,带符号16位整数,为-1时表示根结点*/
	__int16 par;
	/*本节点左子树位置,带符号16位整数,为-1时表示无左子树*/
	__int16 left;
	/*本节点右子树位置,带符号16位整数,为-1时表示无右子树*/
	__int16 right;
	/*本节点深度,带符号16位整数,树根为1,每深一层+1*/
	__int16 depth;
	/*此节点权重,无符号32位整数*/
	unsigned __int32 weight;
	/*数据类型*/
	BinTreeDataType data;
	/*以下三字节为预留位置,无实际意义,方便结构体字节对齐为16字节,提高CPU处理效率*/
	char reserve1;
	char reserve2;
	char reserve3;
}BinTreeTable;
#pragma pack()