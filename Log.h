#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;


void logf(const string& logLevel, const string& tag, const string& message)
{
    cout << logLevel << tag << " " << message << endl;
}


#define LOGE(...) logf("E/", __VA_ARGS__)
#define LOGW(...) logf("W/", __VA_ARGS__)
#define LOGI(...) logf("I/", __VA_ARGS__)
