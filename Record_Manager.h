#include "Minisql.h"

//�ͷ����ݿ���ڴ滺����
void Close_Database(String DB_Name, bool closetype);
//�ͷű���������ڴ滺����
void Close_File(String DB_Name, String filename, int filetype, bool closetype);
//����в���Ԫ��
void Insert_Item(String DB_Name, String Table_Name, String Attr, int & record_Num);
//��ӡѡ������
void Print_Head(attr_info print[32], int count);
//��ӡ��ѡ����
void Print_To_Screen(String record, attr_info print[32], int count);
//��where�����޿���������select���
void Select_Without_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond);
//��where���п���������select ���
void Select_Without_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond, int index);
//�õ���������ѡ�����
void Select_With_Equal_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index);
//�ô��ڻ���ڵ���������ѡ�����
void Select_With_Greater_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type);
//��С�ڻ�С�ڵ���������ѡ�����
void Select_With_Smaller_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type);
//�ж��Ƿ����where������
bool Confirm_To_Where(String record, condition_info conds[10], int count, char cond);
//�ж��Ƿ����ĳ��ָ������
bool Confirm(String record, condition_info condition);
//ѡ����䣨��where��
void Select_No_Where(String DB_Name, String Table_Name, attr_info print[32], int count);
//ѡ����䣨��where��
void Select_With_Where(String DB_Name, String Table_Name, condition_info conds[10], int count, char cond, attr_info print[32], int Count);
//ɾ����䣨��where��
void Delete_With_Where(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond);
//��where, ���޿���������delete���
void Delete_Without_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond);
//��where���п���������delete ���
void Delete_With_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, int index);
//�õ���������ɾ�����
void Delete_With_Equal_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index);
//�ô��ڻ���ڵ���������ɾ�����
void Delete_With_Greater_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index, int type);
//��С�ڻ�С�ڵ���������ɾ�����
void Select_With_Smaller_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, index_info nodes[32], int num, char cond, index_info Index, int type);
//�������޿�������
bool Find_Useful_Cond(condition_info conds[10], int count, int & index);
//�������������ڵ�
void Insert_Index_All(String DB_Name, String Table_Name, String Index_Name, int length, int offset, int type);
//��֤�ɷ����
bool Verify_Insertable(String DB_Name, String Table_Name, index_info nodes[32], int count, String Attr);
//�˳�����
void Quit(String DB_Name);
