Page({
  data: {
    bags: null,
  },
  onLoad(){
    wx.hideHomeButton()
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getBagInfo.php',
      success(res){
        console.log(res)
        that.setData({
          bags:res.data
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
  delBag(event){
    wx.request({
      url: 'http://localhost/tp/public/mid/delBag.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        bag_id: event.currentTarget.dataset['bid'],
      },
      success(res){
        console.log(res)
      }
    })
    wx.reLaunch({
      url: '../bags/bags',
    })
  },
  search(event){
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getBagByWord.php',
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
            icon: 'error'
          })
        }
        else{
          that.setData({
            bags: res.data
          })
        }
      }
    })
  },
  postAdd(){
    wx.navigateTo({
      url: '../addBag/addBag',
    })
  },
  showDetail(event){
    wx.navigateTo({
      url: '../bagDetail/bagDetail?bid=' + event.currentTarget.dataset['bid'],
    })
  }
})