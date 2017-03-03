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
