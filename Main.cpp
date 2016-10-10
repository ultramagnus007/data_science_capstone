#include "TST.h"
#include "Process.h"
#include "Model.h"



int main()
{
	initializeTST("../serial.txt");
	cout<<"LOADED\n";
	string str;
	while (getline (cin, str))
	{	
		vector<string> pred = find(str);
		for(int i = 0; i < pred.size(); i++)
			cout<<pred[i]<<" ";
		cout<<endl;
	}
	return 0;
}


#if 0
int main()
{
	TST tst;
	tst.deserialize("../serial.txt");
	cout<<"FINISHED LOADING"<<endl;
	while(true)
	{
		clock_t begin = clock();
		string str;
		getline(cin, str);
		vector<StringProb> ret;
		StupidBackOff(tst, str, ret);
		if(ret.size() > 0)
		cout<<ret[0].str<<endl;
		else
		cout<<"NA"<<endl;
		clock_t end = clock();
		cout<<"TIME ELAPSED = "<<(double(end - begin) / CLOCKS_PER_SEC)<<endl;
	}
	return 0;
}
#endif

#if 0
int main()
{
	TST tst;
	//GenerateData();
	tst.deserialize("../serial.txt");
	#if 1
	string str;
	while (getline (cin, str))
	{	
		cout<<tst.find(str)<<endl;
	}
	#endif
	return 0;
}
#endif

