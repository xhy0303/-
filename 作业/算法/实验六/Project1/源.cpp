#include<iostream>
#include<stack>
#include<queue>
#include<vector>
#include<algorithm>
#include"TimerCounter.h"

using namespace std;

#define		paperNum		100		//m
#define		profNum			500		//n
#define		profPerPaper	5		//a a < n
#define		paperPerProf	25		//b 


struct Node
{
	string type;
	vector<int>neighbors;
	int father = -1;
	int flow = 0;
	int level = 0;
	Node(string t)
	{
		type = t;
	}
};

int find_path_FF(int flag, int** edges, Node** nodes, int* sign, int& f_min, int num, stack<int>& path)
{
	path.push(flag);
	if (flag == num - 1)
	{
		return 1;
	}
	else
	{
		for (int i = 0; i < nodes[flag]->neighbors.size(); i++)
		{
			if (sign[nodes[flag]->neighbors[i]] == 0)
			{
				sign[nodes[flag]->neighbors[i]] = 1;
				if (edges[flag][nodes[flag]->neighbors[i]] < f_min)
				{
					f_min = edges[flag][nodes[flag]->neighbors[i]];
				}
				if (find_path_FF(nodes[flag]->neighbors[i], edges, nodes, sign, f_min, num, path))
				{
					return 1;
				}
				path.pop();
				sign[nodes[flag]->neighbors[i]] = 0;
			}
		}
		/*for (int i = 0; i < num; i++)
		{
			if (edges[flag][i] && !sign[i])
			{
				sign[i] = 1;
				if (edges[flag][i] < f_min)
				{
					f_min = edges[flag][i];
				}
				int tmp = find_path_FF(i, edges, nodes, sign, f_min, num, path);
				if (tmp == 1)
				{
					return 1;
				}
				else
				{
					path.pop();
					sign[i] = 0;
				}
			}
		}*/
		return 0;
	}
}
int find_path_EK(int flag, int** edges, Node** nodes, int* sign, int num)
{
	queue<int> seq;
	seq.push(flag);
	sign[flag] = 1;
	int tmp = 0;
	while (!seq.empty())
	{
		flag = seq.front();
		seq.pop();
		for (int i = 0; i < nodes[flag]->neighbors.size(); i++)
		{
			if (edges[flag][nodes[flag]->neighbors[i]])
			{
				if (sign[nodes[flag]->neighbors[i]] == 0)
				{
					sign[nodes[flag]->neighbors[i]] = 1;
					nodes[nodes[flag]->neighbors[i]]->father = flag;
					nodes[nodes[flag]->neighbors[i]]->level = nodes[flag]->level + 1;
					seq.push(nodes[flag]->neighbors[i]);
					if (edges[flag][nodes[flag]->neighbors[i]] < nodes[flag]->flow)
					{
						nodes[nodes[flag]->neighbors[i]]->flow = edges[flag][nodes[flag]->neighbors[i]];
					}
					else
					{
						nodes[nodes[flag]->neighbors[i]]->flow = nodes[flag]->flow;
					}
					if (nodes[flag]->neighbors[i] == num - 1)
					{
						return 1;
					}
				}
				else if (nodes[nodes[flag]->neighbors[i]]->level > nodes[flag]->level + 1)
				{
					nodes[nodes[flag]->neighbors[i]]->father = flag;
					nodes[nodes[flag]->neighbors[i]]->level = nodes[flag]->level + 1;
					if (edges[flag][nodes[flag]->neighbors[i]] < nodes[flag]->flow)
					{
						nodes[nodes[flag]->neighbors[i]]->flow = edges[flag][nodes[flag]->neighbors[i]];
					}
					else
					{
						nodes[nodes[flag]->neighbors[i]]->flow = nodes[flag]->flow;
					}
					if (nodes[flag]->neighbors[i] == num - 1)
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void resetSign(int num, int* sign)
{
	for (int i = 0; i < num; i++)
	{
		sign[i] = 0;
	}
}
void resetFather(int num, Node** nodes)
{
	for (int i = 0; i < num; i++)
	{
		nodes[i]->father = -1;
	}
}
void reset(int num, int* sign, Node** nodes)
{
	for (int i = 0; i < num; i++)
	{
		sign[i] = 0;
		nodes[i]->father = -1;
		nodes[i]->flow = 0;
		nodes[i]->level = 0;
	}
	nodes[0]->flow = paperPerProf;
}
void show(int num, int** edges, Node** nodes)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (nodes[i]->type == "prof" && nodes[j]->type == "paper" && edges[i][j] == 0)
			{
				cout << "(" << nodes[i]->type << " " << i << ", " << nodes[j]->type << " " << j - profNum << ")" << endl;
			}
		}
	}
}

int main()
{
	freopen("output.txt", "w", stdout);
	//for (int t = 0; t < 10; t++)
	{
		int num = paperNum + profNum + 2;
		Node** nodes;
		int** edges;
		int* sign;
		nodes = new Node * [num];
		edges = new int* [num];
		sign = new int[num];
		for (int i = 0; i < num; i++)
		{
			edges[i] = new int[num];
			sign[i] = 0;
			for (int j = 0; j < num; j++)
			{
				edges[i][j] = 0;
			}
			if (i == 0)
			{
				nodes[i] = new Node("src");
				nodes[i]->flow = paperPerProf;
			}
			else if (i == num - 1)
			{
				nodes[i] = new Node("dest");
			}
			else if (i > 0 && i <= profNum)
			{
				nodes[i] = new Node("prof");
			}
			else
			{
				nodes[i] = new Node("paper");
			}
		}
		for (int i = 0; i < num; i++)
		{
			for (int j = i + 1; j < num; j++)
			{
				if (nodes[i]->type == "src" && nodes[j]->type == "prof")
				{
					edges[i][j] = paperPerProf;
					nodes[i]->neighbors.push_back(j);
				}
				else if (nodes[i]->type == "prof" && nodes[j]->type == "paper")
				{
					edges[i][j] = 1;
					nodes[i]->neighbors.push_back(j);
				}
				else if (nodes[i]->type == "paper" && nodes[j]->type == "dest")
				{
					edges[i][j] = profPerPaper;
					nodes[i]->neighbors.push_back(j);
				}
			}
		}
		stack<int> path;

		TimerCounter tc;
		double sum = 0;
		tc.Start();
		
		
		int f_min = paperPerProf;
		while (find_path_FF(0, edges, nodes, sign, f_min, num, path))
		{

			resetSign(num, sign);
			int tail = path.top();
			path.pop();
			while (!path.empty())
			{
				int head = path.top();
				path.pop();
				edges[head][tail] -= f_min;
				if (edges[head][tail] == 0)
				{
					auto loc = remove(nodes[head]->neighbors.begin(), nodes[head]->neighbors.end(), tail);
					nodes[head]->neighbors.erase(loc, nodes[head]->neighbors.end());
				}
				edges[tail][head] += f_min;
				if (edges[tail][head] == 1)
				{
					nodes[tail]->neighbors.push_back(head);
				}
				tail = head;
			}
			f_min = paperPerProf;
		}

		/*int f_min;
		int flow = 0;
		while (find_path_EK(0, edges, nodes, sign, num) && flow < (paperNum * profPerPaper))
		{
			f_min = nodes[num - 1]->flow;
			flow += f_min;
			int tail = num - 1;
			while (nodes[tail]->father != -1)
			{
				int head = nodes[tail]->father;
				edges[head][tail] -= f_min;
				
				edges[tail][head] += f_min;
				tail = head;
			}
			reset(num, sign, nodes);
		}*/


		
		tc.Stop();
		cout << tc.dbTime << endl;
		
		//show(num, edges, nodes);

		for (int i = 0; i < num; i++)
		{
			delete nodes[i];
			delete edges[i];
		}
		delete[] nodes;
		delete[] edges;
		delete[] sign;
	}
	
	return 0;
}