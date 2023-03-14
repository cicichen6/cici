#include<iostream>
#include<string>

using namespace std;

class Human
{
private:
    int age;
    string name;

public:
    //默认构造函数
    //Human();
    //重载构造函数
    Human(string humansName);
    Human(int humansAge, string humansName);
    
    void setAge(int humansAge);
    void setName(string humansName);
    void introduceSelf();
};

/*Human::Human(){
    age = 0;
    name = "xiaoming";
    cout << "默认构造函数。"  << endl;
}
*/
Human::Human(string humansName){
    name = humansName;
    age = 0;
    cout << "重载1" << endl;
}

Human::Human(int humansAge = 5, string humansName = "zhangsan")
            :age(humansAge),name(humansName)
{
    cout << "重载2" << endl;
}

void Human::setAge(int humansAge){
    age = humansAge;
}

void Human::setName(string humansName){
    name = humansName;
}

void Human::introduceSelf(){
    cout << "my name is " << name << ", my age is " << age << endl;
}


int main(){
    Human first("meimei");
    //first.setAge(15);

    Human second;
    //second.setAge(12);
    //second.setName("akang");

    first.introduceSelf();
    second.introduceSelf();

}
