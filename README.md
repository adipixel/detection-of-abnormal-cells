# Detection of Region of Interest
This application detectes and counts the abnormal cells from the given medical image using
- Morphological operations like open (Erode and Dilate)
- Color information from the image

Author: Aditya Mhamunkar
Email: amhamun1@binghamton.edu
--------------------------------------------------------------------
### Steps to execute the program
--------------------------------------------------------------------

1. Download and install OpenCV in C:/
2. Open the project in Visual Studio 2012
3. Goto 'Project' tab and select Properties.
4. Select Configuration: All Configurations
	4.1. Select C/C++ ->  Addtional Include Directories
		Add the 'C:\opencv\build\include'
		
	4.2. Select Linker -> General -> Additional Library Directories
		Add 'C:\opencv\build\x64\vc14\lib'
		
5. Select Configuratio: Debug
	Select Linker -> Input -> Additional Libraries
		Add 'opencv_world320d.lib'
		
6. Select Configuratio: Release
	Select Linker -> Input -> Additional Libraries
		Add 'opencv_world320.lib'
		
7. Select Apply.
8. Select 'Release' version from the tab and Execute the program.


