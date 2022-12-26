var ec_right1 = echarts.init(document.getElementById("r1"),"dark");

option_right1 = {

    dataZoom: [
        {
            type: 'inside',//inside,鼠标缩放slider滑块
            xAxisIndex: 0
        }
    ],

	title: {
		text: '地区城市确诊TOP5',
		textStyle: {
			color: 'white'
		},
		left: 'left'
	},

	color: ['#3398DB'],
	tooltip: {
		trigger: 'axis',
		axisPointer: {
			type: 'shadow'
		}
	},

	xAxis: {
		type: 'category',
		//                              scale:true,
		data: []
	},
	yAxis: {
		type: 'value',
		//坐标轴刻度设置
		},
	series: [{
		type: 'bar',
		data: [],
		barMaxWidth: "50%"
	}]
};
ec_right1.setOption(option_right1)
