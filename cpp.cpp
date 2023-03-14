/*#include<iostream>
using namespace std;

int main(){
    auto Number = 250;
    cout << "size Number = " << sizeof(Number) << endl;

    return 0; 
}*/
/*
#include<iostream>
using namespace std;

const float Pi = 3.1415;

float Volume(float);
float Volume(float, float);

int main(){
    cout << "输入一个球的半径" << endl;
    float Radius = 0;
    cin >> Radius;
    cout << "球的体积 = " << Volume(Radius) << endl;

    cout << "输入一个圆柱体的半径和高" << endl;
    float Height = 0;
    cin >> Radius >> Height;
    cout << "圆柱体的体积 = " << Volume(Radius, Height) << endl;

    return 0;
}

float Volume(float Radius){
    return 4 * Pi * Radius * Radius * Radius;
}

float Volume(float Radius, float Height){
    return Pi * Radius * Radius * Height;
}
*/

/*#include<iostream>
using namespace std;

void displayArray(double [], int);

int main(){
    double array[5] = {0};
    int arrayElem = 0;
    //cout << "输入五个数组元素" << endl;
    do{
	cout << "输入第" << arrayElem << "个数组元素" << endl;    
	cin >> array[arrayElem];
	arrayElem++;
    }while(arrayElem != 5);

    displayArray(array, arrayElem); 
    return 0;
}
 
void displayArray(double array[], int Length){
    for(int index = 0; index < Length; index++){
        cout << array[index] << endl;
    }
}*/

#include<iostream>
using namespace std;

void Area(int, float&);

int main(){
    int Radius;
    float area;
    cout << "输入一个圆的半径" << endl;
    cin >> Radius;
    Area(Radius, area);
    cout << "area is :" << area << endl;
}

void Area(int R, float& result){
     result = 3.14 * R * R;
}

