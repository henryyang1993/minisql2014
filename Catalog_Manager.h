#ifndef __MiniSQL__Catalog__Manager__
#define __MiniSQL__Catalog__Manager__

#include "MiniSQL.h"

class CatalogManager
{
	vector<Table> tableList; // ����Ϣ�б�
	vector<Index> indexList; // ������Ϣ�б�
	int tableNum; // �����Ŀ
	
public:
	CatalogManager(){};
	~CatalogManager(){};
	bool API_Catalog(SQLstatement &sql); // API��cm�ĵ���

	Table* findTable(string tn); // �ҵ�����ָ��
	bool createTable(SQLstatement &sql); // ������
	bool checkInsert(Table *t, string value); // ������
	void pushBack_tableList(Table &t); // �����tableList
	void update_tableNum(bool add); // ���±���Ŀ
	bool checkType(Attribute *a, string v); // �������
	bool checkAttribute(Table *t, vector<Attribute> *a, bool index); // �������
	bool checkCondition(Table *t, vector<Condition> *c); // �������
	Index* findIndex(string in); // �ҵ���������ָ��
	bool createIndex(SQLstatement &sql, Table *t); // ��������
	bool clearIndex(Table *t, string in, bool drop); // �������
	void pushBack_indexList(Index &i); // ��������indexList
	bool dropIndex(Table *t, Index *i); // ɾ������
	bool dropTable(Table *t); // ɾ����
	bool save_tableInfo(Table *t, bool add); // �������Ϣ
	void writeAttribute(string fn, Attribute *a); // ��������
	void read_tableInfo(); // ��������Ϣ
	void update_tableInfo(); // ���±���Ϣ
	bool save_indexInfo(Index *i, bool add); // ����������Ϣ
	void read_indexInfo(); // ����������Ϣ
	void update_indexInfo(); // ����������Ϣ

	string msg; // ������Ϣ
};

#endif /* defined(__MiniSQL__Catalog__Manager__) */