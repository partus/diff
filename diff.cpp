// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* The simplest usage of the library.
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

int main(int ac, char* av[])
{
    try {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            // ("", po::value<int>(), "set compression level")
            ("patch,P", po::value< vector<string> >(), "patch F by P")
            ("diff,D", po::value< vector<string> >(), "generate patch F1 -> F2")
            // ("generate", po::value<std::string>(), "Generate patch F1 -> F2 ")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }

        if (vm.count("patch"))
        {
            cout << "Include paths are: "
                 << vm["patch"].as<vector<string>>() << "\n";
        }
        
        if (vm.count("diff"))
        {
            cout << "Input files are: "
                 << vm["diff"].as< vector<string> >() << "\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
