package test_1;

import java.util.Scanner;

/*(3) 编写程序：用户从键盘输入1个正整数，程序逆序打印出该正整数的各位数字
 * （例如：某个正整数为123456，程序输出654321）
 * 要求在不终止程序的情况下能循环10次从键盘读入。
 * 在报告中附上程序截图、运行结果截图和详细的文字说明。（15分）*/
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
