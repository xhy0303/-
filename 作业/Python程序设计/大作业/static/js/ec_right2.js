var ec_right2 = echarts.init(document.getElementById("r2"),("dark"));
var option_right2 = {
    title: {
        text:'每日新增确诊(省）',
        textStyle: {
			color: 'white'
		},
		left: 'left'
    },
    tooltip: {
        trigger: 'item',
        formatter: '{a} <br/>{b} : {c} ({d}%)'
    },
    legend: {
        left:'left',
        data: []
    },


    toolbox: {
        show: true,
        feature: {
            mark: {show: true},
            dataView: {show: true, readOnly: false},
            restore: {show: true},
            saveAsImage: {show: true}
        }
    },
    series: [

        {
            name: '新增详情',
            type: 'pie',
            radius: [50, 140],
            center: ['50%', '50%'],
            roseType: 'area',
            itemStyle: {
                borderRadius: 15
            },
            data: [
                {value: 8, name: '台湾'},
                {value: 8, name: '上海'},
                {value: 8, name: '广东'},
                {value: 8, name: '云南'},
                {value: 3, name: '香港'},
                {value: 2, name: '河南'}
                ]
            }
        ]
    };
ec_right2.setOption(option_right2);
