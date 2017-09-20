// PiLoLab4.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <string>


using namespace std;

static int code[256];

string toPolsk(string str) {

	return str;
}

struct Token
{
	string    type;
	int  number;
	string alph;
	string tree;
	int st=0;
};


vector<Token> ExpressionConverter(string &exp)
{
	string::iterator it, begin;
	vector<Token> result;
	stack<char> expstack;
	Token   t;

	for (it = exp.begin(); it != exp.end(); it++)
	{
		if (!isalnum(*it) && !strchr("=+-*/()", *it))
			continue;

		if (isdigit(*it))
		{
			begin = it;
			for (; it != exp.end(); it++)
			{
				if (!isdigit(*it))
					break;
			}
			t.number = atof(string(begin, it).c_str());
			t.type = 'n'; 
			result.push_back(t);

			it--; 
			continue;
		}
		else if (isalpha(*it))
		{
			begin = it;
			for (; it != exp.end(); it++)
			{
				if (!isalpha(*it))
					break;
			}
			t.alph = string(begin, it).c_str();
			t.type = 'a'; 
			result.push_back(t);

			it--;
			continue;
		}
		else
		{
			if (expstack.empty()) // в пустой стек пихам всегда
			{
				expstack.push(*it);
				continue;
			}

			if (*it == '(') // скобку наверх
			{
				expstack.push(*it);
				continue;
			}

			if (*it == ')') // выталкиваем все до открывающей скобки.
			{              // сами скобки уничтожаем
				while (expstack.top() != '(')
				{
					t.type = expstack.top();
					result.push_back(t);
					expstack.pop();
				}

				expstack.pop();
				continue;
			}
			// заталкиваем операцию в стек в зависимости от приоритета
			while (!expstack.empty() && code[expstack.top()] >= code[*it])
			{
				t.type = expstack.top();
				result.push_back(t);
				expstack.pop();
			}

			expstack.push(*it);
		}
	}
	// выталкиваем все что осталось         
	while (!expstack.empty())
	{
		t.type = expstack.top();
		result.push_back(t);
		expstack.pop();
	}

	return result;
}

void PrintVectorOfTokens(vector<Token> &vs)
{
	vector<Token>::iterator it;

	for (it = vs.begin(); it != vs.end(); it++)
	{
		if (it->type == "n") {
			cout << it->number << ' ';
			cout << it->tree << ' ';
		}
		else if  (it->type == "a") {
			cout << it->alph << ' ';
			cout << it->tree << ' ';
		}
		else {
			cout << it->type << ' ';
			cout << it->tree << ' ';
		}
	}

}
void pCode()
{
	code['*'] = 3;
	code['/'] = 3;
	code['+'] = 2;
	code['-'] = 2;
	code['('] = 1;
	code[')'] = 0;
}
vector<Token> copyT(vector<Token> res) {
	vector<Token> copV;
	for (int i = 0; i < res.size(); i++) {
		copV.resize(i + 1);
		copV[i] = res[i];
	}
	return res;
}

vector<string> opt(vector<string> AsCode) {
	vector<string> copy;
	bool flag = true, flag1=false;
	int begin = 0, br=0,end= 0;
	string str, spStr, swap;
	str.resize(10);
	copy.resize(50);
	spStr.resize(10);
	int p = 0;
	for (int i = 0; i < AsCode.size(); i++) {
		if (AsCode[i].substr(0, 4) == "push" && AsCode[i-1].substr(0,3)=="mov") {
			AsCode[i - 1].replace(4, 2, "dx");
				br++;
				str = AsCode[i - 1];
				begin = i;
				flag = true;
		}
		else if (AsCode[i].substr(0, 3) == "pop" && br!=0 && flag==true) {
			end = i;
			for (int k = end - 1; AsCode[k].substr(0,4)!="push" ; ) {
				AsCode.insert(AsCode.begin() + begin - 1, AsCode[k]);
				AsCode.erase(AsCode.begin() + k+1);
			}
			AsCode.erase(AsCode.begin() + end - 1);
			AsCode.erase(AsCode.begin() + end-1);
			br--;
			flag = false;
		}
		
	}

	if (br!= 0)
		AsCode = opt(AsCode);

	return AsCode;
}

vector<string> opt2(vector<string>AsCode) {

	string swap,buf;
	swap.resize(10);
	//buf.resize(10);
	int cnt = 0;
	for (int k = 0; k < AsCode.size(); k++) {
		if (AsCode[k].substr(0, 4) == "xchg")
			AsCode.erase(AsCode.begin() + k);
		if (AsCode[k].substr(0, 1) == "/" || AsCode[k].substr(0, 1) == "-") {
			swap = AsCode[k - 2];
			AsCode[k - 2] = AsCode[k - 1];
			AsCode[k - 1] = swap;
			AsCode[k - 1].replace(4, 2, "dx");
			AsCode[k - 2].replace(4, 2, "ax");
		}	
	}
	for (int k = 0; k < AsCode.size(); k++) {
		if (AsCode[k].substr(0, 3) == "mov" && (isdigit(AsCode[k][7]) /*|| isalpha(AsCode[k][7])*/)) {
			for (int p = 7; p < AsCode[k].length(); p++)
				if (isdigit(AsCode[k][p])/*|| isalpha(AsCode[k][7])*/)
					buf = buf + AsCode[k][p];
			if (AsCode[k].substr(4, 2) == "ax") {
				AsCode[k + 1].replace(4, 2, "ax");
				AsCode[k + 2].replace(5, buf.length(), buf);
			}
			else AsCode[k + 1].replace(5, buf.length(), buf);
			AsCode.erase(AsCode.begin() + k);
		}
	}
	for (int k = 0; k < AsCode.size(); k++) {
		if (AsCode[k].substr(0, 4) == "push" && AsCode[k + 1].substr(0, 3) == "pop") {
			AsCode.erase(AsCode.begin() + k);
			AsCode.erase(AsCode.begin() + k);
		}
	}
	return AsCode;
}
int main()
{
	string str;
	vector<Token> pol;
	vector<Token> copPol;
	vector <string> AsCode;
	stack<Token> btree;
	AsCode.resize(50);

	pCode();
	setlocale(LC_ALL, "russian");
	cout << "Введите выражение" << endl;
	getline(cin, str);

	pol = ExpressionConverter(str);
	cout << "Converted expression: ";
	//PrintVectorOfTokens(pol);
	cout << endl;
	copPol = copyT(pol);
	//PrintVectorOfTokens(copPol);

	bool flag = false;
	for (int i = 0; i < copPol.size();i++) {
		if (copPol[i].type == "n" || copPol[i].type == "a") {
			btree.push(copPol[i]);
			btree.top().st = i;
		}
		else {
			btree.top().tree = "r";
			copPol[btree.top().st] = btree.top();
			btree.pop();
			btree.top().tree = "l";
			copPol[btree.top().st] = btree.top();
			btree.pop();
			btree.push(copPol[i]);
			btree.top().st = i;
		}
	}
	int j = 0;
	PrintVectorOfTokens(copPol);
	cout << endl;

	for (int i = 1; i < pol.size(); i++) {

		if ( copPol[i].tree == "l") {
			if (copPol[i].type == "n") {
				AsCode[j] = "mov ax," + to_string(copPol[i].number);
				AsCode[j + 1] = "push ax"; 
			}
			else if (copPol[i].type == "a"){
				AsCode[j] = "mov ax, " + copPol[i].alph;
				AsCode[j + 1] = "push ax"; 
			}
			else if (copPol[i].type != "n" || copPol[i].type != "a")
				AsCode[j] = "push ax";
			if (copPol[i].type != "n" && copPol[i].type != "a")
				j = j + 1;
			else j = j + 2;
		}
		else if ( copPol[i].tree == "r" && i != copPol.size() - 2) {
			if (copPol[i].type == "n") {
				AsCode[j] = "mov ax," + to_string(copPol[i].number);
				AsCode[j + 1] = "pop dx";
				AsCode[j + 2] = "xchg ax,dx";
				AsCode[j + 3] = copPol[i + 1].type + " ax,dx";
			}
			else if (copPol[i].type == "a") {
				AsCode[j] = "mov ax, " + copPol[i].alph;
				AsCode[j + 1] = "pop dx";
				AsCode[j + 2] = "xchg ax,dx";
				AsCode[j + 3] = copPol[i + 1].type + " ax,dx";
			}
			else if (copPol[i].type != "n" || copPol[i].type != "a") {
				AsCode[j ] = "pop dx";
				AsCode[j + 1] = "xchg ax,dx";
				AsCode[j + 2] = copPol[i + 1].type + " ax,dx";
			}
			if (copPol[i].type != "n" && copPol[i].type != "a")
				j = j + 3;
			else j = j + 4;
		}
		
	}
	AsCode[j] = "mov " + copPol[0].alph + ",ax";

	for (int i = 0; i < AsCode.size(); i++)
		cout << AsCode[i] << endl;
	cout<<endl;


	AsCode = opt(AsCode);
	AsCode = opt2(AsCode);
	for (int i = 0; i < AsCode.size(); i++)
		cout << AsCode[i] << endl;
	system("pause");
    return 0;
}

