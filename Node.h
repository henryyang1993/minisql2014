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

//����ڵ�ṹ����������
template <class TKeyType>
class Node
{
	bool m_bLeaf;                //��ʾ�Ƿ�ΪҶ�ӽڵ�
	int m_iCount;                //��ʾ�Ѿ�����ڵ��е�key��
	TKeyType* m_Key[FANOUT-1];   //��ʾ�Ѵ���key��ֵ     //ָ������ָ����һ��С����==ÿ��key����һ������
                                 //Ϊ��֧��char���飿
	int NodeSize;                //��ʾÿһ���ڵ�Ĵ�С
	int m_iPointer[FANOUT][2];   //�ڵ���ָ�� //��ʾ�ڵ�ĸ����ӽڵ��ַ //Ҷ�ڵ�pointer��ָ����һ��Ҷ�ڵ�  
	int m_iFatherPosition[2];    //��ʾ�ڵ�ĸ��ڵ��ַ  //��ַ���ã�block��offset)��ʾ,������int
	  //blockֻ=-1��-2     //-1��ʾ��inti������û�д����ļ����������ã����߲�����  -2��AddNode֮���ֵ,��ʾ���� 
	int m_iSelfPosition[2];      //��ʾ�ڵ��Լ��ĵ�ַ    
	int m_iKeyTypeCount;         //ÿ��keyֵ�ķֿ���    //ÿ��key�����Ԫ�أ�Type������
	CFile* m_file;               //��ʾ�ڵ����ڵ��ļ�
	BTree* m_BTree;              //��ʾ�ڵ����ڵ���

public:
	Node(int TypeCount, BTree* Tree);
	~Node();
	bool IsLeaf(){return m_bLeaf;}	        //�ж��Ƿ�ΪҶ�ӽڵ�
	int GetCount(){return m_iCount;}        //��ýڵ��еĴ���key��Ŀ
	TKeyType* GetKey(int Index);            //��ýڵ㴢���keyֵ
 	void SetKey(int Index, TKeyType* pKey);	//���ýڵ㴢���keyֵ

	void GetPointer(int Index, int& PositionBlock, int& PositionOffset);  //��ýڵ��ӽڵ�ĵ�ַ
	void GetFatherPosition(int& Block, int& Offset)                       //��ýڵ㸸�ڵ�ĵ�ַ
	{
		Block = m_iFatherPosition[0];
		Offset = m_iFatherPosition[1];
	}
	void GetSelfPosition(int& Block, int& Offset)                         //��ýڵ�����ĵ�ַ
	{
		Block = m_iSelfPosition[0];
		Offset = m_iSelfPosition[1];
	}
	void GetFrontNodePosition(int& Block, int& Offset);                   //��ýڵ�ǰһ���ڵ�ĵ�ַ
	bool GetEqualLevelNodePosition(int& Block, int& Offset);  //���ӵ����ͬ���ڵ��ǰ���һ���ڵ�ĵ�ַ   //bool?
	int GetFatherLinkToMeIndex();                                         //��ø��ڵ��и��ӽڵ��indexֵ
	int FindGreaterKeyIndex(TKeyType* pValue);                //Ѱ�ҽڵ��д洢�Ÿ���keyֵ��indexֵ    //?
	int FindKeyIndex(TKeyType* pValue);                       //Ѱ�ҽڵ��д洢����ͬkeyֵ��indexֵ    //?


	void SetLeaf(bool Leaf){m_bLeaf = Leaf;};                             //���ýڵ��Ƿ�ΪҶ�ӽڵ�
	void SetCount(int count){m_iCount = count;}	                          //���ýڵ��д����keyֵ��Ŀ
	void SetFileOrganize(CFile* file){m_file = file;}	           //�涨�ڵ�����ĸ��ļ�    
	void SetPointer(int Index, int PositionBlock, int PositionOffset); //���ýڵ�ÿ��indexָ����ӽڵ��ַ
	void SetFatherPosition(int Block, int Offset)                         //���ø��ڵ��ַ
	{	
		m_iFatherPosition[0] = Block;
		m_iFatherPosition[1] = Offset;
	}
	void SetSelfPosition(int Block, int Offset)                           //���ýڵ�����ĵ�ַ
	{	
		m_iSelfPosition[0] = Block;
		m_iSelfPosition[1] = Offset;
	}
	void Read();	          //��ȡ�ڵ���Ϣ    //���ļ�+����λ�ö�ȡ    //��Ҫclass��������λ��
	char* OrganizeNode();     //��֯�ڵ���Ϣ   //���ڵ���Ϣת��Ϊ�ɴ����ļ�����Ϣ			
 
	void InsertKey(TKeyType* pValue, int Block, int Offset, bool AutoSave=1); //����keyֵ  
	                                                        //��AutoSave=0����ΪAutoSave=1  
	void DeleteKey(int Index,bool AutoSave=1);	 //AutoSave��һ�����津����  	//ɾ��keyֵ  
	void AddNode();	 		  //����µĽڵ�    //�Ƚ����࣬��������������ڵ���Ϣд���ļ���λ
	void FreeNode();	      //ɾ���ڵ�������key
	void Drop();		      //ɾ���ڵ�										
	void UpdateNode();		  //���½ڵ�   //��organize�õĽڵ���Ϣд���ļ�												
	void ChangeValue(TKeyType* pSrcValue, TKeyType* pNowValue);	  //���Ľڵ��е�keyֵ
};

#endif // __Node_H__