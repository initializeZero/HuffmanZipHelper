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

extern __int16 getBinTreeNodeNum(BinTree*);

extern __int16 getBinTreeLeavesNum(BinTree*);

extern __int16 getBinTreeNodeNum(BinTreeTable*, __int16);

extern __int16 getBinTreeLeavesNum(BinTreeTable*, __int16);

extern BinTreeTable* convertTreeToTable(BinTree*);

extern BinTree* convertTableToTree(BinTreeTable*);

extern BinTree* getNewNode();

extern __int16 getBinTreeDepth(BinTree*);

extern __int16 getBinTreeDepth(BinTreeTable*);

extern void setBinTreeDepth(BinTree*, __int16);

extern void setBinTreeDepth(BinTreeTable*, __int16, __int16);

extern __int16 compareTree(BinTree*, BinTree*);

extern __int16 compareTree(BinTree*, BinTreeTable*);

extern __int16 compareTree(BinTreeTable*, BinTree*);

extern __int16 compareTree(BinTreeTable*, BinTreeTable*);

extern __int16 destoryBinTree(BinTree*);

extern __int16 destoryBinTree(BinTreeTable*&);