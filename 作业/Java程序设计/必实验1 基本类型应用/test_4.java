package test_1;
/*(4) ��д���򣺼���1-500֮���ж��ٸ����������������������
 * �ڱ����и��ϳ����ͼ�����н����ͼ����ϸ������˵������15�֣�*/
public class test_4 {
	public static void main(String[] args) 
	{
		for(int n=2;n<=500;n++)
		{
			int sign=0;
			for(int n0=2;n0<=Math.sqrt(n);n0++)
			{
				if(n%n0==0)
				{
					sign=1;
					break;
				}
			}
			if(sign==0)
			{
				System.out.printf("%d ", n);
			}
		}
	}
}
