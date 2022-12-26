Page({
  data: {
    customers: null,
  },
  onLoad(){
    wx.hideHomeButton()
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getCusInfo.php',
      success(res){
        console.log(res)
        that.setData({
          customers: res.data
        })
      }
    })
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
  delCus(event){
    wx.request({
      url: 'http://localhost/tp/public/mid/delCus.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        customer_id: event.currentTarget.dataset['cid'],
      }
    })
    wx.reLaunch({
      url: '../cus/cus',
    })
  },
  search(event){
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getCusByWord.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        word: event.detail.value.word,
      },
      success(res){
        if(res.data.length == 0){
          wx.showToast({
            title: '暂无结果',
            icon: 'error',
          })
        }
        else{
          that.setData({
            customers: res.data
          })
        }
      }
    })
  },
  postAdd(){
    wx.navigateTo({
      url: '../addCus/addCus',
    })
  },
  showDetail(event){
    wx.navigateTo({
      url: '../cusDetail/cusDetail?cid=' + event.currentTarget.dataset['cid'],
    })
  }
})