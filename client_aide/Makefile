DEBUG=yes
EXEC=solver
ifeq (${DEBUG},yes)
	#debug
	CXXFLAGS=-O0 -g --std=c++11 -D_LOG_ALL
	LDFLAGS=
else
	#release
	CXXFLAGS=-O2 -std=c++11 -D_LOG_ALL
	LDFLAGS=
endif

SRC= $(wildcard *.cpp)
SRCC= $(wildcard ../clientlib/*.c)
OBJ= $(SRC:.cpp=.o)
OBJC= $(SRCC:.c=.o)

all: $(EXEC)
ifeq ($(DEBUG),no)
	rm -rf *.o
endif

$(EXEC): $(OBJC) $(OBJ)
	echo $^
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	echo $^
	$(CXX) -o $@ -c $< $(CXXFLAGS)

%.cpp: %.hpp
	echo $^

%.o: %.c %.h
	echo $^
	$(CXX) -o $@ -c $< $(CXXFLAGS)

%.c: %.h
	echo $^

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

