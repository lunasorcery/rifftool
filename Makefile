all: rifftool

rifftool: main.c
	clang main.c -o rifftool -Wall -Wextra -std=c11 -O3 -s

install: rifftool
	cp rifftool /usr/local/bin/rifftool

uninstall:
	rm -f /usr/local/bin/rifftool

clean:
	rm -f rifftool
