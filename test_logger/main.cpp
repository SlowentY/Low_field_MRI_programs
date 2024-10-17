#include <iostream>
#include "simplelogger.hpp"

using namespace std;

int main()
{
    std::ostream out(std::cout.rdbuf());
    SimpleLogger out_log = SimpleLogger(out, "newtest");
    out_log.enableConsoleOutput(true);

    for(int i = 0; i < 300; i++)
    {
        out_log << LogPref::Flag(ERROR) << "Hello!\n";
        out_log << "Hallo!\n";
    }

    return 0;
}
