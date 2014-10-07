#include <string>

using std::string;

#if defined(__ANDROID__)

void logf(const string& logLevel, const string& tag, const string& message)
{

}

#else
#include <iostream>

using std::cout;
using std::endl;


void logf(const string& logLevel, const string& tag, const string& message)
{
    cout << logLevel << tag << " " << message << endl;
}
#endif

#define LOGE(...) logf("E/", __VA_ARGS__)
#define LOGW(...) logf("W/", __VA_ARGS__)
#define LOGI(...) logf("I/", __VA_ARGS__)
