run: project2.cpp
	g++ project2.cpp MathHelper.cpp -lglut -lGLU -lGL -lGLEW -g
	
clean: 
	rm -f *.out *~ run
	
