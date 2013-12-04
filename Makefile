NAME=jsonparser
SOURCE=jsonParser.cpp json.cpp
CC=clang++

all:
	$(CC) $(SOURCE) -o $(NAME) 

.PHONY: clean
clean:
	rm $(NAME)

