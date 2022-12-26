package test_1;
/*(5) 编写程序：有一序列（分子为1、2、3、4递增，分母为1、5、25、125倍增）：
 * 1/1，2/5，3/25，4/125，...求出这个数列的前20项之和。
 * 在报告中附上程序截图、运行结果截图和详细的文字说明。（15分）*/
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
