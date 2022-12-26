package test_1;
import java.util.*;
/*(2) 某省居民电价分三个“阶梯”：
对于用电量50度以内的这部分电量，电价为0.55元/度；
对于用电量在51度至220度之间的这部分电量，电价为0.58元/度；
对于用电量超过220度的这部分电量，电价为0.65元/度。
编写程序，用户从键盘输入用电量
（要求在不终止程序的情况下能循环10次从键盘读入10个数字：
48、52、230、90、80、89、510、60、1、10），程序输出用户应缴纳的电费。
在报告中附上程序截图、运行结果截图和详细的文字说明。（15分）*/
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
