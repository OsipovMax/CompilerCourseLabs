// LAB1_3LPO.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "Russian");
	int state = 0, stateError = 0;
	string str;

	getline(cin, str);

	for (int i = 0; i < str.length() && stateError == 0; i++) {
		cout << state << endl;
		if (state == 0) {
			if ( (str[i] == '-' || str[i] == '+') )
				state = 1;
			else if (isdigit(str[i]))
				state = 4;
			else if (isalpha(str[i]))
				state = 3;
			else if (str[i] == ' ')
				state = 0;
			else stateError = -1;

		}
		else if (state == 1){
			if (isdigit(str[i]))
				state = 4;
			else if (isalpha(str[i]))
				state = 3;
			else stateError = -1;
		}

		else if (state == 3) {
			if (str[i] == ' ')
				state = 3;
			else if (str[i] == '+' || str[i] == '-' || str[i]== '/' ||str[i]== '*' ||str[i]== '^' || str[i]=='=')
				state =0 ;
			else stateError = -1;
		}
		else if (state == 4) {
			if (isdigit(str[i]))
				state = 4;
			else if (str[i] == '.')
				state = 5;
			else if (str[i] == ' ')
				state = 3;
			else if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*' || str[i] == '^' || str[i] == '=')
				state = 0;
			else stateError = -1;
		}
		else if (state == 5) {
			if (isdigit(str[i]))
				state = 5;
			else if (str[i] == ' ')
				state = 3;
			else if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*' || str[i] == '^' || str[i] == '=')
				state = 0;
			else stateError = -1;
		}

	}

	if ((state==3 ||state ==4 ||state==5) && stateError != -1) cout << "Строка разобрана\n";
	else cout << "Строка не разобрана\n";
	system("pause");
    return 0;
}

