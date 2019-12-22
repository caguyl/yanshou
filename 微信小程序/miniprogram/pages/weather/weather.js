Page({
  data:{
    today:"                                                               ",
    tomorrow:"                                                            ",
    city:"",
    wea1:" ",
    wea2:" ",
    sentence1:"",
    sentence2:"",
    temperature1:"",
    temperature2:"",
    cloth1:"",
    cloth2:"",
    a:"",
  },

  input:function(e){
    this.setData({
      city:e.detail.value
    })
  },

  returnBack:function(){
    wx.navigateTo({
      url:'../index/index',
    })
  },

  search:function(){
    var that = this;
    wx.request({
      url: 'https://www.tianqiapi.com/api/?version=v1',
      method: 'GET',
      data: {
        'city': that.data.city,
        'appid': 71145664,
        'appsecret': "gwq2caLD",
      }, 
      success: function (res) {
        that.setData({
          sentence1:"今天的天气是",
          sentence2:"明天的天气是",
          today:res.data.data[0].wea,
          tomorrow:res.data.data[1].wea,
          temperature1:res.data.data[0].index[3].level,
          temperature2:res.data.data[1].index[3].level,
          cloth1:res.data.data[0].index[3].desc,
          cloth2:res.data.data[1].index[3].desc,
          a:",",
        })

      },
      fail: function (err) {
        console.log(err);
      },
    })
  }
})