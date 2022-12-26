//初始化echarts实例
var ec_center = echarts.init(document.getElementById("c2"),"dark");
var mydata = []
var scatterData = [
        {
        value: [117.283042, 31.86119]
        }
     ]
var optionMap = {
		title: {
			text: '',
			subtext: '',
			x: 'left'
		},
		tooltip: {
			trigger: 'item'
		},
		//左侧小导航图标
		visualMap: {
			show: true,
			x: 'left',
			y: 'bottom',
			textStyle: {
				fontSize: 8
			},
			splitList: [{



					start: 1,
					end: 9
				},
				{
					start: 10,
					end: 99
				},
				{
					start: 100,
					end: 999
				},
				{
					start: 1000,
					end: 9999
				},
				{
					start: 10000
				}
			],
			color: ['#8A3310','#C64918', '#E55B25','#F2AD92', '#F9DCD1']
		},

			//配置属性
			series: [{
				name: '累积确诊人数',
				type: 'map',
				mapType: 'china',
				roam: false,
				itemStyle: {
					normal: {
						borderWidth: .5,
						borderColor: '#009fe8',
						areaColor: '#ffefd5'
					},
					emphasis: {
						borderWidth: .5,
						borderColor: '#4b0082',
						areaColor: '#fff'
					}
				},
				label: {
					normal: {
						show: true, //省份名称
						fontSize: 8
					},
					emphasis: {
						show: true,
						fontSize: 8
					}
				},
				data: mydata //数据
			},

			{
			    data: scatterData,//配置散点动画效果
			    type: 'effectScatter',
			    coordinateSystem: 'geo' //指明散点使用的坐标系统 geo
//			    rippleEffect: {
//			        scale: 10
//			    }
			}

			],



		};

		//使用制定的配置项和数据显示图表
		ec_center.setOption(optionMap);
