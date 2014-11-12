#if !defined(__MINISQL__H__)
#define __MINISQL__H__

#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <direct.h>
#include <algorithm>
#include <vector>
using namespace std;

#define FILE_COUNTER 1
#define FILE_INFO_LENGTH 671
#define ATTR_INFO_LENGTH 20
#define FILE_HEAD_LENGTH 31

enum TYPE { INT = 0, FLOAT = 1, CHAR = 2 };
enum RELATION_TYPE { EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, SMALLER, SMALLER_EQUAL, AND, OR};
enum STATEMENT_TYPE { CREATE_DATABASE, CREATE_TABLE, CREATE_INDEX, DROP_TABLE, DROP_INDEX, DROP_DATABASE, SELECT, SELECT_WHERE, INSERT, DELETE, DELETE_WHERE, USE, EXECFILE, QUIT, HELP, ILLEGAL};

//�����������ڵ���Ϣ
struct index_info
{
	string index_name;
	int length;
	char type;
	long offset;
	string value;
};

//����������Ϣ
class Attribute
{
public:
	friend class CatalogManager;
	friend class Table;
	string name = "";
	string indexName = "";      //index��
	TYPE type = INT;
	int length = 0;
	bool isPrimaryKey = false;
	bool isUnique = false;

	Attribute(){}
	Attribute(string name, TYPE type, int length, bool isPrimaryKey = false, bool isUnique = false) :name(name), type(type), length(length), isPrimaryKey(isPrimaryKey), isUnique(isUnique){}
};


//����������Ϣ
class Condition
{
public:
	Attribute attribute;
	string value;
	RELATION_TYPE relationType;

	Condition(){}
	Condition(Attribute attribute, string value, RELATION_TYPE relationType) :attribute(attribute), value(value), relationType(relationType){}
};

//�������Ϣ
class Table
{
public:
	string name;							// ����
	int blockNum = 0;						// ��name.table��ռ�õ�block��
	int recordNum = 0;						// ��¼����
	int attriNum = 0;						// ������
	int tupleLength = 0;					// ÿ����¼�ĳ���
	string primaryKey;						// ��������
	//int freeNum = 0;						// �м�����ɾ���ļ�¼
	vector<Attribute> attributes;			// ָ��Ԫ���������ָ��
	vector<Attribute>::iterator AttriItor;	// Attribute��iterator
	//vector<string> data;					// ָ�����������ָ��
	//long dataBlockInFile = -1;			// data��ͷ��file�еĿ��λ�ã�ÿ�ű������һ���Ǵ�һ��Ŀ�ͷ��ʼ��
	//vector<string> emptyList;				// ָ��ȴ�ɾ�������ָ�루�ⶫ�����׸����ã�

	Table(){}
	Table(string name, string primaryKey) :name(name), primaryKey(primaryKey){}
};

// �����
class SQLstatement
{
public:
	STATEMENT_TYPE type;
	string tableName;
	vector<Attribute> attributes;
	vector<Condition> conditions;
	//vector<RELATION_TYPE> relations;
	string content;

	SQLstatement(STATEMENT_TYPE type, string tableName);
	SQLstatement(string SQL);
	~SQLstatement();
	void outputinfo();
	STATEMENT_TYPE identify(string type_code);
};
#endif