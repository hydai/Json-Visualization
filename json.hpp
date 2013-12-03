#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <string>
#include <map>
#include <vector>
using namespace std;

class JsonParser {
    private:
        vector <pair<string, int> > stateStack;
        map <pair<string, int>, string> propMap;
        map <pair<string, int>, vector<string> > arrayMap;
        map <pair<string, int>, vector<pair<string, int> > > objectMap;
        map <pair<string, int>, double> primativeMap;
        int counter;

    public:
        JsonParser();
        void printIndent(int tabSpace);
        void jsonParse(FILE *file);
        void jsonPrinter();
        void pushToStack(string str);
        void addCounter();
        void minusCounter();
        string getString(FILE *file);
        vector<string> getArray(FILE *file);
        vector<pair<string, int> > getObject(FILE *file);
        double getDouble(FILE *file);
};
