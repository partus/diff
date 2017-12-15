#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<algorithm>
using namespace std;

#include <string>
#include <vector>

/* Returns length of LCS for X[0..m-1], Y[0..n-1] */

#include <iterator>

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}


template <class T>
std::pair<std::vector<int>,std::vector<T>> lcs( std::vector<T> X, std::vector<T> Y )
{
   int m = X.size();
   int n = Y.size();
   cout << "size" << X.size() << Y.size();
   int L[m+1][n+1];

   /* Following steps build L[m+1][n+1] in bottom up fashion. Note
      that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
   for (int i=0; i<=m; i++)
   {
     for (int j=0; j<=n; j++)
     {
       if (i == 0 || j == 0)
         L[i][j] = 0;
       else if (X[i-1] == Y[j-1])
         L[i][j] = L[i-1][j-1] + 1;
       else
         L[i][j] = max(L[i-1][j], L[i][j-1]);
     }
   }

   // Following code is used to print LCS
   std::vector<int> ptch;
   // Following code is used to print LCS
   int index = L[m][n];

   // Create a character array to store the lcs string
   vector<T> lcs, diff;
   // lcs[index] = '\0'; // Set the terminating character

   // Start from the right-most-bottom-most corner and
   // one by one store characters in lcs[]
   int i = m, j = n,li = i, lj = j;
   while (i > 0 && j > 0)
   {
      // If current character in X[] and Y are same, then
      // current character is part of LCS
      cout << X[i-1] << Y[j-1];
      if (X[i-1] == Y[j-1])
      {
          if(li > i) {
            ptch.push_back(-(li-i));
          }

          if(lj > j){
            ptch.push_back(+(lj-j));
          }

          lcs.push_back(X[i-1]); // Put current character in result
          ptch.push_back(0);
          i--; j--; index--;     // reduce values of i, j and index
          li=i; lj=j;
      }

      // If not same, then find the larger of two and
      // go in the direction of larger value
      else if (L[i-1][j] > L[i][j-1])
         i--;
      else
         j--;
   }
   if(li > 0) ptch.push_back(-li);
   if(lj > 0) ptch.push_back(lj);

   // Print the lcs
   // std::vector<T> a = std::reverse(lcs.begin(),lcs.end())

   std::reverse(ptch.begin(),ptch.end());
    int fi=0;
    for(auto const mod: ptch) {
      if(mod == 0) {
        fi++;
      } else if( mod > 0 ){
        int m = mod;
        while (m > 0 ) {
          diff.push_back(Y[fi]);
          fi++;
          m--;
        }
      }
    }
    // std::reverse(lcs.begin(),lcs.end());
    //
    cout << "print debug" << endl;
    cout << X<<Y << endl;
    cout << lcs << endl;
    cout << ptch << diff << endl;

   return std::make_pair(ptch,diff);
   // cout << "LCS " <<  lcs << '\n';
   // cout << "patch" << ptch << '\n';
}


template <class T>
std::vector<T> patch(std::vector<int> P, std::vector<T> lcs, std::vector<T> F){
  std::vector<T> patched;
  int mf = F.size();
  int j = lcs.size();
  int iF =0, ilcs = 0;
  for(auto const& mod: P) {
    if(mod == 0 ) {
      patched.push_back(F[iF]);
      iF++;
    } else if( mod < 0 ) {
      iF = iF + (- mod);
    } else {
      int numpop = mod;
      while (numpop > 0) {
        patched.push_back(lcs[ilcs++]);
        numpop--;
      }
    }
  }
  return patched;
}


#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;
#include <sstream>


template <class T>
std::vector<T> fileToVec(std::ifstream f1){
  vector<T> F1;
  for (std::string line; std::getline( f1, line ); /**/ )
     F1.push_back( line );
  return F1;
}

std::pair<std::vector<int>,std::vector<std::string>> deserialize(ifstream& f) {
  // std::string str = "1,2,3,4,5,6";
  std::string str;
  std::getline(f,str);
  std::vector<int> vect;

  std::stringstream ss(str);

  int i;

  while (ss >> i)
  {
      vect.push_back(i);

      if (ss.peek() == ' ')
          ss.ignore();
  }
  vector<std::string> F1;
  for (std::string line; std::getline( f, line ); /**/ )
     F1.push_back( line );
  return std::make_pair(vect,F1);
}

void writeVec(std::string filename, auto v){
  ofstream f (filename);
  if (f.is_open())
  {
    for(auto mod:v) {
        f << mod << std::endl;
    };
    f.close();
  }
  else cout << "Unable to open file for output";
}

void serialize(std::string filename, auto p) {
  std::stringstream ss;
  for(auto const mod: p.first) {
    ss << mod << ' ';
  }
  ofstream f (filename);
  if (f.is_open())
  {
    f << ss.str() << std::endl;
    for(auto mod:p.second) {
        f << mod << std::endl;
    };
    f.close();
  }
  else cout << "Unable to open file for output";


   //
   // ss << 100 << ' ' << 200;
   //
   // int foo,bar;
   // ss >> foo >> bar;
   //
   // std::cout << "foo: " << foo << '\n';
   // std::cout << "bar: " << bar << '\n';
   //
   // return 0;

}





int main(int ac, char* av[])
{
    try {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            // ("", po::value<int>(), "set compression level")
            ("file,F", po::value<std::string>()->required(), "F1 in task")
            ("patch,P", po::value< vector<string> >(), "patch F by P")
            ("diff,D", po::value< vector<string> >(), "generate patch F1 -> F2")
            ("output,O", po::value<std::string>()->required(), "output file")
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
        std::vector<std::string> F1;
        std::vector<std::string> F2;
        if (vm.count("diff"))
        {
          vector<string> fileNames = vm["diff"].as< vector<string> >();
           ifstream f1(vm["file"].as<string>());
           ifstream f2(fileNames[0]);


           for (std::string line; std::getline( f1, line ); /**/ )
              F1.push_back( line );
          for (std::string line; std::getline( f2, line ); /**/ )
              F2.push_back( line);
          auto res = lcs(F1, F2);
          serialize(vm["output"].as<string>(), res);

        } else if (vm.count("patch")) {
          vector<string> fileNames = vm["patch"].as< vector<string> >();
           ifstream f1(vm["file"].as<string>());
           ifstream pfile(fileNames[0]);
           // ifstream f2(fileNames[0]);

           for (std::string line; std::getline( f1, line ); /**/ )
              F1.push_back( line );
          auto des = deserialize(pfile);
          auto F2 = patch(des.first, des.second,F1);

          writeVec(vm["output"].as<string>(), F2);
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
