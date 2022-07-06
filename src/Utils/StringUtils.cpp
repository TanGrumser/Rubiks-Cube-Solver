#include <string>
#include <vector>

using namespace std;

namespace StringUtils {

    vector<string> Split(string& input, const string& delimiter) {
        vector<string> subStrings = {};
        size_t pos = 0;
        std::string token;

        while ((pos = input.find(delimiter)) != string::npos) {
            token = input.substr(0, pos);
            subStrings.push_back(token);
            input.erase(0, pos + delimiter.length());
        }
        
        subStrings.push_back(input);

        return subStrings;
    }
}