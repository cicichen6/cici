#include <iostream>
#include <string>
#include <sstream>
#include <set>
//#include <jsoncpp/json/json.h>
//#include <algorithm>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD "123456"
#define DATABASE "test01"

using namespace std;
using namespace sql;




class zuifan
{
public:
      int num;
      string name;
      int birthday;
      mutable string criminalInString;
      zuifan(string name, int birthday,int num){
          this->name = name;
          this->birthday = birthday;
          this->num = num;
      }

      bool operator == (const zuifan& itemToCompare) const
      {
          return (itemToCompare.num ==  this->num);
      }
      bool operator < (const zuifan& itemToCmp) const
      {
          return (this->num < itemToCmp.num);
      }
     // bool operator()(const zuifan&  d1, const zuifan& d2) const {
       // return (d1.birthday == d2.birthday);}
      //friend class ope;
      //friend bool operator == (const zuifan&) const;
      /*operator const char*() const
      {
          return name.c_str();
      }*/
       operator const char*() const{
            ostringstream formattedCriminal;
            formattedCriminal << name << "/" << birthday;
             criminalInString = formattedCriminal.str();
             return criminalInString.c_str();
        }
      /*{
          return true;
          }
          //else{return false;}
        

      bool operator()(const zuifan& ob1,const  zuifan& ob2)const {
          return (ob1.num < ob2.num);
    }*/
};
/*class a
{
  public:
bool operator () (const zuifan&  d1, const zuifan& d2) const {
        return (d1.birthday == d2.birthday);}
};*/      
/*class changeFin: public zuifan
{
  public:
    //const zuifan& itemToCompare2;
    bool operator == (const zuifan& itemToCompare) const
     {
              return (itemToCompare.name == this->name);
     }  
};*/
/*class a
{
public:
      bool operator()(const zuifan& ob1,const  zuifan& ob2)const {
          return (ob1.num == ob2.num);//|| !((ob1.birthday < ob2.birthday)&&(ob2.birthday < ob1.birthday)); 
      }*/

      /*bool operator()(const zuifan& itemToCmp, const zuifan& item) const
      {
          return (item.name == itemToCmp.name);
      }*/
      
      /*bool cmp(const zuifan& ob, const string& name){
          return ob.name == name;
      }*/
/*};*/
class mysql
{
	public:
	//连接数据库
        Driver *driver = get_driver_instance();
        Connection *conn = driver->connect(DBHOST, USER, PASSWORD);

        Statement *stm;


       if(!conn->isValid()){
            cout<<"数据库连接无效"<<endl;
            
        }else
            cout<<"数据库连接成功"<<endl;
        //创建 test 表，添加数据
	stm = conn->createStatement();
        stm->execute("use " DATABASE);
        //stm->execute("DROP TABLE IF EXISTS test");
       // stm->execute("CREATE TABLE test01(Name CHAR(20),Birth CHAR(50))");
       
	mysql();
        ~mysql();
};

mysql::mysql(){
/*CREATE PROCEDURE add_criminal (IN criminal_name CHAR(20),
                              IN criminal_birth CHAR(50))
BEGIN
    INSERT INTO test01(Name, Birth)
      VALUES (criminal_name, criminal_birth);
END;*/

	//连接数据库
        Driver *driver = get_driver_instance();
        Connection *conn = driver->connect(DBHOST, USER, PASSWORD);

        Statement *stm;


        if(!conn->isValid()){
            cout<<"数据库连接无效"<<endl;
            
        }else
            cout<<"数据库连接成功"<<endl;
        //创建 test 表，添加数据
	stm = conn->createStatement();
        stm->execute("use " DATABASE);
        //stm->execute("DROP TABLE IF EXISTS test");
       // stm->execute("CREATE TABLE test01(Name CHAR(20),Birth CHAR(50))");
}

mysql::~mysql(){
} 

class ope
{
public:
//  friend class zuifan;
      //int birthday = zuifan.birthday;
     /* bool operator == (const zuifan& itemToCompare) const
      {
          return (itemToCompare.birthday == s.birthday);
      }*/
  //    Json::Value root;
	//  Json::FastWriter fast_writer;
  //    string str;// = fast_writer.write(root);

      void add( set<zuifan >&, string, int, int) ;
      void cat(set<zuifan >&);
      void dele();
      void check(set<zuifan >, int);
      //void check(set<zuifan >, const int&);
      void revise(set<zuifan>&, const int);
};

void ope::revise(set<zuifan>& s, const int num){
    auto iter = s.find(zuifan("",0,num));
    if(iter != s.end()){
        cout << "enter birth to revise:" ;
       // int newBirth;
        //cin >> (*iter).name;
        cin >> const_cast<zuifan&>(*iter).birthday;
    }
}

void ope::check(set<zuifan > s, int id){
     set<zuifan >::const_iterator iter = s.cbegin();
     for(int ii = 0; ii < id; ii++){
         iter++;
     }
     cout << (*iter).name;
     //cout << const_cast<zuifan&>(*iter).name;
}

/*void ope::check(set<zuifan > s, const int& num){
  auto iContactFound = s.find(zuifan("",0,num ));
  if(iContactFound != s.end()){
    cout << "ok";
      }
  else {cout << "no";}
  if(iContactFound != s.end()){
      cout << "ok";
  }
  else
    cout << "no";
}*/


void ope::add(set<zuifan >& s, string name, int birth, int num){
    static int ii = 1;
    pair<set<zuifan >::iterator,bool> it = s.insert(zuifan(name, birth, num));
    if (it.second == false) {      
             cout << "插入失败";
    }
    //stm->execute("INSERT INTO test(name, birth) VALUES(?, ?)");

//    mysql::stm = mysql::conn -> Statement("INSERT INTO test01(name,birth) VALUES(?,?)");
  //  mysql::stm -> setString (name,123);
   // int updatecount = 0;
   // updatecount = stm -> executeUpdate();
    /*root[ii]["num"] = num;
    root[ii]["name"] = name;
    root[ii]["birth"] = birth;
    ii++;
    str = fast_writer.write(root);
*/
 }

void ope::cat(set<zuifan >& s){
    for(set<zuifan >::const_iterator  a = s.cbegin(); a != s.cend(); a ++){
       cout << *a << endl;
    }
}

void ope::dele(){
    
}

          
int main(){
    //    mysql mmysql;
        
        Driver *driver = get_driver_instance();
        Connection *conn = driver->connect(DBHOST, USER, PASSWORD);
        Statement *stm;

        if(!conn->isValid()){
            cout<<"数据库连接无效"<<endl;
            return 0;
        }else
            cout<<"数据库连接成功"<<endl;
        stm = conn->createStatement();
        //stm->execute("INSERT INTO test(name, birth) VALUES(?, ?)");
        stm = conn -> Statement("INSERT INTO test01(name,birth) VALUES(?,?)");
        //stm -> setString (name,123);

    set<zuifan > s;
    cout << "add zuifan's name and birthday and num";
    int num;
    ope manage; 
    string name; 
    int birthday;
    cin >> name >> birthday >> num;
    manage.add(s, name, birthday, num);
    stm -> setString(name,123);
    cout << "add zuifan's name and birthday and num";
    cin >> name >> birthday >> num;
    manage.add(s, name, birthday, num);
    cout << "add zuifan's name and birthday and num";
    cin >> name >> birthday >> num;
    manage.add(s, name, birthday, num);
    manage.cat(s);
    //string str = fast_writer.write(ope::str);
    //cout << manage.str << endl;//快速输出,紧凑型
    manage.check(s, 1);
    return 0; 
}

