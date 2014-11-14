#include "API_Module.h"
#include "Catalog_Manager.h"
#include "Interpreter.h"

extern CatalogManager cm;

void APIMoudule::API(SQLstatement &s)
{
	if (s.type == CREATE_TABLE){
		if (cm.API_Catalog(s)){
			// ��record
		}
		else{
			cout << "create table failed" << endl;
		}
	}
	else if (s.type == CREATE_INDEX){
		if (cm.API_Catalog(s)){
			// ��index
		}
		else{
			cout << "create index failed" << endl;
		}
	}
	else if (s.type == DROP_TABLE){
		if (cm.API_Catalog(s)){
			// ��index��record
		}
		else{
			cout << "drop table failed" << endl;
		}
	}
	else if (s.type == DROP_INDEX){
		if (cm.API_Catalog(s)){
			// ��index
		}
		else{
			cout << "create index failed" << endl;
		}
	}
	else if (s.type == SELECT){
		if (cm.API_Catalog(s)){
			cout << "right select statement." << endl;
			// ��index��record
		}
		else{
			cout << "select failed." << endl;
		}
	}
	else if (s.type == SELECT_WHERE){
		if (cm.API_Catalog(s)){
			cout << "right select where statement." << endl;
			// ��index��record
		}
		else{
			cout << "select failed." << endl;
		}
	}
	else if (s.type == INSERT){
		if (cm.API_Catalog(s)){
			cout << "right insert statement." << endl;
			// ��record��index
		}
		else{
			cout << "insert failed." << endl;
		}
	}
	else if (s.type == DELETE){
		if (cm.API_Catalog(s)){
			cout << "right delete statement." << endl;
			// ��record��index
		}
		else{
			cout << "insert failed." << endl;
		}
	}
	else if (s.type == DELETE_WHERE){
		if (cm.API_Catalog(s)){
			cout << "right delete where statement." << endl;
			// ��record��index
		}
		else{
			cout << "insert failed." << endl;
		}
	}
	else if (s.type == EXECFILE){
		fstream fin(s.tableName);
		if (fin){
			string sql;
			string temp;
			bool finish = false;
			sql = "";
			while (fin >> temp){
				sql = sql + ' ' + temp;
				if (sql.at(sql.length() - 1) == ';'){
					sql.erase(0, 1);
					sql = lower(sql);
					cout << sql << endl;
					sql = Interpreter(sql);
					SQLstatement s(sql);
					API(s);
					sql = "";
				}
			}
		}
		else{
			cout << "no such file." << endl;
		}
	}
	else if (s.type == QUIT){
		exit(0);
	}
	else if (s.type == HELP){
		getHelp();
	}
	else if (s.type == ILLEGAL){
		cout << "wrong SQL statement." << endl;
	}
}

void APIMoudule::getHelp()
{
	fstream fin("help.txt");
	string s;
	while (getline(fin, s))
	{
		cout << s << endl;
	}
}

//��ȡ�û�����
string APIMoudule::read_input()
{
	string SQL;
	string temp;
	bool finish = false;
	SQL = "";
	while (!finish)
	{
		cin >> temp;
		SQL = SQL + ' ' + temp;
		if (SQL.at(SQL.length() - 1) == ';'){
			SQL.erase(0, 1);
			finish = true;
		}
	}
	//�������дת��ΪСд
	SQL = lower(SQL);
	//�����û�����
	return SQL;
}

//��ΪСд
string APIMoudule::lower(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}