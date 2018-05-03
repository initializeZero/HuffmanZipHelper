#include "HuffmanDecoder.h"

HuffmanDecoder::HuffmanDecoder() {

}

void HuffmanDecoder::errMsgDisplay(char* errorMessage) {
	cout << errorMessage;
}

void HuffmanDecoder::build(char* c1, char* c2) {
	zippedFilePath.append(c1);
	originalFilePath.append(c2);
	init();
}

void HuffmanDecoder::init() {

	status = 0;
	//memset(freqList, 0, 256 * sizeof(BinTree*));
	//memset(prefixCode, 0, 256);
	memset(&zipFileHeadTag, 0, sizeof(ZIPFileInfo));
	int i;
	/*
	for (i = 0; i < 256; i++) {
		freqList[i] = new BinTree;
		prefixCode[i] = new char[256];
		memset(freqList[i], 0, sizeof(BinTree));
		memset(prefixCode[i], 0xFF, 256);
		freqList[i]->data = i;
	}
	*/
	if (openFile() != 0) {
		status = -1;
		return;
	}
	if (checkZippedFileValidity() != 0) {
		errMsgDisplay("待解压文件格式有误,请重新选择!\n");
		status = -1;
		return;
	}

	hfmTreeTable = (BinTreeTable*)malloc(zipFileHeadTag.cDictSize);
	//hfmTreeTable = new BinTreeTable[zipFileHeadTag.cDictItemNumb];
	memset(hfmTreeTable, 0, zipFileHeadTag.cDictSize);
	if (checkBinTreeTableValidity() != 0) {
		errMsgDisplay("哈夫曼表有误,请重新选择文件!\n");
		status = -1;
		return;
	}

	{
		/*文件标签,即"WT"这两个字符*/
		//zipFileHeadTag.zipTag = 0x5457;
		/*文件版本,当前为1.00*/
		//zipFileHeadTag.zipVer = 0x0001;
		/*原文件大小*/
		//zipFileHeadTag.oFileSize = oFileSize;
		/*单条字典记录大小*/
		//zipFileHeadTag.cDictItemSize = sizeof(BinTreeTable);
		/*字典位置偏移量*/
		//zipFileHeadTag.cDictPosi = sizeof(ZIPFileInfo);
	}
}

int HuffmanDecoder::openFile() {
	zippedFile.open(zippedFilePath, ios::in | ios::binary);
	if (!zippedFile.is_open()) {
		errMsgDisplay("原文件打开失败!\n");
		return -1;
	}
	originalFile.open(originalFilePath, ios::out | ios::binary | ios::trunc);
	if (!originalFile.is_open()) {
		errMsgDisplay("创建并写入文件失败!\n");
		return -1;
	}
	zippedFile.seekg(0, zippedFile.end);
	unsigned __int32 fileSize = zippedFile.tellg();
	zFileSize = fileSize;
	return 0;
}

int HuffmanDecoder::checkZippedFileValidity() {
	/*文件大小至少49字节(文件头32字节+至少一条字典记录16字节+文件末尾码1字节)*/
	if (zFileSize < (sizeof(ZIPFileInfo) + sizeof(BinTreeTable) + 1)) {
		errMsgDisplay("文件大小不符合最小压缩文件大小!\n");
		return -1;
	}
	zippedFile.clear();
	zippedFile.seekg(0,ios::beg);
	zippedFile.read((char*)&zipFileHeadTag, sizeof(ZIPFileInfo));
	if (zipFileHeadTag.zipTag != 0x5457) {
		errMsgDisplay("此文件非压缩文件,格式错误\n");
		return -2;
	}
	if (zipFileHeadTag.zipVer != 0x0001) {
		errMsgDisplay("此压缩文件版本过高,暂无法打开\n");
		return -3;
	}
	if (zipFileHeadTag.cFileSize != zFileSize) {
		errMsgDisplay("此压缩文件实际大小与记录不符\n");
		return -4;
	}
	if (zipFileHeadTag.cDataPosi < zipFileHeadTag.cDictPosi + sizeof(BinTreeTable)) {
		errMsgDisplay("........-5\n");
		return -5;
	}
	if (zipFileHeadTag.cDataSize >= zipFileHeadTag.cFileSize || zipFileHeadTag.cDictSize >= zipFileHeadTag.cFileSize) {
		errMsgDisplay("........-6\n");
		return -6;
	}
	if (zipFileHeadTag.cDataSize + zipFileHeadTag.cDictSize + 1 + sizeof(ZIPFileInfo) != zipFileHeadTag.cFileSize) {
		errMsgDisplay("........-7\n");
		return -7;
	}
	if ((unsigned __int32)zipFileHeadTag.cDictItemNumb * zipFileHeadTag.cDictItemSize != zipFileHeadTag.cDictSize) {
		errMsgDisplay("........-8\n");
		return -8;
	}
	if (zipFileHeadTag.cDataPosi != zipFileHeadTag.cDictPosi + zipFileHeadTag.cDictSize) {
		errMsgDisplay("........-9\n");
		return -9;
	}
	if (zipFileHeadTag.cDataPosi + zipFileHeadTag.cDataSize + 1 != zipFileHeadTag.cFileSize) {
		errMsgDisplay("........-10\n");
		return -10;
	}
	if (zipFileHeadTag.cDictPosi != sizeof(ZIPFileInfo)) {
		errMsgDisplay("........-11\n");
		return -11;
	}
	zippedFile.seekg(-1, ios::end);
	zippedFile.read((char*)&eofTag, 1);
	if (eofTag < 0 || eofTag > 7) {
		errMsgDisplay("........-12\n");
		return -12;
	}
	/*应该还有很多严格的判断条件...*/
	
	return 0;
}

int HuffmanDecoder::checkBinTreeTableValidity() {
	zippedFile.clear();
	zippedFile.seekg(sizeof(ZIPFileInfo), ios::beg);
	zippedFile.read((char*)hfmTreeTable, zipFileHeadTag.cDictSize);
	if (hfmTreeTable[0].par != -1) {
		errMsgDisplay("字典记录哈夫曼码表有误!\n");
		return -1;
	}
	if (hfmTreeTable[0].depth != 1) {
		errMsgDisplay("字典记录哈夫曼码表有误!\n");
		return -2;
	}



	/*应该还有很多严格的判断条件...*/

	return 0;
}

int HuffmanDecoder::getPrefixCodeLen(unsigned char* p) {
	for (int i = 0; i < 256; i++) {
		if (p[i] == 0xFF) {
			return i;
		}
	}
	return -1;
}

unsigned char* HuffmanDecoder::prefixCodeCat(unsigned char* des, unsigned char* src) {
	__int32 deslen = getPrefixCodeLen(des);
	__int32 srclen = getPrefixCodeLen(src);
	memcpy((des + deslen), src, srclen);
	return des;
}

unsigned char* HuffmanDecoder::trimPrefixCode(unsigned char* des, __int32 desLen, __int32 remain) {
	if (remain == 0) {
		memset(des, 0xFF, 256);
		return des;
	}
	memcpy(des, (des + desLen - remain), remain);
	memset(des + remain, 0xFF, 256 - remain);
	return des;
}

void HuffmanDecoder::getCharFromHfmTree(BinTree* tree, unsigned char* q, unsigned char* tmp, int& pLen, int cs) {
	if (tree == NULL) return;
	if ((tree->left == NULL && tree->right != NULL) || (tree->left != NULL && tree->right == NULL)) {
		errMsgDisplay(__FUNCTION__"ExceptionNode!\n");
	}
	if (tree->left == NULL && tree->right == NULL) {
		*q = tree->data;
		return;
	}
	if (tmp[pLen] == 0) {
		pLen++;
		getCharFromHfmTree(tree->left, q, tmp, pLen,cs);
		return;
	}
	if (tmp[pLen] == 1) {
		pLen++;
		getCharFromHfmTree(tree->right, q, tmp, pLen,cs);
		return;
	}
	//errMsgDisplay(__FUNCTION__"\n");
	pLen = cs;
	return;
}

int HuffmanDecoder::readByteStream() {
	
	unsigned char a;
	unsigned char b;
	char* p = (char*)&a;
	char* q = (char*)&b;
	int i, j;
	__int32 tmplen = 0;
	__int32 pLen = 0;
	__int32 tempp;
	unsigned __int32 cLen = 0;
	unsigned __int32 eLen = 0;
	unsigned __int32 zLen = zipFileHeadTag.cDataSize;
	unsigned char tmp[256];
	memset(tmp, 0xFF, 256);
	zippedFile.clear();
	zippedFile.seekg(zipFileHeadTag.cDataPosi, ios::beg);
	originalFile.clear();
	originalFile.seekp(0, ios::beg);

	while (cLen < zLen) {
		tmplen = getPrefixCodeLen(tmp);
		while (cLen < zLen && tmplen < 16) {
			zippedFile.read(p, 1);
			for (i = 0, j = 7; i < 8; i++, j--) {
				tmp[tmplen + j] = (a >> i) % 2;
			}
			tmplen += 8;
			cLen++;
		}
		pLen = 0;
		tempp = 0;
		while (pLen < tmplen - 8) {
			getCharFromHfmTree(hfmTree, (unsigned char*)q, tmp, pLen,pLen);
			if (tempp == pLen) break;
			tempp = pLen;
			originalFile.write(q, 1);
			eLen++;
		}
		trimPrefixCode(tmp, tmplen, tmplen - pLen);
	}
	/*处理压缩数据的最后1个字节*/
	tmplen = getPrefixCodeLen(tmp);
	pLen = 0;
	tempp = 0;
	while (pLen < tmplen - 9 + eofTag) {
		getCharFromHfmTree(hfmTree, (unsigned char*)q, tmp, pLen,pLen);
		if (tempp == pLen) break;
		tempp = pLen;
		originalFile.write(q, 1);
		eLen++;
	}

	if (eLen + 1 == zipFileHeadTag.oFileSize) {
		originalFile.write(&hfmTreeTable->reserve3, 1);
		eLen++;
	}
	cout << "解压得到原文件大小为:" << eLen << "字节" << endl;

	return 0;

}

int HuffmanDecoder::decode() {
	if (status != 0) {
		errMsgDisplay("解码器出错,即将退出...\n");
		return -1;
	}
	hfmTree = convertTableToTree(hfmTreeTable);
	if (hfmTree->left == NULL && hfmTree->right == NULL && hfmTree->weight != 0) {
		originalFile.clear();
		originalFile.seekp(0, ios::beg);
		char* a = (char*)&hfmTree->data;
		unsigned __int32 len = hfmTree->weight;
		unsigned __int32 i;
		for (i = 0; i < len; i++) {
			originalFile.write(a, 1);
		}
	}
	else {
		readByteStream();
	}
	originalFile.close();
	zippedFile.close();
	if (status == 0)
		cout << "解码成功,解压出的文件路径为:" << originalFilePath << endl << "按回车键继续\n";
	cin.get();
}