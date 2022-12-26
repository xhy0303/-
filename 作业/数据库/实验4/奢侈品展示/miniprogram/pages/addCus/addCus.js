Page({
  addCus(event){
    console.log(event)
    wx.request({
      url: 'http://localhost/tp/public/mid/addCus.php',
      method:'POST',
      header: {
      'content-type': 'application/x-www-form-urlencoded'
      },
      data:{
        customer_id: event.detail.value.customer_id,
        last_name: event.detail.value.last_name,
        first_name: event.detail.value.first_name,
        addr: event.detail.value.addr,
        telephone: event.detail.value.telephone,
        cell: event.detail.value.cell,
        email: event.detail.value.email,
        credit_card: event.detail.value.credit_card,
      },
      success(res){
        if(res.data){
          wx.showToast({
            title: '添加成功',
            icon: 'success',
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