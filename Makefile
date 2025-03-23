CC = gcc
CFLAGS = -Wall

TARGETS = calculator addition subtraction multiplication division saver

all: $(TARGETS)
	@./calculator

calculator: calculator.c
	@$(CC) $(CFLAGS) calculator.c -o calculator

addition: addition.c
	@$(CC) $(CFLAGS) addition.c -o addition

subtraction: subtraction.c
	@$(CC) $(CFLAGS) subtraction.c -o subtraction

multiplication: multiplication.c
	@$(CC) $(CFLAGS) multiplication.c -o multiplication

division: division.c
	@$(CC) $(CFLAGS) division.c -o division

saver: saver.c
	@$(CC) $(CFLAGS) saver.c -o saver

clean:
	@rm -f $(TARGETS) results.txt
