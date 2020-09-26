# Without a monitor and a keyboard, how to boot up rasberry pi?

# hardware requirements
  - Rasberry pi 3B board
  - Powwer supplier
  - TF card
  - TF card reader
  - Cable for ethernet
  - Router used in your home
# software requirements
  - image without desktop 
    - rasberri pi os latest version 64bit ;link https://downloads.raspberrypi.org/raspios_lite_arm64/images/raspios_lite_arm64-2020-08-24/2020-08-20-raspios-buster-arm64-lite.zip
    - default pi:rasberry
    - find 64bit os for 64bit cpu, or you miglt loose half performance
  - win32diskimager, put os image into TF card; link https://shumeipai.nxez.com/download
  - Advanced_IP_Scanner_2.5.3850
  - any thing like xshell, putty, mobaXterm, etc

# steps
## 1. make boot disk
  - require:
    - os image
    - TF card
    - TF card reader
    - win32diskimager
  - use win32 to burn image io TF card
 
## 2. enable SSH
  - require: 
    - TF card
    - TF card reader
  - how
    - in TF card
    - find `boot` partion,
    - create an empty file full named `ssh` 
 
## 3. boot up
  - require:
    - rasberri pi board and power supplier
    - Cable for ethernet
    - Router used in your home
    - the TF card after step 1&2
  - how:
    - insert TF card to pi
    - connect pi to Router Lan port
    - power on
  
## 4. login to pi
  - require:
    - mobaXterm
    - Advanced_IP_Scanner_2
   - how
      - Advanced_IP_Scanner to get local ip for Raserry pi
      - ssh pi@the-ip-you-get 
      - password : reaberripi
## 5. config
  - raspi-config
    - choose 1 change your user name and pwd
    - choose Interfacing Options->VNC to enbale remote Desktop; then you can use VNC viewer to connect it remotely
    - choose Network Options-> set up Wifi, then you can get rid of caple
 
 ## 6. start using it
