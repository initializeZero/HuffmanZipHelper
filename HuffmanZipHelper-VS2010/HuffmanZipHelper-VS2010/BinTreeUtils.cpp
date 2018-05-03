/*
* Author: initializeZero CopyRight 2016~2020
* ����Ŀ��ѭGPL 2.0 ����Э��
* ����ʹ���ڷ���ҵ��;
* ����������ҵ��;,����һ��׷���������ε�Ȩ��
* md5(Email)=c1e156dd78b2126ea4b616c74aec9294
*/


#include "BinTreeUtils.h"

/*�ݹ������ȡ������(��̬)�������*/
__int16 getBinTreeNodeNum(BinTree* tree) {
	if (tree == NULL) return 0;
	return getBinTreeNodeNum(tree->left) + getBinTreeNodeNum(tree->right) + 1;
}

/*�ݹ������ȡ������(��̬)��Ҷ����*/
__int16 getBinTreeLeavesNum(BinTree* tree) {
	if (tree == NULL) return 0;
	if (tree->left == NULL && tree->right == NULL) return 1;
	return getBinTreeLeavesNum(tree->left) + getBinTreeLeavesNum(tree->right);
}

/*�ݹ������ȡ������(��̬��)�������*/
__int16 getBinTreeNodeNum(BinTreeTable* table, __int16 seq) {
	if (seq == -1 || table == NULL) return 0;
	return 1 + getBinTreeNodeNum(table, table[seq].left) + getBinTreeNodeNum(table, table[seq].right);
}

/*�ݹ������ȡ������(��̬��)��Ҷ����*/
__int16 getBinTreeLeavesNum(BinTreeTable* table, __int16 seq) {
	if (seq == -1 || table == NULL) return 0;
	if (table[seq].left == -1 && table[seq].right == -1) return 1;
	return getBinTreeLeavesNum(table, table[seq].left) + getBinTreeLeavesNum(table, table[seq].right);
}

/*�ݹ����������(��̬)������ݲ�д�������(��̬��)���*/
void writeTable(BinTreeTable* table, BinTree* tree, __int16& seq, __int16 par) {
	if (table == NULL || tree == NULL) return;
	__int16 cur = seq;
	table[cur].data = tree->data;
	table[cur].weight = tree->weight;
	table[cur].depth = tree->depth;
	table[cur].par = par;
	if (tree->left != NULL) {
		seq++;
		table[cur].left = seq;
		writeTable(table, tree->left, seq, cur);
	}
	else {
		table[cur].left = -1;
	}
	if (tree->right != NULL) {
		seq++;
		table[cur].right = seq;
		writeTable(table, tree->right, seq, cur);
	}
	else {
		table[cur].right = -1;
	}
}

/*ת����̬������Ϊ��̬�����������������Ԫ�ص�ַ*/
BinTreeTable* convertTreeToTable(BinTree* tree) {
	if (tree == NULL) return NULL;
	__int16 num = getBinTreeNodeNum(tree);
	BinTreeTable* table = new BinTreeTable[num];
	__int16 i = 0;
	writeTable(table, tree, i, -1);
	return table;
}

/*�ݹ����������(��̬��)������ݲ�д�������(��̬)���*/
void writeTree(BinTree* tree, BinTreeTable* table, __int16 seq, BinTree* par) {
	if (tree == NULL || table == NULL) return;
	tree->data = table[seq].data;
	tree->depth = table[seq].depth;
	tree->weight = table[seq].weight;
	tree->par = par;
	//if()
	if (table[seq].left != -1) {
		tree->left = getNewNode();
		writeTree(tree->left, table, table[seq].left, tree);
	}
	else {
		tree->left = NULL;
	}
	if (table[seq].right != -1) {
		tree->right = getNewNode();
		writeTree(tree->right, table, table[seq].right, tree);
	}
	else {
		tree->right = NULL;
	}
}

/*ת����̬�������Ϊ��̬�������������������׵�ַ*/
BinTree* convertTableToTree(BinTreeTable* table) {
	if (table == NULL) return NULL;
	BinTree* tree = getNewNode();
	writeTree(tree, table, 0, NULL);
	return tree;
}

/*��ȡ�½��,�����ڴ�ռ䲢�������ַ*/
BinTree* getNewNode() {
	BinTree* p = new BinTree;
	memset(p, 0, sizeof(BinTree));
	return p;
}

/*˳�������ȡ��̬������(���)���*/
__int16 getBinTreeDepth(BinTree* tree) {
	if (tree == NULL) return 0;
	BinTreeTable* table = convertTreeToTable(tree);
	__int16 max = getBinTreeDepth(table);
	delete[] table;
	return max;
}

/*˳�������ȡ��̬�������(���)���*/
__int16 getBinTreeDepth(BinTreeTable* table) {
	if (table == NULL) return 0;
	__int16 s = getBinTreeNodeNum(table, 0);
	if (s < 3) return s;
	__int16 max = 0;
	for (int i = 0; i < s; i++) {
		max = table[i].depth > max ? table[i].depth : max;
	}
	return max;
}

/*�ݹ�������ö�̬���������*/
void setBinTreeDepth(BinTree* tree, __int16 dep) {
	if (tree == NULL) return;
	tree->depth = dep;
	setBinTreeDepth(tree->left, dep + 1);
	setBinTreeDepth(tree->right, dep + 1);
}

/*�ݹ�������þ�̬����������*/
void setBinTreeDepth(BinTreeTable* table, __int16 seq, __int16 dep) {
	if (seq == -1 || table == NULL) return;
	table[seq].depth = dep;
	if (table[seq].left != -1) {
		setBinTreeDepth(table, table[seq].left, dep + 1);
	}
	if (table[seq].right != -1) {
		setBinTreeDepth(table, table[seq].right, dep + 1);
	}
}

/*�Ƚ����ö������Ƿ��ϸ���ͬ,��ͬ����1,��ͬ����-1,����+1*/
__int16 compareTree(BinTree* tree1, BinTree* tree2) {
	if (((__int32)tree1 ^ (__int32)tree2) == 0) return 1;
	BinTreeTable* t1 = convertTreeToTable(tree1);
	BinTreeTable* t2 = convertTreeToTable(tree2);
	__int16 r = compareTree(t1, t2);
	delete[] t1;
	delete[] t2;
	return r;
}

/*�Ƚ����ö������Ƿ��ϸ���ͬ,��ͬ����1,��ͬ����-1,����+2*/
__int16 compareTree(BinTree* tree1, BinTreeTable* table2) {
	BinTreeTable* t1 = convertTreeToTable(tree1);
	__int16 r = compareTree(t1, table2);
	delete[] t1;
	return r;
}

/*�Ƚ����ö������Ƿ��ϸ���ͬ,��ͬ����1,��ͬ����-1,����+3*/
__int16 compareTree(BinTreeTable* table1, BinTree* tree2) {
	BinTreeTable* t2 = convertTreeToTable(tree2);
	__int16 r = compareTree(table1, t2);
	delete[] t2;
	return r;
}

/*�Ƚ����ö������Ƿ��ϸ���ͬ,��ͬ����1,��ͬ����-1,����+4*/
__int16 compareTree(BinTreeTable* table1, BinTreeTable* table2) {
	if (((__int32)table1 ^ (__int32)table2) == 0) return 1;
	__int16 s1 = getBinTreeNodeNum(table1, 0);
	__int16 s2 = getBinTreeNodeNum(table2, 0);
	if (s1 != s2 || 0 != memcmp(table1, table2, s1 * sizeof(BinTreeTable))) {
		return -1;
	}
	return 1;
}

/*�ݹ���������ٶ�̬������*/
__int16 destoryBinTree(BinTree* tree) {
	if (tree == NULL) return 1;
	BinTree* left = tree->left, *right = tree->right;
	delete tree;
	destoryBinTree(left);
	destoryBinTree(right);
	return 0;
}

/*˳��������پ�̬�������*/
__int16 destoryBinTree(BinTreeTable* &table) {
	delete[] table;
	table = NULL;
	return 0;
}