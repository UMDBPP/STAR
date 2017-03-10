Enable ssh on raspberry pi
http://www.instructables.com/id/Use-ssh-to-talk-with-your-Raspberry-Pi/

Configuring to Emulate Ethernet Over USB
Open config.txt and add the following line at the very bottom:
dtoverlay=dwc2
Open cmdline.txt and add the following after rootwait (do not add spaces or alter what is being added):
modules-load=dwc2,g_ether
Another way to enable shh is to create a blank ssh.txt file in the boot directory

Startup Photo_Capture_Loop program on startup
sudo nano /etc/rc.local
- add below line above exit 0
./home/pi/star_github/STAR/Software/Pi_Zero/Photo_Capture_Loop &

Create universal kill function for autostart of Photo_Capture_Loop
From home directory run
vim .bashrc
Add alias ‘killcam’
alias killcam='sudo pkill -f ./Photo_Capture_Loop'

Install cmake
sudo apt-get install cmake

Install raspicam
Follow directions at:
https://www.uco.es/investiga/grupos/ava/node/40
Downloaded from:
https://sourceforge.net/projects/raspicam/files/?

Install openCV - just pulls deb file then unpacks - saves a bunch of time!!!
https://github.com/jabelone/OpenCV-for-Pi
