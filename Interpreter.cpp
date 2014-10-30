//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Interpreter_Module                           ///
///       Produced by: ZZZ                          ///
///       Description: Produced to deal with SQL parse ///
///       date: 2004/12/25                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#include "Interpreter.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
CString Interpreter(CString statement)
{
	CString SQL;
	CString temp;
	CString sql;
	int start=0,end;
	if(statement.IsEmpty())
		//��ȡ�û�����
		SQL=read_input();
	else
		SQL=statement;
	//��ȡ����ĵ�һ������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start); //��һ����
	start=end+1; //�ӵڶ�����start
	//�������룬��ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"syntax error: empty statement!"<<endl;
		SQL="99";
	}
	//��Ϊcreate���
	else if(temp=="create")
		SQL=create_clause(SQL,start);
	//��Ϊdrop���
	else if(temp=="drop")
		SQL=drop_clause(SQL,start);
	//��Ϊselect���
	else if(temp=="select")
		SQL=select_clause(SQL,start);
	//��Ϊinsert���
	else if(temp=="insert")
		SQL=insert_clause(SQL,start);
	//��Ϊdelete���
	else if(temp=="delete")
		SQL=delete_clause(SQL,start);
	//��Ϊuse���
	else if(temp=="use")
		SQL=use_clause(SQL,start);
	//��Ϊexecfile���
	else if(temp=="execfile")
		SQL=execfile_clause(SQL,start);
	//��Ϊquit���
	else if(temp=="quit")
		SQL=quit_clause(SQL,start);
	//��ȡ����
	else if(temp=="help")
		SQL="80";
	//��Ϊ�Ƿ����
	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}
	//�������������ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û�����
CString read_input()
{
	CString SQL;
	CString temp;
	char str[100];
	bool finish=false;
	while(!finish)
	{
		cin>>str;
		temp=str;
		SQL=SQL+" "+temp; //?
		if(SQL.GetAt(SQL.GetLength()-1)==';') //������0��ʼ
		{
			SQL.SetAt(SQL.GetLength()-1,' ');
			SQL+=";";
			finish=true;
		}  //�ڣ�ǰ�Ӹ��ո�
	}
	//�������дת��ΪСд
	SQL.MakeLower();
	//�����û�����
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create����Ƿ���Ч
CString create_clause(CString SQL,int start)
{
	CString temp;
	int end;
	//��ȡ�ڶ�������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;  //start�ӵ������ʿ�ʼ
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for create statement!"<<endl;
		SQL="99";
	}
	//��Ϊdatabase,������֤
	else if(temp=="database")
		SQL=create_database(SQL,start);
	//��Ϊtable,������֤
	else if(temp=="table")
		SQL=create_table(SQL,start);
	//��Ϊindex,������֤
	else if(temp=="index")
		SQL=create_index(SQL,start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create database����Ƿ���Ч
CString create_database(CString SQL,int start)
{
	CString temp;
	int index,end;
	//��ȡ����������
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start); //���ݿ���
	start=end+1;  //start�ӵ��ĸ��ʿ�ʼ
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"error: database name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{	
		while(SQL.GetAt(start)==' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
		{
			cout<<"error12:"<<SQL.Mid(index,SQL.GetLength()-index-2)<<"---is not a valid database name!"<<endl;
			SQL="99";
		}
		//����create database�����ڲ���ʽ
		else
			SQL="00"+temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create table����Ƿ���Ч
CString create_table(CString SQL,int start)
{
	CString temp,sql,T;
	int index,end,length;
	//��ȡ����
	while(SQL.GetAt(start)==' ')
		start++;
	index=start;
	if((end=SQL.Find('(',start))==-1)
	{
		cout<<"error: missing ( in the statement!"<<endl;
		SQL="99";
		return SQL;
	}
	temp=SQL.Mid(start,end-start);  //����
	start=end+1;   // (
	if(!temp.IsEmpty())
	{
		while(SQL.GetAt(start)==' ')
			start++;
		length=temp.GetLength()-1;
		while(temp.GetAt(length)==' ')
			length--;
		temp=temp.Left(length+1);      //?
	}
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"error: error in create table statement!"<<endl;
		SQL="99";
		return SQL;
	}
	//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
	else if(temp.Find(' ')!=-1)   //�������пո�
	{
		cout<<"error: "<<temp<<"---is not a valid table name!"<<endl;
		SQL="99";
		return SQL;
	}
	else 
	{
		sql=temp+",";
		//��ȡÿ������
		while((end=SQL.Find(',',start))!=-1)
		{
			temp=SQL.Mid(start,end-start);  //��һ������
			start=end+1;
			//���п����ԣ���ӡ������Ϣ
			if(temp.IsEmpty())
			{
				cout<<"error: error in create table statement!"<<endl;
				SQL="99";
				return SQL;
			}
			//��������
			else
			{
				sql=get_attribute(temp,sql);
				if(sql=="99")
					return sql;
			}
			while(SQL.GetAt(start)==' ')
				start++;
		}
		//����������
		temp=SQL.Mid(start,SQL.GetLength()-start-1);
		length=temp.GetLength()-1;
		while(temp.GetAt(length)!=')'&&length>=0)
			length--;
		//���ޣ���ӡ������Ϣ
		if(length<1)
		{
			cout<<"error: error in create table statement!"<<endl;
			SQL="99";
			return SQL;
		}
		//�洢����
		else
		{
			temp=temp.Left(length);
			end=SQL.Find(' ',start);
			T=SQL.Mid(start,end-start);
			start=end+1;
			//��Ϊ��������
			if(T=="primary")
			{
				//�ж��Ƿ��йؼ���key
				temp+=")";
				while(SQL.GetAt(start)==' ')
					start++;
				end=SQL.Find('(',start);
				T=SQL.Mid(start,end-start);
				start=end+1;
				length=T.GetLength()-1;
				while(T.GetAt(length)==' ')
					length--;
				T=T.Left(length+1);
				//��Ϊ�գ���ӡ������Ϣ
				if(T.IsEmpty())
				{
					cout<<"syntax error: syntax error in create table statement!"<<endl;
					cout<<"\t missing key word key!"<<endl;
					SQL="99";
					return SQL;
				}
				//���У�������֤
				else if(T=="key")
				{
					//��ȡ����������
					while(SQL.GetAt(start)==' ')
						start++;
					end=SQL.Find(')',start);
					T=SQL.Mid(start,end-start);
					length=T.GetLength()-1;
					while(T.GetAt(length)==' ')
						length--;
					T=T.Left(length+1);
					//���ޣ���ӡ������Ϣ
					if(T.IsEmpty())
					{
						cout<<"error : missing primary key attribute name!"<<endl;
						SQL="99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if(T.Find(' ')!=-1)
					{
						cout<<"error : "<<T<<"---is not a valid primary key attribute name!"<<endl;
						SQL="99";
						return SQL;
					}
					//��������
					else
					{
						sql+=T+" #,";
						SQL="01"+sql;
					}
				}
				//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
				else
				{
					cout<<"error : "<<T<<"---is not a valid key word!"<<endl;
					SQL="99";
					return SQL;
				}				
			}
			//��Ϊһ������
			else   //�����Բ�����������
			{
				sql=get_attribute(temp,sql);
				if(sql=="99")
				{
					SQL="99";
					return SQL;
				}
				else
					SQL="01"+sql;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�������
CString get_attribute(CString temp,CString sql)
{
	int start=0,end,index;
	CString T,C;
	temp+=" ";
	//���������
	end=temp.Find(' ',start);
	T=temp.Mid(start,end-start);
	start=end+1;
	sql+=T+" ";
	//�����������
	while(temp.GetAt(start)==' ')
		start++;
	end=temp.Find(' ',start);
	T=temp.Mid(start,end-start);
	start=end+1;
	//���ޣ���ӡ������Ϣ
	if(T.IsEmpty())
	{
		cout<<"error : error in create table statement!"<<endl;
		sql="99";
		return sql;
	}
	//��Ϊint 
	else if(T=="int")
		sql+="+";
	//��Ϊfloat
	else if(T=="float")
		sql+="-";
	//����
	else
	{
		index=T.Find('(');   //���ǣ���
		C=T.Left(index);
		index++;
		//�����󣬴�ӡ������Ϣ
		if(C.IsEmpty())
		{
			cout<<"error: "<<T<<"---is not a valid data type definition!"<<endl;
			sql="99";
			return sql;
		}
		//��Ϊchar
		else if(C=="char")
		{
			C=T.Mid(index,T.GetLength()-index-1);
			if(C.IsEmpty())
			{
				cout<<"error: the length of the data type char has not been specified!"<<endl;
				sql="99";
				return sql;
			}
			else
				sql+=C;
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout<<"error: "<<C<<"---is not a valid key word!"<<endl;
			sql="99";
			return sql;
		}
	}
	//�Ƿ��и�����Ϣ
	while(start<temp.GetLength()&&temp.GetAt(start)==' ')
		start++;
	if(start<temp.GetLength())
	{
		//��Ϊunique���壬������Ϣ
		end=temp.Find(' ',start);
		T=temp.Mid(start,end-start);
		if(T=="unique")
		{
			sql+=" 1,";
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout<<"error: "<<temp<<"---is not a valid key word!"<<endl;
			sql="99";
			return sql;
		}
	}
	//���޸�����Ϣ
	else
		sql+=" 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index����Ƿ���Ч
CString create_index(CString SQL,int start)
{
	CString temp,sql;
	int end;
	//��ȡ����������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for create index statement!"<<endl;
		SQL="99";
	}
	else
	{
		sql=temp;
		//��ȡ���ĸ�����
		while(SQL.GetAt(start)==' ')
			start++;
		end=SQL.Find(' ',start);
		temp=SQL.Mid(start,end-start);
		start=end+1;
		//���ޣ���ӡ������Ϣ
		if(temp.IsEmpty())
		{
			cout<<"syntax error: syntax error for create index statement!"<<endl;
			SQL="99";
		}
		//��Ϊon,������֤
		else if(temp=="on")
			SQL=create_index_on(SQL,start,sql);
		//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
		else
		{
			cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
			SQL="99";
		}			
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index on����Ƿ���Ч
CString create_index_on(CString SQL,int start,CString sql)
{
	CString temp;
	int end,length;
	//��ȡ����
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find('(',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for create index statement!"<<endl;
		cout<<"\t missing ( !"<<endl;
		SQL="99";
		return SQL;
	}
	else
	{
		//�����Ƿ�Ϊ��Ч�ļ���
		length=temp.GetLength()-1;
		while(temp.GetAt(length)==' ')
			length--;
		temp=temp.Left(length+1);
		//��Ч                            //?
		if(temp.Find(' ')==-1)
		{
			sql+=" "+temp;
			//��ȡ������
			while(SQL.GetAt(start)==' ')
				start++;
			end=SQL.Find(')',start);
			temp=SQL.Mid(start,end-start);
			start=end+1;
			//���ޣ���ӡ������Ϣ
			if(temp.IsEmpty())
			{
				cout<<"syntax error: syntax error for create index statement!"<<endl;
				cout<<"\t missing ) !"<<endl;
				SQL="99";
				return SQL;
			}
			else
			{
				//�����Ƿ�Ϊ��Ч������
				length=temp.GetLength()-1;
				while(temp.GetAt(length)==' ')
					length--;
				temp=temp.Left(length+1);
				//��Ч
				if(temp.Find(' ')==-1)
				{
					sql+=" "+temp;
					while(SQL.GetAt(start)==' ')
						start++;
					if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
					{
						cout<<"syntax error: syntax error for quit!"<<endl;
						SQL="99";
						return SQL;
					}
					//����create index�����ڲ���ʽ
					else
						SQL="02"+sql;
				}
				//��Ч,��ӡ������Ϣ
				else
				{
					cout<<"error:"<<" "<<temp<<"---is not a valid attribute name!"<<endl;
					SQL="99";
					return SQL;
				}
			}
		}
		//��Ч,��ӡ������Ϣ
		else
		{
			cout<<"error:"<<" "<<temp<<"---is not a valid table name!"<<endl;
			SQL="99";
			return SQL;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop����Ƿ���Ч
CString drop_clause(CString SQL,int start)
{

	CString temp;
	int end;
	//��ȡ�ڶ�������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;  //start�ӵ������ʿ�ʼ
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for drop statement!"<<endl;
		SQL="99";
	}
	//��Ϊdatabase,������֤
	else if(temp=="database")
		SQL=drop_database(SQL,start);
	//��Ϊtable,������֤
	else if(temp=="table")
		SQL=drop_table(SQL,start);
	//��Ϊindex,������֤
	else if(temp=="index")
		SQL=drop_index(SQL,start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}
	//����drop�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop database����Ƿ���Ч
CString drop_database(CString SQL,int start)
{
	CString temp;
	int end;
	//��ȡ����������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"error: database name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.GetAt(start)==' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
		{
			cout<<"error:"<<SQL.Mid(index,SQL.GetLength()-index-2)<<"---is not a valid database name!"<<endl;
			SQL="99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL="10"+temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop table����Ƿ���Ч
CString drop_table(CString SQL,int start)  //drop table ���� ;
{
	CString temp;
	int end;
	//��ȡ����������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);   //temp=table��
	start=end+1;
	//���ޣ���ӡ������Ϣ


	if(temp.IsEmpty())
	{
		cout<<"error: table name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.GetAt(start)==' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
		{
			cout<<"error:"<<SQL.Mid(index,SQL.GetLength()-index-2)<<"---is not a valid table name!"<<endl;
			SQL="99";
		}
		//����drop table�����ڲ���ʽ
		else
			SQL="11"+temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop index����Ƿ���Ч
CString drop_index(CString SQL,int start)  //drop index ������ ;
{
	CString temp;
	int end;
	//��ȡ����������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);
	start=end+1;
	//���ޣ���ӡ������Ϣ

	if(temp.IsEmpty())
	{
		cout<<"error: index name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.GetAt(start)==' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if(SQL.GetAt(start)!=';'||start!=SQL.GetLength()-1)
		{
			cout<<"error:"<<SQL.Mid(index,SQL.GetLength()-index-2)<<"---is not a valid index name!"<<endl;
			SQL="99";
		}
		//����drop index�����ڲ���ʽ
		else
			SQL="12"+temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤select ����Ƿ���Ч
CString select_clause(CString SQL,int start)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//�����������ļ����ÿһ��
CString get_part(CString temp,CString sql,CString kind)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤insert����Ƿ���Ч
CString insert_clause(CString SQL,int start)  //insert into ���� values ( ֵ1 , ֵ2 , �� , ֵn ) ;
{
	CString temp,sql;
	int end;
	//��ȡ�ڶ�������
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);  //temp = into
	start=end+1;
	//���ޣ���ӡ������Ϣ
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for insert statement!"<<endl;
		SQL="99";
	}
	else if (temp != 'into')
	{
		cout<<"syntax error: syntax error for insert statement!"<<endl;
		SQL="99";

	}
	else
	{
		//��ȡ����������
		while(SQL.GetAt(start)==' ')
			start++;
		index=start;
		end=SQL.Find(' ',start);
		temp=SQL.Mid(start,end-start);  //temp = table��
		sql += temp;   //sql = table��
		start=end+1;
		//���ޣ���ӡ������Ϣ
		if(temp.IsEmpty())
		{
			cout<<"syntax error: syntax error for insert statement!"<<endl;
			SQL="99";
		}
		else
		{
			
			//��ȡ����������
			while(SQL.GetAt(start)==' ')
				start++;
			index=start;
			end=SQL.Find(' ',start);
			temp=SQL.Mid(start,end-start);  //temp = value
			start=end+1;                    //strat = (
			//���ޣ���ӡ������Ϣ
			if(temp.IsEmpty())
			{
				cout<<"syntax error: syntax error for insert statement!"<<endl;
				SQL="99";
			}
			else if (temp != "value")
			{
				cout<<"syntax error: syntax error for insert statement!"<<endl;
				SQL="99";
			}
			else
			{
				SQL=insert_into_values(SQL,start,sql);
			}
		}
	}

	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤insert into values����Ƿ���Ч    
CString insert_into_values(CString SQL,int start,CString sql)  
{
	CString temp;                      //insert into ���� values ( ֵ1 , ֵ2 , �� , ֵn ) ;
	CString T;
	int end;
	//��ȡ(
	while(SQL.GetAt(start)==' ')
		start++;
	end=SQL.Find(' ',start);
	temp=SQL.Mid(start,end-start);  //temp = (
	start=end+1;
	//���ޣ���ӡ����
	if(temp.IsEmpty())
	{
		cout<<"syntax error: syntax error for insert into statement!"<<endl;
		SQL="99";
	}
	else if(temp != '(')
	{
		cout<<"syntax error: syntax error for insert into statement!"<<endl;
		SQL="99";
	}
	else
	{
		end=SQL.Find(' ',start);
		temp=SQL.Mid(start,end-start);  //temp = ( ֵ1 , ֵ2 , �� , ֵn )
		start=end+1;
		//���ޣ���ӡ����
		if(temp.IsEmpty())
		{
			cout<<"syntax error: syntax error for insert into statement!"<<endl;
			SQL="99";
		}
		else
		{
			SQL="30"+sql+' '+temp;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤delete����Ƿ���Ч
CString delete_clause(CString SQL,int start)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤ delete from where ����Ƿ���Ч
CString delete_from_where(CString SQL,int start,CString sql)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//�����ʽת��Ϊ�ڲ���ʽ
CString get_expression(CString temp,CString sql)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ���ʽ���ÿ�����ʽ
CString get_each(CString T,CString sql,CString condition)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤use����Ƿ���Ч
CString use_clause(CString SQL,int start)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤execfile����Ƿ���Ч
CString execfile_clause(CString SQL,int start)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤quit����Ƿ���Ч
CString quit_clause(CString SQL,int start)
{

}
