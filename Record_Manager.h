#include "Minisql.h"

//释放数据库的内存缓冲区
void Close_Database(String DB_Name, bool closetype);
//释放表或索引的内存缓冲区
void Close_File(String DB_Name, String filename, int filetype, bool closetype);
//向表中插入元组
void Insert_Item(String DB_Name, String Table_Name, String Attr, int & record_Num);
//打印选择属性
void Print_Head(attr_info print[32], int count);
//打印出选择结果
void Print_To_Screen(String record, attr_info print[32], int count);
//有where，但无可用索引的select语句
void Select_Without_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond);
//有where且有可用索引的select 语句
void Select_Without_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond, int index);
//用等于索引的选择语句
void Select_With_Equal_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index);
//用大于或大于等于索引的选择语句
void Select_With_Greater_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type);
//用小于或小于等于索引的选择语句
void Select_With_Smaller_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type);
//判断是否符合where条件组
bool Confirm_To_Where(String record, condition_info conds[10], int count, char cond);
//判断是否符合某个指定条件
bool Confirm(String record, condition_info condition);
//选择语句（无where）
void Select_No_Where(String DB_Name, String Table_Name, attr_info print[32], int count);
//选择语句（有where）
void Select_With_Where(String DB_Name, String Table_Name, condition_info conds[10], int count, char cond, attr_info print[32], int Count);
//删除语句（有where）
void Delete_With_Where(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond);
//有where, 但无可用索引的delete语句
void Delete_Without_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond);
//有where且有可用索引的delete 语句
void Delete_With_Useful_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, int index);
//用等于索引的删除语句
void Delete_With_Equal_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index);
//用大于或大于等于索引的删除语句
void Delete_With_Greater_Cond(String DB_Name, String Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index, int type);
//用小于或小于等于索引的删除语句
void Select_With_Smaller_Cond(String DB_Name, String Table_Name, condition_info conds[32], int count, index_info nodes[32], int num, char cond, index_info Index, int type);
//查找有无可用索引
bool Find_Useful_Cond(condition_info conds[10], int count, int & index);
//插入所有索引节点
void Insert_Index_All(String DB_Name, String Table_Name, String Index_Name, int length, int offset, int type);
//验证可否插入
bool Verify_Insertable(String DB_Name, String Table_Name, index_info nodes[32], int count, String Attr);
//退出程序
void Quit(String DB_Name);
