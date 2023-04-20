#include <ostream>

#ifndef NOSTD_STRING
#define NOSTD_STRING

#ifndef USE_STD
namespace nostd {
    class string {
        private:
            int size;
            char *buffer;
            void clear();
        public:
            string();
            string(const char *str);
            ~string();
            string& operator=(const char *s);
            friend std::ostream& operator<<(std::ostream& out, const string& s);
            int length();
            const char* c_str();
            int stoi();
            double stod();
            bool empty();
    };
}

#else //USO STD COME SUPPORTO

#include <string>

namespace nostd 
{
    class string : public std::string
    {
	    public:
	    	string(const char* pippo); 
    };
}

#endif
#endif
