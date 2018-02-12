
all: a1_n8779333

a1_n8779333: libzdk.a *.h *.c
	gcc *.c -lm libzdk.a -lcurses -o a1_n8779333 -Wall -Werror

libzdk/libzdk.a:
	cd libzdk && $(MAKE)

play: a1_n8779333
	./a1_n8779333

clean:
	-rm a1_n8779333
	-rm libzdk.a

zip: ../pong.zip

../pong.zip: clean
	cd .. && rm pong.zip && zip pong.zip pong/ -r
