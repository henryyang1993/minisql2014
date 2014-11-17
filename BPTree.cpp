#include "Node.h"
#include "Node.cpp"
#include "BPTree.h"



using namespace std;



//���캯��
BTree::BTree(CString KeyType, int KeyTypeCount)
{
	m_sKeyType = KeyType;           //����keyֵ����
	m_iKeyTypeCount = KeyTypeCount; //����keyֵ�Ĵ�С
	TreeSize = sizeof(int) * 4;     //�ʹ�����4��int�͹��ˣ�

	m_iRoot = -1;        //��ʼ����������
	m_iFirstLeaf = -1;

	MaxOffset = -1;
	EmptyOffset = -1;

	m_file = NULL;
}


//����B��
void BTree::CreateBPlusTree()
{
	int tmp;
	if(m_file == NULL)       //�ļ�Ϊ��
	{cout <<"init before create b+tree"<<endl; return;}

	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0)
	{cout <<"error input for create B+ Tree"<<endl; return;}

	if(m_sKeyType=="char") {                            //��key������Ϊcharʱ							
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);  //����keyֵ����Ϊchar����ʱ�ڵ�   //+1�ǿ���'\0'
		tmpNode.SetFileOrganize(m_file);                //���øõ�ĸ���������Ϣ
		tmpNode.SetCount(0);
		tmpNode.SetLeaf(true);
		tmpNode.AddNode();
		tmpNode.GetSelfPosition(tmp, m_iRoot);
	}
	else if(m_sKeyType=="int") {                   //��key������Ϊintʱ
		Node<int> tmpNode(m_iKeyTypeCount, this);  //����keyֵ����Ϊint����ʱ�ڵ�
		tmpNode.SetFileOrganize(m_file);           //���øõ�ĸ���������Ϣ
		tmpNode.SetCount(0);
		tmpNode.SetLeaf(1);
		tmpNode.AddNode();
		tmpNode.GetSelfPosition(tmp, m_iRoot);
	}
	else if(m_sKeyType=="float") {                   //��key������Ϊfloatʱ
		Node<float> tmpNode(m_iKeyTypeCount, this);  //����keyֵ����Ϊfloat����ʱ�ڵ�
		tmpNode.SetFileOrganize(m_file);             //���øõ�ĸ���������Ϣ
		tmpNode.SetCount(0);
		tmpNode.SetLeaf(1);
		tmpNode.AddNode();
		tmpNode.GetSelfPosition(tmp, m_iRoot);
	}
	
	m_iFirstLeaf=m_iRoot;       //����B���е�һ����Ϊ����ʱ�ڵ�

	int* c = new int[TreeSize/4];
	int* p = c;

	p[0] = m_iRoot;      //����B����Ϣ
	
	p[1] = m_iFirstLeaf;
	
	p[2] = EmptyOffset;

	p[3] = MaxOffset;
	
	m_file->SeekToBegin();
	m_file->Write(c, TreeSize);  //д���ļ�
	delete[] c;
}


//����B��
void BTree::UpdateBPlusTree()	
{
	m_file->SeekToBegin();               //���ļ����ݸ�����������
	m_file->Write(&m_iRoot,sizeof(int));
	m_file->Write(&m_iFirstLeaf, sizeof(int));
	m_file->Write(&EmptyOffset, sizeof(int));
	m_file->Write(&MaxOffset, sizeof(int));
}


//��ȡB������Ϣ
void BTree::Read()
{	
	if(m_file == NULL)             //�ļ�Ϊ��
	{cout <<"error now store in b+tree"<<endl; return;}

	int* c = new int[TreeSize/4];
	int* p = c;
	m_file->Read(c, TreeSize);	   //��ȡ�ļ���Ϣ

	m_iRoot = p[0];           //��������ֵ
	
	m_iFirstLeaf = p[1];

	EmptyOffset = p[2];

	MaxOffset = p[3];

	delete[] c;
}






//��B���в����µ�key�Լ���pointer��ָ��ַ
void BTree::InsertValue(void* pValue, int Block, int Offset)
{
	if(m_iRoot < 0 || m_file == NULL)       //���ڵ㲻���ڻ��ļ�Ϊ��
	{cout <<"no root when find value in B+tree"<<endl; return;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
	{cout <<"error input for find value in b+"<<endl; return;}
	if(m_sKeyType == "char") {                    //��key������Ϊcharʱ
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {          //��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((char*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();			//Ѱ�Һ��ʵĵص�
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);      //ͨ��ָ�뵽��Ҷ��
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);	
			tmpNode.Read();
		}
		tmpNode.InsertKey((char*)pValue, Block, Offset,1);	 //����Ҷ�ӽڵ�ʱ�����¼
	}
	else if(m_sKeyType == "int") {                   //��key������Ϊintʱ
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot); 
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {          //��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((int*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//Ѱ�Һ��ʵĵص�
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.InsertKey((int*)pValue, Block, Offset, 1);	 //����Ҷ�ӽڵ�ʱ�����¼
	}
	else if(m_sKeyType == "float") {                    //��key������Ϊfloatʱ
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {          //��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((float*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//Ѱ�Һ��ʵĵص�
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.InsertKey((float*)pValue, Block, Offset, 1);	 //����Ҷ�ӽڵ�ʱ�����¼
	}

}



//ɾ��B���еļ�¼
void BTree::DeleteValue(void* pValue)
{
	if(m_iRoot < 0 || m_file == NULL)       //���ڵ㲻���ڻ��ļ�Ϊ��
	{cout <<"no root when find value in B+tree"<<endl; return;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
	{cout <<"error input for find value in b+"<<endl; return;}
	if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {			  //��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((char*)pValue);
			if(FindIndex == -1)										
				FindIndex = tmpNode.GetCount();			//Ѱ�Һ��ʵĵص�					
			tmpNode.GetPointer(FindIndex,tmpBlock, tmpOffset);	
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);		
			tmpNode.Read();
		}
		tmpNode.DeleteKey(tmpNode.FindKeyIndex((char*)pValue));	 //����Ҷ�ӽڵ�ʱɾ����¼	
	}
	else if(m_sKeyType == "int") {                    //��key������Ϊintʱ
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {			  //��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((int*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//Ѱ�Һ��ʵĵص�
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.DeleteKey(tmpNode.FindKeyIndex((int*)pValue));	 //����Ҷ�ӽڵ�ʱɾ����¼
	}
	else if(m_sKeyType == "float") {                   //��key������Ϊfloatʱ
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {			  //��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((float*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//Ѱ�Һ��ʵĵص�
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.DeleteKey(tmpNode.FindKeyIndex((float*)pValue));	 //����Ҷ�ӽڵ�ʱɾ����¼
	}
}



//ɾ��B���еĽڵ�
void BTree::DropNode(int Block, int Offset)			
{
	if(m_iRoot < 0 || m_file == NULL)       //���ڵ㲻���ڻ��ļ�Ϊ��
	{cout <<"no root when find value in B+tree"<<endl; return;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
	{cout <<"error input for find value in b+"<<endl; return;}
	if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(Block, Offset);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		if(tmpNode.IsLeaf())		//Ҷ�ӽڵ�ʱ
			tmpNode.FreeNode();		//ֱ��ɾ���ڵ�			
		else {						//��Ҷ�ӽڵ�ʱ						
			for(int i = 0; i < tmpNode.GetCount(); ++i) {   //����λ��
				tmpNode.GetPointer(i, tmpBlock, tmpOffset);
				DropNode(tmpBlock, tmpOffset);
			}
			tmpNode.FreeNode();		//ɾ���ڵ�								
		}
	}
	else if(m_sKeyType == "int") {                   //��key������Ϊintʱ
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(Block, Offset);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		if(tmpNode.IsLeaf())		//Ҷ�ӽڵ�ʱ
			tmpNode.FreeNode();		//ֱ��ɾ���ڵ�
		else {						//��Ҷ�ӽڵ�ʱ
			for(int i = 0; i < tmpNode.GetCount(); ++i) {  //����λ��
				tmpNode.GetPointer(i, tmpBlock, tmpOffset);
				DropNode(tmpBlock, tmpOffset);
			}
			tmpNode.FreeNode();			//ɾ���ڵ�	
		}
	}
	else if(m_sKeyType == "float") {                   //��key������Ϊfloatʱ
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(Block, Offset);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		if(tmpNode.IsLeaf())		//Ҷ�ӽڵ�ʱ
			tmpNode.FreeNode();		//ֱ��ɾ���ڵ�
		else {						//��Ҷ�ӽڵ�ʱ
			for(int i = 0; i < tmpNode.GetCount(); ++i) {  //����λ��
				tmpNode.GetPointer(i, tmpBlock, tmpOffset);
				DropNode(tmpBlock, tmpOffset);
			}
			tmpNode.FreeNode();			//ɾ���ڵ�	
		}
	}
}



//ɾ��B��
void BTree::Drop()								
{
	if(m_iRoot < 0 || m_iFirstLeaf < 0 || m_file == NULL)   //���ڵ㣬��Ҷ�ӽڵ㲻���ڣ����ļ�Ϊ��ʱ
		cout <<"error when drop B+"<<endl; return;
	DropNode(-1, m_iRoot);			//ɾ�����ڵ�			

	m_sKeyType = "";                //�������������㣬��ʼ��
	m_iKeyTypeCount = 0;

	TreeSize = 0;

	m_iRoot = -1;
	m_iFirstLeaf = -1;

	MaxOffset = -1;
	EmptyOffset = -1;
}



//Ѱ���ļ��еĿ���λ��
void BTree::FindEmptyOffset(int& Offset, int NodeSize)
{
	if(EmptyOffset >= 0){        //���ڿ���λ��ʱ
		Offset = EmptyOffset;    //��λOffsetֵ
		m_file->Seek(TreeSize + NodeSize * Offset, CFile::begin);
		m_file->Read(&EmptyOffset, sizeof(int));     //?
		UpdateBPlusTree();
	}
	else{                       //�����ڿ���λ��
		Offset = ++MaxOffset;   //���Ѵ洢�ڵ��ĩ�˶�λOffset
		UpdateBPlusTree();
	}
}


//ɾ���ڵ���ǿ���λ��
void BTree::AddEmptyOffset(int Offset, int NodeSize)
{
	m_file->Seek(TreeSize + NodeSize * Offset, CFile::begin);   //�ļ�ָ���ƶ����趨��λ��
	m_file->Write(&EmptyOffset, sizeof(int));      //д��հ�
	EmptyOffset = Offset;
	UpdateBPlusTree();
}		



//����B����keyֵ��ͬ�ļ�¼��ַ
bool BTree::FindValue(void* pValue, int& Block, int& Offset)
{
	if(m_iRoot < 0 || m_file == NULL)       //���ڵ㲻���ڻ��ļ�Ϊ��
	{cout <<"no root when find value in B+tree"<<endl; return false;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0)
	{cout <<"error input for find value in b+"<<endl; return false;}
	if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);    //��root��ʼ
		tmpNode.Read();                        //��root��Ϣ��ȡ����
		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {	      //�ڵ�Ϊ��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((char*)pValue);	
			if(FindIndex == -1)			  //�����ڸ���keyֵ				
				FindIndex = tmpNode.GetCount();					
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);	
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}                                  //�����ӽڵ�
		FindIndex = tmpNode.FindKeyIndex((char*)pValue);		
		if(FindIndex < 0)				 //û���ҵ����������ļ�¼						
			return false;
		tmpNode.GetPointer(FindIndex, Block, Offset);  //�������������ĵ�ĵ�ַ			
	}
	else if(m_sKeyType == "int")                   //��key������Ϊintʱ
	{
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();
		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {	      //�ڵ�Ϊ��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((int*)pValue);
			if(FindIndex == -1)			  //�����ڸ���keyֵ
				FindIndex = tmpNode.GetCount();
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		FindIndex = tmpNode.FindKeyIndex((int*)pValue);
		if(FindIndex < 0)				 //û���ҵ����������ļ�¼	
			return false;
		tmpNode.GetPointer(FindIndex, Block, Offset);  //�������������ĵ�ĵ�ַ
	}

	else if(m_sKeyType == "float")                   //��key������Ϊfloatʱ
	{
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();
		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {	      //�ڵ�Ϊ��Ҷ�ӽڵ�ʱ
			FindIndex = tmpNode.FindGreaterKeyIndex((float*)pValue);
			if(FindIndex == -1)			  //�����ڸ���keyֵ
				FindIndex = tmpNode.GetCount();
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		FindIndex = tmpNode.FindKeyIndex((float*)pValue);
		if(FindIndex < 0)				 //û���ҵ����������ļ�¼
			return false;
		tmpNode.GetPointer(FindIndex, Block, Offset);  //�������������ĵ�ĵ�ַ
	}
	return true;       //�ҵ������������ĵ�
}





class BTreeReader: public CObject    //ʲô�ã���
{
	int m_iFindIndex;
	int m_iBFindIndex;
	void* m_pNode;
public:
	int GetFindIndex(){return m_iFindIndex;}
	int GetBFindIndex(){return m_iBFindIndex;}
	void SetFindIndex(int Index){m_iFindIndex = Index;}
	void SetBFindIndex(int Index){m_iBFindIndex = Index;}
	void* GetNode(){return m_pNode;}
	void SetNode(void* Node){m_pNode = Node;}
};




//��������
BTree::~BTree(void)	
{
	while(m_aReaders.GetSize() != 0) {
		if(m_aReaders.GetAt(0) == NULL) {
			m_aReaders.RemoveAt(0);
		}
		else {
			delete (BTreeReader*)(m_aReaders.GetAt(0));   //�ͷſռ�
			m_aReaders.RemoveAt(0);
		}
	}
}


