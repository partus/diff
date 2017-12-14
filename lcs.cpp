/* Dynamic Programming implementation of LCS problem */
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
int main()
{
  // std::string  x = "AGGTAB";
  // std::string y= "GXTXAYB";
  // std::vector<char> X(x.begin(), x.end());
  // std::vector<char> Y(y.begin(), y.end());
  // // ifstream file("file.txt");
  // lcs<char>(X, Y);
  // return 0;
  // std::string textFilePath = "F1";
  ifstream f1("F1");
  ifstream f2("F2");
  std::vector<std::string> F1;
  std::vector<std::string> F2;
  for (std::string line; std::getline( f1, line ); /**/ )
     F1.push_back( line );
 for (std::string line; std::getline( f2, line ); /**/ )
    F2.push_back( line );
  cout << "F1 " << F1 << '\n';
  cout << "F2 " << F2 << '\n';
  auto res = lcs(F1, F2);
  // lcs(F1, F2);
  cout << res.first << '\n';
  cout << res.second << '\n';
  cout << "patched " << patch(res.first,res.second,F1) << '\n';
  return 0;
}
