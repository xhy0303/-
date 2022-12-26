#include<iostream>
#include<ctime>
#include<cstdlib>
#include<algorithm>
#define LENGTH 1000

using namespace std;

long* Data;
long* Key;

void Merge(long left, long mid, long right)
{
	long i = left, j = mid + 1, k = left;
	while (i <= mid && j <= right) {  //�����ȽϽ���С�Ĳ���
		if (Key[i] <= Key[j])  Data[k++] = Key[i++];
		else  Data[k++] = Key[j++];
	}
	while (i <= mid) { Data[k++] = Key[i++]; }//��midǰʣ��Ĳ���
	while (j <= right) { Data[k++] = Key[j++]; }//��mid��ʣ��Ĳ���
}
void MergeSort()
{
	long left, mid, right, step;
	for (step = 1; step < LENGTH; step *= 2) {//step��ʾÿ�������еĳ���
		for (left = 1; left < LENGTH; left += 2 * step) {
			mid = left + step - 1;   //leftָ���һ�����еĵ�һ��Ԫ��
			if (mid >= LENGTH) break;   //midָ���һ�����е����Ԫ��
			right = left + 2 * step - 1;//rightָ��ڶ������е����Ԫ��
			if (right > LENGTH) right = LENGTH;
			Merge(left, mid, right);
		}
		for (int t = 1; t <= LENGTH; t++) Key[t] = Data[t];//����һ�������Ľ��
	}
}

void QuickSort(long low, long high)
{
	long i, j, Pivotkey;
	i = low;	j = high;	//��¼˳�����ϡ��½�
	Pivotkey = Key[low];       //PrivotKey��¼�����¼
	while (low < high)
	{		//��high>low��ʱ��ѭ��
		while ((low < high) && (Key[high] >= Pivotkey))
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
		while ((low < high) && (Key[low] <= Pivotkey))
			low++;
		if (low < high)
		{
			Key[high] = Key[low];
			high--;
		}
	}
	Key[low] = Pivotkey;		 // low == high
	if (i < low - 1)  QuickSort(i, low - 1); //���Ӷ���������еݹ��������
	if (high + 1 < j) QuickSort(high + 1, j);
}

void Sort1()//ѡ������
{
	for (int i = 1; i <= LENGTH; i++)
	{
		long min = Key[i];
		long flag = i;
		for (int i0 = i + 1; i0 < LENGTH; i0++)
		{
			if (Key[i0] < min)
			{
				min = Key[i0];
				flag = i0;
			}
		}
		long tmp = Key[flag];
		Key[flag] = Key[i];
		Key[i] = tmp;
	}
}

void Sort2(long* sample)//ð������
{
	for (long i0 = 0; i0 < LENGTH - 1; i0++)
	{
		int sign = 0;
		for (long i = 0; i < LENGTH - 1; i++)
		{
			if (sample[i] > sample[i + 1])
			{
				long tmp = sample[i];
				sample[i] = sample[i + 1];
				sample[i + 1] = tmp;
				sign = 1;
			}
		}
		if (sign == 0)
			break;
	}
}

void Sort3()//��������
{
	int i, j, temp;
	for (i = 2; i <= LENGTH; i++) {	     //�ӵ�2��Ԫ�ؿ�ʼ��������
		temp = Key[i];
		for (j = i - 1; j >= 1; j--) {
			if (temp < Key[j])   Key[j + 1] = Key[j];
			else   break;        //�ҵ���Ԫ��λ�ã��˳�ѭ��
		}
		Key[j + 1] = temp;
	}
}



void shift(long k, long m, long* data) {  //���н��k�ĵ������̣�mΪ��
	long i, j;  i = k; j = 2 * i;
	while (j <= m) { //j��ʾ��������
		if (j < m && data[j] > data[j + 1])//�Ƚ�i��������������˭��
			j++;
		if (data[i] <= data[j]) //������Ҫ���������˳������򽻻�
			break;
		else {
			long temp = data[i]; data[i] = data[j]; data[j] = temp;
			i = j; j = 2 * i;
		} //��������
	}
}

int main()
{
	freopen("D:/result.txt", "w", stdout);
	for (int n = 0; n < 20; n++)
	{
		Data = new long[LENGTH+1];
		Key = new long[LENGTH+1];
		for (long i = 1; i <= LENGTH; i++)
		{
			Key[i] = (long)(rand() << 15 | rand());
		}
		int start = clock();
		
		QuickSort(1, LENGTH);

		int end = clock();
		cout << (end - start) << endl;


		//
		/*long* data = new long[LENGTH];
		long* outcome = new long[10];
		for (int i = 0; i < LENGTH; i++)
		{
			data[i] = (long)(rand() << 15 | rand());
			if (i < 10)
			{
				outcome[i] = data[i];
			}
		}

		int start = clock();

		for (long i = 4; i >= 0; i--)
		{
			shift(i, 9, outcome);
		}
		for (long i = 10; i < LENGTH; i++)
		{
			if (data[i] > outcome[0])
			{
				outcome[0] = data[i];
				shift(0, 9, outcome);
			}
		}
		int end = clock();
		cout << end - start << endl;*/

	}
}