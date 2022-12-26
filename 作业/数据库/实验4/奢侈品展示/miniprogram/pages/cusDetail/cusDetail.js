Page({
  data: {
    info: null,
    rental: null,
    total_cost: null,
  },
  onLoad(event){
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getCusById.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        id: event.cid,
      },
      success(res){
        console.log(res)
        that.setData({
          info: res.data[0],  
        })
      }
    })
    wx.request({
      url: 'http://localhost/tp/public/mid/getRentalList.php',
      method:'POST',
        header: {
        'content-type': 'application/x-www-form-urlencoded'
        },
        data:{
          id: event.cid,
        },
        success(res){
          let cost = 0
          for(let i = 0; i < res.data.length; i++){
            cost += parseFloat(res.data[i].cost)
          }
          that.setData({
            rental: res.data,
            total_cost: cost
          })
        }
    })
  }
})