package test_1;
import java.util.*;
/*(2) ĳʡ�����۷����������ݡ���
�����õ���50�����ڵ��ⲿ�ֵ��������Ϊ0.55Ԫ/�ȣ�
�����õ�����51����220��֮����ⲿ�ֵ��������Ϊ0.58Ԫ/�ȣ�
�����õ�������220�ȵ��ⲿ�ֵ��������Ϊ0.65Ԫ/�ȡ�
��д�����û��Ӽ��������õ���
��Ҫ���ڲ���ֹ������������ѭ��10�δӼ��̶���10�����֣�
48��52��230��90��80��89��510��60��1��10������������û�Ӧ���ɵĵ�ѡ�
�ڱ����и��ϳ����ͼ�����н����ͼ����ϸ������˵������15�֣�*/
public class test_2 {
	public static void main(String[] args) 
	{
		Scanner reader=new Scanner(System.in);
		while(reader.hasNextInt())
		{
			double fee;
			int num=reader.nextInt();
			if(num<=50)
			{
				fee=num*0.55;
			}
			else if(num<=220)
			{
				fee=50*0.55+(num-50)*0.58;
			}
			else
			{
				fee=50*0.55+(221-50)*0.58+(num-220)*0.65;
			}
			System.out.printf("%f\n", fee);
		}
	}
}
