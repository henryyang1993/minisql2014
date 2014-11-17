#ifndef __Node_H__
#define __Node_H__
#define FANOUT 3

#include "BPTree.h"

//#include"basichead.h"
#include<afx.h>
#include"stdafx.h"
#include<iostream>
#include <string>

using namespace std;

class BTree;

//定义节点结构，各个参数
template <class TKeyType>
class Node
{
	bool m_bLeaf;                //表示是否为叶子节点
	int m_iCount;                //表示已经存入节点中的key数
	TKeyType* m_Key[FANOUT-1];   //表示已存入key的值     //指针数组指向另一个小数组==每个key都是一个数组
                                 //为了支持char数组？
	int NodeSize;                //表示每一个节点的大小
	int m_iPointer[FANOUT][2];   //节点中指针 //表示节点的各个子节点地址 //叶节点pointer都指向下一个叶节点  
	int m_iFatherPosition[2];    //表示节点的父节点地址  //地址都用（block，offset)表示,所以是int
	  //block只=-1或-2     //-1表示刚inti过（还没有存入文件过，不可用）或者不可用  -2是AddNode之后的值,表示可用 
	int m_iSelfPosition[2];      //表示节点自己的地址    
	int m_iKeyTypeCount;         //每个key值的分块数    //每个key数组的元素（Type）个数
	CFile* m_file;               //表示节点所在的文件
	BTree* m_BTree;              //表示节点所在的树

public:
	Node(int TypeCount, BTree* Tree);
	~Node();
	bool IsLeaf(){return m_bLeaf;}	        //判断是否为叶子节点
	int GetCount(){return m_iCount;}        //获得节点中的存入key数目
	TKeyType* GetKey(int Index);            //获得节点储存的key值
 	void SetKey(int Index, TKeyType* pKey);	//设置节点储存的key值

	void GetPointer(int Index, int& PositionBlock, int& PositionOffset);  //获得节点子节点的地址
	void GetFatherPosition(int& Block, int& Offset)                       //获得节点父节点的地址
	{
		Block = m_iFatherPosition[0];
		Offset = m_iFatherPosition[1];
	}
	void GetSelfPosition(int& Block, int& Offset)                         //获得节点自身的地址
	{
		Block = m_iSelfPosition[0];
		Offset = m_iSelfPosition[1];
	}
	void GetFrontNodePosition(int& Block, int& Offset);                   //获得节点前一个节点的地址
	bool GetEqualLevelNodePosition(int& Block, int& Offset);  //获得拥有相同父节点的前或后一个节点的地址   //bool?
	int GetFatherLinkToMeIndex();                                         //获得父节点中该子节点的index值
	int FindGreaterKeyIndex(TKeyType* pValue);                //寻找节点中存储着更大key值的index值    //?
	int FindKeyIndex(TKeyType* pValue);                       //寻找节点中存储着相同key值的index值    //?


	void SetLeaf(bool Leaf){m_bLeaf = Leaf;};                             //设置节点是否为叶子节点
	void SetCount(int count){m_iCount = count;}	                          //设置节点中储存的key值数目
	void SetFileOrganize(CFile* file){m_file = file;}	           //规定节点存在哪个文件    
	void SetPointer(int Index, int PositionBlock, int PositionOffset); //设置节点每个index指向的子节点地址
	void SetFatherPosition(int Block, int Offset)                         //设置父节点地址
	{	
		m_iFatherPosition[0] = Block;
		m_iFatherPosition[1] = Offset;
	}
	void SetSelfPosition(int Block, int Offset)                           //设置节点自身的地址
	{	
		m_iSelfPosition[0] = Block;
		m_iSelfPosition[1] = Offset;
	}
	void Read();	          //读取节点信息    //从文件+自身位置读取    //需要class中有自身位置
	char* OrganizeNode();     //组织节点信息   //将节点信息转化为可存入文件的信息			
 
	void InsertKey(TKeyType* pValue, int Block, int Offset, bool AutoSave=1); //插入key值  
	                                                        //从AutoSave=0？改为AutoSave=1  
	void DeleteKey(int Index,bool AutoSave=1);	 //AutoSave是一个储存触发？  	//删除key值  
	void AddNode();	 		  //添加新的节点    //先建个类，再用这个函数将节点信息写入文件空位
	void FreeNode();	      //删除节点中所有key
	void Drop();		      //删除节点										
	void UpdateNode();		  //更新节点   //将organize好的节点信息写入文件												
	void ChangeValue(TKeyType* pSrcValue, TKeyType* pNowValue);	  //更改节点中的key值
};

#endif // __Node_H__