#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
vector <pair<string, int> > stateStack;
map <pair<string, int>, string> propMap;
map <pair<string, int>, vector<string> > arrayMap;
map <pair<string, int>, vector<pair<string, int> > > objectMap;
int counter;

void printIndent(int tabSpace);
void jsonParse(FILE *file);
void jsonPrinter();
string getString(FILE *file);
vector<string> getArray(FILE *file);
vector<pair<string, int> > getObject(FILE *file);

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
    
    counter = 0;
    stateStack.push_back(pair<string, int> ("root", counter));

    jsonParse(file);
    jsonPrinter();
    return 0;
}


void printIndent(int tabSpace) {
    for (int i = 0; i < tabSpace; i++) {
        putchar(32);
    }
}

string getString(FILE *file) {
    string tmp = "";
    char ch;
    while ((ch = fgetc(file)) != '\"') {
        tmp += ch;
    }
    return tmp;
}

vector<string> getArray(FILE *file) {
    char ch;
    vector<string> tmp;
    while ((ch = fgetc(file)) != ']') {
        if (ch == '\"') {
            string element = getString(file);
            tmp.push_back(element);
        }
    }
    return tmp;
}

/*
vector<pair<string, int> > getObject(FILE *file) {
    char ch;
    vector<pair<string, int> > object;
    counter++;
    while ((ch = fgetc(file)) != '}') {
        if (ch == '\"') {
            string key = getString(file);
            while ((ch = fgetc(file)) != ':');
            while ((ch = fgetc(file))) {
                if (ch == '\"') {
                    string value = getString(file);
                    propMap[pair<string, int> (key, counter)] = value;
                    object.push_back(pair<string, int> (value, counter+1));
                    break;
                }
                else if (ch == '[') {
                    vector<string> array = getArray(file);
                    arrayMap[pair<string, int> (key, counter)] = array;
                    break;
                }
                else if (ch == '{') {
                    vector<pair<string, int> > innerState = getObject(file);
                    objectMap[pair<string, int> (key, counter)] = innerState;
                    break;
                }
            }
        }
    }
    counter--;
    return object;
}
*/

void jsonParse(FILE *file) {
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '{') {
            stateStack.push_back(pair<string, int> ("{", counter));
            counter++;
        }
        else if(ch == '}'){
            counter--;
            stateStack.push_back(pair<string, int> ("}", counter));
        }
        else if (ch == '\"') {
            string key = getString(file);
            stateStack.push_back(pair<string, int> (key, counter));
            while ((ch = fgetc(file)) != ':');
            while ((ch = fgetc(file))) {
                if (ch == '\"') {
                    string value = getString(file);
                    propMap[pair<string, int> (key, counter)] = value;
                    break;
                }
                else if (ch == '[') {
                    vector<string> array = getArray(file);
                    arrayMap[pair<string, int> (key, counter)] = array;
                    break;
                }
                /*
                else if (ch == '{') {
                    vector<pair<string, int> > innerState = getObject(file);
                    objectMap[pair<string, int> (key, counter)] = innerState;
                    break;
                }
                */
            }
        }
    }
}

void jsonPrinter() {
    vector<pair<string, int> >::iterator it = stateStack.begin();
    int tabSpace = 0;
    for (;it != stateStack.end(); it++) {
        if (it->first == "{") {
            printIndent(tabSpace);
            printf("{\n");
            tabSpace+=2;
        }
        else if (it->first == "}") {
            tabSpace-=2;
            printIndent(tabSpace);
            printf("}\n");
        }
        else {
            printIndent(tabSpace);
            printf("%s", (it->first).c_str());
            if (propMap.find(pair<string, int> (it->first, tabSpace/2)) != propMap.end()) {
                printf(" : %s\n", propMap[pair<string, int> (it->first, tabSpace/2)].c_str());
            }
            else if (arrayMap.find(pair<string, int> (it->first, tabSpace/2)) != arrayMap.end()) {
                printf(" : [\n");
                vector<string>::iterator arrayIt = arrayMap[pair<string, int> (it->first, tabSpace/2)].begin();
                for(; arrayIt != arrayMap[pair<string, int> (it->first, tabSpace/2)].end(); arrayIt++) {
                    printIndent(tabSpace+2);
                    printf("%s\n", arrayIt->c_str());
                }
                printIndent(tabSpace);
                printf("]\n");
            }
            else {
                printf("\n");
            }
        }
    }
}
