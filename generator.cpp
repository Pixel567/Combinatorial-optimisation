#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    int max=2000;//maximal cordinate 
    srand(time(NULL));
    int n;
    cout<<"podaj n: ";
    cin>>n;
    ofstream plik("dane.txt");
    plik<<n<<endl;
    for(int i=1;i<=n;i++)
    {
        plik<<i<<" "<<rand()%max<<" "<<rand()%max<<endl;
    }
    return 0;
}