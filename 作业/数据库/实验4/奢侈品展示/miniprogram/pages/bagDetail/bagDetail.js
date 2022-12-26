Page({
  data: {
    image: '',
    info: null,
    rentalInfo: null,
  },
  onLoad(event){
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getBagById.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        id: event.bid,
      },
      success(res){
        console.log(res)
        that.setData({
          image: '../../rsc/'+res.data[0].bname+'.png',
          info: res.data[0],  
        })
      }
    })
  },
  postRent(event){
    wx.navigateTo({
      url: '../addRent/addRent?bid=' + event.currentTarget.dataset['bid'],
    })
  },
  postReturn(event){
    const that = this
    wx.request({
      url: 'http://localhost/tp/public/mid/getRidByBid.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        id: event.currentTarget.dataset['bid'],
      },
      success(res){
        wx.request({
          url: 'http://localhost/tp/public/mid/return.php',
          method:'POST',
          header: {
            'content-type': 'application/x-www-form-urlencoded'
          },
          data:{
            id: res.data[0].rental_id,
          },
          success(res){
            console.log(res)
            that.setData({
              rentalInfo: res.data[0]
            })
            let info = encodeURIComponent(JSON.stringify(that.data.info))
            let rentalInfo = encodeURIComponent(JSON.stringify(that.data.rentalInfo))
            wx.reLaunch({
              url: '../rentalDetail/rentalDetail?info='+info+'&rentalInfo='+rentalInfo,
            })
          }
        })
      }
    })
  }
})