# watchdog
a practical watchdog solution enabled by raspberry pi
## motivation
not have much confident for my home env security
   - home is located at monitoring blind area at corner of district
   - gate is not closed frequently
   - busy road side & low floor

to keep my meagre assets safe
  - graduation & degree certification
  - my 7-years-old laptop🙂

## requirements analysis
### key concerns in mind
- video monitoring
  - alert and reord proof if somebody break into home
  - whole time video recording is not necessary
- system security
  - privacy safety
  - electrical safety
  - avalibility
- cheap but well functional
  - cheap hardware to save money
  - easy to prepare software to save time and effort
  - good design to save power
- decent performance( both hardware&software) to support future extention
  - speech alarm
  - voice control
  - video projector
 
### requirements reorganized
- capture photo
  - clear enough 
  - both day&night
  - can save to file
- record short video
  - clear enough
  - both day&night
  - can save to file
- object detection
  - detect person
  - detect posture(option)
- whole room monitoring
  - no blind corner
- camera movement
  - default view
  - set to any target view
  - cruise whole room(option)
  - tracking on person(option)
- alert when detected person
  - capture& save video clip
  - send picture to my email
- expected performance
  - capture-video@30fps
  - object-detection@2~5fps; ENOUGH accuracy
  - 100x    ms latency from person in room to picture captured and saved
  - 1000x   ms latency from person in room to picture sent by email
  - 100x    s  latency from person in room to I noice it
  - whole DRAM budget: 1GB(including os, camera capturing, object detection)
  - board not overheated
  - continiously avalibile(option)
  
  ## hardware selection
  |Hardware name|Power                   |CPU Type                  |GPU Type                  | RAM Size| WiFi |Bluetooth| Camera Port| GPIO|Price  |Remarks|
  |----         |----                    | ----                     |----                      | ----    |----  |----     | ----       |----  |----  |----   |
  |Raspberry Pi 3B |micro USB 5V .4~2.5k mA| ARM-A53 @ 1.2GHz 64bit x4|Broadcom VideoCore IV   | 1GB     | Yes  | Yes     |MIPI        |40 Pin| ￥125|second hand ￥10 post fee|
  |Raspberry Pi 3B |micro USB 5V .4~2.5k mA| ARM-A53 @ 1.2GHz 64bit x4|Broadcom VideoCore IV   | 1GB     | Yes  | Yes     |MIPI        |40 Pin| $35 |board only , brand new|
  |Raspberry Pi 3B+|micro USB 5V .5~2.5k mA| ARM-A53 @ 1.4GHz 64bit x4|Broadcom VideoCore IV   | 1GB     | 2.4GHz 5GHz | 4.2 BLE |MIPI |40 Pin| $35 |board only , brand new|
  |Raspberry Pi 4B |usb Type-C 5V 0.6~3k mA| ARM-A72 @ 1.5GHz 64bit x4|Broadcom VideoCore VI   | 1/2 / 4/8 GB    | 2.4GHz 5GHz | 5.0 BLE|MIPI|40 Pin| $35 ~ $75 |board only , brand new|
   
 
  |Name         |Spec.    |Price|Remarks|
  |----         |----     |---- |----   |
  |Power adaptor|5V       |￥8  |second hand, xianyu|   
  |Box          |  pass   |￥0  | included in Pi, xianyu|
  |Fan          |  pass   |￥0  | included in Pi, xianyu|
  |TFcard       | 16GB    |  ￥25.9  |       taobao| 
  |TFcard reader|  USB    |  ￥1.46  |       pinduoduo| 
  |Camera       |  night 5MP for Ras. Pi     |    ￥36+￥7|       taobao|
  |Steer engine |  2 drection|    ￥40| taobao      |
  
  ## software selection
  - os
  - camera control
  - steer engine control
  
  ## object-detection framework selection
  | Name | OS support? | ARM support? |  Model ready?  |Model Size(MB) |Compressed or Quantizated ? | RAM / Frame | FPS | Doc | Tutorial off-the-shelf? |
  |----  |----         |----          |----            |----           |----                     |----       |---- |---- |----                |
  |TFLite| Pi          | arm vx       |yolov3-tiny, ssd| 2MB           | Yes                     | nil       | 11  | Good| No                 |
  
 Refs:
 home monitor https://www.cnblogs.com/chuxiuhong/p/6196268.html
 home monitor https://shumeipai.nxez.com/2019/04/17/raspberry-pi-zero-w-cctv-camera-with-motioneyeos.html
 yolov3 https://zhuanlan.zhihu.com/p/133929606
 huawei bolt https://github.com/huawei-noah/bolt/blob/master/docs/BENCHMARK.md
 tnn https://blog.csdn.net/ljyljyok/article/details/107133300
https://github.com/Tencent/TNN
paddle lite https://github.com/PaddlePaddle/Paddle-Lite
survey https://www.jianshu.com/p/ff1534dffbd1
alibaba MNN https://github.com/alibaba/MNN/blob/master/README_CN.md
darknet https://pjreddie.com/darknet/yolo/
xiaomi https://github.com/XiaoMi/mace-models
Apple https://developer.apple.com/documentation/coreml
Tf lite https://tensorflow.google.cn/lite/models?hl=zh-cn
 ## system design
 - loop, recv command, take action on per frame
 - well modulized
 
