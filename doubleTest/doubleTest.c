//-----------------------------
// This program is for nothing but testing double math
// ------------------------------

#include <iostream>

using namespace std;

int main()
{
	double dub1 = 0.0;
	double dub2 = 0.0;
	double dtemp = 0.0;
	
	int in1 = 0;
	int in2 = 0;
	int itemp = 0;

	cout << "\ndouble 1 :";
	cin >> dub1;
	cout << "\ndouble 2 :";
	cin >> dub2;
	cout << "\ninteger 1 :";
	cin >> in1;
	cout << "\ninteger 2 :";
	cin >> in2;
	
	dtemp = dub1/dub2;
	cout << "\n\ndouble 1 / double 2 :" << dtemp << endl;

	itemp = in1/in2;
	cout << "in1 / in2 :" << itemp << endl;

	dtemp = dub1/in2;
	cout << "dtemp = dub1/in2 :" << dtemp << endl;
	itemp = dub1/in2;
	cout << "itemp = dub1/in2 :" << itemp << endl;
	dtemp = dub1/static_cast<double>(in2);
	cout << "dtemp = dub1/static_cast(in2) :" << dtemp << endl;
	dtemp = in1/in2;
	cout << "dtemp = in1/in2 :" << dtemp << endl;
	dtemp = static_cast<double>(in1)/in2;
	cout << "dtemp = static(in1)/in2 :" << dtemp << endl;
	dtemp = static_cast<double>(in1)/static_cast<double>(in2);
	cout << "dtemp = static(in1)/static(in2) :" << dtemp << endl;
	
	return 0;
}
