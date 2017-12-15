// Utility to generate and apply patches
// patches are generated based on longest common sequence
// Some parts of code is annotated with comments


#include<iostream>
#include<fstream>
#include <sstream>

#include<cstring>
#include<cstdlib>
#include<algorithm>

#include <string>
#include <vector>

#include <iterator>

using namespace std;

// for easy output of a vector
template <class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}


// This function contains the algorithm
// First vector<int> is contains logic for patching
// i==0: lines are the same in both lines
// i< 0 : i lines deleted
// i > 0 : n new lines
// second vector contains strings of lines to add
// complexity: O(mn)
template <class T>
std::pair<std::vector<int>,std::vector<T>> lcs( std::vector<T> X, std::vector<T> Y )
{
   int m = X.size();
   int n = Y.size();
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

   // vector containing patch logic
   std::vector<int> ptch;
   // num of items in lsc
   int index = L[m][n];

   // longest common sequence and new lines vectors
   vector<T> lcs, diff;

   // Start from the right-most-bottom-most corner and
   // one by one store T in lcs[]
   int i = m, j = n,li = i, lj = j;
   while (i > 0 && j > 0)
   {
      // If current character in X[] and Y are same, then
      // current character is part of LCS
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

   // reverse patch as  according to algorithm it starts from the end of file
   std::reverse(ptch.begin(),ptch.end());
    int fi=0;

    //construct diff
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

   return std::make_pair(ptch,diff);
}

// applies patch to a vector storing a file
// returned vector<T> is a representation of resulting file
template <class T>
std::vector<T> patch(std::vector<int> P, std::vector<T> lcs, std::vector<T> F){
  std::vector<T> patched;
  int mf = F.size();
  int j = lcs.size();
  int iF =0, ilcs = 0;
  for(auto const& mod: P) {
    if(mod == 0 ) {
      patched.push_back(iF < F.size()? F[iF]: "");
      iF++;
    } else if( mod < 0 ) {
      iF = iF + (- mod);
    } else {
      int numpop = mod;
      while (numpop > 0) {
        patched.push_back(lcs.size()> ilcs? lcs[ilcs++]: "");
        numpop--;
      }
    }
  }
  return patched;
}


// function to load file into vector
template <class T>
std::vector<T> fileToVec(std::ifstream f1){
  vector<T> F1;
  for (std::string line; std::getline( f1, line ); /**/ )
     F1.push_back( line );
  return F1;
}

//serialses a patch
// patch format: first line - patch logic, other lines - missing lines
std::pair<std::vector<int>,std::vector<std::string>> deserialize(ifstream& f) {

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
}


// MAIN FILE

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

// parses the argumets and binds the logic
int main(int ac, char* av[])
{
    po::options_description desc("Allowed options");
    try {

        desc.add_options()
            // ("help", "produce help message")
            ("file,F", po::value<std::string>()->required(), "F1 in task")
            ("patch,P", po::value< vector<string> >(), "Patch file; output:file2 ")
            ("diff,D", po::value< vector<string> >(), "second file to generate patch; output:patch")
            ("output,O", po::value<std::string>()->required(), "output file")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }

        std::vector<std::string> F1;
        std::vector<std::string> F2;
        ifstream f1(vm["file"].as<string>());
        if (vm.count("diff"))
        {
          vector<string> fileNames = vm["diff"].as< vector<string> >();

           ifstream f2(fileNames[0]);


           for (std::string line; std::getline( f1, line ); /**/ )
              F1.push_back( line );
          for (std::string line; std::getline( f2, line ); /**/ )
              F2.push_back( line);
          auto res = lcs(F1, F2);
          serialize(vm["output"].as<string>(), res);

        } else if (vm.count("patch")) {
          vector<string> fileNames = vm["patch"].as< vector<string> >();
           ifstream pfile(fileNames[0]);

           for (std::string line; std::getline( f1, line ); /**/ )
              F1.push_back( line );
          auto des = deserialize(pfile);
          auto F2 = patch(des.first, des.second,F1);

          writeVec(vm["output"].as<string>(), F2);
        }
    }
    catch(exception& e) {
                  cout << desc << "\n";
                  cout << "Example:./Diff  -F F1 -D F2  --output P2" << "\n";
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
