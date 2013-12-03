#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "json.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("No input file\n");
        printf("Usage: jsonParser file\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File not found\n");
        return 1;
    }
    JsonParser *jsonParser = new JsonParser();
    jsonParser->jsonParse(file);
    jsonParser->jsonPrinter();
    delete jsonParser;
    return 0;
}


