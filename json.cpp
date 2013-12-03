#include "json.hpp"

JsonParser::JsonParser() {
    counter = 0;
    stateStack.clear();
    propMap.clear();
    arrayMap.clear();
    objectMap.clear();
    primativeMap.clear();
    pushToStack("root");
}

void JsonParser::printIndent(int tabSpace) {
    for (int i = 0; i < tabSpace; i++) {
        putchar(32);
    }
}

string JsonParser::getString(FILE *file) {
    string tmp = "";
    char ch;
    while ((ch = fgetc(file)) != '\"') {
        tmp += ch;
    }
    return tmp;
}

vector<string> JsonParser::getArray(FILE *file) {
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

double JsonParser::getDouble(FILE *file) {
    char ch;
    string str = "";
    while ((ch = fgetc(file))) {
        if (isdigit(ch) || ch == '.') {
            str += ch;
        }
        else {
            ungetc(ch, file);
            break;
        }
    }
    double value = atof(str.c_str());
    return value;
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

void JsonParser::jsonParse(FILE *file) {
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
                else if (isdigit(ch)) {
                    ungetc(ch, file);
                    double value = getDouble(file);
                    primativeMap[pair<string, int> (key, counter)] = value;
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

void JsonParser::jsonPrinter() {
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
            else if (primativeMap.find(pair<string, int> (it->first, tabSpace/2)) != primativeMap.end()) {
                printf(" : %g\n", primativeMap[pair<string, int> (it->first, tabSpace/2)]);
            }
            else {
                printf("\n");
            }
        }
    }
}

void JsonParser::pushToStack(string str) {
    stateStack.push_back(pair<string, int> (str, counter));
}

void JsonParser::addCounter() {
    counter++;
}

void JsonParser::minusCounter() {
    counter--;
}
