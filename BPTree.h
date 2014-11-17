#ifndef __BPtree_H__
#define __BPtree_H__

#include"Node.h"


#include"stdafx.h"
#include<afx.h>

#include <string>
using namespace std;


//����
class BTree : public CObject
{
	CString m_sKeyType;		  //����keyֵ������				
	int m_iKeyTypeCount;	  //ÿ��keyֵ�ķֿ���	//ÿ��key�����Ԫ�أ�Type������	
	int m_iRoot;			  //���ڵ��ַ		//һ��int�͹��ˣ�offset��
	int m_iFirstLeaf;         //��Ҷ�ӽڵ��ַ
	int TreeSize;             //���Ĵ�С
	int MaxOffset;            //�ļ���С      //offset�����ļ���С
	int EmptyOffset;          //��һ�����жεĵ�ַ
	CObArray m_aReaders;	  //��
	CFile* m_file;			  //�����ڵ��ļ�ָ��	
public:
	BTree(CString KeyType, int KeyTypeCount);  //B���Ĺ��캯��
	~BTree();
	void CreateBPlusTree();                           //����B��
	void FindEmptyOffset(int& Offset, int NodeSize);  //Ѱ���ļ��еĿ���λ��
	void AddEmptyOffset(int Offset, int NodeSize);    //ɾ���ڵ���ǿ���λ��
	
	void SetFileOrganize(CFile* file)   //����B����Ϣ
	{
		m_file = file;
	}

	void GetRoot(int &Block,int& Offset)    //��ø��ڵ��ַ
	{
		Block = -2;       //��
		Offset = m_iRoot;
	}

	int GetSize()                //������Ĵ�С
	{
		return TreeSize;
	}


	bool FindValue(void* pValue, int& Block, int& Offset);  //����B����keyֵ��ͬ��key��Ӧ��pointer��ָ��ĵ�ַ


	
	void SetRoot(int Block,int Offset)            //���ø��ڵ��ַ
	{
		m_iRoot = Offset;
	}

	void SetFirstLeaf(int Block,int Offset)       //������Ҷ�ӽڵ��ַ
	{
		m_iFirstLeaf = Offset;
	}

	void Read();                 //��ȡB������Ϣ

	void InsertValue(void* pValue, int Block, int Offset);  //��B���в����µ�key�Լ���pointer��ָ��ַ
	void DeleteValue(void* pValue);                         //ɾ��B���еļ�¼
	void DropNode(int Block, int Offset);                   //ɾ��B���еĽڵ�
	void Drop();                 //ɾ��B��
	void UpdateBPlusTree();      //����B��


	CFile *GetFile() { return m_file; } 
	int GetNodeSize()
	{
		return sizeof(m_sKeyType) * m_iKeyTypeCount * (FANOUT - 1) + sizeof(int) * (FANOUT + 1) * 2 + 1; 
	}

	int GetRoot() { return m_iRoot;} ;
};

#endif // __BPTree_H__