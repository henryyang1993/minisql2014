#include "Record_Manager.h"

//bool RecordManager::createTable(Table & table)
//{}
//bool RecordManager::insertValue(Table & table, string values)
//{}
//bool RecordManager::dropTable(Table & table)
//{}
//bool RecordManager::deleteWithwhere(Table & table, vector<Condition> & conditions)
//{}
//bool RecordManager::deleteWithoutwhere(Table & table)
//{}
int RecordManager::selectWithwhere(Table & table, const vector<Attribute> & attributes, const vector<Condition> & conditions)
{
	attributeValuesMap.clear();			// ��շ����б�					���ܴ����ڴ�й¶		TO-DO
	int readSize = table.tupleLength;	// tuple����
	int selectAttrNum;					// ѡ�����Ը���
	vector<Attribute> selectAttrbute;	// ѡȡ�����б�

	if (attributes[0].name == "*")		
	{
		// ȫѡ�����
		selectAttrNum = table.attributes.size();
		selectAttrbute = table.attributes;
	}
	else
	{
		// ��ȫѡ
		selectAttrNum = attributes.size();
		selectAttrbute = attributes;
	}

	vector<string> * cluster = new vector<string>[selectAttrNum];	// ����ֵ�б�ļ���		new

	for (/*read from start to end*/;;)
	{
		// read tuple here
		for (int i = 0; i < table.attriNum; i++)
		{
			// ȡ��ÿ�����Ե�ֵ
			int count = 0;	// ѡ�����Ե��±�
			if (contains(selectAttrbute, table.attributes[i]))
			{
				// ��¼�ǵڼ�����Ҫ������
				cluster[count].push_back(/*��ֵ�����б�*/);
				count++;
			}
		}
	}

	if (cluster[0].size())	// ��ѡ��������
	{
		for (int i = 0; i < selectAttrNum; i++)
		{
			attributeValuesMap.insert(std::pair<string, vector<string> >(selectAttrbute[i].name, cluster[i]));
		}
		return cluster[0].size();
	}
	else
	{
		delete[] cluster;
		return 0;
	}
}

int RecordManager::selectWithoutwhere(Table & table, const vector<Attribute> & attributes)
{
	attributeValuesMap.clear();			// ��շ����б�					���ܴ����ڴ�й¶		TO-DO
	int readSize = table.tupleLength;	// tuple����
	int selectAttrNum;					// ѡ�����Ը���
	vector<Attribute> selectAttrbute;	// ѡȡ�����б�

	if (attributes[0].name == "*")
	{
		// ȫѡ�����
		selectAttrNum = table.attributes.size();
		selectAttrbute = table.attributes;
	}
	else
	{
		// ��ȫѡ
		selectAttrNum = attributes.size();
		selectAttrbute = attributes;
	}

	vector<string> * cluster = new vector<string>[selectAttrNum];	// ����ֵ�б�ļ���		new

	for (/*read from start to end*/;;)
	{
		// read tuple here
		for (int i = 0; i < table.attriNum; i++)
		{
			// ȡ��ÿ�����Ե�ֵ
			int count = 0;
			if (contains(selectAttrbute, table.attributes[i]))
			{
				// ��¼�ǵڼ�����Ҫ������
				cluster[count].push_back(/*��ֵ�����б�*/);
				count++;
			}
		}
	}

	if (cluster[0].size())	// ��ѡ��������
	{
		for (int i = 0; i < selectAttrNum; i++)
		{
			attributeValuesMap.insert(std::pair<string, vector<string> >(selectAttrbute[i].name, cluster[i]));
		}
		return cluster[0].size();
	}
	else
	{
		delete[] cluster;
		return 0;
	}
}

bool RecordManager::contains(const vector<Attribute> & attributes, const Attribute & attr)
{
	for (size_t i = 0; i < attributes.size(); i++)
	{
		if (attributes[i].name == attr.name)
		{
			return true;
		}
	}
	return false;
}