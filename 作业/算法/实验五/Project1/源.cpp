#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<random>



using namespace std;



struct Edge
{
	int head, tail;
	int sign;
	Edge* next;
	Edge(int head = 0, int tail = 0, int sign = 0)
	{
		this->head = head;
		this->tail = tail;
		this->sign = sign;
		next = NULL;
	}
};
struct Node
{
	int num;
	int parent;
	int check;
	int height;
	int sign;
	Edge* path2Parent;
	Node(int num = 0, int sign = 0, int parent = -1)
	{
		this->num = num;
		this->check = sign;
		this->parent = parent;
		height = 0;
		path2Parent = NULL;
	}
};
class Graph
{
	int vectorNum, edgeNum;

	int baseCount;
	int loopCount, repeatCount;
	Node* nodes;
	vector<Edge>edges;
	vector<int>* neighbors;
	Edge* bridge_head;
	
	vector<Edge> bridgeEdge2Be;
	vector<Edge> loopEdge2Be;


public:
	void CreatGraph()
	{
		bridge_head = new Edge;
		int count = 0;
		string str;
		while (getline(cin, str))
		{
			if (count == 0)
			{
				vectorNum = stoi(str);
				neighbors = new vector<int>[vectorNum];
				nodes = new Node[vectorNum];
				for (int i = 0; i < vectorNum; i++)
				{
					nodes[i].num = i;
					nodes[i].check = 0;
				}
			}
			else if (count == 1)
			{
				edgeNum = stoi(str);
			}
			else
			{
				int flag = str.find(' ');
				int head = stoi(str.substr(0, flag));
				int tail = stoi(str.substr(flag + 1, str.length()));
				if (head != tail)
				{
					edges.push_back(Edge(head, tail));
					neighbors[head].push_back(tail);
					neighbors[tail].push_back(head);
				}
			}
			count++;
		}
		baseCount = DFS();
	}
	void CreatGraph_()
	{
		bridge_head = new Edge;
		int count = 0;
		loopCount = 0;
		repeatCount = 0;
		string str;
		while (getline(cin, str))
		{
			if (count == 0)
			{
				vectorNum = stoi(str);
				neighbors = new vector<int>[vectorNum];
				nodes = new Node[vectorNum];
				for (int i = 0; i < vectorNum; i++)
				{
					nodes[i].num = i;
					nodes[i].check = 0;
					nodes[i].sign = i;
					//Union tmp = Union(nodes[i]);
					//unionSet.push_back(tmp);
				}
			}
			else if (count == 1)
			{
				edgeNum = stoi(str);
			}
			else
			{
				int flag = str.find(' ');
				int tmp1 = stoi(str.substr(0, flag));
				int tmp2 = stoi(str.substr(flag + 1, str.length()));
				int head = min(tmp1, tmp2);
				int tail = max(tmp1, tmp2);
				if (head == tail)
				{
					loopCount++;
				}
				else
				{
					Edge tmp = Edge(head, tail);
					/*int sign_head = 0;
					int sign_tail = 0;*/
					/*int flag_head = 0;
					int flag_tail = 0;*/
					/*for (int i = 0; i < unionSet.size(); i++)
					{
						int tmp1 = unionSet[i].find(nodes[head]);
						int tmp2 = unionSet[i].find(nodes[tail]);
						if (tmp1 != 0)
						{
							sign_head = tmp1;
							flag_head = i;
						}
						if (tmp2 != 0)
						{
							sign_tail = tmp2;
							flag_tail = i;
						}
						if (sign_head != 0 && sign_tail != 0)
						{
							break;
						}
					}*/
					//if (sign_head == sign_tail)
					if (nodes[head].sign == nodes[tail].sign)
					{
						loopEdge2Be.push_back(tmp);
					}
					else
					{
						//unionSet[nodes[head].sign].add(unionSet[nodes[tail].sign], nodes);
						//unionSet.erase(unionSet.begin() + flag_tail);
						add(nodes[head].sign, nodes[tail].sign);
						bridgeEdge2Be.push_back(tmp);
						neighbors[head].push_back(tail);
						neighbors[tail].push_back(head);
					}
				}
			}
			count++;
		}
		DFS2GetTree();
	}

	void add(int sign1, int sign2)
	{
		for (int i = 0; i < vectorNum; i++)
		{
			if (nodes[i].sign == sign2)
			{
				nodes[i].sign = sign1;
			}
		}
	}

	int DFS()
	{
		int count = 0;
		for (int i = 0; i < vectorNum; i++)
		{
			if (nodes[i].check != 1)
			{
				count++;
				DFS_visit(i);
			}
		}
		Reset();
		return count;
	}
	void DFS_visit(int flag)
	{
		nodes[flag].check = 1;
		for (int i = 0; i < neighbors[flag].size(); i++)
		{
			if (nodes[neighbors[flag][i]].check != 1)
			{
				nodes[neighbors[flag][i]].parent = flag;
				DFS_visit(neighbors[flag][i]);
			}
		}
	}
	void dfs(int flag, int dst, bool& Sign)
	{
		if (Sign == false)
		{
			nodes[flag].check = 1;
			for (int i = 0; i < neighbors[flag].size(); i++)
			{
				if (nodes[neighbors[flag][i]].check != 1 && Sign == false)
				{
					if (neighbors[flag][i] != dst)
						dfs(neighbors[flag][i], dst, Sign);
					else
					{
						Sign = true;
						return;
					}
				}
			}
		}
	}
	void DFS2GetTree()
	{
		for (int i = 0; i < vectorNum; i++)
		{
			if (nodes[i].check != 1)
			{
				int height = 0;
				DFS_visit(i, height);
			}
		}
	}
	void DFS_visit(int flag, int height)
	{
		nodes[flag].height = height;
		nodes[flag].check = 1;
		for (int i = 0; i < neighbors[flag].size(); i++)
		{
			if (nodes[neighbors[flag][i]].check != 1)
			{
				nodes[neighbors[flag][i]].parent = flag;
				for (int n = 0; n < bridgeEdge2Be.size(); n++)
				{
 					if (bridgeEdge2Be[n].head == min(flag, neighbors[flag][i]) && bridgeEdge2Be[n].tail == max(flag, neighbors[flag][i]))
					{
						nodes[neighbors[flag][i]].path2Parent = &(bridgeEdge2Be[n]);
						break;
					}
				}
				height++;
				DFS_visit(neighbors[flag][i], height);
				height--;
			}
		}
	}

	void Reset()
	{
		for (int i = 0; i < vectorNum; i++)
		{
			nodes[i].check = 0;
		}
	}

	void Base()//n^2~n^4
	{
		bridge_head = new Edge;
		Edge* p = bridge_head;
		for (int i = 0; i < edges.size(); i++)
		{
			Edge p0 = edges[i];
			int flag = i;
			edges.erase(edges.begin() + i);
			vector<int>::iterator flag_tail = find(neighbors[p0.head].begin(), neighbors[p0.head].end(), p0.tail);
			neighbors[p0.head].erase(flag_tail);
			vector<int>::iterator flag_head = find(neighbors[p0.tail].begin(), neighbors[p0.tail].end(), p0.head);
			neighbors[p0.tail].erase(flag_head);
			int count = DFS();
			if (count != baseCount)
			{
				Edge* q = new Edge;
				q->head = p0.head;
				q->tail = p0.tail;
				p->next = q;
				p = p->next;
			}
			edges.insert(edges.begin() + flag, p0);
			neighbors[p0.head].push_back(p0.tail);
			neighbors[p0.tail].push_back(p0.head);
		}

	}
	void BaseOptimiz()//
	{
		bridge_head = new Edge;
		Edge* p = bridge_head;
		for (int i = 0; i < edges.size(); i++)
		{

			{
				Edge p0 = edges[i];
				int flag = i;
				edges.erase(edges.begin() + i);
				vector<int>::iterator flag_tail = find(neighbors[p0.head].begin(), neighbors[p0.head].end(), p0.tail);
				neighbors[p0.head].erase(flag_tail);
				vector<int>::iterator flag_head = find(neighbors[p0.tail].begin(), neighbors[p0.tail].end(), p0.head);
				neighbors[p0.tail].erase(flag_head);
				bool Sign = false;
				Reset();
				dfs(p0.head, p0.tail, Sign);
				Reset();
				if (!Sign)
				{
					Edge* q = new Edge;
					q->head = p0.head;
					q->tail = p0.tail;
					p->next = q;
					p = p->next;
				}
				edges.insert(edges.begin() + flag, p0);
				neighbors[p0.head].push_back(p0.tail);
				neighbors[p0.tail].push_back(p0.head);
			}

		}
	}

	void Performance()
	{
		for (int i = 0; i < loopEdge2Be.size(); i++)
		{
			int head = loopEdge2Be[i].head;
			int tail = loopEdge2Be[i].tail;
			loopCount += LCA(head, tail);
			/*neighbors[head].push_back(tail);
			neighbors[tail].push_back(head);*/
		}

		//cout << edgeNum - loopCount - repeatCount << endl;
	}
	int LCA(int left, int right)
	{
		int count = 1;
		int ptr_l = left;
		int ptr_r = right;
		while (nodes[ptr_l].height > nodes[ptr_r].height)
		{
			if (nodes[ptr_l].path2Parent->sign == 0)
			{
				count++;
				nodes[ptr_l].path2Parent->sign = 1;
			}
			ptr_l = nodes[ptr_l].parent;
		}
		while (nodes[ptr_l].height < nodes[ptr_r].height)
		{
			if (nodes[ptr_r].path2Parent->sign == 0)
			{
				count++;
				nodes[ptr_r].path2Parent->sign = 1;
			}
			ptr_r = nodes[ptr_r].parent;
		}
		while (nodes[ptr_l].num != nodes[ptr_r].num)
		{
			if (nodes[ptr_l].height > 0)
			{
				if (nodes[ptr_l].path2Parent->sign == 0);
				{
					count++;
					nodes[ptr_l].path2Parent->sign = 1;
				}
				ptr_l = nodes[ptr_l].parent;
			}
			if (nodes[ptr_r].height > 0)
			{
				if (nodes[ptr_r].path2Parent->sign == 0)
				{
					count++;
					nodes[ptr_r].path2Parent->sign = 1;
				}
				ptr_r = nodes[ptr_r].parent;
			}
		}
		if (ptr_l != left)
		{
			nodes[left].parent = ptr_l;
		}
		if (ptr_r != right)
		{
			nodes[right].parent = ptr_r;
		}

		return count;
	}
	
	void ShowBridges()
	{
		Edge* p = bridge_head;
		while (p->next)
		{
			p = p->next;
			cout << p->head << " " << p->tail << endl;
		}
	}
	void ShowBridges_()
	{
		for (int i = 0; i < bridgeEdge2Be.size(); i++)
		{
			if (bridgeEdge2Be[i].sign != 1)
			{
				cout << bridgeEdge2Be[i].head << " " << bridgeEdge2Be[i].tail << endl;
			}
		}
	}
};

void graphGenerator(int n)
{
	cout << n << endl;
	cout << 3 * n << endl;
	default_random_engine e(time(0));
	uniform_real_distribution<double> u(0, 1);
	int** tmp = new int* [n];
	for (int n0 = 0; n0 < n; n0++)
	{
		tmp[n0] = new int[n];
		for (int i = 0; i < n; i++)
		{
			tmp[n0][i] = 0;

		}
	}
	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			double t = u(e);
			if (u(e) < (double)3 * n / pow(n, 2))
			{
				tmp[r][c] = 1;
				tmp[c][r] = 1;
			}
		}
	}
	for (int r = 0; r < n; r++)
	{
		for (int c = r; c < n; c++)
		{
			if (tmp[r][c])
			{
				cout << r << " " << c << endl;
			}
		}
	}
}

//int main()
//{
//	freopen("tmp.txt", "w", stdout);
//	graphGenerator(250);
//}

int main()
{
	freopen("output.txt", "w", stdout);
	for (int i = 0; i < 20; i++)
	{
		freopen("tmp.txt", "r", stdin);
		//freopen("sample.txt", "r", stdin);
		//freopen("Ð¡Êý¾Ý.txt", "r", stdin);
		//freopen("2.txt", "r", stdin);
		
		
		Graph sample;

		//sample.CreatGraph();
		sample.CreatGraph_();
		double start = clock();

		for (int i = 0; i < 100000; i++)
		{
			//sample.Base();
			//sample.BaseOptimiz();
			sample.Performance();
			
			//sample.ShowBridges();
			//sample.ShowBridges_();
		}

		cout << (clock() - start) / 100000 << endl;
		cin.clear();
	}
	return 0;
}	