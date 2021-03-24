# makefile for HeartGame.out
# Author: Natan Meirov (NatanMeirov@gmail.com)

TARGET= HeartGame.out
CC= gcc
CFLAGS= -ansi -pedantic -g3 -Wall -Werror
OBJS= HeartGameMain.o Card.o CardsGame.o CardsGamePlayer.o CardsGameRound.o Deck.o Hand.o ScreenUI.o
HEADERS_PATH= ./Modules/Headers/
SOURCES_PATH= ./Modules/Sources/

# TODO: add a new path directory and store the object files there

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

Card.o: $(HEADERS_PATH)Card.h $(SOURCES_PATH)Card.c
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)Card.c

CardsGamePlayer.o: $(SOURCES_PATH)CardsGamePlayer.c $(HEADERS_PATH)CardsGamePlayer.h $(HEADERS_PATH)Card.h $(HEADERS_PATH)Hand.h
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)CardsGamePlayer.c

CardsGameRound.o: $(SOURCES_PATH)CardsGameRound.c $(HEADERS_PATH)CardsGameRound.h $(HEADERS_PATH)CardsGamePlayer.h $(HEADERS_PATH)Card.h $(HEADERS_PATH)Deck.h $(HEADERS_PATH)Hand.h $(HEADERS_PATH)UI.h
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)CardsGameRound.c

Deck.o: $(SOURCES_PATH)Deck.c $(HEADERS_PATH)Deck.h $(HEADERS_PATH)Card.h
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)Deck.c

Hand.o: $(SOURCES_PATH)Hand.c $(HEADERS_PATH)Hand.h $(HEADERS_PATH)Card.h
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)Hand.c

ScreenUI.o: $(SOURCES_PATH)ScreenUI.c $(HEADERS_PATH)UI.h
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)ScreenUI.c

CardsGame.o: $(SOURCES_PATH)CardsGame.c $(HEADERS_PATH)CardsGame.h $(HEADERS_PATH)CardsGamePlayer.h $(HEADERS_PATH)CardsGameRound.h $(HEADERS_PATH)UI.h
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)CardsGame.c

HeartGameMain.o: HeartGameMain.c $(HEADERS_PATH)CardsGame.h
	$(CC) $(CFLAGS) -c HeartGameMain.c


clean:
	rm -f $(TARGET) $(OBJS)
