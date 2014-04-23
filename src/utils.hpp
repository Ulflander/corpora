#include <sstream>
#include <istream>

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
}