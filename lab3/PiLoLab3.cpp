// PiLoLab3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include  <iostream>
#include <string>
#include <stack>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	string inputStr, outputStr;
	int state = 1, a, stError=0;
	stack <int> myStack;
	cout << "1 - примитивный МП-автомат" << endl;
	cout << "2 - непримитивный МП-автомат" << endl;
	cin >> a;
	if (a == 1) {
    cout << "Введите строку"<<endl;
	cin >> inputStr;
	for (int i = 0; i < inputStr.length() && stError == 0; i++) {
		if (state == 1) {
			if (myStack.empty() && inputStr[i] == 'a')
				myStack.push(1);
			else if (!myStack.empty() && myStack.top() == 1 && inputStr[i] == '.')
				myStack.push(2);
			else if (!myStack.empty() && myStack.top() == 2 && inputStr[i] == 'b')
				myStack.push(3);
			else if (!myStack.empty() && myStack.top() == 3 && inputStr[i] == 'a')
				myStack.push(1);
			else if (!myStack.empty() &&myStack.top() == 3 && inputStr[i] == 'b') {
				state = 2;
				i--;
			}
			else stError = -1;
		}
		else { 
			if (myStack.top() == 3 && inputStr[i] == 'b')
				myStack.pop();
			if (myStack.top() == 2 && inputStr[i] == '.')
				myStack.pop();
			if (myStack.top() == 1 && inputStr[i] == 'a')
				myStack.pop();
			}
		}
	if (stError == 0 && myStack.empty() /*|| myStack.top() == 3 */)
		cout << "строка - верна" << endl;
	else cout << "Ошибка" << endl;

	}
	else {
		cout << "Введите строку" << endl;
		cin >> inputStr;
		myStack.push(0);
		myStack.push(1);
		for (int i = 0; i < inputStr.length() && stError ==0; i++) {

			if (inputStr[i] == '1') {
				if (myStack.top() == 1)
					outputStr = outputStr + '1';
				else stError = -1;
			}
			else if (inputStr[i] == '0' && myStack.top() == 1 && i!=0) {
			      myStack.pop();
				  i--;
			}

			else if (inputStr[i] == '0') {
					if (myStack.top() == 0)
						outputStr =outputStr + '2' +'2';
					else stError = -1;
			}
			else stError = -1;
			
		}
		if (stError == -1 || myStack.top()==1)
			cout << "Ошибка" << endl;
		else
			for (int i = 0; i < outputStr.length(); i++ )
				cout<< outputStr[i];
		cout << endl;
	}

	
   
	system("pause");
    return 0;
}

