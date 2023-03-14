#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
/* uncomment for applications that use vectors */
/*#include <vector>*/

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

class criminal
{
        public:
                int id;
                string name;
                int birth;
        private:
};

class mysqlManage
{
	public:
		mysqlManage(string database, string name, string password);
		mysqlManage(string host, int port, string database, string name, string password);
		~mysqlManage();

		void add(criminal);
		void modify(int);
		void remove(int);
		void queryById(int);

		void showMenu(criminal);

        private:
		Statement *stmt;
                Connection *conn;

};

/*class criminal
{
	public:
		int id;
		string name;
		int birth;
        private:		
};*/
