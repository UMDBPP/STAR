Startup Photo_Capture_Loop program on startup
sudo nano /etc/rc.local
- add below line above exit 0
./home/pi/star_github/STAR/Software/Pi_Zero/Photo_Capture_Loop &

Create universal kill function for autostart of Photo_Capture_Loop
From home directory run
vim .bashrc
Add alias ‘killcam’
alias killcam='sudo pkill -f ./Photo_Capture_Loop'