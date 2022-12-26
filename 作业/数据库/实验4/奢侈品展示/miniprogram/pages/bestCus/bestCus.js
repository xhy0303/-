Page({
  data: {
    customers: null,
  },
  onLoad(){
    wx.hideHomeButton()
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getBestCus.php',
      success(res){
        console.log(res)
        that.setData({
          customers: res.data
        })
      }
    })
  },
})