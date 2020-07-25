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
  - object-detection@2~5fps; enough accuracy
  - 100x    ms latency from person in room to picture captured and saved
  - 1000x   ms latency from person in room to picture sent by email
  - 100x    s  latency from person in room to I noice it
  - whole DRAM budget: 1GB(including os, camera capturing, object detection)
  - board not overheated
  - continiously avalibile(option)
  
  ## hardware selection
  pi version & price
    ram
    cpu
    wifi
    bluetooth(option)
    ethernet
  power
  
  sd card
  
  fan
  
  box
  
  camera
  
  steer engine
  
  ## sofware selection
  os
  camera
  steer engine driver
  video capture
  
  ## objection-detection framework selection
  arch supported: arm
  os   supported: pi os or ubuntu
  perf: fps(quantization)
  model ready: yolov3-tiny, ssd
  well doced?
  tutotial ready?
 
 ## system design
 pass
