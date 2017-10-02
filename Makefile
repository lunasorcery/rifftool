all: rifftool

rifftool: main.cpp
	clang main.cpp -o rifftool -Wall -Wextra -lstdc++ -std=c++11 -O3

install: rifftool
	cp rifftool /usr/local/bin/rifftool

uninstall:
	rm -f /usr/local/bin/rifftool

clean:
	rm -f rifftool
