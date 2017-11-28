ALL: merge

INC_DIR = -I$(BOOST_INC) -I../include
LIB_DIR = -L$(BOOST_LIB)
LIB = -lboost_system -lboost_filesystem

SRC_DIR = 
OBJ = string_operations.o vector_operations.o feature_count.o merge_files.o main.o

merge: $(OBJ)
	@echo "Linking..."
	g++ -O0 -g $^ -o $@ $(LIB_DIR) $(LIB)

%.o: %.cpp
	g++ -std=c++11 -O0 -g -Wall $(INC_DIR) $^ -c

.PHONY: clean

clean:
	rm -f *.o *~ merge
