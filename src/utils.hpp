#include <sstream>
#include <istream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <locale>



namespace ulf {
    /* Split a token */
    void split(std::vector<std::string> &tokens, const std::string &text, char sep) {
        int start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
    }



    /* Print error to STD_ERR and exit with code 1 */
    void error(std::string msg) {
        std::cerr << msg << std::endl;
        exit(1);
    }

    /* Print error to STD_ERR and exit with code 1 */
    void warning(std::string msg) {
        std::cout << "WARNING " << msg << std::endl;
    }

    /* Print error to STD_ERR and exit with code 1 */
    void debug(std::string msg) {
        std::cout << "DEBUG " << msg << std::endl;
    }

    /* Print error to STD_ERR and exit with code 1 */
    void info(std::string msg) {
        std::cout << "INFO " << msg << std::endl;
    }

    /* Turn a int into string */
    std::string to_string(int n) {
       std::stringstream ss;
       ss << n;
       return ss.str();
    }

    /* Turn a string into int */
    int to_int(std::string s) {
        int n;
        std::istringstream ( s ) >> n;
        return n;
    }

    int get_time() {
        struct timeval tv;

        gettimeofday(&tv, NULL);

        int ret = tv.tv_usec;
        /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
        ret /= 1000;

        /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
        ret += (tv.tv_sec * 1000);

        return ret;
    }

    int get_microtime() {
        struct timeval tv;

        gettimeofday(&tv, NULL);

        int ret = tv.tv_usec;

        /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
        ret += (tv.tv_sec * 1000000);

        return ret;
    }


    // trim from start
    std::string &ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }

    // trim from end
    std::string &rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    // trim from both ends
    std::string &trim(std::string s) {
            return ltrim(rtrim(s));
    }

    std::string &to_lower_case(std::string &s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    std::string &clean(std::string s) {
        return trim(to_lower_case(s));
    }


}