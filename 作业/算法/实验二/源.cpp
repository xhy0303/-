#include<iostream>
#include<random>
#include<time.h>

#define LENGTH 10000

using namespace std;

struct Point
{
	long x, y;

	void show()
	{
		cout << "(" << x << "," << y << ")";
	}
};


void QuickSort(long low, long high, Point* Key)//��������
{
	long i, j, Pivotkey;
	Point tmp;
	i = low;	j = high;	//��¼˳�����ϡ��½�
	Pivotkey = Key[low].y;       //PrivotKey��¼�����¼
	tmp = Key[low];
	while (low < high)
	{		//��high>low��ʱ��ѭ��
		while ((low < high) && (Key[high].y >= Pivotkey))
		{
			high--;
		}
		if (low < high)
		{
			Key[low] = Key[high];
			low++;
		}
		else
			break;
		while ((low < high) && (Key[low].y <= Pivotkey))
			low++;
		if (low < high)
		{
			Key[high] = Key[low];
			high--;
		}
	}
	Key[low] = tmp;		 // low == high
	if (i < low - 1)  QuickSort(i, low - 1, Key); //���Ӷ���������еݹ��������
	if (high + 1 < j) QuickSort(high + 1, j, Key);
}

double distance(Point a, Point b)//������������
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double min(double a, double b, double c)
{
	if (a <= b && a <= c)
		return a;
	else if (b <= a && b <= c)
		return b;
	else if (c <= a && c <= b)
		return c;
}
double min(double a, double c)
{
	if (a < c)
		return a;
	else
		return c;
}


double doneByForce(Point* data)//������
{
	double min = distance(data[0], data[1]);
	
	for (long i = 0; i < LENGTH; i++)
	{
		for (long n = i + 1; n < LENGTH; n++)
		{
			if (min > distance(data[i], data[n]))
			{
				min = distance(data[i], data[n]);
			}
		}
	}
	
	return min;
}

double doneDivided(Point* data, long low, long high)//���η�
{
	if (high - low == 1)//ֻ��������
	{
		return distance(data[low], data[high]);
	}
	else if (high - low == 2)//ֻ��������
	{
		double tmp1, tmp2, tmp3;
		tmp1 = distance(data[low], data[low + 1]);
		tmp2 = distance(data[high - 1], data[high]);
		tmp3 = distance(data[low], data[high]);
		double Min = min(tmp1, tmp2, tmp3);
		return Min;
	}
	else
	{
		long mid = (low + high) / 2;
		double d_left, d_right;
		d_left = doneDivided(data, low, mid);
		d_right = doneDivided(data, mid + 1, high);
		//�ֱ���������Ӽ�����̾���
		double d = min(d_left, d_right);
		//����֮�е���̾���
		long index_l = 0, index_r = 0;
		Point* left = new Point[high - low+1];
		Point* right = new Point[high - low+1];
		for (long i = mid; abs(data[mid].x - data[i].x) < d && i >= low && index_l < high - low + 1; i--, index_l++)
		{
			left[index_l] = data[i];
		}
		for (long i = mid + 1; abs(data[mid].x - data[i].x) < d && i <= high && index_r < high - low + 1; i++, index_r++)
		{
			right[index_r] = data[i];
		}
		//�ֱ𱣴���������С���δ��еĵ�

		if (index_r != 0)//����Ҳ���δ���û�е㣬�򲻽������������Ĵ���
		{
			
			QuickSort(0, index_l-1, left);
			QuickSort(0, index_r-1, right);
			//���ҷֱ�����
			long index = 0;
			for (long i = 0; i < index_l; i++)
			{
				
				while (right[index].y < left[i].y - d && index < index_r)
					index++;
				//�����ĳһ���µ�һ���㿪ʼ
				for (long n = 0; index + n < index_r && n < 6 && right[index + n].y < left[i].y + d; n++)
					//����6���㡢�����Ҳ�㼯���ޡ�����6�������ڵ�d*2d�ľ����ڶ�����
				{
					if (distance(left[i], right[n + index]) < d)
					{
						d = distance(left[i], right[n + index]);
					}
				}
			}
		}
		
		return d;

	}
}

bool cmpByX(Point a, Point b)
{
	return a.x < b.x;
}


int main()
{
	//freopen("out.txt", "w", stdout);
	uniform_int_distribution<long> u(0, 2*LENGTH);
	default_random_engine e(time(0));
	for (int i = 0; i < 20; i++)
	{
		Point* data = new Point[LENGTH];
		for (long i = 0; i < LENGTH; i++)
		{
			data[i].x = u(e);
			data[i].y = u(e);
		}//�㼯����

		sort(data, data + LENGTH, cmpByX);//Ԥ����

		int start = clock();
		double min1 = doneByForce(data);
		double min2 = doneDivided(data, 0, LENGTH - 1);
		int end = clock();
		cout << end - start << endl;

		
		/*if (min1 - min2 != 0)
			cout << "��������" << min1 << " ���η���" << min2 << endl;*/
	}
}