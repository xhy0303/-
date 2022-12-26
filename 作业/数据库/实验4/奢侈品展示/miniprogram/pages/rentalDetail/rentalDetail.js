Page({
  data: {
    info: null,
    rentalInfo: null,
    cusInfo: null,
  },
  onLoad(event){
    wx.hideHomeButton()
    this.setData({
      info: JSON.parse(decodeURIComponent(event.info)),
      rentalInfo: JSON.parse(decodeURIComponent(event.rentalInfo)),
    })
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getCusById.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        id: that.data.rentalInfo.customer_id,
      },
      success(res){
        console.log(res)
        that.setData({
          cusInfo: res.data[0]
        })
      }
    })
    console.log(this.data.info)
    console.log(this.data.rentalInfo)
    console.log(this.data.cusInfo)
  },
  postBags(){
    wx.reLaunch({
      url: '../bags/bags',
    })
  }
})