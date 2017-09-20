// PiLoLab2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <string.h>
#include<iostream>
#include <vector>
#include <iomanip>

using namespace std;

#define SZ 15

struct  structA {

	vector<vector <int>> matr;

	vector<string> ParsMatr;
	string expr;
	
	int partExpr , h;
	
	vector<int>state;
	
};
//объявление функций ( без этого нельзя вызывать бы их было)
structA *pars(structA&); 
structA *Del_nState(structA&);
structA *Del_e(structA&);
structA *Del_eqv(structA&);
structA *to_DKA(structA&);
structA *to_ODKA(structA&);


structA *NKA (string inStr) {
	structA *object = new structA; //объект структуры 
	object->matr.resize(SZ); 
	for (int i = 0; i < object->matr.size(); i++)
		object->matr[i].assign(SZ, -1);  
	object->ParsMatr.resize(SZ); //аналогично 
	for (int i = 0; i < object->ParsMatr.size(); i++)
		object->ParsMatr[i].resize(SZ);
	object->state.resize(SZ);
	for (int i = 0; i < SZ; i++)
		object->state.assign(SZ, -1);
	object->partExpr = 1; 
	object->h = 2;
	object->state[1] = 2; 
	object->state[2] = 1; 
	object->matr[1][0] = 1;
    object->matr[2][0] = 2; 
	object->matr[1][1] = 2;
	object->ParsMatr[1] = inStr;
	object = pars(*object); 
	return object;
}
// строим оптимизацию НКА
structA *ONKA(structA& obj) {

	obj = *Del_e(obj); 

	obj = *Del_nState(obj);

	obj = *Del_eqv(obj); 

	//obj = *Del_nState(obj);
	
	return &obj; 
}

structA *DKA(structA& obj) {
	obj = *to_DKA(obj);
	//obj.matr[5][2] = 2;
	//obj = *Del_eqv(obj);
	return &obj;
}

structA *ODKA(structA& obj) {
	obj = *to_ODKA(obj);
	return &obj;
}


// функция разбора регулярки
structA *pars(structA& obj) {
	bool flag = false, flag1 = false;
	string left, right;
	int cnt = 2;

	vector<string> brackets;
	brackets.resize(SZ);
	for (int i = 0; i < brackets.size(); i++)
		brackets[i].resize(SZ);

			// парсим (  ) 
	for (int i = 1; i <= obj.partExpr; i++) {
		int begin = -1, br=0;
		for (int j = 0; j < obj.ParsMatr[i].length(); j++) {
			if (obj.ParsMatr[i][j] == '(') {
				if (begin == -1)
					begin = j;
				else
					br++;
			}
			if (obj.ParsMatr[i][j] == ')') {
				if (br == 0) {
					left = obj.ParsMatr[i].substr(0, begin);
					brackets[cnt-2] = obj.ParsMatr[i].substr(begin + 1, j - begin - 1);// положили сюда то что внури скобок , а потом мы просто будем копировать отсюда в матрицу разбора 
					if (j < obj.ParsMatr[i].length() - 1)
						right = obj.ParsMatr[i].substr(j + 1);
					obj.ParsMatr[i] = left + to_string(cnt) + right;
					
					cnt++;
					j = 1;
					begin = -1;
				}
				else br--;
			}
		}

	}

	while (flag == false)
	{
		flag = true;


		//парсим |
		for (int i = 1; i <= obj.partExpr; i++)
			for (int j = 0; j < obj.ParsMatr[i].length(); j++) {
				if (obj.ParsMatr[i][j] == '|') {
					flag = false;
					obj.partExpr++;
					left = obj.ParsMatr[i].substr(0, j);
					right = obj.ParsMatr[i].substr(j + 1, obj.ParsMatr[i].length());
					obj.ParsMatr[i] = left;
					obj.ParsMatr[obj.partExpr] = right;
					for (int k = 1; k <= obj.h; k++)
						obj.matr[k][obj.partExpr] = obj.matr[k][i];
				}
			}

		//парсим х
		for (int i = 1; i <= obj.partExpr; i++)
			for (int j = 0; j < obj.ParsMatr[i].length(); j++) {
				if (obj.ParsMatr[i][j] == 'x') {
					flag = false;
					obj.partExpr++;
					obj.h++;
					obj.state[obj.h] = 0;
					left = obj.ParsMatr[i].substr(0, j);
					right = obj.ParsMatr[i].substr(j + 1, obj.ParsMatr[i].length());
					obj.ParsMatr[i] = left;
					obj.ParsMatr[obj.partExpr] = right;
					obj.matr[obj.h][0] = obj.h; 
					for (int k = 1; k <= obj.h; k++)
						if (obj.matr[k][i] != -1)
							obj.matr[obj.h][obj.partExpr] = obj.matr[k][i];
					for (int k = 1; k <= obj.h; k++)
						if (obj.matr[k][i] != -1) {
							obj.matr[k][i] = obj.h;
						}
				}
			}

		//pArsim *
		for (int i = 1; i <= obj.partExpr; i++)
			for (int j = 0; j < obj.ParsMatr[i].length(); j++) {
				if (obj.ParsMatr[i][j] == '*') {
					flag = false;
					obj.partExpr++;
					obj.h++;
					obj.state[obj.h] = 0;
					left = obj.ParsMatr[i].substr(0, j);
					right = obj.ParsMatr[i].substr(j + 1, obj.ParsMatr[i].length());
					obj.ParsMatr[i] = left;
					obj.matr[obj.h][0] = obj.h;
					obj.ParsMatr[obj.partExpr] = to_string(-2);//e

					for (int l = 1; l <= obj.h; l++)
						if (obj.matr[l][i] != -1)
						{
							obj.matr[l][obj.partExpr] = obj.h;
							obj.matr[obj.h][obj.partExpr] = obj.matr[l][i];
						}
					for (int l = 1; l <= obj.h; l++)
						if (obj.matr[l][i] != -1)
							obj.matr[l][i] = -1;
					obj.matr[obj.h][i] = obj.h ;
					if (right.length() != 0)
					{
						obj.partExpr++;
						obj.ParsMatr[obj.partExpr] = right;
						obj.matr[obj.h][obj.partExpr] = obj.h;
					}
				}
			}

		//парсим +
		for (int i = 1; i <= obj.partExpr; i++)
			for (int j = 0; j < obj.ParsMatr[i].length(); j++) {
				if (obj.ParsMatr[i][j] == '+') {
					flag = false;
					obj.partExpr++;
					obj.h++;
					obj.state[obj.h] = 0;
					left = obj.ParsMatr[i].substr(0, j);
					right = obj.ParsMatr[i].substr(j + 1, obj.ParsMatr[i].length());
					obj.ParsMatr[i] = left;
					obj.matr[obj.h][0] = obj.h;
					obj.ParsMatr[obj.partExpr] = to_string(-2);

					for (int l = 1; l <= obj.h; l++)
						if (obj.matr[l][i] != -1)
						{
							obj.matr[obj.h][obj.partExpr] = obj.matr[l][i];
						}
					for (int l = 1; l <= obj.h; l++)
						if (obj.matr[l][i] != -1)
							obj.matr[l][i] = obj.h ;
					obj.matr[obj.h][i] = obj.h;
					if (right.length() != 0)
					{
						obj.partExpr++;
						obj.ParsMatr[obj.partExpr] = right;
						obj.matr[obj.h][obj.partExpr] = obj.h;

					}
				}
			}
	}
	// нулевая строка выходной матрицы
	for (int i = 1; i < obj.partExpr + 1; i++)
		obj.matr[0][i] = atoi(obj.ParsMatr[i].c_str());

	// Объединим переходы по одинаковым символам
	for (int i = 1; i <= obj.partExpr; i++)
		for (int j = 1; j <= obj.partExpr; j++)
			if (obj.matr[0][i] == obj.matr[0][j] && i != j)
				for (int l = 1; l <= obj.h; l++)
				{
					if (obj.matr[l][i] != -1)
					{
						if (obj.matr[l][j] != -1)
						{
							if (obj.matr[l][i] != obj.matr[l][j])
							{
								obj.matr[l][i] = obj.matr[l][i] * 100 + obj.matr[l][j];
							}
						}
					}
					else
						obj.matr[l][i] = obj.matr[l][ j];
				}


	bool indicator = false;
	int k = 0;

	// Обратная замена терминалов на выражения из скобок
	for (int i = 1; i <= obj.partExpr; i++)
		if (obj.matr[0][i] != 0 && obj.matr[0][i] != 1 && obj.matr[0][i] != -2)
		{
			indicator = true;
			obj.ParsMatr[i] = brackets[k];
			k++;
		}

	// Рекурсивный спуск для парсинга содержимого скобок
	if (indicator == true)
	obj= *pars(obj);

	
	int cut_width = 0;
	// Удаляем одинаковые столбцы
	for (int i = 1; i <= obj.partExpr; i++)
		for (int j = 1; j <= obj.partExpr; j++)
			if (obj.matr[0][ i] == obj.matr[0][ j] && i != j && obj.matr[0][ i] != -1)	{
				for (int l = 0; l <= obj.h; l++)
					obj.matr[l][j] = -1;
						cut_width++;
			}

	// Сдвигаем столбцы, чтобы не было пропусков
	for (int i = 1; i <= obj.partExpr; i++)
		for (int j = i + 1; j <= obj.partExpr; j++)
			if (obj.matr[0][ i] == -1 && obj.matr[0][ j] != -1)
				for (int l = 0; l <= obj.h; l++){
					obj.matr[l][ i] = obj.matr[l][ j];
					obj.matr[l][ j] = -1;
					}
					obj.partExpr = obj.partExpr - cut_width;


	return &obj;

	

}
// удаление e - символа
structA *Del_e(structA& obj) {
	bool flag = false;
	int buf = 0,dl=0,pl=0,cnt=0;
	vector<int> sup;
	//sup.resize(2);
	for (int i =1; i<=obj.partExpr; i++)
		if( obj.matr[0][i]== -2)
			for (int j = 1; j <= obj.h; j++) {
				if (obj.matr[j][i] != -1) {
					buf = obj.matr[j][i];

					if (buf > 100) {
						dl = buf;
						while (dl) {
							pl = dl % 100;
							dl = dl / 100;
							sup.push_back(pl);
						}
					}
					for (int l = 1; l <= obj.partExpr; l++) {
						if (buf > 100) {
							
							for (int ch = 1; ch >= 0; ch--) {
								if (i != l) {
									if (obj.matr[j][l] == -1)
										obj.matr[j][l] = obj.matr[sup[ch]][l];

									else if (obj.matr[j][l] != -1 && obj.matr[sup[ch]][l] != -1)
										obj.matr[j][l] = obj.matr[j][l] * 100 + obj.matr[sup[ch]][l];
								}
								else
									obj.matr[j][i] = obj.matr[sup[ch]][i];
								if (obj.state[sup[ch]] == 1 && obj.state[j] % 2 == 0) // %
									obj.state[j]++;
							}
							
						}
						else {
							if (i != l) {
								if (obj.matr[j][l] == -1)
								//	if (obj.matr[buf][l] == buf)
									//	obj.matr[j][l] = j;
									obj.matr[j][l] = obj.matr[buf][l];

								else if (obj.matr[j][l] != -1 && obj.matr[buf][l] != -1)
									obj.matr[j][l] = obj.matr[j][l] * 100 + obj.matr[buf][l];
							}
							else
								obj.matr[j][i] = obj.matr[buf][i];
						}
					}
					if(buf<100)
					if (obj.state[buf] == 1 && obj.state[j] % 2 == 0) // %
						obj.state[j]++;
				}

			}

	for (int i = 1; i <= obj.partExpr; i++)
		if (obj.matr[0][i] == -2)
			for (int j = 1; j <= obj.h; j++)
				if (obj.matr[j][i] != -1) {
					flag = true;
	
				}

	if (flag == true ) 
		obj = *Del_e(obj);

	for (int i = 1; i <= obj.partExpr; i++)
			if (obj.matr[0][i] == -2)
				for (int j = 0; j <= obj.h; j++)
					obj.matr[j].erase(obj.matr[j].begin() + i);

	return &obj;
}

void push_unique(std::vector<int>& arr, int number) {
	for (int i = 0; i < arr.size(); ++i)
		if (arr[i] == number)
			return;

	arr.push_back(number);
}

structA *Del_nState(structA& obj) {
	bool flag = true;
	vector<int> arr;
	int dl,pl, kl=0;

	for (int i = 1; i <= obj.partExpr; i++) {
		if (obj.matr[1][i] != -1) {
			if (obj.matr[1][i] >= 100) {
				dl = obj.matr[1][i];
				while (dl >= 1) {
					pl = dl % 100;
					dl = dl / 100;
					if (pl != 0)
						arr.push_back(pl);
				}
			}
			else arr.push_back(obj.matr[1][i]);
		}
	}
	dl = 0, pl = 0;

	for (int i = 0; i < arr.size(); i++) 

		for (int j = 1; j <= obj.partExpr; j++) {
			if (obj.matr[arr[i]][j] != -1) {
				dl = obj.matr[arr[i]][j];
				while (dl) {
					pl = dl % 100;
					dl = dl / 100;
					push_unique(arr, pl);// то что выше 
				}
			}
		}
	

	

	int  ct = 0;
	for (int i = 3; i <= obj.h ; i++) {   ////// i = 2 
		int inc = 0;
		for (int j = 0; j < arr.size(); j++)
			if (obj.matr[i][0] == arr[j]) {
				inc++;
			}
		if (inc == 0) {
			obj.matr[i].erase(obj.matr[i].begin(), obj.matr[i].begin() + 4);
		
		}
		}

	for (int i = 1; i <= obj.h; i++)
		for (int j = i + 1; j <= obj.h; j++)
			if (obj.matr[i][0] == -1 && obj.matr[j][0] != -1) {
				obj.state[i] = obj.state[j];
				obj.state[j] = -1;
				for (int l = 0; l <= obj.partExpr; l++) {
					obj.matr[i][l] = obj.matr[j][l];
					obj.matr[j][l] = -1;
				}
			}

	for (int i = 1; i <= obj.h; i++)
		if (obj.matr[i][0] == -1) {
			obj.h = i - 1;
			break;
		}
	for (int i = obj.h + 1; i < obj.state.size(); i++)
		obj.state[i] = -1;
		

	int b = 1;
	//удаляем состояния не пораждающие цепочки
for (int i = 1; i <= obj.h; i++) 
			if (obj.matr[i][0] == obj.matr[i][1] && obj.matr[i][0] == obj.matr[i][2]) {
				for (int p = 1; p <= obj.h; p++)
					for (int g = 1; g < obj.partExpr; g++) {
						if (obj.matr[p][g] == obj.matr[i][0])
							obj.matr[p][g] = obj.matr[p][0];
						else if (obj.matr[p][g] > 100) {
							dl = obj.matr[p][g];
							while (dl) {
								pl = dl % 100;
								dl = dl / 100;
								if (pl == obj.matr[i][0])
									pl = obj.matr[p][0];
								kl = kl + pl * b;
								b = b * 100;
							}
							obj.matr[p][g] = kl;
							//cout << kl;
						}
					}
				obj.matr[i].erase(obj.matr[i].begin(), obj.matr[i].begin() + 4);

			}

	for (int i = 1; i <= obj.h; i++)
		for (int j = i + 1; j <= obj.h; j++)
			if (obj.matr[i][0] == -1 && obj.matr[j][0] != -1) {
				obj.state[i] = obj.state[j];
				obj.state[j] = -1;
				for (int l = 0; l <= obj.partExpr; l++) {
					obj.matr[i][l] = obj.matr[j][l];
					obj.matr[j][l] = -1;	
				}
			}

	for (int i = 1; i <= obj.h; i++)
		if (obj.matr[i][0] == -1) {
			obj.h = i - 1;
			break;
		}
	for (int i = obj.h + 1; i < obj.state.size(); i++)
		obj.state[i] = -1;
	/*for (int i = 0; i < arr.size(); i++)
		cout <<"_"<< arr[i];*/

	
	return &obj;
}
// удаляет состояния эквивалентные
structA *Del_eqv(structA& obj) {
	vector<vector<int>>qv;
	int counter = 2,dl=0,pl=0,sw=0, kl=0; 
	bool flag1 = false, flag2 = false,flagR=false;
	qv.resize(15);

	for (int i = obj.h; i >=1; i--) {
		if (obj.state[i] % 2 == 0)
			qv[0].emplace(qv[0].begin(),obj.matr[i][0]);
		else 
			qv[1].emplace(qv[1].begin(),obj.matr[i][0]);
	}

	for (int i = 0; i < qv.size(); i++) {
		for (int j = 0; j < qv[i].size(); j++)
			cout << qv[i][j];
		cout << endl;
	}
	cout << endl;
	if (qv[0].empty() || qv[1].empty())
		return &obj;
	for (int i = 0; i < counter; i++)
		for (int j = 0; j < qv[i].size()- 1; j++)
			for (int k = j + 1; k < qv[i].size(); k++)
			{
				flag1 = false;
				for (int l = 1; l <=obj.partExpr; l++)
				{
					int p1 = -1;
					int p2 = -1;
					for (int n = 0; n < counter; n++)
						for (int m = 0; m < qv[n].size(); m++)
						{
							if (obj.matr[qv[i][j]][l]!=-1)
							{
								
								if (qv[n][m] == obj.matr[qv[i][j]][l])  
										p1 = n;

							}
							if (obj.matr[qv[i][k]][l] !=-1)
							{
									if (qv[n][m] == obj.matr[qv[i][k]][l])
										p2 = n;
							}
						}

					if (obj.matr[qv[i][j]][l] ==-1)
					{
						if (obj.matr[qv[i][k]][l] == -1)
						{
							flag1 = flag1;
						}
						else
						{
							flag1 = true;
						}
					}
					else
					{
						if (obj.matr[qv[i][k]][l] == -1)
						{
							flag1 = true;
						}
						else
						{
							if (p1 == p2)
								flag1 = flag1;
							else
								flag1 = true;
						}
					}
				}
				if (flag1 == true)
				{
					flagR = true;
					int a = 0;
					string b = "";
					qv[counter].emplace(qv[counter].begin(),qv[i][k]);
					counter++;

					qv[i].erase(qv[i].begin() + k);

					for (int loop = k - 1; loop < qv[i].size(); loop++)
					{
						flag1 = false;
						for (int l = 1; l <= obj.partExpr; l++)
						{
							int p1 = -1;
							int p2 = -1;
							for (int n = 0; n < counter; n++)
								for (int m = 0; m < qv[n].size(); m++)
								{
								
									if (obj.matr[qv[i][loop]][l]!=-1)
									{
											if (qv[n][m] == obj.matr[qv[i][loop]][l])
												p1 = n;
										
									}
									if (obj.matr[qv[counter-1][0]][l] !=-1)
									{
										
										
											if (qv[n][m]== obj.matr[qv[counter-1][0]][l])
												p2 = n;
									}
								}
							
							if (obj.matr[qv[i][loop]][l]==-1)
							{
								
								if (obj.matr[qv[counter-1][0]][l]==-1)
								{
									flag1 = flag1;
								}
								else
								{
									flag1 = true;
								}
							}
							else
							{
							
								if (obj.matr[qv[counter-1][0]][l] == -1)
								{
									flag1 = true;
								}
								else
								{
									if (p1 == p2)
										flag1 = flag1;
									else
										flag1 = true;
								}
							}
						}
						if (flag1 == false)
						{

							qv[counter-1].emplace(qv[counter-1].begin(), qv[i][loop]);
							qv[i].erase(qv[i].begin() + loop);
						}
					}
					k--;
				}
			}
			for (int i = 0; i < qv.size(); i++)
				if (qv[i].size() > 1)
					flagR = true;
				else flagR = false;
				if (flagR == true)
					obj = *Del_eqv(obj); // рекурсия 

	for (int i = 0; i < qv.size(); i++) 
		for (int j = 0; j < qv[i].size(); j++)
			if (qv[i].size()>1)
				if (qv[i][0] == 4) {
					sw = qv[i][0];
					qv[i][0] = qv[i][1];
					qv[i][1] = sw;
				}
	for (int i = 3; i <= obj.h; i++) {   ////// i = 2 
		int inc = 0;
		for (int j = 0; j < counter; j++)
			if (obj.matr[i][0] == qv[j][0]) {
				inc++;
			}
		if (inc == 0) {
			obj.matr[i].erase(obj.matr[i].begin(), obj.matr[i].begin() + 4);

		}
	}


		for (int i = 1; i <= obj.h; i++)
			for (int j = i + 1; j <= obj.h; j++)
				if (obj.matr[i][0] == -1 && obj.matr[j][0] != -1) {
					obj.state[i] = obj.state[j];
					obj.state[j] = -1;
					for (int l = 0; l <= obj.partExpr; l++) {
						obj.matr[i][l] = obj.matr[j][l];
						obj.matr[j][l] = -1;

					}
				}
		for (int i = 1; i <= obj.h; i++)
			if (obj.matr[i][0] == -1) {
				obj.h = i - 1;
				break;
			}
		for (int i = obj.h + 1; i < obj.state.size(); i++)
			obj.state[i] = -1;
		
		bool fl = false;
		for (int i = 1; i <= obj.h; i++) {
			for (int j = 1; j < obj.partExpr; j++) {
				fl = false;
				for (int p = 1; p <= obj.h; p++)
					if (obj.matr[i][j] == -1 || obj.matr[i][j]>100 || obj.matr[i][j] == obj.matr[p][0])
						fl = true;
				if (fl == false)
					obj.matr[i][j] = i;
			}
		}

				
			

	return &obj;
}

structA *to_DKA(structA& obj) {
	int dl = 0, pl = 0, cnt=0;
	bool flag = false;
	vector <int > dk;
	dk.resize(2);
	for (int i = 1; i <= obj.h; i++) 
		for (int j =1;j<= obj.partExpr;j++){
			cnt = 1;
			if (obj.matr[i][j] > 100) {
				dl = obj.matr[i][j];
				while (dl) {
					pl = dl % 100;
					dl = dl / 100;
					for (int p = 1; p <= obj.h; p++) {
						if (pl == obj.matr[p][0]) {
							dk[cnt] = pl;
							cnt--;
						}
					}
				}
				for (int k = 0; k < dk.size() - 1; k++)
					for (int l = k + 1; l < dk.size(); l++) {
						for (int h = 1; h <= obj.h; h++)
							if (dk[k] == obj.matr[h][0]) {
								for (int s = 1; s <= obj.h; s++)
									if (dk[l] == obj.matr[s][0]){
										// копируем строку до склейки
										for(int d =1; d<=obj.h; d++)
											for (int t=1;t<obj.partExpr;t++)
												if (obj.matr[h][0] == obj.matr[d][t]) {
													obj.matr[obj.h + 1][0] = obj.matr[h][0];
													flag = true;
												}
										obj.state[obj.h + 1] = obj.state[h];
										obj.matr[h][0] = obj.matr[h][0] * 100 + obj.matr[s][0];
								for (int w = 1; w <= obj.partExpr; w++)
									if (obj.matr[h][w] != -1) {
										if (flag == true)
									    obj.matr[obj.h + 1][w] = obj.matr[h][w];
										if (obj.matr[s][w] != -1)
											obj.matr[h][w] = obj.matr[h][w] * 100 + obj.matr[s][w];
									}
									else obj.matr[h][w] = obj.matr[s][w];
									if (obj.state[s] % 2 == 1)
										obj.state[h] ++;
									
							}
								
					}
				}
			}
		}
//	obj.state[1] = 1; obj.state[2] = 2;

	//obj.h++;
	return &obj;
}

structA *to_ODKA(structA& obj) {
	vector<vector<int>>dqv;
	int counter = 2, dl = 0, pl = 0, sw = 0, kl = 0;
	bool flag1 = false, flag2 = false, flagR = false;
	dqv.resize(15);
	for (int i = obj.h; i >= 1; i--) {
		if (obj.state[i] % 2 == 0)
			dqv[0].emplace(dqv[0].begin(), obj.matr[i][0]);
		else
			dqv[1].emplace(dqv[1].begin(), obj.matr[i][0]);
	}
	cout << obj.h << endl;
	for (int i = 0; i < dqv.size(); i++) {
	for (int j = 0; j < dqv[i].size(); j++)
	cout << "_"<< dqv[i][j];
	cout << endl;
	}
	

	for (int i = 0; i < counter; i++)
		for (int j = 0; j < dqv[i].size() - 1; j++)
			for (int k = j + 1; k < dqv[i].size(); k++)
			{
				flag1 = false;
				for (int l = 1; l <= obj.partExpr; l++)
				{
					int p1 = -1;
					int p2 = -1;
					for (int n = 0; n < counter; n++)
						for (int m = 0; m < dqv[n].size(); m++)
						{ 
							for(int ll=1;ll<=obj.h;ll++)
								if (obj.matr[ll][0] == dqv[i][j]) {
									if (obj.matr[ll][l] != -1)
									{
										if (dqv[n][m] == obj.matr[ll][l])
											p1 = n;
									}
								}
							for (int ll = 1; ll <= obj.h; ll++)
								if (obj.matr[ll][0] == dqv[i][j]) {
									if (obj.matr[ll][l] != -1)
									{
										if (dqv[n][m] == obj.matr[ll][l])
											p2 = n;
									}
								}
						}
					for (int ll = 1; ll <= obj.h; ll++)
						if (obj.matr[ll][0] == dqv[i][j]) {
							if (obj.matr[ll][l] == -1)
							{
								for (int pp = 1; pp <= obj.h; pp++)
									if (obj.matr[pp][0] == dqv[i][k]) {
										if (obj.matr[pp][l] == -1)
										{
											flag1 = flag1;
										}
										else
										{
											flag1 = true;
										}
									}
									else
									{
										for (int pp = 1; pp <= obj.h; pp++)
											if (obj.matr[pp][0] == dqv[i][k]) {
												if (obj.matr[pp][l] == -1)
												{
													flag1 = true;
												}
												else
												{
													if (p1 == p2)
														flag1 = flag1;
													else
														flag1 = true;
												}
											}
									}
							}
						}
				}
				if (flag1 == true)
				{
					flagR = true;
					int a = 0;
					string b = "";
					dqv[counter].emplace(dqv[counter].begin(), dqv[i][k]);
					counter++;

					dqv[i].erase(dqv[i].begin() + k);

					for (int loop = k - 1; loop < dqv[i].size(); loop++)
					{
						flag1 = false;
						for (int l = 1; l <= obj.partExpr; l++)
						{
							int p1 = -1;
							int p2 = -1;
							for (int n = 0; n < counter; n++)
								for (int m = 0; m < dqv[n].size(); m++)
								{
									for (int ll = 1; ll <= obj.h; ll++)
										if (obj.matr[ll][0] == dqv[i][loop]) {
											if (obj.matr[ll][l] != -1)
											{
												if (dqv[n][m] == obj.matr[ll][l])
													p1 = n;
											}
										}
									for (int ll = 1; ll <= obj.h; ll++)
										if (obj.matr[ll][0] == dqv[counter - 1][0]) {
											if (obj.matr[ll][l] != -1)
											{

												if (dqv[n][m] == obj.matr[ll][l])
													p2 = n;
											}
										}
								}
							for (int ll = 1; ll <= obj.h; ll++)
								if (obj.matr[ll][0] == dqv[i][loop]) {
									if (obj.matr[ll][l] == -1)
									{
										for (int pp = 1; pp <= obj.h; pp++)
											if (obj.matr[pp][0] == dqv[counter - 1][0]) {
												if (obj.matr[pp][l] == -1)
												{
													flag1 = flag1;
												}
												else
												{
													flag1 = true;
												}
											}

									}
									else
									{
										for (int pp = 1; pp <= obj.h; pp++)
											if (obj.matr[pp][0] == dqv[counter - 1][0]) {
												if (obj.matr[pp][l] == -1)
												{
													flag1 = true;
												}
												else
												{
													if (p1 == p2)
														flag1 = flag1;
													else
														flag1 = true;
												}
											}

									}
								}

						}
						if (flag1 == false)
						{

							dqv[counter - 1].emplace(dqv[counter - 1].begin(),dqv[i][loop]);
							dqv[i].erase(dqv[i].begin() + loop);
						}
					}
					k--;
				}
			}

	for (int i = 0; i < dqv.size(); i++)
		if (dqv[i].size() > 1)
			flagR = true;
		else flagR = false;
		//if (flagR == true)
			//obj = *to_ODKA(obj);
		for (int i = 0; i < dqv.size(); i++)
			for (int j = 0; j < dqv[i].size(); j++)
				if (dqv[i].size()>1)
					if (dqv[i][0] == 4) {
						sw = dqv[i][0];
						dqv[i][0] =dqv[i][1];
						dqv[i][1] = sw;
					}
		for (int i = 3; i <= obj.h; i++) {   ////// i = 2 
			int inc = 0;
			for (int j = 0; j < counter; j++)
				if (obj.matr[i][0] == dqv[j][0]) {
					inc++;
				}
			if (inc == 0) {
				obj.matr[i].erase(obj.matr[i].begin(), obj.matr[i].begin() + 4);

			}
		}


		for (int i = 1; i <= obj.h; i++)
			for (int j = i + 1; j <= obj.h; j++)
				if (obj.matr[i][0] == -1 && obj.matr[j][0] != -1) {
					obj.state[i] = obj.state[j];
					obj.state[j] = -1;
					for (int l = 0; l <= obj.partExpr; l++) {
						obj.matr[i][l] = obj.matr[j][l];
						obj.matr[j][l] = -1;

					}
				}
		for (int i = 1; i <= obj.h; i++)
			if (obj.matr[i][0] == -1) {
				obj.h = i - 1;
				break;
			}
		for (int i = obj.h + 1; i < obj.state.size(); i++)
			obj.state[i] = -1;


	return &obj;
}

string exprWithX(string str) {
	for (int i = 0; i<str.size(); i++) {
		//for (int j =1;j<st.size();j++){
		if (isdigit(str[i]) && isdigit(str[i + 1]))
			str.insert(i + 1, "x");
		else if ((str[i] == '*' || str[i] == '+') && isdigit(str[i + 1]))
			str.insert(i + 1, "x");
		else if (str[i] == ')' && isdigit(str[i + 1]))
			str.insert(i + 1, "x");
		else if (isdigit(str[i]) && str[i + 1] == '(')
			str.insert(i + 1, "x");

	}

	return str;
}


int main()
{
	string str;
	setlocale(LC_ALL, "russian");
	cout << "vvedite regexp" << endl;
	getline(cin, str);
	str = exprWithX(str);
	//cout << str;
	structA *A = NKA(str);

	cout << "Недетерминированный конечный автомат " << endl;
	for (int i = 0; i < A->matr.size(); i++) {
		for (int j = 0; j < A->matr[i].size(); j++)
			cout << setw(5) << (A->matr[i][j] != -1 ? to_string(A->matr[i][j]) : std::string(""));
		    cout << setw(5) <<(A->state[i] != -1 ? to_string(A->state[i]) : std::string(""));
			//cout << A->matr[i][j];
			cout << endl;
	}

	cout << endl;
	cout << " Оптимизированный недетерминированный конечный автомат " << endl;
	ONKA(*A);
	for (int i = 0; i < A->matr.size(); i++) {
		for (int j = 0; j < A->matr[i].size(); j++)
			cout << setw(5) << (A->matr[i][j] != -1 ? to_string(A->matr[i][j]) : std::string(""));
		cout << setw(5) <<(A->state[i] != -1 ? to_string(A->state[i]) : std::string(""));
		//cout << A->matr[i][j];
		cout << endl;
	}
	cout << endl;
	cout << " Детерминированный конечный автомат " << endl;
	DKA(*A);
	for (int i = 0; i < A->matr.size(); i++) {
		for (int j = 0; j < A->matr[i].size(); j++)
		cout << setw(5) << (A->matr[i][j] != -1 ? to_string(A->matr[i][j]) : std::string(""));
		cout << setw(5) << (A->state[i] != -1 ? to_string(A->state[i]) : std::string(""));
		//cout << A->matr[i][j];
		cout << endl;
	}
	/*cout << "Оптимизированный детерминированный конечный автомат" << endl;
	ODKA(*A);
	for (int i = 0; i < A->matr.size(); i++) {
		for (int j = 0; j < A->matr[i].size(); j++)
			cout << setw(5) << (A->matr[i][j] != -1 ? to_string(A->matr[i][j]) : std::string(""));
		cout << setw(5) << (A->state[i] != -1 ? to_string(A->state[i]) : std::string(""));
		//cout << A->matr[i][j];
		cout << endl;
	}*/

	system("pause");
    return 0;
}

