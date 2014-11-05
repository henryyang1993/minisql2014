#include "Interpreter.h"
using namespace std;
string allchar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
string Interpreter()
{
	string SQL;
	string temp;
	int start = 0, end;
	SQL = read_input();
	//��ȡ����ĵ�һ������
	start = SQL.find_first_not_of(' ', 0);
	end = SQL.find_first_not_of(allchar, start);
	temp = SQL.substr(start, end - start);//��һ����
	//��ȡ�ڶ�������
	start = end + 1;

	//��Ϊcreate���
	if (temp == "create")
		SQL = create_clause(SQL, start);
	//��Ϊdrop���
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);
	//��Ϊselect���
	else if (temp == "select")
		SQL = select_clause(SQL, start);
	//��Ϊinsert���
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//��Ϊdelete���
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//��Ϊuse���
	else if (temp == "use")
		SQL = use_clause(SQL, start);
	//��Ϊexecfile���
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//��Ϊquit���
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//��ȡ����
	else if (temp == "help")
		SQL = "80";
	//��Ϊ�Ƿ����
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//�������������ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û�����
string read_input()
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

/////////////////////////////////////////////////////////////////////////////////////////////
//��ΪСд
string lower(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create����Ƿ���Ч
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	end = SQL.find_first_of(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;  //start�ӵ������ʿ�ʼ

	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create database����Ƿ���Ч
string create_database(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ����������
	end = SQL.find_first_of(';', start);
	temp = SQL.substr(start, end - start);  //���ݿ���
	start = end + 1;  //start�ӵ��ĸ��ʿ�ʼ

	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(end) != ';' || end != SQL.length() - 1)
		{
			cout << "error:" << temp << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����create database�����ڲ���ʽ
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create table����Ƿ���Ч
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int end;
	//��ȡ����
	end = SQL.find_first_not_of(allchar, start);
	temp = SQL.substr(start, end - start);// ����

	if ((SQL.find('(', end)) == -1)
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	start = SQL.find_first_of('(', end) + 1;

	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		start = SQL.find_first_of(allchar, start);
		//��ȡÿ������
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);  //��һ������
			start = end + 1;
			//���п����ԣ���ӡ������Ϣ
			if (start == 0 || temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL.at(start) == ' ' || SQL.at(start) == '\n')
				start++;
		}
		//����������
		end = SQL.find_last_of(')');
		temp = SQL.substr(start, end - start);
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//�洢����
		else
		{
			end = SQL.find_first_of(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//��Ϊ��������
			if (T == "primary")
			{
				//�ж��Ƿ��йؼ���key
				while (SQL.at(start) == ' ')
					start++;
				end = SQL.find_first_not_of(allchar, start);
				T = SQL.substr(start, end - start);
				start = end;

				//��Ϊ�գ���ӡ������Ϣ
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//���У�������֤
				else if (T == "key")
				{
					//��ȡ����������
					start = SQL.find_first_of('(', start);
					start = SQL.find_first_of(allchar, start);
					end = SQL.find_first_not_of(allchar, start);
					T = SQL.substr(start, end - start);
					//���ޣ���ӡ������Ϣ
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if (T.find(' ') != -1 || (sql.find(T) == -1))
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��������
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//��Ϊһ������
			else   //�����Բ�����������
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
				{
					SQL = "99";
					return SQL;
				}
				else
					SQL = "01" + sql;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�������
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string name, type, C, uni;
	temp += " ";
	//���������
	end = temp.find_first_of(' ', start);
	name = temp.substr(start, end - start);
	start = end + 1;
	sql = sql + name + ' ';
	//�����������
	while (temp.at(start) == ' ')
		start++;
	end = temp.find(' ', start);
	type = temp.substr(start, end - start);
	start = end;
	//���ޣ���ӡ������Ϣ
	if (name.empty() || type.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//��Ϊint
	else if (type == "int")
		sql += "+";
	//��Ϊfloat
	else if (type == "float")
		sql += "-";
	//����
	else
	{
		index = type.find('(');
		C = type.substr(0, index);
		index++;
		//�����󣬴�ӡ������Ϣ
		if (C.empty())
		{
			cout << "error: " << type << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//��Ϊchar
		else if (C == "char")
		{
			C = type.substr(index, type.length() - index - 1);
			if (C.empty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//�Ƿ��и�����Ϣ
	while (start < temp.length() - 1 && temp.at(start) == ' ')
		start++;
	if (start < temp.length() - 1)
	{
		//��Ϊunique���壬������Ϣ
		end = temp.find_first_of(' ', start);
		uni = temp.substr(start, end - start);
		if (uni == "unique")
		{
			sql += " 1,";
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << uni << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//���޸�����Ϣ
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index����Ƿ���Ч
string create_index(string SQL, int start)
{
	string temp, sql;
	int end;
	//��ȡ����������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find_first_of(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//��ȡ���ĸ�����
		while (SQL.at(start) == ' ')
			start++;
		end = SQL.find_first_of(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (start == 0 || temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//��Ϊon,������֤
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
		else
		{
			cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index on����Ƿ���Ч
string create_index_on(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	//��ȡ����
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find_first_of('(', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//�����Ƿ�Ϊ��Ч�ļ���
		length = temp.length() - 1;
		while (temp.at(length) == ' ')
			length--;
		temp = temp.substr(0, length + 1);
		//��Ч                            //?
		if (temp.find(' ') == -1) // ��������Ƿ����
		{
			sql += " " + temp;
			//��ȡ������
			while (SQL.at(start) == ' ')
				start++;
			end = SQL.find(')', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (start == 0 || temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//�����Ƿ�Ϊ��Ч������
				length = temp.length() - 1;
				while (temp.at(length) == ' ')
					length--;
				temp = temp.substr(0, length + 1);
				//��Ч
				if (temp.find(' ') == -1)	// ��������������Ƿ����
				{
					sql += " " + temp;
					while (SQL.at(start) == ' ')
						start++;
					if (SQL.at(start) != ';' || start != SQL.length() - 1)
					{
						cout << "syntax error: syntax error for create index on statement!" << endl;
						SQL = "99";
						return SQL;
					}
					//����create index�����ڲ���ʽ
					else
						SQL = "02" + sql;
				}
				//��Ч,��ӡ������Ϣ
				else
				{
					cout << "error:" << " " << temp << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//��Ч,��ӡ������Ϣ
		else
		{
			cout << "error:" << " " << temp << "---is not a valid table name!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop����Ƿ���Ч
string drop_clause(string SQL, int start)
{

	string temp;
	int end;
	//��ȡ�ڶ�������
	end = SQL.find_first_of(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;  //start�ӵ������ʿ�ʼ
	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = drop_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����drop�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop database����Ƿ���Ч
string drop_database(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ����������
	end = SQL.find_first_of(';', start);
	temp = SQL.substr(start, end - start);  //���ݿ���
	start = end + 1;  //start�ӵ��ĸ��ʿ�ʼ
	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(end) != ';' || end != SQL.length() - 1)
		{
			cout << "error:" << temp << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop table����Ƿ���Ч
string drop_table(string SQL, int start)  //drop table ���� ;
{
	string temp;
	int end;
	//��ȡ����������
	end = SQL.find_first_of(';', start);
	temp = SQL.substr(start, end - start);  //���ݿ���
	start = end + 1;  //start�ӵ��ĸ��ʿ�ʼ
	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(end) != ';' || end != SQL.length() - 1)
		{
			cout << "error:" << temp << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//����drop table�����ڲ���ʽ
		else
			SQL = "11" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop index����Ƿ���Ч
string drop_index(string SQL, int start)  //drop index ������ ;
{
	string temp;
	int end;
	//��ȡ����������
	end = SQL.find_first_of(';', start);
	temp = SQL.substr(start, end - start);  //���ݿ���
	start = end + 1;  //start�ӵ��ĸ��ʿ�ʼ
	//���ޣ���ӡ������Ϣ

	if (start == 0 || temp.empty())
	{
		cout << "error: index name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(end) != ';' || end != SQL.length() - 1)
		{
			cout << "error:" << temp << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//����drop index�����ڲ���ʽ
		else
			SQL = "12" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤select ����Ƿ���Ч
string select_clause(string SQL, int start)
{
	int end;
	string temp;
	string attr;
	string table;
	string condition;
	end = SQL.find("from", start);
	if (end == -1)	//û��from
	{
		cout << "syntax error: lack of 'from'" << endl;
		SQL = "99";
	}
	else
	{
		temp = SQL.substr(start, end - start);	//ѡ������
		temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());	//ȥ�ո�
		start = end + 4;
		if (temp.empty())	//�Ƿ�������
		{
			cout << "syntax error: please specify the attributes" << endl;
			SQL = "99";
		}
		else
		{
			attr = temp;
			start = SQL.find_first_of(allchar, start);
			end = SQL.find_first_not_of(allchar, start);
			if (end == -1)
			{
				cout << "syntax error: please specify the Table" << endl;
				SQL = "99";
			}
			else
			{
				table = SQL.substr(start, end - start);	//����
				//cout << attr << endl << start << endl << end << endl << table << endl;

				if (table == "where")
				{
					cout << "syntax error: please specify the Table" << endl;
					SQL = "99";
					return SQL;
				}
				start = end + 1;
				end = SQL.find_first_not_of(allchar, start);
				temp = SQL.substr(start, end - start);	//ȡwhere

				if (temp != "where")	//�Ƿ���where
				{
					//��where��
					if (end < SQL.length() - 1)
					{
						//ȱ�ٴ�������'where'ƴд����
						cout << "syntax error: please specify the Table or may be something wrong with 'where' " << endl;
						SQL = "99";
					}
					else
					{
						//��where����ȷ��䣻																	//	TO-DO
						//cout << "fuck" << endl;
						SQL = "20" + table + ' ' + attr;
					}
				}
				else
				{
					//��where��
					//cout << temp << endl;
					if (table.empty())
					{
						//ȱ�ٴ����
						cout << "syntax error: please specify the tables" << endl;
						SQL = "99";
					}
					else
					{
						//�д����
						start = end + 1;
						temp = SQL.substr(start, SQL.length() - start - 1);

						size_t index = 0;
						while (true) {	//�滻" and "Ϊ'&'
							index = temp.find(" and ", index);
							//cout << temp << endl << index << endl;
							if (index == string::npos) break;
							temp.replace(index, 5, "&");
							index += 1;
						}
						index = 0;
						while (true) {	//�滻" or "Ϊ'|'
							index = temp.find(" or ", index);
							//cout << temp << endl << index << endl;
							if (index == string::npos) break;
							temp.replace(index, 5, "|");
							index += 1;
						}

						//cout << start << endl << end << endl << temp << endl;
						temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());	//ȥ�ո�
						if (temp.empty())	//�Ƿ���where����
						{
							//ȱ��where����
							cout << "syntax error: please specify the condition or remove 'where' " << endl;
							SQL = "99";
						}
						else
						{
							//��where����
							condition = temp;
							SQL = "21" + table + ' ' + attr + ' ' + condition;
						}
					}
				}
			}
		}
	}
	//cout << attr << ' ' << table << ' ' << condition << endl;
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�����������ļ����ÿһ��
string get_part(string temp, string sql, string kind)
{
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤insert����Ƿ���Ч
string insert_clause(string SQL, int start)  //insert into ���� values ( ֵ1 , ֵ2 , �� , ֵn ) ;
{
	string temp;
	string sql = "";
	int end;

	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);  //temp = "into"

	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: can't find the keyword 'into' " << endl;
		SQL = "99";
	}
	else if (temp != "into")
	{
		cout << "syntax error: might be 'into' " << endl;
		SQL = "99";
	}
	else
	{
		//��ȡ����������
		start = SQL.find_first_not_of(' ', start);
		//index = start;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);  //temp = table��
		sql += temp;   //sql = table��
		start = end + 1;

		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: can't find the target Table" << endl;
			SQL = "99";
		}
		else if (temp == "values" || temp == "value" || temp == "into")
		{
			cout << "syntax error: can't find the target Table" << endl;
			SQL = "99";
		}
		else
		{

			//��ȡ���ĸ�����
			start = SQL.find_first_not_of(' ', start);
			//index = start;
			end = SQL.find_first_not_of(allchar, start);
			temp = SQL.substr(start, end - start);  //temp = value
			start = end;                    //start = (

			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: can't find keyword 'values' or 'value' " << endl;
				SQL = "99";
			}
			else if (temp != "values" && temp != "value")
			{
				cout << "syntax error: maybe 'value' or 'values' " << endl;
				SQL = "99";
			}
			else
			{
				//cout << SQL;
				SQL = insert_into_values(SQL, start, sql);
			}
		}
	}

	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤insert into values����Ƿ���Ч
string insert_into_values(string SQL, int start, string sql)
{
	string temp;                      //insert into ���� values ( ֵ1 , ֵ2 , �� , ֵn ) ;
	string T;
	int end;
	//��ȡ(
	start = SQL.find_first_not_of(' ', start);
	end = SQL.find_first_not_of('(', start);
	temp = SQL.substr(start, end - start);  //temp = (
	start = end;
	//���ޣ���ӡ����
	if (temp.empty())
	{
		cout << "syntax error: lack of '('" << endl;
		SQL = "99";
	}
	else if (temp != "(")
	{
		cout << "syntax error: multiple '(' have been found" << endl; //������ȱʧ�����
		SQL = "99";
	}
	else
	{
		end = SQL.rfind(')', SQL.length());	//���һ��������
		if (end == -1)
		{
			cout << "syntax error: lack of ')'" << endl;
			SQL = "99";
		}
		else
		{
			temp = SQL.substr(start, end - start);  //temp = ( ֵ1 , ֵ2 , �� , ֵn )
			temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());	//ȥ�ո�
			start = end + 1;
			//���ޣ���ӡ����
			if (temp.empty())
			{
				cout << "syntax error: syntax error for insert into statement!" << endl;
				SQL = "99";
			}
			else
			{
				//replace(temp.begin(), temp.end(), ' ', ''); // replace allchar 'x' to 'y'
				SQL = "30" + sql + ' ' + temp;
				//cout << SQL;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤delete����Ƿ���Ч
string delete_clause(string SQL, int start)
{
	int end;
	string temp;
	string table;
	end = SQL.find("from", start);
	if (end == -1)
	{
		//û��from
		cout << "syntax error: lack of 'from' " << endl;
		SQL = "99";
	}
	else
	{
		start = end + 5;	//ָ�����
		end = SQL.find_first_not_of(allchar, start);
		temp = SQL.substr(start, end - start);	//ȡ������������ȱʧʱȡ��where;
		if (temp.empty() || temp == "where")
		{
			//����Ϊ��
			cout << "syntax error: please specify the target Table " << endl;
			SQL = "99";
		}
		else
		{
			//�б���
			table = temp;
			//cout << table << endl;
			start = end + 1;
			end = SQL.find_first_not_of(allchar, start);
			temp = SQL.substr(start, end - start);	//ȡ������֮��Ĵ���������';'������'where'���������ʽ
			//cout << start << endl << end << endl << temp << endl;
			if (temp.empty() || temp == ";")
			{
				//��where��ȷ����;																			//TO-DO
				SQL = "40" + table;
			}
			else if (temp == "where")
			{
				//��where��ȷ����;
				start = end;
				SQL = delete_from_where(SQL, start, table);
			}
			else
			{
				//where����
				cout << "syntax error: lack of 'where' " << endl;
				SQL = "99";
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤ delete from where ����Ƿ���Ч
string delete_from_where(string SQL, int start, string sql)
{
	int end;
	string temp;

	end = SQL.find_first_of(';', start);
	temp = SQL.substr(start, end - start);

	size_t index = 0;
	while (true) {	//�滻" and "Ϊ','

		index = temp.find(" and ", index);
		//cout << temp << endl << index << endl;
		if (index == string::npos) break;
		temp.replace(index, 5, "&");
		index += 1;
	}
	index = 0;
	while (true) {	//�滻" or "Ϊ'|'
		index = temp.find(" or ", index);
		//cout << temp << endl << index << endl;
		if (index == string::npos) break;
		temp.replace(index, 5, "|");
		index += 1;
	}

	temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());	//ȥ�ո�

	if (temp.empty())
	{
		cout << "syntax error: please specify condition " << endl;
		SQL = "99";
	}
	else
	{
		//where ��ȷ����;
		SQL = "41" + sql + ' ' + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�����ʽת��Ϊ�ڲ���ʽ
string get_expression(string temp, string sql)
{
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ���ʽ���ÿ�����ʽ
string get_each(string T, string sql, string condition)
{
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤use����Ƿ���Ч
string use_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	end = SQL.find_first_not_of(allchar, start);
	temp = SQL.substr(start, end - start);
	end = SQL.find_first_not_of(' ', end);
	start = end + 1;

	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: syntax error for use statement!" << endl;
		SQL = "99";
	}
	else
	{
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(end) != ';' || end != SQL.length() - 1)
		{
			cout << "error:" << temp << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����create database�����ڲ���ʽ
		else
			SQL = "50" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤execfile����Ƿ���Ч
string execfile_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	end = SQL.find_last_of(allchar) + 1;
	temp = SQL.substr(start, end - start);
	end = SQL.find_first_not_of(' ', end);
	start = end + 1;

	//���ޣ���ӡ������Ϣ
	if (start == 0 || temp.empty())
	{
		cout << "syntax error: syntax error for execfile statement!" << endl;
		SQL = "99";
	}
	else
	{
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(end) != ';' || end != SQL.length() - 1)
		{
			cout << "error:" << temp << "---is not a valid file name!" << endl;
			SQL = "99";
		}
		//����create database�����ڲ���ʽ
		else
			SQL = "60" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤quit����Ƿ���Ч
string quit_clause(string SQL, int start)
{
	SQL = "70";
	return SQL;
}