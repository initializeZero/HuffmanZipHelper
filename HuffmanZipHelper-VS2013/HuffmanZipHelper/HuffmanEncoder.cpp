/*
* Author: initializeZero CopyRight 2016~2020
* ����Ŀ��ѭGPL 2.0 ����Э��
* ����ʹ���ڷ���ҵ��;
* ����������ҵ��;,����һ��׷���������ε�Ȩ��
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#include "HuffmanEncoder.h"

void HuffmanEncoder::errMsgDisplay(char* errorMessage) {
	cout << errorMessage;
}

int HuffmanEncoder::openFile() {
	originalFile.open(originalFilePath, ios::in | ios::binary);
	if (!originalFile.is_open()) {
		errMsgDisplay("ԭ�ļ���ʧ��!\n");
		return -1;
	}
	zippedFile.open(zippedFilePath, ios::out | ios::binary | ios::trunc);
	if (!zippedFile.is_open()) {
		errMsgDisplay("������д���ļ�ʧ��!\n");
		return -1;
	}
	originalFile.seekg(0, originalFile.end);
	unsigned __int32 fileSize = originalFile.tellg();
	oFileSize = fileSize;
	return 0;
}

// '\n'

void HuffmanEncoder::generateFreqList() {
	unsigned __int32 i;
	unsigned char c;
	originalFile.clear();
	originalFile.seekg(0, ios::beg);
	for (i = 0; i < oFileSize; i++) {
		/*I/O����,�˴��кܴ���Ż��ռ�*/
		originalFile.read((char*)&c, 1);
		freqList[c]->weight++;
	}
}
int HuffmanEncoder::getMinFreqElemSeq() {
	BinTree* node = NULL;
	int i,j,k;
	int pos = 0;
	unsigned __int32 minWeight = 0xFFFFFFFF;
	unsigned __int32 curWeight = 0;
	for (j = 0,k = 0; j < 256;j++) {
		if (freqList[j]->weight != 0) {
			k = 1;
			break;
		}
	}
	if (k == 0) return -1;
	for (i = 0; i < 256; i++) {
		curWeight = freqList[i]->weight;
		if (curWeight != 0 && minWeight > curWeight) {
			minWeight = curWeight;
			pos = i;
		}
	}
	return pos;
}
HuffmanEncoder::HuffmanEncoder() {

}

void HuffmanEncoder::build(char* c1, char* c2) {
	originalFilePath.append(c1);
	zippedFilePath.append(c2);
	init();
}

void HuffmanEncoder::init() {

	status = 0;
	memset(freqList, 0, 256 * sizeof(BinTree*));
	memset(prefixCode, 0, 256);
	memset(&zipFileHeadTag, 0, sizeof(ZIPFileInfo));
	int i;
	for (i = 0; i < 256;i++) {
		freqList[i] = new BinTree;
		prefixCode[i] = new char[256];
		memset(freqList[i], 0, sizeof(BinTree));
		memset(prefixCode[i], 0xFF, 256);
		freqList[i]->data = i;
	}
	if (openFile() != 0) {
		status = -1;
		return;
	}

	{
		/*�ļ���ǩ,��"WT"�������ַ�*/
		zipFileHeadTag.zipTag = 0x5457;
		/*�ļ��汾,��ǰΪ1.00*/
		zipFileHeadTag.zipVer = 0x0001;
		/*ԭ�ļ���С*/
		zipFileHeadTag.oFileSize = oFileSize;
		/*�����ֵ��¼��С*/
		zipFileHeadTag.cDictItemSize = sizeof(BinTreeTable);
		/*�ֵ�λ��ƫ����*/
		zipFileHeadTag.cDictPosi = sizeof(ZIPFileInfo);
	}
}
int HuffmanEncoder::generateHFMTree() {
	/*���ڼ�¼Ƶ�ʱ�����С��Ȩֵ�Լ���С��Ȩֵ��˳��*/
	int min1, min2;
	/*BinTreeָ��,��Ž��������е���ؽ���ַ*/
	BinTree* node1, *node2, *temp, *newNode;
	while (true) {
		/*��ȡ����Ȩ�ؼ�Ƶ����С���ַ���˳��ASCII��*/
		min1 = getMinFreqElemSeq();
		/*�б���Ӧ��������һ��Ȩ�ط�0��Ԫ��,���û��,˵������*/
		if (min1 == -1) {
			errMsgDisplay("���������ִ���!\n");
			/*����ʧ��*/
			return -1;
		}
		/*����Ϊһ��ȫ�µĽ��*/
		node1 = copyNode(freqList[min1]);
		/*ͬʱ�������ƹ��Ľ��״̬��Ϊ�Ѵ���*/
		freqList[min1]->weight = 0;
		/*��ȡ�±���Ȩ�ؼ�Ƶ����С���ַ���˳��ASCII��*/
		min2 = getMinFreqElemSeq();
		/*��min2Ϊ-1ʱ,���������*/
		if (min2 == -1) {
			/*��ǰ����ʵ����hfmTree��ֵΪ�������һ��Ȩֵ��0Ԫ��*/
			hfmTree = node1;
			/*���ɳɹ�*/
			return 0;
		}
		/*���±���Ȩֵ��С�Ľ�㸳ֵ����ʱָ�����*/
		temp = freqList[min2];
		/*Ϊnode2�����µ��ڴ�ռ�*/
		node2 = copyNode(temp);
		/*��ԭ���б����ƹ��Ľ����Ϊ��Ч״̬*/
		freqList[min2]->weight = 0;
		/*ΪnewNode�����µ��ڴ�ռ�,newNode�����洢�����ӽڵ�ϲ����½��ĵ�ַ*/
		newNode = getNewNode();
		/*ԭ����Ȩ����С�Ľ���˫�׽�㸳ֵΪnewNode*/
		node1->par = newNode;
		/*ԭ����Ȩ�ش�С�Ľ���˫�׽�㸳ֵΪnewNode*/
		node2->par = newNode;
		/*newNode��������ֵΪԭ����Ȩ����С�Ľ��*/
		newNode->left = node1;
		/*newNode��������ֵΪԭ����Ȩ�ش�С�Ľ��*/
		newNode->right = node2;
		/*�����½��Ȩ��Ϊ������С����Ȩ��֮��*/
		newNode->weight = node1->weight + node2->weight;
		/*ɾ����ʱ���,�����ڴ�ռ�*/
		delete temp;
		/*���½�����ڱ�ɾ���Ĵ�С���ԭλ����*/
		freqList[min2] = newNode;
	}
	return -2;
}

void HuffmanEncoder::writePrefixCodeTable(BinTree* node, BinTree* par, char* temp) {
	if (node == NULL || temp == NULL) return;
	if (par != NULL) {
		__int32 seq = (node->depth) - 2;
		if (node == par->left) {
			temp[seq] = 0;
		}
		else if(node == par->right){
			temp[seq] = 1;
		}
		else {
			errMsgDisplay("ERROR!\n");
			return;
		}
	}
	if (node->left == NULL && node->right == NULL) {
		memcpy(prefixCode[node->data], temp, node->depth-1);
		return;
	}
	writePrefixCodeTable(node->left, node, temp);
	writePrefixCodeTable(node->right, node, temp);
}

int HuffmanEncoder::generatePrefixCodeTable() {
	if (hfmTree->left == NULL && hfmTree->right == NULL && hfmTree->weight != 0) {
		return 0;
	}
	char temp[256];
	memset(temp, 0xFF, 256);
	writePrefixCodeTable(hfmTree, NULL, temp);
	return 0;
}

int HuffmanEncoder::getPrefixCodeLen(unsigned char* p) {
	for (int i = 0; i < 256; i++) {
		if (p[i] == 0xFF) {
			return i;
		}
	}
	return -1;
}

unsigned char* HuffmanEncoder::prefixCodeCat(unsigned char* des, unsigned char* src) {
	__int32 deslen = getPrefixCodeLen(des);
	__int32 srclen = getPrefixCodeLen(src);
	memcpy((des + deslen), src, srclen);
	return des;
}

unsigned char* HuffmanEncoder::trimPrefixCode(unsigned char* des, __int32 desLen, __int32 remain) {
	if (remain == 0) {
		memset(des, 0xFF, 256);
		return des;
	}
	memcpy(des, (des + desLen - remain), remain);
	memset(des + remain, 0xFF, 256 - remain);
	return des;
}

int HuffmanEncoder::writeByteStream() {
	/*��ǰ��ת����ԭ�ļ��ֽ���*/
	unsigned __int32 cLen = 0;
	/*��ǰ�������ѹ���ļ��еı�����������ֽ���*/
	unsigned __int32 eLen = 0;
	/*ͬ�������ʱ��������Ϊ����*/
	unsigned char tmp[256];
	/*��ʱ��������Ч���볤��,��Ч����Ϊ0xFF,��Ч����Ϊ0����1*/
	__int32 tmplen;
	/*����ʱ��������ת���Ķ�������bit��*/
	__int32 pLen;
	/*��������ѭ���ĸ�������*/
	int i, j;
	/*������Ŷ�����ַ��Լ�Ҫд����ַ�*/
	unsigned char a = 0, b = 0;
	/*��������ʱ�������ƶ���ָ��*/
	unsigned char* p = tmp;
	/*�����ַ�ָ��*/
	char* q = (char*)&b;
	char* r = (char*)&a;
	/*��ʼ����ʱ�����ڴ�����Ϊ0xFF*/
	memset(tmp, 0xFF, 256);
	/*���ԭ�ļ��쳣״̬,�ָ�����״̬*/
	originalFile.clear();
	/*��λ�ļ�ָ�뵽�ļ���ͷ*/
	originalFile.seekg(0, ios::beg);
	/*������ѹ���ļ��ļ��쳣״̬,�ָ�����״̬*/
	zippedFile.clear();
	/*��λ�ļ�ָ�뵽�����ļ���ͷƫ����Ϊ����ƫ��λ��*/
	zippedFile.seekp(zipFileHeadTag.cDataPosi, ios::beg);
	/*����ֻ��һ���ַ����ļ�,ֱ����ת��д�ļ�ĩβ�����*/
	if (hfmTree->left == NULL && hfmTree->right == NULL && hfmTree->weight != 0)
		goto label;
	/*����ȡ��ԭ�ļ��ֽ���С��ԭ�ļ���Сʱѭ��*/
	while (cLen < oFileSize) {
		/*����ȡ��ԭ�ļ��ֽ���С��ԭ�ļ���С����ʱ������Ч����С��8ʱѭ��*/
		while (cLen < oFileSize && getPrefixCodeLen(tmp) < 8) {
			/*����һ���ֽڵ�����qָ����ָ����a��,I/O��ʱ�Ƚ�����,�������Ż��Ŀռ�*/
			originalFile.read(q, 1);
			/*���ַ�a��Ӧ��ǰ׺��ƴ�ӵ���ʱ������*/
			prefixCodeCat(tmp, (unsigned char*)prefixCode[b]);
			/*�Ѷ���ԭ�ļ��ֽ�����һ*/
			cLen++;
		}
		/*��ȡ��ʱ������Ч����*/
		tmplen = getPrefixCodeLen(tmp);
		/*����ָ��pָ����ʱ����ͷ��*/
		p = tmp;
		/*��������p��ֵΪ��ʱ���鳤��*/
		pLen = tmplen;
		/*��ʱ������δ�����ǰ׺����������7ʱѭ��*/
		while (pLen > 7) {
			/*��ʼ��aΪ0*/
			a = 0;
			/*��ȡ��ʱ������ǰ8λ��������,ת��Ϊһ���ֽ�,����ʹ��λ�������Ч��*/
			for (i = 7,j = 0; i > -1; i--,j++) {
				/*ʹ��λ����,ÿ8bitת��Ϊһ��char*/
				a += (p[i] << j);
			}
			/*��ת����������ֽ�д��ѹ���ļ���,I/O��ʱ�Ƚ�����,�������Ż��Ŀռ�*/
			zippedFile.write(r, 1);
			/*�ѱ������ݳ��ȼ�һ*/
			eLen++;
			/*��ʱ������δ����Ķ����������ȼ�8*/
			pLen -= 8;
			/*ָ��p����ƶ�8���ֽ�*/
			p += 8;
		}
		/*������ʱ����,ʹ��ʣ��δת���Ķ�������ת�Ƶ�ͷ��*/
		trimPrefixCode(tmp, tmplen, pLen);
	}
	/*����b��ֵΪ0*/
	b = 0;
	/*������󼸸�bit,����8bit��ʣ�ಿ����Ϊ0*/
	if (pLen != 0) {
		a = 0;
		b = pLen;
		p = tmp;
		while (pLen > 0) {
			pLen--;
			a += (p[pLen] << (7 - pLen));
		}
		zippedFile.write(r, 1);
		eLen++;
	}
	label:
	/*д���ļ����һ���ֽ�,���ֽڱ�ʾ����õĶ�����������8bit��ʣ�ಿ��bit��С,��Χ0~7*/
	zippedFile.write(q, 1);
	/*ѹ��������ݴ�С*/
	cDataSize = eLen;


	return 0;
}

BinTree* HuffmanEncoder::copyNode(BinTree* node) {
	if (node == NULL) return NULL;
	BinTree* p = new BinTree;
	memcpy(p, node, sizeof(BinTree));
	return p;
}
int HuffmanEncoder::encode() {
	if (status != 0) {
		errMsgDisplay("����������,�����˳�...\n");
		return -1;
	}
	generateFreqList();
	if (generateHFMTree() != 0) {
		errMsgDisplay(__FUNCTION__);
	}
	else {
		//cout << "success" << endl << getBinTreeNodeNum(hfmTree) << endl << getBinTreeLeavesNum(hfmTree) << endl;
		setBinTreeDepth(hfmTree, 1);
		BinTreeTable* table = convertTreeToTable(hfmTree);
		BinTree* temp = convertTableToTree(table);
		//cout << compareTree(hfmTree, temp);
		__int16 itemNum = getBinTreeNodeNum(table, 0);

		{
			zipFileHeadTag.cDictItemNumb = itemNum;
			zipFileHeadTag.cDictSize = itemNum * zipFileHeadTag.cDictItemSize;
			zipFileHeadTag.cDataPosi = sizeof(ZIPFileInfo) + zipFileHeadTag.cDictSize;
		}

		{
			zippedFile.clear();
			zippedFile.seekp(0, ios::beg);
			zippedFile.write((char*)&zipFileHeadTag, sizeof(ZIPFileInfo));
			zippedFile.write((char*)table, zipFileHeadTag.cDictSize);
		}
		
		generatePrefixCodeTable();
		writeByteStream();
		zipFileHeadTag.cDataSize = cDataSize;
		zippedFile.clear();
		zippedFile.seekp(0, ios::end);
		unsigned __int32 cFileSize = zippedFile.tellp();
		zippedFile.seekp(((char*)&zipFileHeadTag.cFileSize - (char*)&zipFileHeadTag), ios::beg);
		zippedFile.write((char*)&cFileSize, sizeof(cDataSize));
		zippedFile.write((char*)&cDataSize, sizeof(cDataSize));


		{
			char a;
			originalFile.clear();
			originalFile.seekg(-1, ios::end);
			originalFile.read(&a, 1);
			zippedFile.clear();
			zippedFile.seekp(sizeof(ZIPFileInfo) + sizeof(BinTreeTable) - 1, ios::beg);
			zippedFile.write(&a, 1);
		}

		zippedFile.close();
		originalFile.close();
		if(status == 0)
			cout << "����ɹ�,ѹ���ļ�·��Ϊ:" << zippedFilePath << endl << "ѹ����Ϊ: " << 100.0 * cFileSize / zipFileHeadTag.oFileSize << '%' << endl << "���س�������\n";
		cin.get();
	}

	return 0;
}