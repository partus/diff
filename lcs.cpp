/* Dynamic Programming implementation of LCS problem */
#include<iostream>
#include<cstring>
#include<cstdlib>
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
void lcs( std::vector<T> X, std::vector<T> Y, int m, int n )
{

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
   vector<T> lcs;;
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
        cout << i << j << li << lj << '\n';
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
   cout << "LCS of " << X << " and " << Y << " is " << lcs << '\n';
   cout << ptch << '\n';
}

/* Driver program to test above function */
int main()
{
  std::string  x = "AGGTAB";
  std::string y= "GXTXAYB";
  std::vector<char> X(x.begin(), x.end());
  std::vector<char> Y(y.begin(), y.end());
  int m = x.length();
  int n = y.length();
  lcs<char>(X, Y, m, n);
  return 0;
}
