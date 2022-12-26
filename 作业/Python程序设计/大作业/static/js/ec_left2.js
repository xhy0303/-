var ec_left2 = echarts.init(document.getElementById("l2"),"dark");

var option_left2 = {

      	// backgroundColor: '#FFF0F5',

      	title: {
      		text: '全国新增趋势',
      		// subtext: '模拟数据',
      		// x: 'center',
			textStyle: {

			},
			left: 'left'
      	},
		
      	legend: {
      		// orient 设置布局方式，默认水平布局，可选值：'horizontal'（水平） ¦ 'vertical'（垂直）
      		// orient: 'horizontal',
      		// x 设置水平安放位置，默认全图居中，可选值：'center' ¦ 'left' ¦ 'right' ¦ {number}（x坐标，单位px）
      		// x: 'left',
      		// y 设置垂直安放位置，默认全图顶端，可选值：'top' ¦ 'bottom' ¦ 'center' ¦ {number}（y坐标，单位px）
      		// y: 'top',
      		data: ['新增确诊','新增重症','新增疑似'],
			left: 'right'
      	},

      	//  图表距边框的距离,可选值：'百分比'¦ {number}（单位px）
      	grid: {
      		top: 50, // 等价于 y: '16%'
      		left: '4%',
      		right: '6%',
      		bottom: '4%',
      		containLabel: true
      	},

      	// 提示框
      	tooltip: {
      		trigger: 'axis',
			axisPointer: {
				type: 'line',
				lineStyle: {
					color: '#7171C6'
				}
			}
      	},

      	//工具框，可以选择
      	toolbox: {
      		feature: {
      		    dataView: {show: true, readOnly: false},
      			saveAsImage: {} //下载工具
      		}
      	},

      	    dataZoom: [
        {
            type: 'inside',//inside,鼠标缩放slider滑块
            xAxisIndex: 0
        }
    ],

      	xAxis: {
      		type: 'category',
      		 axisLabel: {
      		 	rotate: 30, // 旋转角度
      		 	interval: 6 //设置X轴数据间隔几个显示一个，为0表示都显示
      		 },
      		data: []
      	},

      	yAxis: {
      		// name: '数值',
      		type: 'value',
      		axisLine: {
				show: true
      		},
			axisLabel: {
				show: true,
				color: 'white',
				fontSize: 12,
				formatter: function(value) {
					if (value >= 1000) {
						value = value / 1000 + 'k';
					}
					return value;
				}
			},
			splitLine: {
				show: true,
				lineStyle: {
					color: '#172738',
					width: 1,
					type: 'solid'
				}
			}
      	},

      	series: [{
      			name: '新增确诊',
      			data: [],
      			type: 'line',
      			// 设置小圆点消失
      			// 注意：设置symbol: 'none'以后，拐点不存在了，设置拐点上显示数值无效
      			symbol: 'none',
      			// 设置折线弧度，取值：0-1之间
      			smooth: true,
      			      			//填充颜色
      			areaStyle:{
      			color: new echarts.graphic.LinearGradient(
      			0,
      			0,
      			0,
      			1,
      			[
      			    {
      			        offset:0,
      			        color:"rgba(205, 92, 92, 0.9)"},
      			    {
      			        offset:0.8,
      			        color:"rgba(205, 92, 92, 0.1)"}
      			    ],
      			    false),
      			    //影音
      			shadowColor:"rgba(0, 0, 0, 0.2)"
      			},
      		},

      		{
      			name: '新增重症',
      			data: [],
      			type: 'line',
      			symbol: 'none',
      			// 设置折线上圆点大小
      			smooth: true,

      			areaStyle:{
      			color: new echarts.graphic.LinearGradient(
      			0,
      			0,
      			0,
      			1,
      			[
      			    {
      			        offset:0,
      			        color:"rgba(1, 132, 213, 0.9)"},
      			    {
      			        offset:0.8,
      			        color:"rgba(1, 132, 213, 0.1)"}
      			    ],
      			    false),
      			shadowColor:"rgba(0, 0, 0, 0.2)"
      			},

      		},

      		{
      			name: '新增疑似',
      			data: [],
      			type: 'line',
      			symbol: 'none',
      			// 设置折线上圆点大小
      			smooth: true,
      			areaStyle:{
      			color: new echarts.graphic.LinearGradient(
      			0,
      			0,
      			0,
      			1,
      			[
      			    {
      			        offset:0,
      			        color:"rgba(255,182,193, 0.9)"},
      			    {
      			        offset:0.8,
      			        color:"rgba(255,182,193, 0.1)"}
      			    ],
      			    false),
      			shadowColor:"rgba(0, 0, 0, 0.2)"
      			},


      		}
      	]

      	// color: ['#00EE00', '#FF9F7F', '#FFD700']
      };
	  
ec_left2.setOption(option_left2);
