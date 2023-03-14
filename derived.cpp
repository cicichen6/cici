#include<iostream>
using namespace std;

class shape
{
public:	
    shape(int num){
	cout << "具有形状" << num << endl;
    }
    void output(){
        cout << "1"  << endl;
    }
};    

class polygon:private shape
{
public:
    polygon():shape(111){	
        cout << "多边形" << endl;
    }
    //void output(){
//	cout << "2" << endl;
  //  }
};

class triangle:protected polygon
{
public:	
    triangle(){ 
        cout << "三角形" << endl;
    }
    void output_(){
        output();
    }
};	       

int main(){
    triangle sanjiao;
    cout << "创建三角形" << endl;
    sanjiao.output_();
	return 0;
}
