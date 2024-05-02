# -- Macros
CXXFLAGS = -std=c++17 -Wall
OBJ = tools.o Job.o JobTable.o Kid.o Mom.o main.o
TARGET = chores

# -- Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# -- Dependencies

tools.o: tools.cpp tools.hpp
	$(CXX) -c $(CXXFLAGS) tools.cpp

Job.o: Job.cpp Job.hpp tools.hpp
	$(CXX) -c $(CXXFLAGS) Job.cpp
	
JobTable.o: JobTable.cpp JobTable.hpp Job.hpp tools.hpp
	$(CXX) -c $(CXXFLAGS) JobTable.cpp
	
Kid.o: Kid.cpp Kid.hpp Job.hpp JobTable.hpp tools.hpp
	$(CXX) -c $(CXXFLAGS) Kid.cpp
	
Mom.o: Mom.cpp Mom.hpp Job.hpp JobTable.hpp Kid.hpp tools.hpp
	$(CXX) -c $(CXXFLAGS) Mom.cpp
	
main.o: main.cpp Mom.hpp tools.hpp
	$(CXX) -c $(CXXFLAGS) main.cpp
