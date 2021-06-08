#include <fstream>
#include <sstream>

#include "utils.h"

std::string readFile(std::string name)
{
    std::ifstream fs(name);

    if (fs.bad())
        return "";

    std::stringstream ss;
    ss << fs.rdbuf();
    return ss.str();
}
