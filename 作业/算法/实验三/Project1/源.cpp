#include<iostream>
#include<string>
#include<ctime>

#define VECTOE_LEN 450
#define COLOR_NUM 5

using namespace std;

int ansNum;
int** graph;
int* colors;
int* neiNum;
int** choices;
int** neis;


bool check(int flag, int color)
{
	for (int i = 1; i <= VECTOE_LEN; i++)
	{
		if (graph[flag][i] && colors[i] == color)
			return false;
	}
	return true;
}

int MRV()
{
	int min = COLOR_NUM;
	int ptr = 1;
	for (int i = 1; i <= VECTOE_LEN; i++)
	{
		if (choices[i][0] < min && !colors[i])
		{
			min = choices[i][0];
			ptr = i;
		}
		if (min == 0)
			return 0;
	}
	return ptr;
}

int MRV_DH()
{
	int min = COLOR_NUM;
	int ptr = 1;
	int max = neiNum[ptr];
	for (int i = 1; i <= VECTOE_LEN; i++)
	{
		if (choices[i][0] < min && !colors[i])
		{
			min = choices[i][0];
			ptr = i;
			max = neiNum[i];
		}
		else if (choices[i][0] == min && !colors[i] && max < neiNum[i])
		{
			ptr = i;
			max = neiNum[i];
		}
		if (min == 0)
			return 0;
	}
	return ptr;
}

void paint(int flag, int color)
{
	colors[flag] = color;
	for (int i = 1; i <= neiNum[flag]; i++)
	{
		int tmp = neis[flag][i];
		choices[tmp][color]--;
		if (choices[tmp][color] == 0)
		{
			choices[tmp][0]--;
		}
	}
}

void recover(int flag, int color)
{
	colors[flag] = 0;
	for (int i = 1; i <= neiNum[flag]; i++)
	{
		int tmp = neis[flag][i];
		choices[tmp][color]++;
		if (choices[tmp][color] == 1)
		{
			choices[tmp][0]++;
		}
		if (choices[tmp][color] > 1)
		{
			choices[tmp][color] = 1;
		}
	}
}

int checkForward(int flag)
{
	int ptr_1 = 0;
	for (int i = 1; i <= neiNum[flag]; i++)
	{
		int tmp = neis[flag][i];
		if (choices[tmp][0] == 0 && !colors[tmp])
			return -1;
		else if (choices[tmp][0] == 1 && !colors[tmp])
		{
			ptr_1 = tmp;
		}
	}
	return ptr_1;
}




void dfs3(int flag, int color, int count)//带有MRV_DH+向前探查一步优化的回溯
{
	if (ansNum < 1000000000)
	{
		paint(flag, color);
		if (count >= VECTOE_LEN)
		{
			ansNum++;
			cout << ansNum << endl;
			for (int i = 1; i <= VECTOE_LEN; i++)
			{
				cout << colors[i] << " ";
			}
			cout << endl;
		}
		else
		{
			int sign = checkForward(flag);
			if (sign != -1)
			{
				//int ptr = sign;
				int ptr;
				if (sign != 0)
					ptr = sign;
				else
					ptr = MRV_DH();
				if (ptr != 0)
				{
					for (int i = 1; i <= COLOR_NUM; i++)
					{
						if (choices[ptr][i] > 0)
						{
							count++;
							dfs3(ptr, i, count);
							count--;
							recover(ptr, i);
						}
					}
				}
				
			}
		}
	}
}
void dfs2(int flag, int color, int count)//带有MRV_DH优化的回溯
{
	if (ansNum < 1000000000)
	{
		paint(flag, color);
		//cout << count << " ";
		if (count >= VECTOE_LEN)
		{
			//cout << endl;
			ansNum++;
			/*cout << ansNum << endl;
			for (int i = 1; i <= VECTOE_LEN; i++)
			{
				cout << colors[i] << " ";
			}
			cout << endl;*/
		}
		else
		{
			int ptr = MRV_DH();
			if (ptr != 0)
			{
				for (int i = 1; i <= COLOR_NUM; i++)
				{
					if (choices[ptr][i] > 0)
					{
						count++;
						dfs2(ptr, i, count);
						count--;
						recover(ptr, i);
					}
				}
			}
		}
	}
}
void dfs1(int flag, int color, int count)//带有MRV优化的回溯
{
	if (ansNum < 1000000000)
	{
		paint(flag, color);
		//cout << count << " ";
		if (count >= VECTOE_LEN)
		{
			//cout << endl;
			ansNum++;
			/*cout << ansNum << endl;
			for (int i = 1; i <= VECTOE_LEN; i++)
			{
				cout << colors[i] << " ";
			}
			cout << endl;*/
		}
		else
		{
			int ptr = MRV();
			if (ptr != 0)
			{
				for (int i = 1; i <= COLOR_NUM; i++)
				{
					if (choices[ptr][i] > 0)
					{
						count++;
						dfs1(ptr, i, count);
						count--;
						recover(ptr, i);
					}
				}
			}
		}
	}
}
void dfs0(int flag, int color)//无剪枝的朴素回溯
{
	colors[flag] = color;
	if (flag >= VECTOE_LEN)
	{
		ansNum++;
		cout << ansNum << endl;
		for (int i = 1; i <= VECTOE_LEN; i++)
		{
			cout << colors[i] << " ";
		}
		cout << endl;
	}
	else
	{
		
		for (int i = 1; i <= COLOR_NUM; i++)
		{
			if (check(flag + 1, i))
			{
				dfs0(flag + 1, i);
				colors[flag+1] = 0;
			}

		}
		
	}
	
}
int main()
{
	//freopen("data.txt", "r", stdin);
	//freopen("random_map.txt", "r", stdin);
	freopen("le450_5a.col", "r", stdin);
	//freopen("le450_15b.col", "r", stdin);
	//freopen("le450_25a.col", "r", stdin);
	freopen("output.txt", "w", stdout);

	graph = new int* [VECTOE_LEN + 1];
	colors = new int[VECTOE_LEN + 1];
	neiNum = new int[VECTOE_LEN + 1];
	choices = new int* [VECTOE_LEN + 1];
	neis = new int* [VECTOE_LEN + 1];
	for (int r = 1; r <= VECTOE_LEN; r++)
	{
		graph[r] = new int[VECTOE_LEN + 1];
		neiNum[r] = 0;
		colors[r] = 0;
		choices[r] = new int[COLOR_NUM + 1];
		choices[r][0] = COLOR_NUM;
		neis[r] = new int[VECTOE_LEN + 1];
		neis[r][0] = 0;
		for (int c = 1; c <= VECTOE_LEN; c++)
		{
			graph[r][c] = 0;
			neis[r][c] = 0;
		}
		for (int i = 1; i <= COLOR_NUM; i++)
		{
			choices[r][i] = 1;
		}
	}
	string str;
	while (getline(cin, str))
	{
		
		int flag = str.find(' ');
		string tmp = str.substr(0, flag);
		str = str.substr(flag + 1, str.length());
		if (tmp == "e")
		{
			int head, tail;
			flag = str.find(' ');
			head = stoi(str.substr(0, flag));
			tail = stoi(str.substr(flag + 1, str.length()));

			graph[head][tail] = 1;
			graph[tail][head] = 1;

			neiNum[head]++;
			neiNum[tail]++;

			neis[head][0]++;
			neis[head][neis[head][0]] = tail;
			neis[tail][0]++;
			neis[tail][neis[tail][0]] = head;
		}
	}

	
	//for (int t = 0; t < 20; t++)
	{
		ansNum = 0;
		int counter = 0;
		double start = clock();
		//for (int n = 0; n < 100; n++)
		{
			//for (int i = 1; i <= COLOR_NUM; i++)
			{

				counter++;
				dfs3(1, 1, counter);
				counter--;
				recover(1, 1);

			}
		}
		double end = clock();
		cout << (end - start)/*/100*/ << endl;
	}
	return 0;
}