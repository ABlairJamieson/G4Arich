

all: rich_analysis.exe

rich_analysis.exe: rich_analysis.o ringfitchi2.o ellipse.o hough.o ellipsefitter.o eventdisplay.o RecEventReader.o
	g++ -std=c++11 -g -Wall -o rich_analysis.exe -O2 `root-config --cflags --libs`  rich_analysis.o ringfitchi2.o ellipse.o hough.o ellipsefitter.o eventdisplay.o RecEventReader.o

rich_analysis.o: rich_analysis.cpp ../include/PTFTTree.hh ../include/TruthTTree.hh ellipse.h
	g++  -std=c++11 -g -Wall -O2 -I"../include" `root-config --cflags --libs` -c rich_analysis.cpp -o rich_analysis.o 

eventdisplay.o: eventdisplay.cpp
	g++  -std=c++11 -g -Wall -O2 -I"../include" `root-config --cflags --libs` -c eventdisplay.cpp -o eventdisplay.o 

RecEventReader.o: ../src/RecEventReader.cc ../include/RecEventReader.hh
	g++  -std=c++11 -g -Wall -O2 -I"../include" `root-config --cflags --libs` -c ../src/RecEventReader.cc -o RecEventReader.o 

#dbscan.o: dbscan.cpp dbscan.h 
#	g++ -std=c++11 -g -Wall -O2 -c dbscan.cpp -o dbscan.o 

ringfitchi2.o: ringfitchi2.cpp ringfitchi2.h ellipse.h ellipsefitter.h
	g++ -std=c++11 -g -Wall -O2 `root-config --cflags` -c ringfitchi2.cpp -o ringfitchi2.o

ellipse.o: ellipse.cpp ellipse.h xypoint.h
	g++ -std=c++11 -g -Wall -O2 -c ellipse.cpp -o ellipse.o

hough.o: hough.cpp hough.h xypoint.h
	g++ -std=c++11 -g -Wall -O2 `root-config --cflags` -c hough.cpp -o hough.o

ellipsefitter.o: ellipsefitter.cpp ellipse.h hough.h
	g++ -std=c++11 -g -Wall -O2 `root-config --cflags` -c ellipsefitter.cpp -o ellipsefitter.o



clean:
	rm rich_analysis.exe *.o 
