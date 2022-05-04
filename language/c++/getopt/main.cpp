#include <unistd.h>
#include <iostream>
#include <string.h>

using namespace std;

bool forced_update = false;

void set_mode(const char* OptArg)
{
    cout << "size=" << sizeof("true_value") << std::endl;
    cout << "size=" << sizeof(OptArg) << std::endl;
    if (strncmp("true_value", OptArg, sizeof("true_value"))==0) {
        cout << "true" << std::endl;
        forced_update = true;
    } else
        forced_update = false;
}

void parse_opt(int argc, char** argv)
{
    int c;
    while ((c = getopt(argc, argv, "f:")) != -1){
        switch (c) {
            case 'f':
                cout << "optarg=>" << c << "arg=>" << optarg << std::endl;
                set_mode(optarg);
                break;
            default:
                cout << "error";
        }
    }
}

void run(int argc, char ** argv) {
    cout << "forced_update= " << boolalpha<<forced_update << std::endl;
    parse_opt(argc, argv);
    cout << "forced_update= " << boolalpha<<forced_update << std::endl;
}

int main (int argc, char ** argv) {
    run(argc, argv);
}
