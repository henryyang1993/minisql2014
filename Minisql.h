#if !defined(_MINISQL_H_)
#define _MINISQL_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <algorithm>
using namespace std;

#define FILE_COUNTER 1
#define FILE_INFO_LENGTH 671
#define ATTR_INFO_LENGTH 20
#define FILE_HEAD_LENGTH 31

//�����������ڵ���Ϣ
struct index_info
{
	string index_name;
	int length;
	char type;
	long offset;
	string value;
};

//����delete ����������Ϣ
struct condition_info
{
	char left_type;
	int left_len;
	long left_offset;
	string left_index_name;
	char right_type;
	int right_len;
	long right_offset;
	string right_index_name;
	string const_data;
	string condition;
};	

//����select �����ʾ��������Ϣ
struct attr_info
{
	string attr_name;
	int offset;
	int length;
	char type;
};

#endif