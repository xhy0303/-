package test_1;
/*(1) ����һ��100��100�Ķ�ά���飬�������е�Ԫ�ؽ��������ֵ��Ҫ��ʹ��Math.random()����0-1֮��ĸ���������
 * ͨ���㷨�ҵ�������������5������Ҫ��Ӵ�С�����
 * ͬʱ���������������ķѵ�ʱ�䣬�������㷨�ĸ��Ӷȣ�������ӶȽϸ߻��5�֣���
 * �ڱ����и��ϳ����ͼ�����н����ͼ����ϸ������˵������20�֣�*/
public class test_1 {
	public static void main(String[] args) 
	{
		double [][]array_f1=new double[100][100];
		for(int n0=0;n0<100;n0++)
		{
			for(int n1=0;n1<100;n1++)
			{
				array_f1[n0][n1]=Math.random();
			}
		}
		double []array_f2= {array_f1[0][0],array_f1[0][0],array_f1[0][0],array_f1[0][0],array_f1[0][0]};
		for(int r=0;r<100;r++)
		{
			for(int c=0;c<100;c++)
			{
				for(int n=0;n<5;n++)
				{
					if(array_f1[r][c]>array_f2[n])
					{
						array_f2[n]=array_f1[r][c];
						break;
					}
				}
			}
		}
		for(int n=0;n<5;n++)
		{
			System.out.printf("%f ",array_f2[n]);
		}
	}
}
