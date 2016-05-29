CC=g++
CFLAGS=-g -c -Wall -I$(shell pwd)/inc
LDFLAGS=-lm 

SRCS := ./src/ahrs.cpp ./src/I2C.cpp ./src/AKM8975.cpp ./src/MPU6050.cpp main.cpp
SRCS += 
OBJS := $(SRCS:%.cpp=%.o)

PROG = main
all:$(PROG)

%.o: %.cpp
	$(CC) $(CFLAGS)  $< -o $@
	
$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

.phony:clean
clean:
	rm -r ./src/*.o ./*.o $(PROG)
