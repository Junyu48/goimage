# goimage
ENGI 7854 Image Processing and Analysis Project

Problem Statement:

Determining the result of a Go game by manually counting the territory/points can be tedious and error-prone, 
especially after an intense long match because the resulting Go board can be very complicated. 
One could use an automated machine vision system to facilitate this process.

System Description:

The system will take a picture of a Go board after a match as input 
and give the result of the game as following quickly (less than 15s) and with no error:
•	Black Total Points
•	White Total Points
•	Winner of the game
This will eliminate potential error made by human and save people the time spend on counting the result and focus on the post-game analysis.

Software and Hardware Tools:
OpenCV, digital camera

Deliverables:

The system will calculate the result of a Go match using the Area Scoring rule set, 
which counts the points that are completely surrounded by stones plus the number of stones the player have on the board. 
If time permits, we will also implement another rule set: Territory Scoring, 
then the user can choose either of these two in the UI. 
We will also allow the picture to be taken from different angles if we have time. 
In demonstration, we will have a Go board with pieces on it 
and use a digital camera to take a picture of the board from a fixed angle (the top of the board) 
and feed the picture into the system. 
We will change some pieces on the board to produce different outputs. 
We will also count the points by hand to compare it with the result that out system produces.

Pertinent References:

How to Score a Game of Go [Online]. Available: http://www.wikihow.com/Score-a-Game-of-Go

A. Carta and M. Corsolini, “A New Approach to an Old Problem: The Reconstruction of a Go Game through a Series of Photographs,” in Second International Go Game Science Conference, Liberec, 2015. https://arxiv.org/pdf/1508.03269.pdf

T. Musil (2014, July 30). Optical Game Position Recognition in the Board Game of Go [Online]. Available: http://tomasm.cz/imago_files/go_image_recognition.pdf

Existing Software that is similar to what we are doing:

PhotoKifu: it not only recognizes the state of the Go board, but also record the moves

Kifu-Snap: automatic go-board image recognition

