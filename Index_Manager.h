#ifndef __Index_Manager_H__
#define __Index_Manager_H__


#include "BPTree.h"
#include "Node.h"
#include "Buffer_Manager.h"
#include "Minisql.h"

using namespace std;

class IndexManager
{
public:
	 void Create_Index(string indexName, Attribute attribute);
	 void Drop_Index(string indexName);

	 void Drop_Table_Index(Table *Table);
	 void Delete_Without_Where_Index(Table *Table);
	 void Insert_Index(Table *Table, string s, int block, int position);

	 bool Find_Index(Attribute attribute, string s, int& block, int& position);

};


#endif // __Index_Manager_H__