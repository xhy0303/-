package test_1;
/*(5) ��д������һ���У�����Ϊ1��2��3��4��������ĸΪ1��5��25��125��������
 * 1/1��2/5��3/25��4/125��...���������е�ǰ20��֮�͡�
 * �ڱ����и��ϳ����ͼ�����н����ͼ����ϸ������˵������15�֣�*/
public class test_5 {
	public static void main(String[] args) 
	{
		long sum_num=1;
		long sum_den=1;
		
		for(int n=2;n<=20;n++)
		{
			sum_den*=5;
			sum_num=sum_num*sum_den+n;
		}
		System.out.printf("%d/%d", sum_num,sum_den);
	}
}
