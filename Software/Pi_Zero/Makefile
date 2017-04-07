all:
	# gcc -Wall -o pulse pulse.c -lwiringPi
	#g++ -o Photo_Capture_Loop Photo_Capture_Loop.cpp Pulse_Functions.c -I/usr/local/include/ -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui
	g++ -Wall Photo_Capture_Loop.cpp -o Photo_Capture_Loop Pulse_Functions.c -I/usr/local/include/ -lraspicam -lraspicam_cv -lopencv_core -lopencv_highgui -lopencv_imgcodecs -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util -lwiringPi
