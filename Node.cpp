#include "Node.h"



using namespace std;

class BTree;

//�ڵ�Ĺ��캯��
template <class TKeyType>
Node<TKeyType>::Node(int TypeCount, BTree* Tree)	
{
	m_iCount = -1;                        //��ʼ���ڵ��и�������
	m_bLeaf = 1;
	m_file = NULL;
	for(int i = 0; i < FANOUT; ++i) {
		m_iPointer[i][0] = -1;
		m_iPointer[i][1] = -1;
	}

	m_iFatherPosition[0] = -1;
	m_iSelfPosition[0] = -1;    //Block

	m_iFatherPosition[1] = -1;
	m_iSelfPosition[1] = -1;    //Offset

	m_iKeyTypeCount = TypeCount;    
	NodeSize = sizeof(TKeyType) * m_iKeyTypeCount * (FANOUT - 1)   //��FANOUT+1��ָ�ӽڵ�ָ��+���ڵ�ָ��
	  + sizeof(int) * (FANOUT + 1) * 2 + 1;   //���+1���ж��Ƿ�Ҷ�ڵ�
	                                                      //���кܶ�û�氡��
	m_BTree = Tree;             //���ýڵ����ڵ���

	for(int j = 0; j < FANOUT - 1; ++j)
		m_Key[j] = new TKeyType[m_iKeyTypeCount];    //Ϊkeyֵ�Ĵ洢�����㹻�ռ�  
}





//�ڵ���������
template <class TKeyType>
Node<TKeyType>::~Node()		
{
	for(int i = 0; i < FANOUT - 1; ++i)
		delete[] m_Key[i];           //ɾ������keyֵ���ͷſռ�
}




//��֯�ڵ���Ϣ   //���ڵ���Ϣת��Ϊ�ɴ����ļ�����Ϣ
template <class TKeyType>
char* Node<TKeyType>::OrganizeNode()			
{
	if(m_iCount < 0)              //��δ�����¼							
		return NULL;
	char* pNodeStore = new char[NodeSize];	//����charָ��			
	char* p = pNodeStore;    //�����׵�ַ
	if(m_bLeaf)										
		*p = (char)(128 | (m_iCount << 25 >> 25));	//����m_iCountֵ��ǰ25λ���㣬��һλ��Ϊ1  												
	else											//Ϊʲô��25λ��
		*p = (char)(m_iCount << 25 >> 25);		    
	p += sizeof(char);								
	for(int i = 0; i < FANOUT - 1; ++i) 
	{			//�ڽڵ����Indexѭ��
		((int*)p)[0] = m_iPointer[i][0];            //����m_iPointerֵ
		((int*)p)[1] = m_iPointer[i][1];
		p += sizeof(int) * 2;
		for(int j = 0; j < m_iKeyTypeCount; ++j) 
		{		//����keyֵ
			(*(TKeyType*)p) = *(m_Key[i] + j * sizeof(TKeyType));    //��key��ָ�������Ž�ȥ
			p += sizeof(TKeyType);
		}

	}
	((int*)p)[0] = m_iPointer[FANOUT-1][0];         //�������һ��m_iPointerֵ
	((int*)p)[1] = m_iPointer[FANOUT-1][1];
	p += sizeof(int) * 2;
	((int*)p)[0] = m_iFatherPosition[0];            //��ȡ����m_iFatherPositionֵ
	((int*)p)[1] = m_iFatherPosition[1];
	
	return pNodeStore;       //��������õ���Ϣ�׵�ַָ��
}




//��ȡ�ڵ���Ϣ    //���ļ�+����λ�ö�ȡ    //��Ҫclass��������λ��
template <class TKeyType>
void Node<TKeyType>::Read()	
{
	if(m_file == NULL){							
		cout <<"init node file !"<<endl; 
		return;	//�ļ���ϢΪ��
	}
	int Block = m_iSelfPosition[0];                 //��¼�����ַ
	int Offset = m_iSelfPosition[1];	
	if(Block == -1 || Offset < 0){				    //��ַ���ڴ���
		cout <<"No this position node!"<<endl; 
		return;
	}
	m_file->Seek(m_BTree->GetSize() + NodeSize * Offset, CFile::begin);  //�ļ�ָ��������Ҫ��ȡ�Ľڵ��ַ
                     //Tree��4��int            
	char* pNodeLink = new char[NodeSize];//������ʱ�ڵ�
	char* p = pNodeLink;                            //�������ڻ��λ��ָ��
	m_file->Read((void*)pNodeLink, NodeSize);			            //��ȡһ���ڵ��С���ļ�
	m_bLeaf = ((*p) < 0);							//��ȡm_bLeafֵ	  
	m_iCount = ((*p) & 127);						//��ȡm_iCountֵ  //��count���Ի�ԭ��
	p += sizeof(char);								//�ƶ�ָ��	
	for(int i = 0; i < FANOUT - 1; ++i) {			//��ȡm_iPointerֵ
		m_iPointer[i][0] = ((int*)p)[0];
		m_iPointer[i][1] = ((int*)p)[1];
		p += sizeof(int) * 2;
		for(int j = 0; j < m_iKeyTypeCount; ++j) {	//��ȡkeyֵ
			*(m_Key[i] + j * sizeof(TKeyType)) = (*(TKeyType*)p);
			p += sizeof(TKeyType);
		}
	}
	m_iPointer[FANOUT-1][0] = ((int*)p)[0];         //��ȡ���һ��m_iPointerֵ
	m_iPointer[FANOUT-1][1] = ((int*)p)[1];
	p += sizeof(int) * 2;
	m_iFatherPosition[0] = ((int*)p)[0];            //��ȡm_iFatherPositionֵ
	m_iFatherPosition[1] = ((int*)p)[1];
	delete[] pNodeLink;                             //ɾ����ʱ�ڵ㣬�ͷſռ�
}






//���½ڵ�
template <class TKeyType>
void Node<TKeyType>::UpdateNode()						
{
	if(m_file == NULL || m_iSelfPosition[0] == -1 || m_iSelfPosition[1] < 0)	//�ļ�Ϊ�ջ��ߵ�ַ����ȷ
	{cout <<"init file in node"<<endl; return;	}					
	char* c = OrganizeNode();							
	int Block = m_iSelfPosition[0], Offset = m_iSelfPosition[1];
	m_file->Seek(m_BTree->GetSize() + NodeSize * Offset, CFile::begin);
	m_file->Write((void*)c,NodeSize);	               //���ڵ���Ϣд���ļ�
	delete[] c;										   //�ͷ��ڴ�	
}




//����µĽڵ�    //�Ƚ����࣬��������������ڵ���Ϣд���ļ���λ
template <class TKeyType>
void Node<TKeyType>::AddNode()									
{
	if(m_file == NULL)	{					//�ļ�Ϊ��					
		cout <<"init file in node"<<endl;
		return;			
	}
	char* c = OrganizeNode();             //����ڵ���ϢΪ�ջ���֯�ڵ���Ϣʧ��
	if(c == NULL){												
		cout <<"cant organize node"<<endl; return;	}			

	int Offset;
	m_BTree->FindEmptyOffset(Offset, NodeSize);      //���ļ���Ѱ�ҿ���λ��
	m_file->Seek(m_BTree->GetSize() + Offset * NodeSize, CFile::begin);   
	m_file->Write((void*)c, NodeSize);				 //���ļ��п���λ�ô洢�½ڵ���Ϣ
	SetSelfPosition(-2, Offset);								
	delete[] c;
}

//ɾ���ڵ�������key
template <class TKeyType>
void Node<TKeyType>::FreeNode()									
{
	if(m_file == NULL)	{							//�ļ�Ϊ��			
		cout <<"init file in node before drop"<<endl; return;	}
	int Block, Offset;

	GetSelfPosition(Block, Offset);            //ȡ�ô˽ڵ��ַ���������
	m_BTree->AddEmptyOffset(Offset, NodeSize);		//����ֻ��offset					
}




//���ýڵ��index���ӽڵ�ĵ�ַ
template <class TKeyType>
void Node<TKeyType>::SetPointer(int Index, int PositionBlock, int PositionOffset)
{
	if(Index < 0 || Index > FANOUT - 1)	
	{
		PositionBlock = -1;                 //���indexֵ���ǽڵ��м�¼��
		PositionOffset = -1;                //�򷵻أ�-1��-1��
	}
	else{
	m_iPointer[Index][0] = PositionBlock;   //������ĵ�ֵַ��ֵ��m_iPointer[Index]
	m_iPointer[Index][1] = PositionOffset;
	}
}


//��ýڵ��index���ӽڵ�ĵ�ַ
template <class TKeyType>
void Node<TKeyType>::GetPointer(int Index, int& PositionBlock, int& PositionOffset)
{
	if(Index < 0 || Index > FANOUT - 1) {
		PositionBlock = -1;                //���indexֵ���ǽڵ��м�¼��
		PositionOffset = -1;               //���أ�-1��-1��
	}
	else{
	PositionBlock = m_iPointer[Index][0];  //���������ĵ�ֵַ
	PositionOffset = m_iPointer[Index][1];
	}
}


//��ýڵ㴢���keyֵ
template <class TKeyType>
TKeyType* Node<TKeyType>::GetKey(int Index)
{
	if(Index < 0 || Index > FANOUT - 2){
		cout <<"error when getting search key in node"<<endl; 
		return NULL;
	}
	return m_Key[Index];                   //����keyֵ
}



//���ýڵ㴢���keyֵ
template <class TKeyType>
void Node<TKeyType>::SetKey(int Index, TKeyType* pKey)
{
	if(Index < 0 || Index > FANOUT - 2){
		cout <<"error when setting search key in node"<<endl; 
		return;
	}
	for(int i = 0; i < m_iKeyTypeCount; ++i)
		m_Key[Index][i] = pKey[i];              //����keyֵΪpKey
}




//Ѱ�ҽڵ��д洢����ͬkeyֵ��indexֵ
template <class TKeyType>
int Node<TKeyType>::FindKeyIndex(TKeyType* pValue)			
{
	if(pValue==NULL)      //keyֵΪ��
		return -1;
	int i;
	for(i = 0; i < GetCount(); ++i) {		     //�ڴ���keyֵ�ļ�¼�в���					
		int j;
		for(j = 0; j < m_iKeyTypeCount; ++j) {                
			if(m_Key[i][j] == '\0' && pValue[j] == '\0')  //�����char���жϷ�
				return i;
			else if(m_Key[i][j] == pValue[j])		 //����������������keyֵ���		
				continue;								
			else										
				break;                              //��Ҫkey��ָ�������ȫ��
		}
		if(j == m_iKeyTypeCount)				 //����������������Indexֵ		
			return i;								
	}
	return -1;											
}







//Ѱ�ҽڵ��д洢�Ÿ���keyֵ��indexֵ
template <class TKeyType>
int Node<TKeyType>::FindGreaterKeyIndex(TKeyType* pValue)
{
	if(pValue==NULL)        //keyֵΪ��
		return -1;
	int i;
	for(i = 0; i < GetCount(); ++i) {		     //�ڴ���keyֵ�ļ�¼�в���				
		int j;
		for(j = 0; j < m_iKeyTypeCount; ++j) {	 		
			if(m_Key[i][j] > pValue[j])			 //����������������keyֵ�󣬷�����Indexֵ		
				return i;								
			else if(m_Key[i][j] == pValue[j])	 //��������������������		
				continue;								
			else if(m_Key[i][j] < pValue[j])	 //����������������		
				break;							
		}
	}
	return -1;         //����������㣬����-1
}








//��ýڵ�ǰһ���ڵ�ĵ�ַ
template <class TKeyType>
void Node<TKeyType>::GetFrontNodePosition(int& Block, int& Offset)
{
	if(!IsLeaf()){
		cout <<"No front node of none leaf"<<endl; 
		return;   //����Ҷ�ӽڵ�ʱ
	}
	int tmpBlock, tmpOffset;
	m_BTree->GetRoot(tmpBlock, tmpOffset);            //��øýڵ��������ĸ��ڵ��ַ
	Node<TKeyType> tmpNode(m_iKeyTypeCount, m_BTree); //����һ����ʱ�Ľڵ�
	tmpNode.SetFileOrganize(m_file);         //������ʱ�ڵ�����������ڵ���ͬ  //���ýڵ������ļ�
	tmpNode.SetSelfPosition(tmpBlock, tmpOffset);  //������ʱ�ڵ��ַΪ���ڵ��ַ  //���ýڵ������ļ���λ��
	tmpNode.Read();                         //��ȡ��ʱ�ڵ�����   //�����ڵ㣨��ȫ��¡�ɹ���
	while(!tmpNode.IsLeaf()) {									
		int GIndex = tmpNode.FindKeyIndex(GetKey(0)); //�Ӹ��ڵ㿪ʼѰ���׸���¼��keyֵ������ڵ���ͬ�ڵ�Indexֵ		
		if(GIndex < 0)											
			GIndex = tmpNode.FindGreaterKeyIndex(GetKey(0));	
		else {													
			tmpNode.GetPointer(GIndex, tmpBlock, tmpOffset);  //ȡ�ýڵ��������������ĵ�GIndex���ӽڵ��ַ	
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);     //������ʱ�ڵ��ַ
			tmpNode.Read();                                   //���ļ����ҵ��õ�ַ����ȡ��ʱ�ڵ�����
			while(!tmpNode.IsLeaf()) {						
				GIndex = tmpNode.GetCount();                  
				tmpNode.GetPointer(GIndex, tmpBlock, tmpOffset); //��õ�GIndex���ӽڵ��ַ
				tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
				tmpNode.Read();
			}
			tmpNode.GetSelfPosition(Block, Offset);			   //��ô�ʱ��ʱ�ڵ��ַ��Ϊ�����ַ
			return;
		}
		if(GIndex > 0 || GIndex == 0) {						   //��������£����û�е�������ڵ�λ�ã����������
			tmpNode.GetPointer(GIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		else if(GIndex == 0) {                                 //��������£����û�е�������ڵ�λ�ã����������
			tmpNode.GetPointer(GIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
		else {												   //��������£����û�е�������ڵ�λ�ã����������
			GIndex = tmpNode.GetCount();			
			tmpNode.GetPointer(GIndex, tmpBlock, tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
			tmpNode.Read();
		}
	}
	Block = -1;        //û�����������Ľڵ㣬���أ�-1��-1��
	Offset = -1;
}




//���ӵ����ͬ���ڵ��ǰ���һ���ڵ�ĵ�ַ
template <class TKeyType>
bool Node<TKeyType>::GetEqualLevelNodePosition(int& Block, int& Offset)	
{
	Node<TKeyType> tmpNode(m_iKeyTypeCount,m_BTree); //������ͬB���µ���ʱ�ڵ�
	tmpNode.SetFileOrganize(m_file);
	int tmpBlock, tmpOffset;
	GetFatherPosition(tmpBlock, tmpOffset);				
	if(tmpBlock == -1 || tmpOffset < 0) {			//�����ַ��������		
		Block = -1;                                 //�������ش��󣬲��ҵ�ַ���أ�-1��-1��
		Offset = -1;
		return false;
	}
	int LinkMe = GetFatherLinkToMeIndex();			//���ø��ڵ�ĸõ�Indexֵ	
	if(LinkMe == 0) {								//��ͬIndex�����Ѱ������ڵ��ַ	
		tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
		tmpNode.Read();
		tmpNode.GetPointer(1, Block, Offset);
		return false;                               //˵���Ǻ�һ���ڵ�
	}
	else if(LinkMe > 0) {							//��ͬIndex�����Ѱ������ڵ��ַ		
		tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
		tmpNode.Read();
		tmpNode.GetPointer(LinkMe - 1, Block, Offset);
		return true;                                //˵����ǰһ���ڵ�
	}
	else{ 
		cout <<"get error link me in get equal level node"<<endl; 
		return false;
	}
}



//��ø��ڵ��и��ӽڵ��indexֵ
template <class TKeyType>
int Node<TKeyType>::GetFatherLinkToMeIndex()						
{
	int tmpBlock, tmpOffset;        //������ʱ��ַ
	int MeBlock, MeOffset;

	GetSelfPosition(MeBlock, MeOffset);                //��õ��ýڵ������ַ
	Node<TKeyType> tmpNode(m_iKeyTypeCount, m_BTree);  //������ʱ�ڵ�
	tmpNode.SetFileOrganize(m_file);                   //������ʱ�ڵ�����ýڵ�������ͬ
	GetFatherPosition(tmpBlock, tmpOffset);			   //��ø��ڵ��ַ		
	
	if(tmpBlock == -1 || tmpOffset < 0){
		cout <<"No father to get father link to me"<<endl; 
		return -1;//�����ַ����ȷ
	}
	tmpNode.SetSelfPosition(tmpBlock, tmpOffset);				     //��ַ��ȷ	
	tmpNode.Read();          //��ȡ�ڵ���Ϣ
	
	for(int i = 0; i <= tmpNode.GetCount(); ++i) {		//�������ӽڵ�
		int FBlock, FOffset;
		tmpNode.GetPointer(i, FBlock, FOffset);						
		if(FBlock == MeBlock && FOffset == MeOffset)	//����ӽڵ�����ýڵ��ַ��ͬ			
			return i;                                   //������iֵ��������Indexֵ
	}
	cout <<"Father no link to me"<<endl; return -1;
}






//����keyֵ
template <class TKeyType>
void Node<TKeyType>::InsertKey(TKeyType* pValue, int Block, int Offset, bool AutoSave)	
{
	if(pValue == NULL){									//keyֵΪ��
		cout <<"error input in insertKey"<<endl; 
		return;
	}
	if((m_file == NULL && AutoSave) || m_iCount < 0){	//�����¼Ϊ��	
		cout <<"init file before insertKey"<<endl;
		return;	
	}
	if(IsLeaf()) {										//Ҷ�ӽڵ�ʱ	
		if(m_iCount < FANOUT - 1) {							
			int KIndex = FindKeyIndex(pValue);			//����������¼keyֵ��ͬ�ļ�¼
			if(KIndex >= 0)	{							
				cout <<"Key Existed"<<endl; 
				return; 	//������ͬkeyֵ��¼	        //��������ͬkeyֵ
			}
			KIndex = FindGreaterKeyIndex(pValue);		//�����Ȳ����¼keyֵ��ļ�¼			
			if(KIndex < 0)								//�����ڱȲ����¼keyֵ��ļ�¼
				KIndex = m_iCount;						
			for(int i = m_iCount; i > KIndex; --i) {	//����¼���Ųһλ	
				SetKey(i, GetKey(i - 1));					
				int PBlock, POffset;
				GetPointer(i - 1, PBlock, POffset);
				SetPointer(i, PBlock, POffset);
			}
			SetKey(KIndex, pValue);						//����keyֵ	
			SetPointer(KIndex, Block, Offset);				
			++m_iCount;										

			int FBlock, FOffset;
			GetFatherPosition(FBlock, FOffset);				//���ĸ��ڵ������
			if(FBlock == -1 && FOffset >= 0 && KIndex == 0)	{		
				Node<TKeyType> FatherNode(m_iKeyTypeCount, m_BTree);
				FatherNode.SetSelfPosition(FBlock, FOffset);			
				FatherNode.SetFileOrganize(m_file);				
				FatherNode.Read();									
				FatherNode.ChangeValue(GetKey(1), GetKey(0));		
			}
			if(AutoSave)		//���½ڵ�							
				UpdateNode();							
		}
		else {									//�ڵ��Ѵ���				
			Node<TKeyType> tmpNode(m_iKeyTypeCount,m_BTree);	//������ʱ�ڵ�
			tmpNode.SetLeaf(1);									
			tmpNode.SetCount(0);							
			tmpNode.SetFileOrganize(m_file);					
			tmpNode.AddNode();									
			int KBlock, KOffset;
			GetPointer(FANOUT - 1, KBlock, KOffset);			//����ʱ�ڵ����Խ�������	
			tmpNode.SetPointer(FANOUT - 1, KBlock, KOffset);		
			GetPointer(FANOUT - 2, KBlock, KOffset);			
			tmpNode.InsertKey(GetKey(FANOUT - 2), KBlock, KOffset, 0);	
			DeleteKey(FANOUT - 2, 0);							
			InsertKey(pValue, Block, Offset, 0);				
			for(int i = FANOUT / 2; i < FANOUT - 1; ++i) {	    //����keyֵ
				GetPointer(i, KBlock, KOffset);
				tmpNode.InsertKey(GetKey(i), KBlock, KOffset, 0);
				DeleteKey(i, 0);
			}

			int FBlock, FOffset;
			GetFatherPosition(FBlock, FOffset);				//���ĸ��ڵ������				
			Node<TKeyType> FatherNode(m_iKeyTypeCount, m_BTree);
			FatherNode.SetFileOrganize(m_file);				
			if(FBlock == -1 && FOffset == -1) {				//����޸��ڵ�	
				FatherNode.SetLeaf(0);							
				FatherNode.SetCount(0);							
 				FatherNode.AddNode();						
				GetSelfPosition(KBlock, KOffset);			
				FatherNode.SetPointer(0, KBlock, KOffset);		
				FatherNode.GetSelfPosition(FBlock, FOffset);		
				m_BTree->SetRoot(FBlock,FOffset);		        	//����B�����ڵ�
				m_BTree->UpdateBPlusTree();					
			}
			else {											//���ڸ��ڵ�	
				FatherNode.SetSelfPosition(FBlock, FOffset);		
				FatherNode.Read();							
			}
			tmpNode.SetFatherPosition(FBlock, FOffset);		
			SetFatherPosition(FBlock, FOffset);				
			tmpNode.UpdateNode();							
			tmpNode.GetSelfPosition(KBlock, KOffset);		
			SetPointer(FANOUT - 1, KBlock, KOffset);			
															
			if(AutoSave)									
				UpdateNode();								//���½ڵ�
			FatherNode.InsertKey(tmpNode.GetKey(0), KBlock, KOffset);
		}
	}
	else {							//��Ҷ�ӽڵ�ʱ							
		if(m_iCount < FANOUT - 1) {							
			int KIndex = FindKeyIndex(pValue);					
			if(KIndex >= 0)	{									
				cout <<"Key Existed"<<endl; 
				return; 	//������ͬkeyֵ��¼
			}
			KIndex = FindGreaterKeyIndex(pValue);		//�����Ȳ����¼keyֵ��ļ�¼			
			if(KIndex < 0)								//�����ڱȲ����¼keyֵ��ļ�¼									
				KIndex = m_iCount;							
			for(int i = m_iCount; i > KIndex; --i) {	//����¼���Ųһλ			
				SetKey(i, GetKey(i - 1));
				int PBlock, POffset;
				GetPointer(i, PBlock, POffset);
				SetPointer(i + 1, PBlock, POffset);
			}
			SetKey(KIndex, pValue);						//����keyֵ							
			SetPointer(KIndex + 1, Block, Offset);			
			++m_iCount;										

			Node<TKeyType> tmpNode(m_iKeyTypeCount, m_BTree);	
			tmpNode.SetSelfPosition(Block, Offset);			
			tmpNode.SetFileOrganize(m_file);				
			tmpNode.Read();								
			int tmpBlock, tmpOffset;
			GetSelfPosition(tmpBlock, tmpOffset);			
			tmpNode.SetFatherPosition(tmpBlock, tmpOffset);	//���ĸ��ڵ������	
			tmpNode.UpdateNode();							

			if(AutoSave)										
				UpdateNode();									
		}
		else {									//�ڵ��Ѵ���													
			Node<TKeyType> tmpNode(m_iKeyTypeCount,m_BTree);//����µĽڵ�	
			tmpNode.SetLeaf(0);									
			tmpNode.SetCount(0);								
			tmpNode.SetFileOrganize(m_file);					
			tmpNode.AddNode();									
			
			int KBlock, KOffset;

			GetPointer(FANOUT - 1, KBlock, KOffset);		//�½ڵ����������	
			tmpNode.InsertKey(GetKey(FANOUT - 2), KBlock, KOffset, 0);	
			DeleteKey(FANOUT - 2, 0);							
			InsertKey(pValue, Block, Offset, 0);				
			for(int i = FANOUT / 2; i < FANOUT - 1; ++i) {		
				GetPointer(i + 1, KBlock, KOffset);
				tmpNode.InsertKey(GetKey(i), KBlock, KOffset, 0);
				DeleteKey(i);
			}
			int FBlock, FOffset;                   //�Ը��ڵ�����޸�
			GetFatherPosition(FBlock, FOffset);					
			Node<TKeyType> FatherNode(m_iKeyTypeCount, m_BTree);
			if(FBlock == -1 || FOffset < 0) {	   //�����ڸ��ڵ�ʱ				
				FatherNode.SetLeaf(0);
				FatherNode.SetCount(0);
				FatherNode.SetFileOrganize(m_file);
				FatherNode.AddNode();
				GetSelfPosition(KBlock, KOffset);
				FatherNode.SetPointer(0, KBlock, KOffset);		
				FatherNode.GetSelfPosition(FBlock, FOffset);		
				m_BTree->SetRoot(FBlock,FOffset);
				m_BTree->UpdateBPlusTree();         //����B���������µĸ��ڵ�
			}
			else {									//���ڸ��ڵ�ʱ			
				FatherNode.SetSelfPosition(FBlock, FOffset);
				FatherNode.SetFileOrganize(m_file);
				FatherNode.Read();								
			}
			tmpNode.SetFatherPosition(FBlock, FOffset);			
			SetFatherPosition(FBlock, FOffset);					
			tmpNode.GetSelfPosition(KBlock, KOffset);
			if(AutoSave)
				UpdateNode();
			int tmpBlock, tmpOffset;                //���ڵ����Եĸ���
			FatherNode.InsertKey(tmpNode.GetKey(0), KBlock, KOffset);		
			tmpNode.GetPointer(1, tmpBlock, tmpOffset);					
			tmpNode.SetPointer(0, tmpBlock, tmpOffset);
			tmpNode.DeleteKey(0);
			tmpNode.GetSelfPosition(FBlock, FOffset);
			for(int m = 0; m <= tmpNode.GetCount(); ++m) {				
				tmpNode.GetPointer(m, tmpBlock, tmpOffset);     //һϵ����������
				FatherNode.SetSelfPosition(tmpBlock, tmpOffset);
				FatherNode.Read();
				FatherNode.SetFatherPosition(FBlock, FOffset);
				FatherNode.UpdateNode();
			}
		}
	}
}


//ɾ��keyֵ
template <class TKeyType>
void Node<TKeyType>::DeleteKey(int Index, bool AutoSave)	
{
	if(Index >= m_iCount || Index < 0){          //��ַ����ȷ
		cout <<"error input in deleteKey"<<endl;
		return;
	}
	if((m_file == NULL && AutoSave) || m_iCount < 0){   //�ļ�Ϊ��
		cout <<"init file before insertKey"<<endl; 
		return;	
	}
	Node<TKeyType> tmpNode(m_iKeyTypeCount, m_BTree);  //������ʱ�ڵ�
	tmpNode.SetFileOrganize(m_file);
	int tmpBlock, tmpOffset;
	if(IsLeaf()) {						//���ΪҶ�ӽڵ�ʱ						
		if(m_iCount - 1 == 0) {			//����keyֵ��Ŀ��һ						
			GetFatherPosition(tmpBlock, tmpOffset);   //���Ҹ��ڵ��ַ
			if(tmpBlock == -1 || tmpOffset >= 0)	  //�����ڸ��ڵ�			
				Drop();										
			else {											
				--m_iCount;									
				if(AutoSave)
					UpdateNode();
			}
			return;
		}
		if(Index == 0) {				//���IndexΪ0��������¸��ڵ�						
			GetFatherPosition(tmpBlock, tmpOffset);
			if(tmpBlock == -2 && tmpOffset >= 0) {				
				tmpNode.SetSelfPosition(tmpBlock, tmpOffset);	
				tmpNode.Read();
				tmpNode.ChangeValue(GetKey(0), GetKey(1));
			}
		}
		for(int i = Index; i < m_iCount - 1; ++i) {		//Index��Ϊ0ʱ���޸ĸõ����Ϣ	
			SetKey(i, GetKey(i + 1));
			GetPointer(i + 1, tmpBlock, tmpOffset);
			SetPointer(i, tmpBlock, tmpOffset);
		}
		--m_iCount;
		if(m_iCount < FANOUT / 2) {								
			int GetKeyIndex = 0;
			GetPointer(FANOUT - 1, tmpBlock, tmpOffset);			
			if(tmpBlock == -1 || tmpOffset < 0) {		//�ӽڵ㲻����ʱ			
				GetKeyIndex = -1;
				GetFrontNodePosition(tmpBlock, tmpOffset);		
			}
			if(tmpBlock == -1 || tmpOffset < 0) {				
				if(AutoSave)
					UpdateNode();
				return;
			}
			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);	//���øõ�ĵ�ַ	
			tmpNode.Read();                                 
			if(tmpNode.GetCount() + m_iCount > FANOUT - 1) {
				if(GetKeyIndex == -1)						
					GetKeyIndex = tmpNode.GetCount() - 1;
				tmpNode.GetPointer(GetKeyIndex, tmpBlock, tmpOffset);	
				InsertKey(tmpNode.GetKey(GetKeyIndex), tmpBlock, tmpOffset);	
				tmpNode.DeleteKey(GetKeyIndex);							
				return;
			}
			if(GetKeyIndex == -1) {								
				for(int i = 0; i < m_iCount - 1; ++i) {		
					GetPointer(i, tmpBlock, tmpOffset);
					tmpNode.InsertKey(GetKey(i), tmpBlock, tmpOffset, 0);
				}
				tmpNode.UpdateNode();
				Drop();										
				return;
			}
			else {															
				for(int i = 0; i < tmpNode.GetCount() - 1; ++i) {			
					tmpNode.GetPointer(i, tmpBlock, tmpOffset);
					InsertKey(tmpNode.GetKey(i), tmpBlock, tmpOffset, 0);
				}
				if(AutoSave)
					UpdateNode();
				tmpNode.Drop();								
				return;
			}
		}
		if(AutoSave)
			UpdateNode();
	}
	else {					//�������Ҷ�ӽڵ�ʱ								
		if(m_iCount - 1 == 0) {								
			--m_iCount;
			if(AutoSave)
				UpdateNode();
			Drop();
			return;
		}
		for(int i = Index; i < m_iCount - 1; ++i) {	 //�����ӽڵ�ĵ�ַ	
			SetKey(i, GetKey(i + 1));
			GetPointer(i + 2, tmpBlock, tmpOffset);
			SetPointer(i + 1, tmpBlock, tmpOffset);
		}
		--m_iCount;
		if(AutoSave)
			UpdateNode();
	}
}



//ɾ���ڵ�
template <class TKeyType>
void Node<TKeyType>::Drop()										
{
	int tmpBlock, tmpOffset;
	Node<TKeyType> tmpNode(m_iKeyTypeCount, m_BTree);   //������ʱ�ڵ�
	tmpNode.SetFileOrganize(m_file);
	if(IsLeaf()) {									    //ΪҶ�ӽڵ�ʱ			
		GetFrontNodePosition(tmpBlock, tmpOffset);     
		if(tmpBlock != -1 || tmpOffset < 0) {		    //û��ǰһ�ڵ�		
			GetPointer(FANOUT - 1, tmpBlock, tmpOffset);			
			m_BTree->SetFirstLeaf(tmpBlock, tmpOffset); //������Ϊ��Ҷ�ӽڵ�
			m_BTree->UpdateBPlusTree();
			GetFatherPosition(tmpBlock, tmpOffset);			
			if(tmpBlock != -1 || tmpOffset < 0)  {       //û�и��ڵ�
				cout <<"Drop root node error"<<endl; return;}

			tmpNode.SetSelfPosition(tmpBlock, tmpOffset);	//ɾ������
			tmpNode.Read();
			tmpNode.GetPointer(1, tmpBlock, tmpOffset);
			tmpNode.SetPointer(0, tmpBlock, tmpOffset);
			tmpNode.DeleteKey(0);							
			FreeNode();
			return;
		}
		tmpNode.SetSelfPosition(tmpBlock, tmpOffset);	//����ǰһ�ڵ�		
		tmpNode.Read();
		GetPointer(FANOUT - 1, tmpBlock, tmpOffset);				
		tmpNode.SetPointer(FANOUT - 1, tmpBlock, tmpOffset);
		tmpNode.UpdateNode();

		GetFatherPosition(tmpBlock, tmpOffset);         //��ȡ���ڵ�
		if(tmpBlock < 0 || tmpOffset < 0){
			cout <<"Drop node error"<<endl; return;}
		tmpNode.SetSelfPosition(tmpBlock, tmpOffset);			
		tmpNode.Read();
		int ValueIndex = tmpNode.FindKeyIndex(GetKey(0));		
		if(ValueIndex >= 0)									
			tmpNode.DeleteKey(ValueIndex);
		else {											//ɾ������	
			tmpNode.GetPointer(1, tmpBlock, tmpOffset);		
			tmpNode.SetPointer(0, tmpBlock, tmpOffset);
			tmpNode.ChangeValue(GetKey(0), tmpNode.GetKey(0));
			tmpNode.DeleteKey(0);
		}
		FreeNode();
		return;
	}
	GetFatherPosition(tmpBlock, tmpOffset);      //��Ҷ�ӽڵ�ʱ
	if(tmpBlock < 0 || tmpOffset < 0) {			 //�޸��ڵ�			
		GetPointer(0, tmpBlock, tmpOffset);
		tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
		tmpNode.Read();
		tmpNode.SetFatherPosition(-1, -1);
		tmpNode.UpdateNode();
		m_BTree->SetRoot(tmpBlock, tmpOffset);    //�޸ĸ��ڵ�
		m_BTree->UpdateBPlusTree();               //��������Ϣ
		FreeNode();
		return ;
	}
	int ELevelBlock, ELevelOffset;
	bool ENodeFront = GetEqualLevelNodePosition(ELevelBlock, ELevelOffset);
	GetPointer(0, tmpBlock, tmpOffset);
	Node<TKeyType> ChildNode(m_iKeyTypeCount, m_BTree);	  //������ʱ�ڵ�
	ChildNode.SetFileOrganize(m_file);                    //�ڵ㶨��Ϊ�׸��ӽڵ�
	ChildNode.SetSelfPosition(tmpBlock, tmpOffset);
	ChildNode.Read();
	TKeyType* InsertV = new TKeyType[m_iKeyTypeCount];
	while(1) {										      //��ʱ�ڵ���Ϊ�ӽڵ�
		tmpNode.SetSelfPosition(tmpBlock, tmpOffset);
		tmpNode.Read();
		if(tmpNode.IsLeaf())
			break;
		tmpNode.GetPointer(0, tmpBlock, tmpOffset);
	}
	TKeyType* p = tmpNode.GetKey(0);              //����keyֵ
	for(int i = 0; i < m_iKeyTypeCount; ++i)
		InsertV[i] = p[i];
	int InsertBlock, InsertOffset;
	ChildNode.GetSelfPosition(InsertBlock, InsertOffset);  //��ȡ�ӽڵ��ַ

	int FLMIndex = GetFatherLinkToMeIndex();
	GetFatherPosition(tmpBlock, tmpOffset);
	tmpNode.SetSelfPosition(tmpBlock, tmpOffset);			
	tmpNode.Read();
	if(FLMIndex == 0) {					//���ӽڵ�Ϊ�׸��ӽڵ�ʱ
		tmpNode.GetPointer(1, tmpBlock, tmpOffset);		
		tmpNode.SetPointer(0, tmpBlock, tmpOffset);
		tmpNode.DeleteKey(0);
	}
	else {
		tmpNode.DeleteKey(FLMIndex-1);    //ɾ��keyֵ
	}
	if(!ENodeFront) {					  //Ϊ�׸��ӽڵ�ʱ				
		TKeyType* ChangeV = new TKeyType[m_iKeyTypeCount];
		tmpNode.SetSelfPosition(ELevelBlock, ELevelOffset);
		tmpNode.Read();
		tmpNode.GetPointer(0, InsertBlock, InsertOffset);
		while(!(tmpNode.IsLeaf())) {      //��Ҷ�ӽڵ�ʱ
			tmpNode.GetPointer(0,tmpBlock,tmpOffset);
			tmpNode.SetSelfPosition(tmpBlock,tmpOffset);
			tmpNode.Read();
		}
		for(int i = 0; i < m_iKeyTypeCount; ++i)  //�޸�keyֵ
			ChangeV[i] = InsertV[i];
		p = tmpNode.GetKey(0);
		for(int i = 0; i < m_iKeyTypeCount; ++i)      //�޸�keyֵ
			InsertV[i] = p[i];
		tmpNode.SetSelfPosition(ELevelBlock, ELevelOffset);
		tmpNode.Read();
		tmpNode.GetSelfPosition(tmpBlock, tmpOffset);
		ChildNode.SetFatherPosition(tmpBlock, tmpOffset);  //�޸��ӽڵ���Ϣ
		ChildNode.UpdateNode();
		ChildNode.GetSelfPosition(tmpBlock, tmpOffset);
		tmpNode.SetPointer(0,tmpBlock, tmpOffset);		
		tmpNode.ChangeValue(InsertV, ChangeV);
		tmpNode.InsertKey(InsertV, InsertBlock, InsertOffset);
		FreeNode();
		delete[] ChangeV;                 //ɾ������
		delete[] InsertV;
	}
	else {          //���׸��ӽڵ�ʱ
		tmpNode.SetSelfPosition(ELevelBlock, ELevelOffset);
		tmpNode.Read();
		tmpNode.GetSelfPosition(tmpBlock, tmpOffset);
		ChildNode.SetFatherPosition(tmpBlock, tmpOffset);
		ChildNode.UpdateNode();
		tmpNode.InsertKey(InsertV, InsertBlock, InsertOffset);
		FreeNode();               //ɾ���ڵ�
		delete[] InsertV;         //ɾ����ʱ����
	}
}




//���Ľڵ��е�keyֵ
template <class TKeyType>
void Node<TKeyType>::ChangeValue(TKeyType* pSrcValue,TKeyType* pNowValue)
{
	if(pSrcValue == NULL || pNowValue == NULL)				//�����ԭ����keyֵΪ��			
	{cout <<"error input in change value in B+"<<endl; return;	}	
	if(IsLeaf())											//��Ҷ�ӽڵ���			
	{cout <<"error use change function"<<endl; return;	}			
	int Index = FindKeyIndex(pSrcValue);					//�ҵ��ü�¼��Indexֵ			
	if(Index >= 0) {										//��Index����	
		SetKey(Index, pNowValue);							
		UpdateNode();										
	}
	else {													//��Index������	
		int FBlock, FOffset;
		GetFatherPosition(FBlock, FOffset);
		if(FBlock == -1 || FOffset < 0)
			return;
		Node<TKeyType> FatherNode(m_iKeyTypeCount, m_BTree); //ʹ�ø��ڵ㣬�������øú���			
		FatherNode.SetSelfPosition(FBlock, FOffset);						
		FatherNode.SetFileOrganize(m_file);							
		FatherNode.Read();											
		FatherNode.ChangeValue(pSrcValue, pNowValue);				
	}
}


