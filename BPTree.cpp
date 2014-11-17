#include "Node.h"
#include "Node.cpp"
#include "BPTree.h"



using namespace std;



//构造函数
BTree::BTree(CString KeyType, int KeyTypeCount)
{
	m_sKeyType = KeyType;           //设置key值类型
	m_iKeyTypeCount = KeyTypeCount; //设置key值的大小
	TreeSize = sizeof(int) * 4;     //就存以下4个int就够了？

	m_iRoot = -1;        //初始化各个变量
	m_iFirstLeaf = -1;

	MaxOffset = -1;
	EmptyOffset = -1;

	m_file = NULL;
}


//创建B树
void BTree::CreateBPlusTree()
{
	int tmp;
	if(m_file == NULL)       //文件为空
	{cout <<"init before create b+tree"<<endl; return;}

	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0)
	{cout <<"error input for create B+ Tree"<<endl; return;}

	if(m_sKeyType=="char") {                            //当key的类型为char时							
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);  //建立key值类型为char的临时节点   //+1是考虑'\0'
		tmpNode.SetFileOrganize(m_file);                //设置该点的各个变量信息
		tmpNode.SetCount(0);
		tmpNode.SetLeaf(true);
		tmpNode.AddNode();
		tmpNode.GetSelfPosition(tmp, m_iRoot);
	}
	else if(m_sKeyType=="int") {                   //当key的类型为int时
		Node<int> tmpNode(m_iKeyTypeCount, this);  //建立key值类型为int的临时节点
		tmpNode.SetFileOrganize(m_file);           //设置该点的各个变量信息
		tmpNode.SetCount(0);
		tmpNode.SetLeaf(1);
		tmpNode.AddNode();
		tmpNode.GetSelfPosition(tmp, m_iRoot);
	}
	else if(m_sKeyType=="float") {                   //当key的类型为float时
		Node<float> tmpNode(m_iKeyTypeCount, this);  //建立key值类型为float的临时节点
		tmpNode.SetFileOrganize(m_file);             //设置该点的各个变量信息
		tmpNode.SetCount(0);
		tmpNode.SetLeaf(1);
		tmpNode.AddNode();
		tmpNode.GetSelfPosition(tmp, m_iRoot);
	}
	
	m_iFirstLeaf=m_iRoot;       //设置B树中第一个点为该临时节点

	int* c = new int[TreeSize/4];
	int* p = c;

	p[0] = m_iRoot;      //存入B树信息
	
	p[1] = m_iFirstLeaf;
	
	p[2] = EmptyOffset;

	p[3] = MaxOffset;
	
	m_file->SeekToBegin();
	m_file->Write(c, TreeSize);  //写入文件
	delete[] c;
}


//更新B树
void BTree::UpdateBPlusTree()	
{
	m_file->SeekToBegin();               //将文件数据各个变量更新
	m_file->Write(&m_iRoot,sizeof(int));
	m_file->Write(&m_iFirstLeaf, sizeof(int));
	m_file->Write(&EmptyOffset, sizeof(int));
	m_file->Write(&MaxOffset, sizeof(int));
}


//读取B树的信息
void BTree::Read()
{	
	if(m_file == NULL)             //文件为空
	{cout <<"error now store in b+tree"<<endl; return;}

	int* c = new int[TreeSize/4];
	int* p = c;
	m_file->Read(c, TreeSize);	   //读取文件信息

	m_iRoot = p[0];           //给变量赋值
	
	m_iFirstLeaf = p[1];

	EmptyOffset = p[2];

	MaxOffset = p[3];

	delete[] c;
}






//向B树中插入新的key以及其pointer所指地址
void BTree::InsertValue(void* pValue, int Block, int Offset)
{
	if(m_iRoot < 0 || m_file == NULL)       //根节点不存在或文件为空
	{cout <<"no root when find value in B+tree"<<endl; return;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
	{cout <<"error input for find value in b+"<<endl; return;}
	if(m_sKeyType == "char") {                    //当key的类型为char时
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {          //非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((char*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();			//寻找合适的地点
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);      //通过指针到达叶子
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);	
			tmpNode.Read();
		}
		tmpNode.InsertKey((char*)pValue, Block, Offset,1);	 //到达叶子节点时插入记录
	}
	else if(m_sKeyType == "int") {                   //当key的类型为int时
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot); 
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {          //非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((int*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//寻找合适的地点
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.InsertKey((int*)pValue, Block, Offset, 1);	 //到达叶子节点时插入记录
	}
	else if(m_sKeyType == "float") {                    //当key的类型为float时
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {          //非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((float*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//寻找合适的地点
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.InsertKey((float*)pValue, Block, Offset, 1);	 //到达叶子节点时插入记录
	}

}



//删除B树中的记录
void BTree::DeleteValue(void* pValue)
{
	if(m_iRoot < 0 || m_file == NULL)       //根节点不存在或文件为空
	{cout <<"no root when find value in B+tree"<<endl; return;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
	{cout <<"error input for find value in b+"<<endl; return;}
	if(m_sKeyType == "char") {                   //当key的类型为char时
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {			  //非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((char*)pValue);
			if(FindIndex == -1)										
				FindIndex = tmpNode.GetCount();			//寻找合适的地点					
			tmpNode.GetPointer(FindIndex,tmpBlock, tmpOffset);	
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);		
			tmpNode.Read();
		}
		tmpNode.DeleteKey(tmpNode.FindKeyIndex((char*)pValue));	 //到达叶子节点时删除记录	
	}
	else if(m_sKeyType == "int") {                    //当key的类型为int时
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {			  //非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((int*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//寻找合适的地点
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.DeleteKey(tmpNode.FindKeyIndex((int*)pValue));	 //到达叶子节点时删除记录
	}
	else if(m_sKeyType == "float") {                   //当key的类型为float时
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {			  //非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((float*)pValue);
			if(FindIndex == -1)
				FindIndex = tmpNode.GetCount();		//寻找合适的地点
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		tmpNode.DeleteKey(tmpNode.FindKeyIndex((float*)pValue));	 //到达叶子节点时删除记录
	}
}



//删除B树中的节点
void BTree::DropNode(int Block, int Offset)			
{
	if(m_iRoot < 0 || m_file == NULL)       //根节点不存在或文件为空
	{cout <<"no root when find value in B+tree"<<endl; return;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0 )
	{cout <<"error input for find value in b+"<<endl; return;}
	if(m_sKeyType == "char") {                   //当key的类型为char时
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(Block, Offset);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		if(tmpNode.IsLeaf())		//叶子节点时
			tmpNode.FreeNode();		//直接删除节点			
		else {						//非叶子节点时						
			for(int i = 0; i < tmpNode.GetCount(); ++i) {   //查找位置
				tmpNode.GetPointer(i, tmpBlock, tmpOffset);
				DropNode(tmpBlock, tmpOffset);
			}
			tmpNode.FreeNode();		//删除节点								
		}
	}
	else if(m_sKeyType == "int") {                   //当key的类型为int时
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(Block, Offset);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		if(tmpNode.IsLeaf())		//叶子节点时
			tmpNode.FreeNode();		//直接删除节点
		else {						//非叶子节点时
			for(int i = 0; i < tmpNode.GetCount(); ++i) {  //查找位置
				tmpNode.GetPointer(i, tmpBlock, tmpOffset);
				DropNode(tmpBlock, tmpOffset);
			}
			tmpNode.FreeNode();			//删除节点	
		}
	}
	else if(m_sKeyType == "float") {                   //当key的类型为float时
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);
		tmpNode.SetSelfPosition(Block, Offset);
		tmpNode.Read();

		int tmpBlock, tmpOffset;
		if(tmpNode.IsLeaf())		//叶子节点时
			tmpNode.FreeNode();		//直接删除节点
		else {						//非叶子节点时
			for(int i = 0; i < tmpNode.GetCount(); ++i) {  //查找位置
				tmpNode.GetPointer(i, tmpBlock, tmpOffset);
				DropNode(tmpBlock, tmpOffset);
			}
			tmpNode.FreeNode();			//删除节点	
		}
	}
}



//删除B树
void BTree::Drop()								
{
	if(m_iRoot < 0 || m_iFirstLeaf < 0 || m_file == NULL)   //根节点，首叶子节点不存在，或文件为空时
		cout <<"error when drop B+"<<endl; return;
	DropNode(-1, m_iRoot);			//删除根节点			

	m_sKeyType = "";                //将各个变量清零，初始化
	m_iKeyTypeCount = 0;

	TreeSize = 0;

	m_iRoot = -1;
	m_iFirstLeaf = -1;

	MaxOffset = -1;
	EmptyOffset = -1;
}



//寻找文件中的空余位置
void BTree::FindEmptyOffset(int& Offset, int NodeSize)
{
	if(EmptyOffset >= 0){        //存在空闲位置时
		Offset = EmptyOffset;    //定位Offset值
		m_file->Seek(TreeSize + NodeSize * Offset, CFile::begin);
		m_file->Read(&EmptyOffset, sizeof(int));     //?
		UpdateBPlusTree();
	}
	else{                       //不存在空闲位置
		Offset = ++MaxOffset;   //在已存储节点的末端定位Offset
		UpdateBPlusTree();
	}
}


//删除节点后标记空余位置
void BTree::AddEmptyOffset(int Offset, int NodeSize)
{
	m_file->Seek(TreeSize + NodeSize * Offset, CFile::begin);   //文件指针移动到设定的位置
	m_file->Write(&EmptyOffset, sizeof(int));      //写入空白
	EmptyOffset = Offset;
	UpdateBPlusTree();
}		



//查找B树中key值相同的记录地址
bool BTree::FindValue(void* pValue, int& Block, int& Offset)
{
	if(m_iRoot < 0 || m_file == NULL)       //根节点不存在或文件为空
	{cout <<"no root when find value in B+tree"<<endl; return false;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
		m_sKeyType != "float") ||
		m_iKeyTypeCount <= 0)
	{cout <<"error input for find value in b+"<<endl; return false;}
	if(m_sKeyType == "char") {                   //当key的类型为char时
		Node<char> tmpNode(m_iKeyTypeCount + 1, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);    //从root开始
		tmpNode.Read();                        //将root信息读取出来
		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {	      //节点为非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((char*)pValue);	
			if(FindIndex == -1)			  //不存在更大key值				
				FindIndex = tmpNode.GetCount();					
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);	
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}                                  //到达子节点
		FindIndex = tmpNode.FindKeyIndex((char*)pValue);		
		if(FindIndex < 0)				 //没有找到满足条件的记录						
			return false;
		tmpNode.GetPointer(FindIndex, Block, Offset);  //返回满足条件的点的地址			
	}
	else if(m_sKeyType == "int")                   //当key的类型为int时
	{
		Node<int> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();
		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {	      //节点为非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((int*)pValue);
			if(FindIndex == -1)			  //不存在更大key值
				FindIndex = tmpNode.GetCount();
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		FindIndex = tmpNode.FindKeyIndex((int*)pValue);
		if(FindIndex < 0)				 //没有找到满足条件的记录	
			return false;
		tmpNode.GetPointer(FindIndex, Block, Offset);  //返回满足条件的点的地址
	}

	else if(m_sKeyType == "float")                   //当key的类型为float时
	{
		Node<float> tmpNode(m_iKeyTypeCount, this);
		tmpNode.SetFileOrganize(m_file);	
		tmpNode.SetSelfPosition(-2, m_iRoot);
		tmpNode.Read();
		int tmpBlock, tmpOffset;
		int FindIndex = 0;
		while(!tmpNode.IsLeaf()) {	      //节点为非叶子节点时
			FindIndex = tmpNode.FindGreaterKeyIndex((float*)pValue);
			if(FindIndex == -1)			  //不存在更大key值
				FindIndex = tmpNode.GetCount();
			tmpNode.GetPointer(FindIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		FindIndex = tmpNode.FindKeyIndex((float*)pValue);
		if(FindIndex < 0)				 //没有找到满足条件的记录
			return false;
		tmpNode.GetPointer(FindIndex, Block, Offset);  //返回满足条件的点的地址
	}
	return true;       //找到了满足条件的点
}





class BTreeReader: public CObject    //什么用？？
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




//析构函数
BTree::~BTree(void)	
{
	while(m_aReaders.GetSize() != 0) {
		if(m_aReaders.GetAt(0) == NULL) {
			m_aReaders.RemoveAt(0);
		}
		else {
			delete (BTreeReader*)(m_aReaders.GetAt(0));   //释放空间
			m_aReaders.RemoveAt(0);
		}
	}
}




//查找B树中key值更大的记录地址    

/*
bool BTree::FindValueBigger(void* pValue, record& Record)	
{
	if(m_iRoot < 0 || m_file == NULL)       //根节点不存在或文件为空
		cout <<"no root when find value in B+tree"<<endl; return false;
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
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

		if(m_sKeyType == "char") {                   //当key的类型为char时
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
	if(m_sKeyType == "char") {                   //当key的类型为char时
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

//查找B树中key值更小的记录地址
bool BTree::FindValueSmaller(void* pValue, record& Record)	
{
	if(m_iRoot < 0 || m_file == NULL)                     //根节点不存在或文件为空
		cout <<"no root when find value in B+tree"<<endl; return false;
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
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
		if(m_sKeyType == "char") {                   //当key的类型为char时
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
	if(m_sKeyType == "char") {                   //当key的类型为char时
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

//查找B树中key值介于所给2者之间的记录地址
bool BTree::FindValueBetween(void* pBValue, void* pSValue, record& Record)
{
	if(m_iRoot < 0 || m_file == NULL)                    //根节点不存在或文件为空
	{cout <<"no root when find value in B+tree"<<endl; return false;}
	if((m_sKeyType != "char" && m_sKeyType != "int" &&    //储存文件的类型错误
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

		if(m_sKeyType == "char") {                   //当key的类型为char时
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

