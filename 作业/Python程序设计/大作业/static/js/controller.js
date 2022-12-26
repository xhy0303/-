function gettime() {
	$.ajax({
		url: "/time",
		timeout: 10000, //超时时间设置为10秒；
		success: function(data) {
			$("#time").html(data)
		},
		error: function(xhr, type, errorThrown) {

		}
	});
}

function get_c1_data() {
	$.ajax({
		url: "/c1",
		success: function(data) {
			$(".num h1").eq(0).html(data.confirm)
			$(".num h1").eq(1).html(data.suspect)
			$(".num h1").eq(2).html(data.heal)
			$(".num h1").eq(3).html(data.dead)
		},
		error: function(xhr, type, errorThrown) {

		}
	})
}
function get_c2_data() {
	$.ajax({
		url:"/c2",
		success: function(data) {
			optionMap.series[0].data = data.data
			ec_center.setOption(optionMap)
		},
		error: function(xhr, type, errorThrown) {
		
		}
	})
}

function get_l1_data() {
	$.ajax({
		url:"/l1",
		success: function(data) {
			option_left1.xAxis.data = data.day
			option_left1.series[0].data = data.confirm
			option_left1.series[1].data = data.imported_case
			option_left1.series[2].data = data.heal
			option_left1.series[3].data = data.dead
			ec_left1.setOption(option_left1)
		},
		error: function(xhr, type, errorThrown) {

		}
	})
}



function get_l2_data() {
	$.ajax({
		url:"/l2",
		success: function(data) {
			option_left2.xAxis.data = data.day
			option_left2.series[0].data = data.now_confirm
			option_left2.series[1].data = data.now_severe
			option_left2.series[2].data = data.suspect
			ec_left2.setOption(option_left2)
		},
		error: function(xhr, type, errorThrown) {

		}
	})
}


function get_r1_data() {
	$.ajax({
		url:"/r1",
		success: function(data) {
			option_right1.xAxis.data = data.city
			option_right1.series[0].data = data.confirm_add
			ec_right1.setOption(option_right1)
		},
		error: function(xhr, type, errorThrown) {

		}
	})
}


function get_r2_data() {
    $.ajax({
        type: 'get',
        async : false,
		url:"/r2",
		data : {},
		dataType : 'json',
		success : function(result) {
		    for (var i = 0; i < result.date.length; i++) {
		        city.push(res.data[i].city);
		        confirm_add.push(res.date[i].confirm_add);

		        }
            },
		error: function(xhr, type, errorThrown) {

		}
	})
}

gettime()
get_c1_data()
get_c2_data()
get_l1_data()
get_l2_data()
get_r1_data()
get_r2_data()
setInterval(gettime, 1000)
setInterval(get_c1_data, 1000*10)
setInterval(get_c2_data, 1000*10)
setInterval(get_l1_data, 1000*10)
setInterval(get_l1_data, 1000*10)
setInterval(get_r1_data, 1000*10)
setInterval(get_r1_data, 1000*10)
