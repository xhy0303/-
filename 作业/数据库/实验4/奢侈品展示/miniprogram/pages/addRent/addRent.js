Page({
  data: {
    bid: null,
  },
  onLoad(event){
    this.setData({
      bid: event.bid,
    })
  },
  addRent(event){
    console.log(event)
    wx.request({
      url: 'http://localhost/tp/public/mid/addRent.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        customer_id: event.detail.value.customer_id,
        bag_id: this.data.bid,
        date_return: event.detail.value.date_return,
      },
      success(res){
        if(res.data){
          wx.showToast({
            title: '出租成功',
            icon: 'success',
            success(res){
              wx.reLaunch({
                url: '../bags/bags',
              })
            }
          })
        }
        else{
          wx.showToast({
            title: '用户不存在',
            icon: 'error'
          })
        }
      }
    })
  }
})