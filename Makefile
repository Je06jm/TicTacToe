PROG := tictactoe

all:
	gcc src/*.c -o $(PROG).exe -O2 -Wall -Wextra -fstack-protector -pipe

debug:
	gcc src/*.c -o $(PROG).exe -g -Wall -Wextra -fstack-protector -pipe

clean:
	@- rm $(PROG).exe

.PHONY: clean