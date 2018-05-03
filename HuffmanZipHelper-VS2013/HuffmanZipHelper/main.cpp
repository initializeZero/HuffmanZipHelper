/*
 * Author: initializeZero CopyRight 2016~2020
 * 本项目遵循GPL 2.0 共享协议
 * 可以使用于非商业用途
 * 请勿用于商业用途,保留一切追究法律责任的权利
 * md5(Email)=c1e156dd78b2126ea4b616c74aec9294
 */

#include "commen.h"
#include "HuffmanEncoder.h"
#include "HuffmanDecoder.h"

int main() {
	//文件最好不要大于5MB
	string s = "D:\\5.bmp";

	string s1 = s + "-zipped";
	string s2 = s1 + "-unzipped";

	HuffmanEncoder mEncoder;
	mEncoder.build((char*)s.c_str(), (char*)s1.c_str());
	mEncoder.encode();
	
	HuffmanDecoder mDecoder;
	mDecoder.build((char*)s1.c_str(), (char*)s2.c_str());
	mDecoder.decode();
	
	return 0;
}