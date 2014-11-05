#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

#include "MiniSQL.h"

//��ȡ�û�����
string read_input();

//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
string Interpreter();

//��ΪСд
string lower(string s);

//��֤create����Ƿ���Ч
string create_clause(string SQL,int start);

//��֤create database����Ƿ���Ч
string create_database(string SQL,int start);

//��֤create table����Ƿ���Ч
string create_table(string SQL,int start);

//�������
string get_attribute(string temp,string sql);

//��֤create index����Ƿ���Ч
string create_index(string SQL,int start);

//��֤create index on����Ƿ���Ч
string create_index_on(string SQL,int start,string sql);

//��֤drop����Ƿ���Ч
string drop_clause(string SQL,int start);

//��֤drop database����Ƿ���Ч
string drop_database(string SQL,int start);

//��֤drop table����Ƿ���Ч
string drop_table(string SQl,int start);

//��֤drop index����Ƿ���Ч
string drop_index(string SQL,int start);

//��֤select ����Ƿ���Ч
string select_clause(string SQL,int start);

//�����������ļ����ÿһ��
string get_part(string temp,string sql,string kind);

//��֤insert ����Ƿ���Ч
string insert_clause(string SQL,int start);

//��֤insert into values����Ƿ���Ч
string insert_into_values(string SQL,int start,string sql);

//��֤delete����Ƿ���Ч
string delete_clause(string SQL,int start);

//��֤ delete from where ����Ƿ���Ч
string delete_from_where(string SQL,int start,string sql);

//�����ʽת��Ϊ�ڲ���ʽ
string get_expression(string temp,string sql);

//��ȡ���ʽ���ÿ�����ʽ
string get_each(string T,string sql,string condition);

//��֤use����Ƿ���Ч
string use_clause(string SQL,int start);

//��֤execfile����Ƿ���Ч
string execfile_clause(string SQL,int start);

//��֤quit����Ƿ���Ч
string quit_clause(string SQL,int start);

#endif