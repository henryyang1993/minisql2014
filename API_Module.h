#ifndef _API_MODULE_H_
#define _API_MODULE_H_

#include "MiniSQL.h"

class APIMoudule
{
public:
	APIMoudule(){};
	~APIMoudule(){};
	void API(SQLstatement s);
	void getHelp();
	//��ȡ�û�����
	string read_input();
	//��ΪСд
	string lower(string s);

};

#endif
