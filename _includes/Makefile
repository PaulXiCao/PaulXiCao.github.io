CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++20

.PHONY: all, clean

all: part2 part3_pthreads part3_thread part3_mutex_locks part4_server part4_client

part2: operating-systems-part2_pipe.c
	$(CC) $(CFLAGS) $< -o $@

part3_pthreads: operating-systems-part3_pthreads.c
	$(CC) $(CFLAGS) $< -o $@

part3_thread: operating-systems-part3_thread.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

part3_mutex_locks: operating-systems-part3_mutex_locks.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

part4_server: operating-systems-part4_server.c
	$(CC) $(CFLAGS) $< -o $@

part4_client: operating-systems-part4_client.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f part2 part3_pthreads part3_thread part3_mutex_locks part4_server part4_client
