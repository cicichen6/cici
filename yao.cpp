#include <iostream>
using namespace std;

void sortArray(int array[], int count){
    int tempCount = count;
    if(array[tempCount] < array[tempCount - 1]){
        do{
	    swap(array[tempCount], array[tempCount - 1]);
	    tempCount = tempCount -1;
        }while(array[tempCount] >= array[tempCount -1]);
    }
}

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

void arrDisplay(int array[], int count){
    for(int ii = 0; ii <= count; ii++){
        cout << array[ii];
    }
}

int main(){
    int array[20] = {0};
    int input = 1;
    int count = 0;
    cout << "输入一个数（输入0停止输入）：";
    cin >> input;
    array[count] = input;
    count ++;
    while(1){    
        cout << "输入一个数（输入0停止输入）：";
        cin >> input;
	if(input != 0){
	array[count] = input;
	if(count >= 2){
	    sortArray(array, count);
	}
	else{
	   if(array[0] > array[1]){
	       swap(array[0], array[1]);
	   } 
	}
	}
	else{break;}
        count++;	
    }
    cout << "排序后的数组元素值依次为：";
    arrDisplay(array, count-1);
    cout << endl;
return 0;
}
