NAME=jsonparser
CC=clang++

all:
	$(CC) jsonParser.cpp json.cpp -o $(NAME) 

.PHONY: clean
clean:
	rm $(NAME)

