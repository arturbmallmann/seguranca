#include<iostream>
using namespace std;
 
// Function to compute num (mod a)
int mod(string num, int a)
{
    // Initialize result
    int res = 0;
 
    // One by one process all digits of 'num'
    for (int i = 0; i < num.length(); i++)
         res = (res*10 + (int)num[i] - '0') %a;
 
    return res;
}
 
// Driver program
int main()
{
    string num = "25324491751450021092519644148294241530969316574173600220069660279729467982835116547732502601675991164337570374264619485798835975031047232438353220965025782973186929583077902112807385563136";
    cout << mod(num, 1073);
    return 0;
}
