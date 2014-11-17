#include "Index_Manager.h"
#include <stdio.h>
#include <windows.h> 

using namespace std;


void IndexManager::Create_Index(string indexName, Attribute attribute)
{
	CFileFind finder;

	LPCSTR f = indexName.c_str();

	BOOL bWorking=finder.FindFile(f);

	
	if(bWorking)
		cout<<"The index name has already exist!"<<endl;
	else if(attribute.isUnique == 0)
		cout<<"Can't create an index here, because thhe attribute is not unique!"<<endl;
	else
	{
		attribute.indexName = indexName;

		TYPE Type = attribute.type;
		CString KeyType;

		if(Type == MYINT)
			KeyType = "int";
		else if(Type == MYFLOAT)
			KeyType = "float";
		else if(Type == MYCHAR)
			KeyType = "char"; 

		int KeyTypeCount;

		if(Type == MYINT || Type == MYFLOAT)
			KeyTypeCount = 1;
		else
			KeyTypeCount = attribute.length;

		LPCSTR relative = indexName.c_str();

		CFile file(relative ,CFile::modeCreate | CFile::modeReadWrite);
		CFile* myFile;

		myFile = &file;

		BTree CI(KeyType, KeyTypeCount);

		CI.SetFileOrganize(myFile);

		CI.CreateBPlusTree();

		file.Close();  
		
	}
}




void IndexManager::Drop_Index(string indexName)
{

	LPCSTR relative = indexName.c_str();

	DeleteFileA(relative);	

}



void IndexManager::Drop_Table_Index(Table *Table)
{
	vector<Attribute>::iterator iter;
	for(iter = Table->attributes.begin(); iter != Table->attributes.end(); iter++)
	{
		if(iter->indexName != "")
		{
			Drop_Index(iter->indexName);
		}
	}

}

void IndexManager::Delete_Without_Where_Index(Table *Table)
{
	vector<Attribute>::iterator iter;
	for(iter = Table->attributes.begin(); iter != Table->attributes.end(); iter++)
	{
		if(iter->indexName != "")
		{
			Drop_Index(iter->indexName);
			Create_Index(iter->indexName, *iter);
		}

	}
}



void IndexManager::Insert_Index(Table *Table, string s, int block, int position)
{
	int slength;
	string temp;
	string indexName;
	slength = s.length();
	int i=0;

	vector<Attribute>::iterator iter;
	for(iter = Table->attributes.begin(); iter != Table->attributes.end(); iter++)
	{
		if(iter->isUnique == 0)
		{
			i = s.find_first_of(',',i)+1;
			
		}
		else
		{
			temp = s.substr(i, (s.find_first_of(',',i)-i));
			
			if((indexName = iter->indexName) == ""){
				return ;
			}
			
			TYPE Type = iter->type;

			CString KeyType;

			if(Type == MYINT)
				KeyType = "int";
			else if(Type == MYFLOAT)
				KeyType = "float";
			else //if(Type == MYCHAR)
				KeyType = "char"; 

			int KeyTypeCount;

			if(Type == MYINT || Type == MYFLOAT)
				KeyTypeCount = 1;
			else
				KeyTypeCount = iter->length;

			LPCSTR relative = indexName.c_str();

			CFile file(relative , CFile::modeReadWrite);
			CFile* myFile;

			myFile = &file;

			BTree II(KeyType, KeyTypeCount);

			II.SetFileOrganize(myFile);

			//II.CreateBPlusTree();

			II.Read();

			int root = II.GetRoot();
			if(root<0)
				II.CreateBPlusTree();


			int v1[1];
			float v2[1];
			char v3[100];
			if(Type == MYINT)
			{
				KeyType = "int";
				const char* c_s = temp.c_str();

				*v1 = atoi(c_s);

				II.InsertValue(v1, block, position);
			}
			else if(Type == MYFLOAT)
			{
				KeyType = "float";
				const char* c_s = temp.c_str();

				*v2 = atof(c_s);

				II.InsertValue(v2, block, position);

			}
			else //if(Type == MYCHAR)
			{
				KeyType = "char"; 
				temp.copy(v3, temp.length()-1, 1);

				v3[temp.length()-1]='\0';

				II.InsertValue(v3, block, position);
			}

			i = s.find_first_of(',',i)+1;



		

			file.Close();
		}

	}

}



bool IndexManager::Find_Index(Attribute attribute, string s, int& block, int& position)
{
	string indexName;
	indexName = attribute.indexName;
	if(indexName == "")
		return FALSE;
	else
	{
		TYPE Type = attribute.type;
		CString KeyType;

		

		int KeyTypeCount;

		if(Type == MYINT || Type == MYFLOAT)
			KeyTypeCount = 1;
		else
			KeyTypeCount = attribute.length;

		LPCSTR relative = indexName.c_str();

		if(Type == MYINT)
			KeyType = "int";
		else if(Type == MYFLOAT)
			KeyType = "float";
		else if(Type == MYCHAR)
			KeyType = "char"; 

		CFile file(relative , CFile::modeReadWrite);
		CFile* myFile;

		myFile = &file;

		BTree FI(KeyType, KeyTypeCount);

		FI.SetFileOrganize(myFile);

		FI.Read();


		int v1[1];
		float v2[1];
		char v3[100];
		if(Type == MYINT)
		{
			const char* c_s = s.c_str();

			*v1 = atoi(c_s);

			return FI.FindValue(v1, block, position);
		}
		else if(Type == MYFLOAT)
		{

			const char* c_s = s.c_str();

			*v2 = atof(c_s);

			return FI.FindValue(v2, block, position);

		}
		else //if(Type == MYCHAR)
		{

			s.copy(v3, s.length(), 0);
			v3[s.length()]='\0';


			return FI.FindValue(v3, block, position);
		}


		file.Close();

	}

}