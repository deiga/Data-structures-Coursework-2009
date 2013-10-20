// Demonstrate the vector<bool> specialization.
#include <bitset>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

fstream filestr;
 
int main() {

filestr.open("lol.bin",fstream::in | fstream::out | fstream::binary);
vector<bool> vb(10, true);
vector<bool> bb(10);
vector<bool>::iterator it;
ostream_iterator<bool> out(cout, "");
filestr.write(reinterpret_cast<char*>(&vb), sizeof(vb));
filestr.seekg (0);
filestr.read(reinterpret_cast<char*>(&bb), sizeof(vb));
cout<<"test: ";
copy(bb.begin(), bb.end(), out);
cout << endl;
cout<<"end."<<endl;
for(it = vb.begin(); it != vb.end(); it++)
cout << *it;
cout << endl;
vb.push_back(false);

copy(vb.begin(), vb.end(), out);
cout << endl;
bool ab[] = { true, false, false, true, true,
true, true, false, false, true };
// There's a similar constructor:
vb.assign(ab, ab + sizeof(ab)/sizeof(bool));
copy(vb.begin(), vb.end(), out);
cout << endl;
vb.flip(); // Flip all bits
copy(vb.begin(), vb.end(), out);
cout << endl;
for(size_t i = 0; i < vb.size(); i++)
vb[i] = 0; // (Equivalent to "false")
vb[4] = true;
vb[5] = 1;
vb[7].flip(); // Invert one bit
copy(vb.begin(), vb.end(), out);
cout << endl;
filestr.close();
}