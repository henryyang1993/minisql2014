#ifndef __bufferManager_h__
#define __bufferManager_h__
using namespace std;
#include<iostream>
#include<string>
#include<fstream> 
#include<vector>
#include"Minisql.h"

#define BLOCKSIZE 1000
#define MAXBLOCKNUMBER 1000

class buffer{
	friend class bufferManager;
	buffer();
public:
	string filename;
	int blockOffset;
	int LRU;
	char value[BLOCKSIZE+1];
	bool isValid;
	bool isWritten;
	
	
	void init();
	string getvalue(int start,int end);
	char getvalue(int pos);
};

class insertPos{
public:
	int bufferNUM;
	int position;
};






class bufferManager{
	friend class RecoedManager;
	friend class IndexManager;
	friend class Leaf;
	friend class Branch;
	friend class BPlusTree;

	bufferManager();
	~bufferManager();

private:
	buffer bufferBlock[MAXBLOCKNUMBER];
	void writeBack(int bufferNum);//��buffer�е�����д�ص��ļ���
	int getbufferNum(string filename,int blockOffset) ; //����ض��ļ����ض�����buffer�е����,���û�оͽ��鵼��buffer
	int getIfIsInBuffer(string filename ,int blockOffset); //����ض��ļ����ض�����buffer�е����,���û�оͷ���-1
	void readBlock(string filename,int blockOffset,int bufferNum); //��ȡ�ض��鵽buffer[bufferNum]��
	void writeBlock(int bufferNum);//���buffer[bufferNum]��д��λ��ͬʱ����LRU
	void LRU(int bufferNum);//LRU
	int getEmptyBuffer(); //�õ�һ���յ�buffer����ţ����buffer���˰���LRU�����滻��
	int getEmptyBufferExcept(string filename); //�õ�һ���յ�buffer�����(�滻��ʱ���滻�����ض�filename���ݵ�buffer
	insertPos getInsertPosition(Table& fileinformation);//  �������ĳ�ļ�����д�룬��������������д��λ��
	int addBlockInFile(Table& fileinformation);//  Ϊһ���ļ���buffer���¿���һ���飨Ϊ��д�룩
	int addBlockInFile(Table& indexinfor) ; //Ϊһ��index�ļ���buffer���¿���һ���飨Ϊ��д�룩
	void scanIn(Table fil); //�������ļ�����buffer��
	void setInvalid(string filename); //����ļ���ɾ�����������������ؿ���Ϊinvalid
};




#endif