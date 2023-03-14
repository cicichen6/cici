#include <iostream>
#include <string>

using namespace std;

class zuifan
{       
        public:
                string name;
                int birthday;
        
        /*public:
               zuifan(string Name, int Birthday)
               {   
                   name = Name;
                   birthday = Birthday;
               }*/
}

class operate
{
    public:
	    void add(zuifan, string, int);
}

void operate::add(zuifan zf, string name, int birth){
    zf.name = name;
    zf.birthday = birth;
    cout << zf << "'s name :" << zf.name << " birth :" << zf.birthday << endl; 
}

int main(){
    cout << "add zuifan's name and birthday:";
    zuifan zf;
    operate manage;
    string name;
    int birthday;
    cin << name << birthday;
    manage.add(zf, name, birthday);
    return 0;
}
