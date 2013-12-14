// template operators made by darkvee
// write the operators once and reuse the generic code in any project.

#include <iostream>
#include <string>
using namespace std;

template<class T>
class Math
	{
	public:
		Math(T type);
		T operator+(T v);
		T operator-(T v);
		T operator*(T v);
		T operator/(T v);
		T Evaluate();
		void Output();

	private:
		T mTotalNumber;
	};

template<class T>
Math<T>::Math(T type)
{
	mTotalNumber = type;
}

template<class T>
void Math<T>::Output()
{
	cout << mTotalNumber << "\n";
}

template<class T>
T Math<T>::Evaluate()
{
	return mTotalNumber;
}

template<class T>
T Math<T>::operator+(T type)
{
	Math<T> obj(type);
	return mTotalNumber + obj.mTotalNumber;
}

template<class T>
T Math<T>::operator-(T type)
{
	Math<T> obj(type);
	return mTotalNumber - obj.mTotalNumber;
}

template<class T>
T Math<T>::operator*(T type)
{
	Math<T> obj(type);
	return mTotalNumber * obj.mTotalNumber;
}

template<class T>
T Math<T>::operator/(T type)
{
	Math<T> obj(type);
	return mTotalNumber / obj.mTotalNumber;
}


int main()
{
	string input = "0";
	
	Math<string> sObj("Jake an Jill went ");
	sObj = sObj + "up the hill.";
	sObj.Output();

	Math<float> fObj(15.6);
	fObj = fObj + 4.5;
	fObj.Output();

	Math<double> dObj(34.6);
	dObj = dObj - fObj.Evaluate();
	dObj.Output();

	Math<string> sObj2(" They saw a snake and");
	sObj2 = sObj2 + "Jake and Jill fell down the hill.";

	sObj = sObj + sObj2.Evaluate();
	sObj.Output();

	dObj = dObj / 100*fObj.Evaluate();
	dObj.Output();

	// hold window opend
	cin >> input;
	getline(cin,input);
	return 0;
}