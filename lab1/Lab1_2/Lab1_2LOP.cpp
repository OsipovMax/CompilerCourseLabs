// Lab1_2LOP.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "Russian");
	int state= 1, stateError = 0;
	string str;
	getline(cin, str);
	for (int i = 0; i < str.length() && stateError==0; i++) {
		if (state == 1) {
			if (str[i] == '1')
				state= 2;
		}
		else if (state == 2) {
			if (str[i] == '1')
				state = 3;
			else state = 1;
		}
		else if (state == 3) {
			if (str[i] == '0')
				state = 4;
			else stateError = -1;
		}
		else if (state == 4) {
			if (str[i] == '0')
				state = 1;
			else state = 2;
		}

	}
	for (int i = 0; i < str.length(); i++)
		if (!isdigit(str[i]))
			stateError = -1;
	if (stateError == -1) cout << "��������� �������, ������ �� ���������" << endl;
	else cout << "������ ���������\n";
		system("pause");
    return 0;
}

