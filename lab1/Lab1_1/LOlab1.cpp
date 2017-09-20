// LOlab1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;


vector<string> leftOpt(vector<string>&, vector<string>&);
vector<string> Del_E(vector<string>&);
vector<string> rightOpt(vector<string>&, vector<string>& );


int main()
{    /*char **inputArr = new char *[str];
		for (int i = 0; i < str; i++)
			inputArr[i] = new char [inColm];*/
	setlocale(LC_ALL, "Russian");
	int str = 3, len;
	int kindGram;
	vector<string>inputArr(str);
	vector<string>outGram(str);
	
	bool flag;
	do {
		flag = false;
		cout << "Введите матрицу\n";
		for (int i = 0; i < str ; i++) {
			getline(cin, inputArr[i]);
			if (inputArr[i].length() == 0) {
				cout << "Ошибка в программе, не вводите пустые строки!" << endl;
				system("pause");
				exit(0);
			}
		}
		for (int i=0;i<str;i++)
			for (int j = 0; j < 2; j++) {
				if ((inputArr[i][j] != 'A' && inputArr[i][j] != 'B' && inputArr[i][j] != 'C'&& inputArr[i][j] !=' ' ) || (inputArr[i][2] != '0' && inputArr[i][2] != '1'))
					flag = true;
			}
	} while (flag);

	cout << "1 - Построить правостороннюю грамматику\n";
	cout << "2 - постороить левостороннюю грамматику\n";
	cin >> kindGram;
	outGram[0] = "A::=";
	outGram[1] = "B::=";
	outGram[2] = "C::=";
	/*for (int i = 0; i < str; i++) {
		for (int j=0; j<2;j++)
		if (inputArr[i][j] == '\\' && inputArr[i][j+1] == '0') {
			inputArr[i][j] = '\0';
			if (j==0)
				inputArr[i][1] = inputArr[i][2];
			inputArr[i].erase(2, 1);
		}
	}*/
	
	if (kindGram == 1) {
		for (int i = 0; i < str; i++) {
			if (inputArr[i][0] != '\0') {
				if (inputArr[i][0] != ' ')
					outGram[i] = outGram[i] + "0" + inputArr[i][0];
				else if (inputArr[i][1] != ' ' && inputArr[i][1] != '\0')
					outGram[i] = outGram[i] + "1" + inputArr[i][1];
				else outGram[i] = outGram[i];
				if (inputArr[i][0] != ' ' && inputArr[i][1] != '\0' && inputArr[i][1] != ' ')
					outGram[i] = outGram[i] + "|" + "1" + inputArr[i][1];
			}
		}

		for ( int i =0; i<str; i++)
			if (inputArr[i][0] != '\0') {
				if (inputArr[i][2] == '1') {
					if (outGram[i].length() > 4)
						outGram[i] = outGram[i] + "|" + "e";
					else outGram[i] = outGram[i] + "e";
				}
			}

	}
	if (kindGram == 2) {
		vector<string>forLeftGram(3);
		forLeftGram[0] = "A";
		forLeftGram[1] = "B";
		forLeftGram[2] = "C";

		for (int i = 0; i < str; i++) {
			for (int j = 0; j < 2; j++) {
				if (inputArr[i][0] != '\0') {
					if (inputArr[i][j] == 'A') {
						outGram[0] = outGram[0] + forLeftGram[i] + to_string(j) + "|";
					}
					if (inputArr[i][j] == 'B') {
						outGram[1] = outGram[1] + forLeftGram[i] + to_string(j) + "|";
					}
					if (inputArr[i][j] == 'C') {
						outGram[2] = outGram[2] + forLeftGram[i] + to_string(j) + "|";
					}
				}
			}
		}
		for (int i = 0; i < str; i++)
		if (outGram[i][outGram[i].length() - 1] == '|')
				outGram[i].erase(outGram[i].length()-1, 1);
		for (int i = 0; i<str; i++)
			if (inputArr[i][0] != '\0') {
				if (inputArr[i][2] == '1') {
					if (outGram[i].length() > 4)
						outGram[i] = outGram[i] + "|" + "e";
					else outGram[i] = outGram[i] + "e";
				}
			}
	}
	if (kindGram == 1)
	{
		rightOpt(outGram,inputArr);
		Del_E(outGram);
	}
	
	if (kindGram == 2) {
		leftOpt(outGram,inputArr);
		Del_E(outGram);
	}
	for (int i = 0; i < inputArr.size(); i++) {
		if (outGram[i][3] != '=' || outGram[i][4] == '\0') {
			outGram[i].erase(0, 4);
		}
		cout << outGram[i] << endl;
	}
	
	system("pause");
    return 0;
}

vector<string> rightOpt(vector<string>&gram, vector<string>&inArr) {
	if (inArr[0][2] == '1') {
		if (inArr[0][0] == ' ' && inArr[0][1] == ' ') {
			for (int i = 0; i < 3; i++)
				for (int j = 1; j < gram[i].length(); j++)
					if (gram[i][j] == 'A')
						gram[i].erase(j, 1);
		}
		else for (int i = 0; i < 3; i++)
			for (int j = 1; j < gram[i].length(); j++)
				if (gram[i][j] == 'A')
					if (gram[i][j - 1] == '0')
						gram[i] = gram[i] + "|0";

					else
						gram[i] = gram[i] + "|1";
	}
	if (inArr[1][2] == '1') {
		if (inArr[1][0] == ' ' && inArr[1][1] == ' ') {
			for (int i = 0; i < 3; i++)
				for (int j = 1; j < gram[i].length(); j++)
					if (gram[i][j] == 'B')
						gram[i].erase(j, 1);
		}
		else for (int i = 0; i < 3; i++)
			for (int j = 1; j < gram[i].length(); j++)
				if (gram[i][j] == 'B')
					if (gram[i][j - 1] == '0')
						gram[i] = gram[i] + "|0";

					else
						gram[i] = gram[i] + "|1";
	}
	if (inArr[2][2] == '1') {
		if (inArr[2][0] == ' ' && inArr[2][1] == ' ') {
			for (int i = 0; i < 3; i++)
				for (int j = 1; j < gram[i].length(); j++)
					if (gram[i][j] == 'C')
						gram[i].erase(j, 1);
		}
		else for (int i = 0; i < 3; i++)
			for (int j = 1; j < gram[i].length(); j++)
				if (gram[i][j] == 'C')
					if (gram[i][j - 1] == '0')
						gram[i] = gram[i] + "|0";

					else
						gram[i] = gram[i] + "|1";
	}

	return gram;
}
vector<string> leftOpt(vector<string>&gram, vector<string>&inArr) {
	int st = 0;
	if (inArr[0][2] == '1') {
		if (inArr[0][0] == ' ' && inArr[0][1] == ' ') {
			for (int i = 0; i < 3; i++)
				for (int j = 1; j < gram[i].length(); j++)
					if (gram[i][j] == 'A')
						gram[i].erase(j, 1);
		}
		else for (int i = 0; i < 3; i++)
			for (int j = 1; j < gram[i].length(); j++)
				if (gram[i][j] == 'A')
					if (gram[i][j+1 ] == '0')
						gram[i] = gram[i] + "|0";

					else
						gram[i] = gram[i] + "|1";
	}
	if (inArr[1][2] == '1') {
		if (inArr[1][0] == ' ' && inArr[1][1] == ' ') {
			for (int i = 0; i < 3; i++)
				for (int j = 1; j < gram[i].length(); j++)
					if (gram[i][j] == 'B' )
						gram[i].erase(j, 1);
		}
		else for (int i = 0; i < 3; i++)
			for (int j = 1; j < gram[i].length(); j++)
				if (gram[i][j] == 'B' )
					if (gram[i][j+1] == '0'&& gram[i][gram[i].length()-1]!='0' && gram[i][gram[i].length()-3] != '0')
						gram[i] = gram[i] + "|0";
					else if (gram[i][j + 1] == '1'&& gram[i][gram[i].length() - 1] != '1' && (gram[i][gram[i].length() - 3] != '1' ||
						isalpha(gram[i][gram[i].length() - 4])))
						gram[i] = gram[i] + "|1";
	}
	if (inArr[2][2] == '1') {
		if (inArr[2][0] == ' ' && inArr[2][1] == ' ') {
			for (int i = 0; i < 3; i++)
				for (int j = 1; j < gram[i].length(); j++)
					if (gram[i][j] == 'C')
						gram[i].erase(j, 1);
		}
		else for (int i = 0; i < 3; i++)
			for (int j = 1; j < gram[i].length(); j++)
				if (gram[i][j] == 'C')
					if (gram[i][j + 1] == '0'&& gram[i][gram[i].length() - 1] != '0' && gram[i][gram[i].length() - 3] != '0')
						gram[i] = gram[i] + "|0";
					else if (gram[i][j + 1] == '1'&& gram[i][gram[i].length() - 1] != '1' && (gram[i][gram[i].length() - 3] != '1'||
						isalpha(gram[i][gram[i].length() - 4])) )
						gram[i] = gram[i] + "|1";
	}
	
		return gram;
}

vector<string> Del_E(vector<string>& gram) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < gram[i].length(); j++) {
			if (gram[i][j] == 'e')
				gram[i] = gram[i].erase(j - 1, 2);
}
	return gram;
}