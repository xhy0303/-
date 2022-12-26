#include<iostream>
#include<random>
#include<ctime>

#define LEN 78000
using namespace std;
int* pods;
int sumA;
int** states;
int* sign;
int* loc;
int** choice;
int** statesOpt;
int** choiceOpt;

int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

void InitStates()
{
	states = new int* [LEN];
	for (int i = 0; i < LEN; i++)
	{
		states[i] = new int[LEN];
		for (int n = 0; n < LEN; n++)
		{
			states[i][n] = 0;
		}
	}
	for (int i = 0; i < LEN; i++)
	{
		for (int r = 0; r < LEN; r++)
		{
			if (r + i < LEN)
			{
				if (i == 0)//对角线初始化
				{
					states[r][r + i] = pods[r];
					choice[r][r + i] = r;
				}
				else if (i == 1)
				{
					states[r][r + i] = max(pods[r], pods[r + i]);
					if (pods[r] > pods[r + i])
					{
						choice[r][r + i] = r;
					}
					else
					{
						choice[r][r + i] = r + i;
					}
				}
				else
				{
					int ptr_l = choice[r + 1][r + i];
					int ptr_r = choice[r][r + i - 1];
					int sum_l, sum_r;
					if (ptr_l == r + 1)
					{
						sum_l = pods[r] + states[r + 2][r + i];
					}
					else
					{
						sum_l = pods[r] + states[r + 1][r + i - 1];
					}
					if (ptr_r == r + i - 1)
					{
						sum_r = pods[r + i] + states[r][r + i - 2];
					}
					else
					{
						sum_r = pods[r + i] + states[r + 1][r + i - 1];
					}
					states[r][r + i] = max(sum_l, sum_r);
					if (sum_l > sum_r)
						choice[r][r + i] = r;
					else
						choice[r][r + i] = r + i;



					/*int tmp1;
					if (pods[r + 1] > pods[r + i])
					{
						tmp1 = pods[r] + states[r + 2][r + i];
					}
					else
					{
						tmp1 = pods[r] + states[r + 1][r + i - 1];
					}
					int tmp2;
					if (pods[r] > pods[r + i - 1])
					{
						tmp2 = pods[r + i] + states[r + 1][r + i - 1];
					}
					else
					{
						tmp2 = pods[r + i] + states[r][r + i - 2];
					}
					states[r][r + i] = max(tmp1, tmp2);*/
				}
			}
			else
				continue;
		}
	}
}


void optimiz()
{
	for (int i = 0; i < LEN; i++)
	{
		for (int r = 0; r < LEN; r++)
		{
			if (i + r < LEN)
			{

				if (i == 0)
				{
					statesOpt[0][r] = pods[r];
					choiceOpt[0][r] = r;
				}
				else if (i == 1)
				{
					if (pods[r] > pods[r + i])
					{
						statesOpt[1][r] = pods[r];
						choiceOpt[1][r] = r;
					}
					else
					{
						statesOpt[1][r] = pods[r + i];
						choiceOpt[1][r] = r + i;
					}
				}
				else
				{
					int sum_l = pods[r];
					int sum_r = pods[r + i];
					int ptr_l = choiceOpt[1][r + 1];
					int ptr_r = choiceOpt[1][r];
					if (ptr_l == r + 1)
					{
						sum_l += statesOpt[0][r + 2];
					}
					else
					{
						sum_l += statesOpt[0][r + 1];
					}
					if (ptr_r == r + i - 1)
					{
						sum_r += statesOpt[0][r];
					}
					else
					{
						sum_r += statesOpt[0][r + 1];
					}

					if (sum_l > sum_r)
					{
						statesOpt[2][r] = sum_l;
						choiceOpt[2][r] = r;
					}
					else
					{
						statesOpt[2][r] = sum_r;
						choiceOpt[2][r] = r + i;
					}

					
				}
			}
			else
				continue;
		}
		if (i > 1)
		{
			for (int t = 0; t < 2; t++)
			{
				for (int c = 0; c < LEN; c++)
				{
					statesOpt[t][c] = statesOpt[t + 1][c];
					choiceOpt[t][c] = choiceOpt[t + 1][c];
				}
			}
			for (int t = 0; t < LEN; t++)
			{
				statesOpt[2][t] = 0;
				choiceOpt[2][t] = 0;
			}
		}
	}
}

void showPath(int left, int right)
{
	if (left == right)
	{
		cout << left << " ";
	}
	else
	{
		if (right - left == 1)
		{
			if (pods[left] == states[left][right])
				showPath(left, left);
			else
				showPath(right, right);
		}
		else
		{
			int ptr = choice[left][right];
			int tmp;
			if (ptr == left)
			{
				tmp = choice[left + 1][right];
				if (tmp == left + 1)
				{
					showPath(left+2, right);
				}
				else
				{
					showPath(left+1, right - 1);
				}
			}
			else
			{
				tmp = choice[left][right - 1];
				if (tmp == right - 1)
				{
					showPath(left, right-2);
				}
				else
				{
					showPath(left + 1, right-1);
				}
			}
			cout << ptr << " ";
		}
	}
}


int Max = 0;
int tmp = 0;
int left = 0;
int right = LEN - 1;
int Sign = 0;
void processByFroce(int left, int right)
{
	if (left < right && right < LEN)
	{
		if (sign[left] == 1)
		{
			tmp += pods[left];
			sign[left] = 0;
			if (left + 1 <= right)
			{
				if (pods[left + 1] < pods[right])
				{
					sign[right] = -1;
					processByFroce(left + 1, right - 1);
					sign[right] = 1;
				}
				else
				{
					sign[left + 1] = -1;
					processByFroce(left + 2, right);
					sign[left + 1] = 1;
				}
			}
			if (tmp > Max)
			{
				Max = tmp;
				for (int i = 0; i < LEN; i++)
				{
					loc[i] = 0;
					if (sign[i] == 0)
					{
						loc[i] = 1;
					}
				}
			}
			
			tmp -= pods[left];
			sign[left] = 1;
		}
		if (sign[right] == 1)
		{
			tmp += pods[right];
			sign[right] = 0;
			if (left <= right - 1)
			{
				if (pods[left] < pods[right - 1])
				{
					sign[right - 1] = -1;
					processByFroce(left, right - 2);
					sign[right - 1] = 1;
				}
				else
				{
					sign[left] = -1;
					processByFroce(left + 1, right - 1);
					sign[left] = 1;
				}
			}
			if (tmp > Max)
			{
				Max = tmp;
				for (int i = 0; i < LEN; i++)
				{
					loc[i] = 0;
					if (sign[i] == 0)
					{
						loc[i] = 1;
					}
				}
			}
			tmp -= pods[right];
			sign[right] = 1;
		}
	}
	else if (left == right && left < LEN)
	{
		tmp += pods[left];
		sign[left] = 0;
		if (tmp > Max)
		{
			Max = tmp;
			for (int i = 0; i < LEN; i++)
			{
				loc[i] = 0;
				if (sign[i] == 0)
				{
					loc[i] = 1;
				}
			}
		}
		tmp -= pods[left];
		sign[left] = 1;
	}
}

int main()
{
	freopen("output.txt", "w", stdout);
	//for (int t = 0; t < 10000000; t++)
	{
		pods = new int[LEN];
		sign = new int[LEN];
		loc = new int[LEN];
		choice = new int* [LEN];
		default_random_engine e(time(0));
		uniform_int_distribution<unsigned> u(1, LEN * 2);
		for (int i = 0; i < LEN; i++)
		{
			//cin >> pods[i];
			pods[i] = u(e);
			sign[i] = 1;
			loc[i] = 0;
			choice[i] = new int[LEN];
			for (int n = 0; n < LEN; n++)
			{
				choice[i][n] = 0;
			}
		}
		statesOpt = new int* [3];
		choiceOpt = new int* [3];
		for (int t = 0; t < 3; t++)
		{
			statesOpt[t] = new int[LEN];
			choiceOpt[t] = new int[LEN];
			for (int i = 0; i < LEN; i++)
			{
				statesOpt[t][i] = 0;
				choiceOpt[t][i] = 0;
			}
		}
		int start = clock();
		//InitStates();
		optimiz();

		//cout << clock() - start << endl;

		//穷举验证
		/*Max = 0;
		processByFroce(0, LEN - 1);
		if (states[0][LEN - 1] != Max)
		{
			for (int i = 0; i < LEN; i++)
			{
				cout << pods[i] << " ";
			}
			cout << endl;
			cout << "动态规划：" << states[0][LEN - 1] << endl;
			showPath(0, LEN - 1);
			cout << endl;
			cout << "穷举："<<Max << endl;
			for (int i = 0; i < LEN; i++)
			{
				if (loc[i] == 1)
					cout << i << " ";
			}
			cout << endl;
		}*/
	}
}