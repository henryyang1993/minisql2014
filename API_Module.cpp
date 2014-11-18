#include "API_Module.h"
#include "Catalog_Manager.h"
#include "Interpreter.h"
#include "Buffer_Manager.h"
#include "Record_Manager.h"
#include "Index_Manager.h"

extern CatalogManager cm;
RecordManager rm;
IndexManager im;
extern BufferManager bm;
extern bool quitFlag;
void APIMoudule::API(SQLstatement &s)
{
	if (s.type == CREATE_TABLE){
		if (cm.API_Catalog(s)){
			// 调record
		}
		else{
			cout << "create table failed" << endl;
		}
	}
	else if (s.type == CREATE_INDEX){
		if (cm.API_Catalog(s)){
			Table *t = cm.findTable(s.tableName);
			Attribute *a;
			vector<Attribute>::iterator iter;
			for(iter = t->attributes.begin(); iter != t->attributes.end(); iter++){
				if(iter->name == s.attributes.data()->name)
				{
					a = &(*iter);
				}
			}
			im.Create_Index(s.indexName, *a);
			// 调index
		}
		else{
			cout << "create index failed" << endl;
		}
	}
	else if (s.type == DROP_TABLE){
		if (cm.API_Catalog(s)){
			// 调index和record
			rm.dropTable(cm.msg);
		}
		else{
			cout << "drop table failed" << endl;
		}
	}
	else if (s.type == DROP_INDEX){
		if (cm.API_Catalog(s)){
			// 调index
			im.Drop_Index(s.indexName);
		}
		else{
			cout << "drop index failed" << endl;
		}
	}
	else if (s.type == SELECT){
		if (cm.API_Catalog(s)){
			//cout << "right select statement." << endl;
			// 调index和record
			int resultCount = rm.selectWithoutwhere(*cm.findTable(s.tableName), s.attributes);
			if (resultCount)
			{
				rm.outputMap(resultCount);
				cout << resultCount << " row(s) selected:" << endl;
			}
			else
			{
				cout << "no match row" << endl;
			}
		}
		else{
			cout << "select failed." << endl;
		}
	}
	else if (s.type == SELECT_WHERE){
		if (cm.API_Catalog(s)){
			//cout << "right select where statement." << endl;
			// 调index和record
			Table *t = cm.findTable(s.tableName);
			Index *i;
			if((i = cm.findIndex(cm.getAttributebya(t, s.conditions[0].attribute.name)->indexName)) && (s.conditions[0].relationType == EQUAL)){
				Attribute *a = cm.getAttributebyi(t, i->index_name);
				insertPos ip;
				string value="";
				vector<Attribute>::iterator iter;
				if(im.Find_Index(*a, s.conditions[0].value, ip.bufferNUM, ip.position)){
					if(s.attributes[0].name =="*"){
						for(iter = t->attributes.begin(); iter != t->attributes.end(); iter++){
							cout << iter->name <<  "\t\t";
						}
						cout << endl;
						vector<Attribute>::iterator iter2;
						int l = 1;
						for(iter2 = t->attributes.begin(); iter2 != t->attributes.end(); iter2 ++){
							value = rm.toString(bm.bufferBlock[ip.bufferNUM].value+ip.position+l, iter2->length, iter2->type); 
							if(value.length()>5){
								cout << value <<  "\t";
							}else{
								cout << value <<  "\t\t";
							}
							l += iter2->length;
						}
					}
					else{
						for(iter = s.attributes.begin(); iter != s.attributes.end(); iter++){
							cout << iter->name <<  "\t\t";
						}
						cout << endl;
						int l=1;
						for(iter = t->attributes.begin(); iter != t->attributes.end(); iter ++){
							vector<Attribute>::iterator iter2;
							for(iter2 = s.attributes.begin(); iter2 != s.attributes.end(); iter2++){
								if(iter->name == iter2->name){
									value = rm.toString(bm.bufferBlock[ip.bufferNUM].value+ip.position+l, iter->length, iter->type); 
									if(value.length()>5){
										cout << value <<  "\t";
									}else{
										cout << value <<  "\t\t";
									}
									break;
								}
							}
							l += iter->length;
						}
					}
					cout << endl << "1 row seleted." << endl;
					return;
				}
			}else{
				int resultCount = rm.selectWithwhere(*cm.findTable(s.tableName), s.attributes, s.conditions);
				if (resultCount)
				{
					rm.outputMap(resultCount);
					cout << resultCount << " row(s) selected:" << endl;
				}
				else
				{
					cout << "no match row" << endl;
				}
			}
		}
		else{
			cout << "select failed." << endl;
		}
	}
	else if (s.type == INSERT){
		if (cm.API_Catalog(s)){
			//cout << "right insert statement." << endl;
			// 调record和index
			insertPos ip;
			if (rm.insertValue(*cm.findTable(s.tableName), s.content, ip))
			{
				cout << "inserted" << endl;
				
				im.Insert_Index(cm.findTable(s.tableName), s.content, ip.bufferNUM, ip.position); 
			}
			else
			{
				cout << "insert failed." << endl;
			}
		}
		else{
			cout << "insert failed." << endl;
		}
	}
	else if (s.type == MYDELETE){
		if (cm.API_Catalog(s)){
			//cout << "right delete statement." << endl;
			// 调record和index
			int deleteCount = rm.deleteWithoutwhere(*cm.findTable(s.tableName));
			if (deleteCount)
			{
				cout << deleteCount << " row(s) deleted" << endl;
			}
			else
			{
				cout << "no row deleted" << endl;
			}
		}
		else{
			cout << "delete failed." << endl;
		}
	}
	else if (s.type == DELETE_WHERE){
		if (cm.API_Catalog(s)){
			//cout << "right delete where statement." << endl;
			// 调record和index
			int deleteCount = rm.deleteWithwhere(*cm.findTable(s.tableName), s.conditions);
			if (deleteCount)
			{
				cout << deleteCount << " row(s) deleted" << endl;
			}
			else
			{
				cout << "no row deleted" << endl;
			}
		}
		else{
			cout << "delete failed." << endl;
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
		quitFlag = true;
		// 保存表信息
		cm.update_tableInfo();
		cm.update_indexInfo();
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

//获取用户输入
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
	//将输入大写转化为小写
	SQL = lower(SQL);
	//返回用户输入
	return SQL;
}

//改为小写
string APIMoudule::lower(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}