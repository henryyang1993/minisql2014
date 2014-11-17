#ifndef __BPtree_H__
#define __BPtree_H__

#include"Node.h"


#include"stdafx.h"
#include<afx.h>

#include <string>
using namespace std;


//树类
class BTree : public CObject
{
	CString m_sKeyType;		  //树中key值的类型				
	int m_iKeyTypeCount;	  //每个key值的分块数	//每个key数组的元素（Type）个数	
	int m_iRoot;			  //根节点地址		//一个int就够了？offset？
	int m_iFirstLeaf;         //首叶子节点地址
	int TreeSize;             //树的大小
	int MaxOffset;            //文件大小      //offset决定文件大小
	int EmptyOffset;          //第一个空闲段的地址
	CObArray m_aReaders;	  //？
	CFile* m_file;			  //树所在的文件指针	
public:
	BTree(CString KeyType, int KeyTypeCount);  //B树的构造函数
	~BTree();
	void CreateBPlusTree();                           //创建B树
	void FindEmptyOffset(int& Offset, int NodeSize);  //寻找文件中的空余位置
	void AddEmptyOffset(int Offset, int NodeSize);    //删除节点后标记空余位置
	
	void SetFileOrganize(CFile* file)   //设置B树信息
	{
		m_file = file;
	}

	void GetRoot(int &Block,int& Offset)    //获得根节点地址
	{
		Block = -2;       //？
		Offset = m_iRoot;
	}

	int GetSize()                //获得树的大小
	{
		return TreeSize;
	}


	bool FindValue(void* pValue, int& Block, int& Offset);  //查找B树中key值相同的key对应的pointer所指向的地址


	
	void SetRoot(int Block,int Offset)            //设置根节点地址
	{
		m_iRoot = Offset;
	}

	void SetFirstLeaf(int Block,int Offset)       //设置首叶子节点地址
	{
		m_iFirstLeaf = Offset;
	}

	void Read();                 //读取B树的信息

	void InsertValue(void* pValue, int Block, int Offset);  //向B树中插入新的key以及其pointer所指地址
	void DeleteValue(void* pValue);                         //删除B树中的记录
	void DropNode(int Block, int Offset);                   //删除B树中的节点
	void Drop();                 //删除B树
	void UpdateBPlusTree();      //更新B树


	CFile *GetFile() { return m_file; } 
	int GetNodeSize()
	{
		return sizeof(m_sKeyType) * m_iKeyTypeCount * (FANOUT - 1) + sizeof(int) * (FANOUT + 1) * 2 + 1; 
	}

	int GetRoot() { return m_iRoot;} ;
};

#endif // __BPTree_H__