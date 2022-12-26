Page({
  data:{
    price: null,
  },
  onLoad(){
    wx.hideHomeButton()
  },
  postBags(){
    wx.reLaunch({
      url: "../bags/bags"
    })
  },
  postCus(){
    wx.reLaunch({
      url: "../cus/cus"
    })
  },
  postIndex(){
    wx.reLaunch({
      url: "../index/index"
    })
  },
  postBestCus(){
    wx.navigateTo({
      url: '../bestCus/bestCus',
    })
  }
})