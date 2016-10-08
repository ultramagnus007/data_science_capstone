#include "TST.h"
#include "Process.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <stack>

void CreateFinalTST(TST &tst)
{
	tst.read("../ngram/1prune.txt");
	tst.read("../ngram/2prune.txt");
	tst.read("../ngram/3prune.txt");
	tst.read("../ngram/4prune.txt");
	tst.read("../ngram/5prune.txt");
}


void ReadFromRawData(TST &tst, int ngram)
{
	string line;
	//ifstream myfile ("/home/lotus/Data_Science_Capstone/");
	//ifstream myfile("/home/lotus/Data_Science_Capstone/S_Clean/str_all_withoutstem.txt");
        ifstream myfile("/home/lotus/Data_Science_Capstone/UNI_CODE/Clean/str.txt");
	//ifstream myfile("/home/lotus/Data_Science_Capstone/S_Clean/str_all_withoutstemAndstopwords.txt");
	while (getline(myfile,line))
	{
		istringstream iss(line);
		vector<string> V;
		while(iss)
		{
			string sub;
			iss >> sub;
			if(sub=="")
				break;
			V.push_back(sub);
			if(V.size() >= ngram)
			{
				string str;
				bool ispre = true;
				for(int i = V.size()-ngram; i< V.size(); i++)
				{
					str+=V[i];
					if(i==V.size()-2)
					{
					 if(tst.find(str))
						ispre = true;
					 else
						ispre = false;
					}
					if(i != V.size()-1)
						str+=" ";
				}
				if(ispre)
				{
					tst.insert(str);
				}
			}
		}
	}
	myfile.close();
}

void writeTST(TST & tst, int ngram, const char * filename, const char * prunefilename)
{
        tst.write(filename, ngram);
	tst.~TST();
	TST tsttmp;
	//i.e less than 4 count ngrams does not write
	tsttmp.read(filename, 2);
	tsttmp.write(prunefilename, ngram);
}

void _1gram()
{
	TST tst;
	ReadFromRawData(tst, 1);
	writeTST(tst, 1, "../ngram/1.txt", "../ngram/1prune.txt");
}

void _2gram()
{
	TST tst;
	tst.read("../ngram/1prune.txt");
	ReadFromRawData(tst, 2);
	writeTST(tst, 2, "../ngram/2.txt", "../ngram/2prune.txt");
}
void _3gram()
{
	TST tst;
	tst.read("../ngram/2prune.txt");
	ReadFromRawData(tst, 3);
	writeTST(tst, 3, "../ngram/3.txt", "../ngram/3prune.txt");
}
void _4gram()
{
	TST tst;
	tst.read("../ngram/3prune.txt");
	ReadFromRawData(tst, 4);
	writeTST(tst, 4, "../ngram/4.txt", "../ngram/4prune.txt");
}
void _5gram()
{
	TST tst;
	tst.read("../ngram/4prune.txt");
	ReadFromRawData(tst, 5);
	writeTST(tst, 5, "../ngram/5.txt", "../ngram/5prune.txt");
}


void GenerateData()
{
	clock_t begin = clock();
	_1gram();
	_2gram();
	_3gram();
	_4gram();
	_5gram();
	TST tst;
	CreateFinalTST(tst);
	tst.serialize("../serial.txt");
	clock_t end = clock();
	cout<<"TIME ELAPSED = "<<(double(end - begin) / CLOCKS_PER_SEC)<<endl;
}


