#include "studymysql.hpp"

int main(){
    criminal ccriminal;
    cout << "依次输入要操作的mysql数据库名称，用户名以及密码：";
    string database, name, passwd;
    cin >> database >> name >> passwd;
    mysqlManage mmysqlManage(database, name, passwd);
    mmysqlManage.showMenu(ccriminal);
    
return 0;
}
//constructor1
mysqlManage::mysqlManage(string database, string name, string password){
    Driver* driver = get_driver_instance();
    conn = driver->connect("tcp://127.0.0.1:3306", name, password);
    if(conn != nullptr && conn->isClosed() == 0){
	cout << "数据库连接成功！" << endl;
    }
    string useDatabase = "use " + database;
    stmt = conn->createStatement();
    stmt->execute(useDatabase);

}
//constructor2
mysqlManage::mysqlManage(string host, int port, string database, string name, string password){
    
    Driver* driver = get_driver_instance();
    string hostAndPort = host + to_string(port);
    conn = driver->connect(hostAndPort, name, password);
    
    if(conn != nullptr && conn->isClosed() == 0){
	cout << "数据库连接成功！" << endl;
    }    
	
    string useDatabase = "use " + database;
    stmt = conn->createStatement();
    stmt->execute(useDatabase);
} 
//destructor
mysqlManage::~mysqlManage(){
        stmt->close();
        delete stmt;
	conn->close();
       	delete conn;
}
 
void mysqlManage::showMenu(criminal ccriminal){
    int choice;
    cout << "-----输入0：退出操作-----" << endl;	
    cout << "-----输入1：插入数据-----" << endl;
    cout << "-----输入2：删除数据-----" << endl;
    cout << "-----输入3：修改数据-----" << endl;
    cout << "-----输入4：查看数据-----" << endl;
    do{
    cout << "输入选项：" << endl;
    cin >> choice;	    
    switch(choice){
	case(0) : break;    
        case(1) : { cout << "输入新罪犯的名字以及生日：" << endl;
		    cin >> ccriminal.name >> ccriminal.birth;
	            add(ccriminal);
		    break;
		  }  
	case(2) : { cout << "输入要删除信息的罪犯的id: " << endl;
		    int idToRm;
		    cin >> idToRm;
		    remove(idToRm);
		    break;
		  }
        case(3) : { cout << "输入要修改信息的罪犯的id：" << endl;
		    int idToMod;
		    cin >> idToMod;
	            modify(idToMod);
	            break;	    
		  }
        case(4) : { cout << "输入要查看信息的罪犯的id：" << endl;
		    int idToQue;
		    cin >> idToQue;
		    queryById(idToQue);
		    break;
		  }	
	default : { cout << "无效数字，重新输入。" << endl;
	            break;
	          }	    
    }
    }while(choice != 0);

    cout << "bye!" << endl;
}

void mysqlManage::add(criminal newCrime){
   string insertStr = "INSERT INTO test(name, birth) VALUES(?,?)";
   PreparedStatement * pstm = conn->prepareStatement(insertStr);
   //pstm->setInt(1,newCrime.id);
   pstm->setString(1,newCrime.name);
   pstm->setInt(2,newCrime.birth);
   int result = pstm->execute();
   if(result == 1){
       cout << "successful." << endl;
   } 
   pstm->close();
}

void mysqlManage::modify(int id){
    cout << "输入修改后的姓名，生日：" << endl;
    string newName;
    int newBirth;
    cin >> newName >> newBirth;
    string sql = "UPDATE  test  SET name = '" + newName + "' , birth = " + to_string(newBirth) + " WHERE id  = " + to_string(id);
    int  effectRows = stmt->executeUpdate(sql);
    cout << "修改数据成功" << effectRows << "行受影响！" << endl;  
}

void mysqlManage::remove(int idToRm){
    string  sql = "DELETE  FROM test WHERE id = " + to_string(idToRm) ;
    bool ret = stmt->execute(sql);
    if (ret)
    {//代表是SELECT 操作，可以获取结果集
        ResultSet* rs = stmt->getResultSet();
	cout << "查询操作，返回了数据集" << rs << endl;
	rs->close();
    }
    else
    {//代表是UPDATE INSERT  DELETE操作，可以获取受影响的行数
        int  effectRows = stmt->getUpdateCount();
        cout << "删除数据成功" << effectRows << "行受影响！" << endl;
    }
}

void mysqlManage::queryById(int idToQue){
    SQLString  sql = "SELECT  FROM  test WHERE id = " + to_string(idToQue);
    ResultSet* rs = stmt->executeQuery(sql);
    while(rs != nullptr){
	string name = rs->getString("name");//按 列名 获取数据
	int birth=  rs->getInt(3);//按 序号方式 获取列数据
	cout << name.c_str() << "\t" << birth << endl;
    }			 
    //关闭结果集
    rs->close();
}

