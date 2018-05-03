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
		errMsgDisplay("����ѹ�ļ���ʽ����,������ѡ��!\n");
		status = -1;
		return;
	}

	hfmTreeTable = (BinTreeTable*)malloc(zipFileHeadTag.cDictSize);
	//hfmTreeTable = new BinTreeTable[zipFileHeadTag.cDictItemNumb];
	memset(hfmTreeTable, 0, zipFileHeadTag.cDictSize);
	if (checkBinTreeTableValidity() != 0) {
		errMsgDisplay("������������,������ѡ���ļ�!\n");
		status = -1;
		return;
	}

	{
		/*�ļ���ǩ,��"WT"�������ַ�*/
		//zipFileHeadTag.zipTag = 0x5457;
		/*�ļ��汾,��ǰΪ1.00*/
		//zipFileHeadTag.zipVer = 0x0001;
		/*ԭ�ļ���С*/
		//zipFileHeadTag.oFileSize = oFileSize;
		/*�����ֵ��¼��С*/
		//zipFileHeadTag.cDictItemSize = sizeof(BinTreeTable);
		/*�ֵ�λ��ƫ����*/
		//zipFileHeadTag.cDictPosi = sizeof(ZIPFileInfo);
	}
}

int HuffmanDecoder::openFile() {
	zippedFile.open(zippedFilePath, ios::in | ios::binary);
	if (!zippedFile.is_open()) {
		errMsgDisplay("ԭ�ļ���ʧ��!\n");
		return -1;
	}
	originalFile.open(originalFilePath, ios::out | ios::binary | ios::trunc);
	if (!originalFile.is_open()) {
		errMsgDisplay("������д���ļ�ʧ��!\n");
		return -1;
	}
	zippedFile.seekg(0, zippedFile.end);
	unsigned __int32 fileSize = zippedFile.tellg();
	zFileSize = fileSize;
	return 0;
}

int HuffmanDecoder::checkZippedFileValidity() {
	/*�ļ���С����49�ֽ�(�ļ�ͷ32�ֽ�+����һ���ֵ��¼16�ֽ�+�ļ�ĩβ��1�ֽ�)*/
	if (zFileSize < (sizeof(ZIPFileInfo) + sizeof(BinTreeTable) + 1)) {
		errMsgDisplay("�ļ���С��������Сѹ���ļ���С!\n");
		return -1;
	}
	zippedFile.clear();
	zippedFile.seekg(0,ios::beg);
	zippedFile.read((char*)&zipFileHeadTag, sizeof(ZIPFileInfo));
	if (zipFileHeadTag.zipTag != 0x5457) {
		errMsgDisplay("���ļ���ѹ���ļ�,��ʽ����\n");
		return -2;
	}
	if (zipFileHeadTag.zipVer != 0x0001) {
		errMsgDisplay("��ѹ���ļ��汾����,���޷���\n");
		return -3;
	}
	if (zipFileHeadTag.cFileSize != zFileSize) {
		errMsgDisplay("��ѹ���ļ�ʵ�ʴ�С���¼����\n");
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
	/*Ӧ�û��кܶ��ϸ���ж�����...*/
	
	return 0;
}

int HuffmanDecoder::checkBinTreeTableValidity() {
	zippedFile.clear();
	zippedFile.seekg(sizeof(ZIPFileInfo), ios::beg);
	zippedFile.read((char*)hfmTreeTable, zipFileHeadTag.cDictSize);
	if (hfmTreeTable[0].par != -1) {
		errMsgDisplay("�ֵ��¼�������������!\n");
		return -1;
	}
	if (hfmTreeTable[0].depth != 1) {
		errMsgDisplay("�ֵ��¼�������������!\n");
		return -2;
	}



	/*Ӧ�û��кܶ��ϸ���ж�����...*/

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
	/*����ѹ�����ݵ����1���ֽ�*/
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
	cout << "��ѹ�õ�ԭ�ļ���СΪ:" << eLen << "�ֽ�" << endl;

	return 0;

}

int HuffmanDecoder::decode() {
	if (status != 0) {
		errMsgDisplay("����������,�����˳�...\n");
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
		cout << "����ɹ�,��ѹ�����ļ�·��Ϊ:" << originalFilePath << endl << "���س�������\n";
	cin.get();
}