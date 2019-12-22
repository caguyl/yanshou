Page({
  data:{
    t1:"未检测到衣物",
  },

  b0:function(){
    wx.request({
      url: '',
      method:'GET',
    })

  },

  b1:function(){
    wx.request({
      url: 'http://api.heclouds.com/devices/578080224/datapoints',
      method:'POST',
      header:{
        "api-key": "j3YuAfvJclkAwAi4fWWJ0ZYACDM=",
      },
      data:{
          "datastreams":[{
            "id":"Movement",
            "datapoints":[{
              "value":"d"
            }]
          }]
        },
    })

  },

  b2:function(){
    wx.request({
      url: 'http://api.heclouds.com/devices/578080224/datapoints',
      method: 'POST',
      header: {
        "api-key": "j3YuAfvJclkAwAi4fWWJ0ZYACDM=",
      },
      data: {
        "datastreams": [{
          "id": "Movement",
          "datapoints": [{
            "value": "u"
          }]
        }]
      },
    })

  },

  b3:function(){
    wx.request({
      url: 'http://api.heclouds.com/devices/578080224/datapoints',
      method: 'POST',
      header: {
        "api-key": "j3YuAfvJclkAwAi4fWWJ0ZYACDM=",
      },
      data: {
        "datastreams": [{
          "id": "Movement",
          "datapoints": [{
            "value": "s"
          }]
        }]
      },
    })

  },

  b4:function(){
    wx.navigateTo({
      url: '../weather/weather',
    })

  },
})