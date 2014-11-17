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




//����B����keyֵ����ļ�¼��ַ    

/*
bool BTree::FindValueBigger(void* pValue, record& Record)	
{
	if(m_iRoot < 0 || m_file == NULL)       //���ڵ㲻���ڻ��ļ�Ϊ��
		cout <<"no root when find value in B+tree"<<endl; return false;
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0)
	{cout <<"error init for find bigger value in b+"<<endl; return false;}
	int ReadNumber = Record.attribute_count;
	int FindIndex = -1;
	int BFindIndex = -1;
	int tmpBlock, tmpOffset;
	BTreeReader* Reader;
	void* tmpNode;
	int Block, Offset;
	if(ReadNumber == -1) {									
		Reader = new BTreeReader;
		ReadNumber = m_aReaders.GetSize();
		Record.position.block_position = Record.position.index_position = 0;
		m_aReaders.Add((CObject*)Reader);
		FindIndex = -1;
		BFindIndex = -1;
		Reader->SetFindIndex(-1);
		Reader->SetBFindIndex(-1);

		if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
			tmpNode = new Node<char>(m_iKeyTypeCount + 1, this);	
			((Node<char>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
		else if(m_sKeyType == "int") {
			tmpNode = new Node<int>(m_iKeyTypeCount, this);
			((Node<int>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
		else if(m_sKeyType == "float") {
			tmpNode = new Node<float>(m_iKeyTypeCount, this);
			((Node<float>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
	}
	else {										
		Reader = (BTreeReader*)(m_aReaders.GetAt(ReadNumber));
		FindIndex = Reader->GetFindIndex();
		BFindIndex = Reader->GetBFindIndex();
		tmpNode = Reader->GetNode();
	}
	if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
		if(FindIndex == -1) {
			((Node<char>*)tmpNode)->SetSelfPosition(-2, m_iRoot);	
			((Node<char>*)tmpNode)->Read();
			while(!((Node<char>*)tmpNode)->IsLeaf()) {		 				
				FindIndex = ((Node<char>*)tmpNode)->FindGreaterKeyIndex((char*)pValue);	
				if(FindIndex == -1)											
					FindIndex = ((Node<char>*)tmpNode)->GetCount();		
				((Node<char>*)tmpNode)->GetPointer(FindIndex, tmpBlock, tmpOffset);
				((Node<char>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
				((Node<char>*)tmpNode)->Read();
			}
			FindIndex = ((Node<char>*)tmpNode)->FindKeyIndex((char*)pValue);	
		}
		if(FindIndex < 0) {														
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<char>*)(Reader->GetNode());						
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;													
		}
		++FindIndex;															
		if(FindIndex >= ((Node<char>*)tmpNode)->GetCount()) {					
			((Node<char>*)tmpNode)->GetPointer(FANOUT - 1, tmpBlock, tmpOffset);	
			if(tmpBlock < 0 || tmpOffset < 0) {								
				Record.position.block_position = Record.position.index_position = -1;
				FindIndex = -1;
				BFindIndex = -1;
				delete (Node<char>*)(Reader->GetNode());					
				delete Reader;
				m_aReaders.SetAt(ReadNumber,NULL);
				while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
					m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
					if(m_aReaders.GetSize() == 0)
						break;
				}
				return false;												
			}
			((Node<char>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);	
			((Node<char>*)tmpNode)->Read();
			FindIndex = 0;
		}
		((Node<char>*)tmpNode)->GetPointer(FindIndex, Block, Offset);	
		Record.position.block_position = Block;
		Record.position.index_position = Offset;							
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
	else if(m_sKeyType == "int") {
		if(FindIndex == -1) {
			((Node<int>*)tmpNode)->SetSelfPosition(-2, m_iRoot);
			((Node<int>*)tmpNode)->Read();
			while(!((Node<int>*)tmpNode)->IsLeaf()) {
				FindIndex = ((Node<int>*)tmpNode)->FindGreaterKeyIndex((int*)pValue);
				if(FindIndex == -1)
					FindIndex = ((Node<int>*)tmpNode)->GetCount();
				((Node<int>*)tmpNode)->GetPointer(FindIndex, tmpBlock, tmpOffset);
				((Node<int>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
				((Node<int>*)tmpNode)->Read();
			}
			FindIndex = ((Node<int>*)tmpNode)->FindKeyIndex((int*)pValue);
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<int>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		++FindIndex;
		if(FindIndex >= ((Node<int>*)tmpNode)->GetCount()) {
			((Node<int>*)tmpNode)->GetPointer(FANOUT - 1, tmpBlock, tmpOffset);
			if(tmpBlock < 0 || tmpOffset < 0) {
				Record.position.block_position = Record.position.index_position = -1;
				FindIndex = -1;
				BFindIndex = -1;
				delete (Node<int>*)(Reader->GetNode());
				delete Reader;
				m_aReaders.SetAt(ReadNumber,NULL);
				while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
					m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
					if(m_aReaders.GetSize() == 0)
						break;
				}
				return false;
			}
			((Node<int>*)tmpNode)->SetSelfPosition(tmpBlock,tmpOffset);
			((Node<int>*)tmpNode)->Read();
			FindIndex = 0;
		}
		((Node<int>*)tmpNode)->GetPointer(FindIndex, Block, Offset);
		Record.position.block_position = Record.position.index_position = -1;
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
	else if(m_sKeyType == "float") {
		if(FindIndex == -1) {
			((Node<float>*)tmpNode)->SetSelfPosition(-2, m_iRoot);
			((Node<float>*)tmpNode)->Read();
			while(!((Node<float>*)tmpNode)->IsLeaf()) {
				FindIndex = ((Node<float>*)tmpNode)->FindGreaterKeyIndex((float*)pValue);
				if(FindIndex == -1)
					FindIndex=((Node<float>*)tmpNode)->GetCount();
				((Node<float>*)tmpNode)->GetPointer(FindIndex, tmpBlock, tmpOffset);
				((Node<float>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
				((Node<float>*)tmpNode)->Read();
			}
			FindIndex = ((Node<float>*)tmpNode)->FindKeyIndex((float*)pValue);
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<float>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		++FindIndex;
		if(FindIndex >= ((Node<float>*)tmpNode)->GetCount()) {
			((Node<float>*)tmpNode)->GetPointer(FANOUT - 1, tmpBlock, tmpOffset);
			if(tmpBlock < 0 || tmpOffset < 0) {
				Record.position.block_position = Record.position.index_position = -1;
				FindIndex = -1;
				BFindIndex = -1;
				delete (Node<float>*)(Reader->GetNode());
				delete Reader;
				m_aReaders.SetAt(ReadNumber, NULL);
				while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
					m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
					if(m_aReaders.GetSize() == 0)
						break;
				}
				return false;
			}
			((Node<float>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
			((Node<float>*)tmpNode)->Read();
			FindIndex = 0;
		}
		((Node<float>*)tmpNode)->GetPointer(FindIndex, Block, Offset);
		Record.position.block_position = Record.position.index_position = -1;
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}

}

//����B����keyֵ��С�ļ�¼��ַ
bool BTree::FindValueSmaller(void* pValue, record& Record)	
{
	if(m_iRoot < 0 || m_file == NULL)                     //���ڵ㲻���ڻ��ļ�Ϊ��
		cout <<"no root when find value in B+tree"<<endl; return false;
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
		cout <<"error init for find bigger value in b+"<<endl; return false;
	int ReadNumber = Record.attribute_count;
	int FindIndex = -1;
	int BFindIndex = -1;
	int tmpBlock, tmpOffset;
	BTreeReader* Reader;
	void* tmpNode;
	int Block, Offset;
	if(ReadNumber == -1) {								
		Reader = new BTreeReader;
		ReadNumber = m_aReaders.GetSize();
		Record.attribute_count = ReadNumber;
		m_aReaders.Add((CObject*)Reader);
		FindIndex = -1;
		BFindIndex = -1;
		Reader->SetFindIndex(-1);
		Reader->SetBFindIndex(-1);
		if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
			tmpNode = new Node<char>(m_iKeyTypeCount + 1, this);
			((Node<char>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
		else if(m_sKeyType == "int") {
			tmpNode = new Node<int>(m_iKeyTypeCount, this);
			((Node<int>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
		else if(m_sKeyType == "float") {
			tmpNode = new Node<float>(m_iKeyTypeCount,this);
			((Node<float>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
	}
	else {													
		Reader = (BTreeReader*)(m_aReaders.GetAt(ReadNumber));
		FindIndex = Reader->GetFindIndex();
		BFindIndex = Reader->GetBFindIndex();
		tmpNode = Reader->GetNode();
	}
	if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
		if(FindIndex == -1) {
			((Node<char>*)tmpNode)->SetSelfPosition(-2, m_iFirstLeaf);
			((Node<char>*)tmpNode)->Read();
			BFindIndex = ((Node<char>*)tmpNode)->FindKeyIndex((char*)pValue);
		}
		if(FindIndex < 0) {					
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<char>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		if(FindIndex + 1 > BFindIndex && BFindIndex != -1) {					
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<char>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		((Node<char>*)tmpNode)->GetPointer(FindIndex, Block, Offset);	
		Record.position.block_position = Record.position.index_position = -1;
		++FindIndex;
		if(FindIndex >= ((Node<char>*)tmpNode)->GetCount()) {
			((Node<char>*)tmpNode)->GetPointer(FANOUT - 1, tmpBlock, tmpOffset);
			if(tmpBlock < 0 || tmpOffset < 0) {
				FindIndex = -2;
			}
			((Node<char>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
			((Node<char>*)tmpNode)->Read();
			FindIndex = 0;
			BFindIndex = ((Node<char>*)tmpNode)->FindKeyIndex((char*)pValue);
		}
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
	else if(m_sKeyType == "int") {
		if(FindIndex == -1) {
			((Node<int>*)tmpNode)->SetSelfPosition(-2, m_iFirstLeaf);
			((Node<int>*)tmpNode)->Read();
			BFindIndex = ((Node<int>*)tmpNode)->FindKeyIndex((int*)pValue);
			FindIndex = 0;
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<int>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		if(FindIndex + 1 > BFindIndex && BFindIndex != -1) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<int>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		((Node<int>*)tmpNode)->GetPointer(FindIndex, Block, Offset);
		Record.position.block_position = Record.position.index_position = -1;
		++FindIndex;

		if(FindIndex >= ((Node<int>*)tmpNode)->GetCount()) {
			((Node<int>*)tmpNode)->GetPointer(FANOUT - 1, tmpBlock, tmpOffset);
			if(tmpBlock < 0 || tmpOffset < 0) {
				FindIndex = -2;
			}
			((Node<int>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
			((Node<int>*)tmpNode)->Read();
			FindIndex = 0;
			BFindIndex = ((Node<int>*)tmpNode)->FindKeyIndex((int*)pValue);
		}
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
	else if(m_sKeyType == "float") {
		if(FindIndex == -1) {
			((Node<float>*)tmpNode)->SetSelfPosition(-2, m_iFirstLeaf);
			((Node<float>*)tmpNode)->Read();

			BFindIndex = ((Node<float>*)tmpNode)->FindKeyIndex((float*)pValue);
			FindIndex = 0;
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<float>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		if(FindIndex + 1 > BFindIndex && BFindIndex != -1) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<float>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		((Node<float>*)tmpNode)->GetPointer(FindIndex, Block, Offset);
		Record.position.block_position = Block;
		Record.position.index_position = Offset;
		++FindIndex;

		if(FindIndex >= ((Node<float>*)tmpNode)->GetCount()) {
			((Node<float>*)tmpNode)->GetPointer(FANOUT - 1, tmpBlock, tmpOffset);
			if(tmpBlock < 0 || tmpOffset < 0) {
				FindIndex = -2;
			}
			((Node<float>*)tmpNode)->SetSelfPosition(tmpBlock, tmpOffset);
			((Node<float>*)tmpNode)->Read();
			FindIndex = 0;
			BFindIndex = ((Node<float>*)tmpNode)->FindKeyIndex((float*)pValue);
		}
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
}

//����B����keyֵ��������2��֮��ļ�¼��ַ
bool BTree::FindValueBetween(void* pBValue, void* pSValue, record& Record)
{
	if(m_iRoot < 0 || m_file == NULL)                    //���ڵ㲻���ڻ��ļ�Ϊ��
	{cout <<"no root when find value in B+tree"<<endl; return false;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //�����ļ������ʹ���
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
		cout <<"error init for find bigger value in b+"<<endl; return false;
	int ReadNumber = Record.attribute_count;
	int FindIndex = -1;
	int BFindIndex = -1;
	int tmpBlock, tmpIndex;
	BTreeReader* Reader;
	void* tmpNode;
	int Block, Index;
	if(ReadNumber == -1) {
		Reader = new BTreeReader;
		ReadNumber = m_aReaders.GetSize();
		Record.attribute_count = ReadNumber;
		m_aReaders.Add((CObject*)Reader);
		FindIndex = -1;
		BFindIndex = -1;
		Reader->SetFindIndex(-1);
		Reader->SetBFindIndex(-1);

		if(m_sKeyType == "char") {                   //��key������Ϊcharʱ
			tmpNode = new Node<char>(m_iKeyTypeCount + 1, this);
			((Node<char>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
		else if(m_sKeyType == "int") {
			tmpNode = new Node<int>(m_iKeyTypeCount, this);
			((Node<int>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
		else if(m_sKeyType == "float") {
			tmpNode = new Node<float>(m_iKeyTypeCount, this);
			((Node<float>*)tmpNode)->SetFileOrganize(m_file);
			Reader->SetNode((void*)tmpNode);
		}
	}
	else {
		Reader = (BTreeReader*)(m_aReaders.GetAt(ReadNumber));
		FindIndex = Reader->GetFindIndex();
		BFindIndex = Reader->GetBFindIndex();
		tmpNode = Reader->GetNode();
	}
	if(m_sKeyType == "char") {
		if(FindIndex == -1) {
			while(!((Node<char>*)tmpNode)->IsLeaf()) {
				FindIndex = ((Node<char>*)tmpNode)->FindGreaterKeyIndex((char*)pSValue);
				if(FindIndex == -1)
					FindIndex = ((Node<char>*)tmpNode)->GetCount();
				((Node<char>*)tmpNode)->GetPointer(FindIndex, tmpBlock, tmpIndex);
				((Node<char>*)tmpNode)->SetSelfPosition(tmpBlock, tmpIndex);
				((Node<char>*)tmpNode)->Read();
			}
			FindIndex = ((Node<char>*)tmpNode)->FindKeyIndex((char*)pSValue);
			BFindIndex = ((Node<char>*)tmpNode)->FindKeyIndex((char*)pBValue);
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<char>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		++FindIndex;
		if(FindIndex > BFindIndex && BFindIndex != -1) {
			FindIndex = -2;
			Reader->SetFindIndex(-2);
			return false;
		}
		if(FindIndex >= ((Node<char>*)tmpNode)->GetCount()) {
			((Node<char>*)tmpNode)->GetFrontNodePosition(tmpBlock, tmpIndex);
			if(tmpBlock < 0 || tmpIndex < 0) {
				FindIndex = -2;
				Reader->SetFindIndex(-2);
				return false;
			}
			((Node<char>*)tmpNode)->SetSelfPosition(tmpBlock, tmpIndex);
			((Node<char>*)tmpNode)->Read();
			BFindIndex=((Node<char>*)tmpNode)->FindKeyIndex((char*)pBValue);
			FindIndex = 0;
		}
		((Node<char>*)tmpNode)->GetPointer(FindIndex, Block, Index);
			Record.position.block_position = Block;
			Record.position.index_position = Index;
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
	else if(m_sKeyType == "int") {
		if(FindIndex == -1) {
			while(!((Node<int>*)tmpNode)->IsLeaf()) {
				FindIndex = ((Node<int>*)tmpNode)->FindGreaterKeyIndex((int*)pSValue);
				if(FindIndex == -1)
					FindIndex = ((Node<int>*)tmpNode)->GetCount();
				((Node<int>*)tmpNode)->GetPointer(FindIndex, tmpBlock, tmpIndex);
				((Node<int>*)tmpNode)->SetSelfPosition(tmpBlock, tmpIndex);
				((Node<int>*)tmpNode)->Read();
			}
			FindIndex = ((Node<int>*)tmpNode)->FindKeyIndex((int*)pSValue);
			BFindIndex = ((Node<int>*)tmpNode)->FindKeyIndex((int*)pBValue);
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<int>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		++FindIndex;
		if(FindIndex > BFindIndex && BFindIndex != -1) {
			FindIndex = -2;
			Reader->SetFindIndex(-2);
			return false;
		}
		if(FindIndex >= ((Node<int>*)tmpNode)->GetCount()) {
			((Node<int>*)tmpNode)->GetFrontNodePosition(tmpBlock, tmpIndex);
			if(tmpBlock < 0 || tmpIndex < 0) {
				FindIndex = -2;
				Reader->SetFindIndex(-2);
				return false;
			}
			((Node<int>*)tmpNode)->SetSelfPosition(tmpBlock, tmpIndex);
			((Node<int>*)tmpNode)->Read();
			BFindIndex=((Node<int>*)tmpNode)->FindKeyIndex((int*)pBValue);
			FindIndex = 0;
		}
		((Node<int>*)tmpNode)->GetPointer(FindIndex, Block, Index);
		Record.position.block_position = Block;
		Record.position.index_position = Index;
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
	else if(m_sKeyType == "float") {
		if(FindIndex == -1) {
			while(!((Node<float>*)tmpNode)->IsLeaf()) {
				FindIndex = ((Node<float>*)tmpNode)->FindGreaterKeyIndex((float*)pSValue);
				if(FindIndex == -1)
					FindIndex = ((Node<float>*)tmpNode)->GetCount();
				((Node<float>*)tmpNode)->GetPointer(FindIndex, tmpBlock, tmpIndex);
				((Node<float>*)tmpNode)->SetSelfPosition(tmpBlock, tmpIndex);
				((Node<float>*)tmpNode)->Read();
			}
			FindIndex = ((Node<float>*)tmpNode)->FindKeyIndex((float*)pSValue);
			BFindIndex = ((Node<float>*)tmpNode)->FindKeyIndex((float*)pBValue);
		}
		if(FindIndex < 0) {
			Record.position.block_position = Record.position.index_position = -1;
			FindIndex = -1;
			BFindIndex = -1;
			delete (Node<float>*)(Reader->GetNode());
			delete Reader;
			m_aReaders.SetAt(ReadNumber, NULL);
			while(m_aReaders.GetAt(m_aReaders.GetUpperBound()) == NULL) {
				m_aReaders.RemoveAt(m_aReaders.GetUpperBound());
				if(m_aReaders.GetSize() == 0)
					break;
			}
			return false;
		}
		++FindIndex;
		if(FindIndex > BFindIndex && BFindIndex != -1) {
			FindIndex = -2;
			Reader->SetFindIndex(-2);
			return false;
		}
		if(FindIndex >= ((Node<float>*)tmpNode)->GetCount()) {
			((Node<float>*)tmpNode)->GetFrontNodePosition(tmpBlock, tmpIndex);
			if(tmpBlock < 0 || tmpIndex < 0) {
				FindIndex = -2;
				Reader->SetFindIndex(-2);
				return false;
			}
			((Node<float>*)tmpNode)->SetSelfPosition(tmpBlock, tmpIndex);
			((Node<float>*)tmpNode)->Read();
			BFindIndex = ((Node<float>*)tmpNode)->FindKeyIndex((float*)pBValue);
			FindIndex = 0;
		}
		((Node<float>*)tmpNode)->GetPointer(FindIndex, Block, Index);
		Record.position.block_position = Block;
		Record.position.index_position = Index;
		Reader->SetFindIndex(FindIndex);
		Reader->SetBFindIndex(BFindIndex);
		return true;
	}
}                                                                              
*/

