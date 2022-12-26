package test_1;
/*(1) 创建一个100×100的二维数组，对数组中的元素进行随机赋值（要求使用Math.random()生成0-1之间的浮点数）。
 * 通过算法找到该数组中最大的5个数，要求从大到小输出，
 * 同时计算整个程序所耗费的时间，并分析算法的复杂度（如果复杂度较高会扣5分）。
 * 在报告中附上程序截图、运行结果截图和详细的文字说明。（20分）*/
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
