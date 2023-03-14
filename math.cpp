#include <iostream>
#include <math.h>
using namespace std;

int pw_func(const double& x){
	if(x >= 1 && x < 3){
	        cout << sqrt((double)x) << endl;
		return 0;
	}
	if(x >= 3 && x < 5){
		cout << x << endl;
		return 0;
	}
	if(x >= 5 && x < 7){
		cout << pow((double)x, 2) << endl;
		return 0;
	}
	
	cout << "over scope!" << endl;
        return 0;	
}

int main(){
    double x, y = 0;
    while(true){	    
        cout << "input number (if u gonna quit, please enter -1) :" << endl;
        cin >> x;
	if(x == -1) { break; }
        y = pw_func(x);
    }
    cout << "bye!" << endl;
    return 0;
}
