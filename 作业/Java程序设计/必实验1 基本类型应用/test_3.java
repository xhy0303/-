package test_1;

import java.util.Scanner;

/*(3) ��д�����û��Ӽ�������1�������������������ӡ�����������ĸ�λ����
 * �����磺ĳ��������Ϊ123456���������654321��
 * Ҫ���ڲ���ֹ������������ѭ��10�δӼ��̶��롣
 * �ڱ����и��ϳ����ͼ�����н����ͼ����ϸ������˵������15�֣�*/
public class test_3 {
	public static void main(String[] args) 
	{
		Scanner reader=new Scanner(System.in);
		while(reader.hasNextInt())
		{
			int n=reader.nextInt();
			int m=0;
			while(n/10>0)
			{
				m+=(n%10);
				m*=10;
				n=n/10;
			}
			m+=n;
			System.out.printf("%d\n",m);
		}
	}
}
