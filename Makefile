CC = gcc
CC1 = g++
CFLAGS = -Wall -Werror
 
BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
 
all: create_dirs $(BIN_DIR)/main $(BIN_DIR)/mainmySiCo $(BIN_DIR)/mainTerm $(BIN_DIR)/mainChars $(BIN_DIR)/mainKey $(BIN_DIR)/mySiAs $(BIN_DIR)/mySiBa
 
$(BIN_DIR)/main : $(OBJ_DIR)/main.o $(OBJ_DIR)/libmySimpleComputer.a $(OBJ_DIR)/libmyTerm.a $(OBJ_DIR)/libmyBigChars.a $(OBJ_DIR)/libgraphics.a $(OBJ_DIR)/libmyReadkey.a $(OBJ_DIR)/libmyCU.a $(OBJ_DIR)/libmyALU.a $(OBJ_DIR)/libmySignal.a
	$(CC) -L$(OBJ_DIR)/ $(OBJ_DIR)/main.o -lmySimpleComputer -lmyTerm -lmyBigChars -lgraphics -lmyReadkey -lmyCU -lmyALU -lmySignal -o $(BIN_DIR)/main
 
$(BIN_DIR)/mainmySiCo : $(OBJ_DIR)/mainmySiCo.o $(OBJ_DIR)/libmySimpleComputer.a
	$(CC) -L$(OBJ_DIR)/ $(OBJ_DIR)/mainmySiCo.o -lmySimpleComputer -o $(BIN_DIR)/mainmySiCo
 
$(BIN_DIR)/mainTerm : $(OBJ_DIR)/mainTerm.o $(OBJ_DIR)/libmyTerm.a
	$(CC) -L$(OBJ_DIR)/ $(OBJ_DIR)/mainTerm.o -lmyTerm -o $(BIN_DIR)/mainTerm
	
$(BIN_DIR)/mainChars : $(OBJ_DIR)/mainChars.o $(OBJ_DIR)/libmySimpleComputer.a $(OBJ_DIR)/libmyTerm.a $(OBJ_DIR)/libmyBigChars.a $(OBJ_DIR)/libgraphics.a
	$(CC) -L$(OBJ_DIR)/ $(OBJ_DIR)/mainChars.o -lmySimpleComputer -lmyTerm -lmyBigChars -lgraphics -o $(BIN_DIR)/mainChars
	
$(BIN_DIR)/mainKey : $(OBJ_DIR)/mainKey.o $(OBJ_DIR)/libmySimpleComputer.a $(OBJ_DIR)/libmyReadkey.a
	$(CC) -L$(OBJ_DIR)/ $(OBJ_DIR)/mainKey.o -lmySimpleComputer -lmyReadkey -o $(BIN_DIR)/mainKey
	
$(BIN_DIR)/mySiAs : $(OBJ_DIR)/mySimpleAssembler.o $(OBJ_DIR)/libmySimpleComputer.a
	$(CC) -L$(OBJ_DIR)/ $(OBJ_DIR)/mySimpleAssembler.o -lmySimpleComputer -o $(BIN_DIR)/mySiAs
	
$(BIN_DIR)/mySiBa : $(OBJ_DIR)/mySimpleBasic.o
	$(CC1) -L$(OBJ_DIR)/ $(OBJ_DIR)/mySimpleBasic.o -o $(BIN_DIR)/mySiBa
	
	
	
$(OBJ_DIR)/libmySimpleComputer.a : $(OBJ_DIR)/mySimpleComputer.o
	ar cr $(OBJ_DIR)/libmySimpleComputer.a $^
	
$(OBJ_DIR)/libmyTerm.a : $(OBJ_DIR)/myTerm.o
	ar cr $(OBJ_DIR)/libmyTerm.a $^
 
$(OBJ_DIR)/libmyBigChars.a : $(OBJ_DIR)/myBigChars.o
	ar cr $(OBJ_DIR)/libmyBigChars.a $^
	
$(OBJ_DIR)/libgraphics.a : $(OBJ_DIR)/graphics.o $(OBJ_DIR)/myBigChars.o
	ar cr $(OBJ_DIR)/libgraphics.a $^
	
$(OBJ_DIR)/libmyReadkey.a : $(OBJ_DIR)/myReadkey.o
	ar cr $(OBJ_DIR)/libmyReadkey.a $^
	
$(OBJ_DIR)/libmyCU.a : $(OBJ_DIR)/myCU.o $(OBJ_DIR)/mySimpleComputer.o $(OBJ_DIR)/myReadkey.o $(OBJ_DIR)/myTerm.o $(OBJ_DIR)/myALU.o $(OBJ_DIR)/myBigChars.o
	ar cr $(OBJ_DIR)/libmyCU.a $^
	
$(OBJ_DIR)/libmyALU.a : $(OBJ_DIR)/myALU.o $(OBJ_DIR)/mySimpleComputer.o
	ar cr $(OBJ_DIR)/libmyALU.a $^
	
$(OBJ_DIR)/libmySignal.a : $(OBJ_DIR)/mySignal.o $(OBJ_DIR)/myCU.o $(OBJ_DIR)/mySimpleComputer.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/myALU.o
	ar cr $(OBJ_DIR)/libmySignal.a $^
	
$(OBJ_DIR)/mySimpleAssembler.o : translators/mySimpleAssembler.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ_DIR)/mySimpleBasic.o : translators/mySimpleBasic.cpp $(SRC_DIR)/*.h
	$(CC1) $(CFLAGS) -c $< -o $@
	
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@
	
create_dirs:
	mkdir -p bin/
	mkdir -p obj/
 
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf *.bin
