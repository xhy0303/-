package test_1;
/*(4) 编写程序：计算1-500之间有多少个素数，并输出所有素数。
 * 在报告中附上程序截图、运行结果截图和详细的文字说明。（15分）*/
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
