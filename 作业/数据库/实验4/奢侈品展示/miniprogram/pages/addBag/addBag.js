Page({
  addBag(event){
    console.log(event)
    wx.request({
      url: 'http://localhost/tp/public/mid/addBag.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        bag_id: event.detail.value.bag_id,
        bname: event.detail.value.bname,
        color: event.detail.value.color,
        maker: event.detail.value.maker,
        price: event.detail.value.price
      },
      success(res){
        if(res.data){
          wx.showToast({
            title: '添加成功',
            icon: 'success'
          })
        }
        else{
          wx.showToast({
            title: '编号重复',
            icon: 'error'
          })
        }
      }
    })
  }
})