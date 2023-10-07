SOURCES=src/startup.c src/task.c src/listener.c src/executor.c src/worker.c ../hnconvert.c ../sendrecv/recv.c ../sendrecv/send.c
INCLUDE=include
ARGS=-Wall -o server -s

all:
	gcc ${SOURCES} -I${INCLUDE} ${ARGS}