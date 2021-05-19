# makefile for HomeLibrarySystem.out
# Author: Natan Meirov (NatanMeirov@gmail.com)

TARGET= HomeLibrarySystem.out
CC= g++
CFLAGS= -ansi -pedantic -g3 -std=c++11 -Wall -Wextra # -Werror
OBJS= CollectableItem.o ArtWork.o Book.o Movie.o ScreenUIManager.o HomeLibrarySystemUIHandler.o HomeLibrarySystemManager.o FindItemCriteria.o MatchingSubStringInNameCriteria.o PublishYearInCorrectRangeCriteria.o CollectedYearInCorrectRangeCriteria.o HomeLibrarySystemMain.o
HEADERS_PATH= ./Include/
SOURCES_PATH= ./Source/

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
	make clean_objs

CollectableItem.o: $(SOURCES_PATH)CollectableItem.cpp $(HEADERS_PATH)CollectableItem.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)CollectableItem.cpp

ArtWork.o: $(SOURCES_PATH)ArtWork.cpp $(HEADERS_PATH)ArtWork.hpp $(HEADERS_PATH)CollectableItem.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)ArtWork.cpp

Book.o: $(SOURCES_PATH)Book.cpp $(HEADERS_PATH)Book.hpp $(HEADERS_PATH)ArtWork.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)Book.cpp

Movie.o: $(SOURCES_PATH)Movie.cpp $(HEADERS_PATH)Movie.hpp $(HEADERS_PATH)ArtWork.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)Movie.cpp

ScreenUIManager.o: $(SOURCES_PATH)ScreenUIManager.cpp $(HEADERS_PATH)ScreenUIManager.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)ScreenUIManager.cpp

HomeLibrarySystemUIHandler.o: $(SOURCES_PATH)HomeLibrarySystemUIHandler.cpp $(HEADERS_PATH)HomeLibrarySystemUIHandler.hpp $(HEADERS_PATH)ScreenUIManager.hpp $(HEADERS_PATH)Book.hpp $(HEADERS_PATH)Movie.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)HomeLibrarySystemUIHandler.cpp

MatchingSubStringInNameCriteria.o: $(SOURCES_PATH)MatchingSubStringInNameCriteria.cpp $(HEADERS_PATH)MatchingSubStringInNameCriteria.hpp $(HEADERS_PATH)ArtWork.hpp $(HEADERS_PATH)CollectableItem.hpp $(HEADERS_PATH)FindItemCriteria.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)MatchingSubStringInNameCriteria.cpp

PublishYearInCorrectRangeCriteria.o: $(SOURCES_PATH)PublishYearInCorrectRangeCriteria.cpp $(HEADERS_PATH)PublishYearInCorrectRangeCriteria.hpp $(HEADERS_PATH)ArtWork.hpp $(HEADERS_PATH)CollectableItem.hpp $(HEADERS_PATH)FindItemCriteria.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)PublishYearInCorrectRangeCriteria.cpp

CollectedYearInCorrectRangeCriteria.o: $(SOURCES_PATH)CollectedYearInCorrectRangeCriteria.cpp $(HEADERS_PATH)CollectedYearInCorrectRangeCriteria.hpp $(HEADERS_PATH)CollectableItem.hpp $(HEADERS_PATH)FindItemCriteria.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)CollectedYearInCorrectRangeCriteria.cpp

FindItemCriteria.o: $(SOURCES_PATH)FindItemCriteria.cpp $(HEADERS_PATH)FindItemCriteria.hpp $(HEADERS_PATH)CollectableItem.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)FindItemCriteria.cpp

HomeLibrarySystemManager.o: $(SOURCES_PATH)HomeLibrarySystemManager.cpp $(HEADERS_PATH)HomeLibrarySystemManager.hpp $(HEADERS_PATH)HomeLibrarySystemUIHandler.hpp $(HEADERS_PATH)CollectableItem.hpp $(HEADERS_PATH)Book.hpp $(HEADERS_PATH)Movie.hpp $(HEADERS_PATH)FindItemCriteria.hpp $(HEADERS_PATH)MatchingSubStringInNameCriteria.hpp $(HEADERS_PATH)PublishYearInCorrectRangeCriteria.hpp $(HEADERS_PATH)CollectedYearInCorrectRangeCriteria.hpp $(HEADERS_PATH)TLibraryIterator_Inline.hpp
	$(CC) $(CFLAGS) -c $(SOURCES_PATH)HomeLibrarySystemManager.cpp

HomeLibrarySystemMain.o: HomeLibrarySystemMain.cpp $(HEADERS_PATH)HomeLibrarySystemManager.hpp
	$(CC) $(CFLAGS) -c HomeLibrarySystemMain.cpp

clean:
	rm -f $(TARGET) $(OBJS)

clean_objs:
	rm -f $(OBJS)