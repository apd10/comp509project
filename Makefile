EXEC=solve
FILE=splitting_method.cpp
OBJ= $(EXEC)
OPT=-O2
all=$(OBJ)
$(EXEC) : $(FILE)
	g++ $(OPT) -o $(EXEC) $(FILE)
run:
	./$(OBJ) file.in
clean:
	/bin/rm -rf $(OBJ) 
