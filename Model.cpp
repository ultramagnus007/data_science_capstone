#include "Model.h"

void getString(vector<string> & inputWordV, int ngram, string &s)
{
	if(inputWordV.size() < ngram)
		return;
	//form the string from inputWordV
	for(int i = inputWordV.size() - ngram; i < inputWordV.size(); i++)
	{
		s+=inputWordV[i];
		if(i != inputWordV.size()-1)
		s+=" ";
	}
}

//static vector<double> weights = {0.03, 0.13, 0.28, 0.56};
//static vector<double> weights = {0.025, 0.135, 0.27, 0.57};
static vector<double> weights = {0.01, 0.025, 0.135, 0.27, 0.56};
static double uniTotal = 56199450;
void StupidBackOff(TST& tst, string & line, vector<StringProb> & ret)
{
	istringstream iss(line);
	vector<string> inputWordV;
	while(iss)
	{
		string sub;
		iss>>sub;
		if(sub == "")
			break;
		inputWordV.push_back(sub);
	}
	int N = (inputWordV.size() > 4)? 4:inputWordV.size();
	PQ_TYPE predQ;
	HT_TYPE HT;
	for(int ngram = N; ngram > 0; ngram--)
	{
		string s;
		getString(inputWordV, ngram, s);
		//minimum priority queue
		if(s.size() == 0)
			continue;
		tst.populateTop5(s, predQ);
		if(predQ.size() == 0)
			continue;
		int V = tst.find(s);
		while(!predQ.empty())
		{
			StringCount sval = predQ.top();
			predQ.pop();
			unordered_map<string, double>::iterator it = HT.find(sval.str);
			double val = sval.count*weights[ngram-1]/V;
			if(it == HT.end())
			{
				int unicount = tst.find(sval.str);
				HT[sval.str] = val + unicount/uniTotal;
			}
			else
				HT[sval.str] = it->second+ val;
		}
			
	}
	priority_queue<StringProb, vector<StringProb>, CompMAX> finalPQ;
	for(auto it = HT.begin(); it != HT.end(); it++)
	{
		StringProb v;
		v.str = it->first;
		v.prob = it->second;
		finalPQ.push(v);
	}
	for(int i = 0; i < 3 && i < finalPQ.size(); i++)
	{
		ret.push_back(finalPQ.top());
		finalPQ.pop();
	}
}


static TST tst;
static bool initialized = false;
static double timeelapsed = 0;
static bool isfirst = true;
vector<double> probs;


void initializeTST(const char * filepath)
{
        tst.deserialize(filepath);
        initialized = true;
}

// [[Rcpp::export]]
vector<string> find(string s)
{
	vector<string> ret;
	probs.clear();
	clock_t begin = clock();
	vector<StringProb> V;
	StupidBackOff(tst, s, V);

	for(int i = 0; i<3 && i < V.size(); i++)
	{
		ret.push_back(V[i].str);
		probs.push_back(V[i].prob);
	}
	clock_t end = clock();

	
	if(ret.size() < 3)
	{
		int n = 3-ret.size();
		vector<string> a = {"the", "to", "and"};
		for(int i = 0; i < n; i++)
		{
			ret.push_back(a[i]);
			probs.push_back(.33);
		}
	}
	timeelapsed = 1000*(double(end - begin)/ CLOCKS_PER_SEC);
	return ret;
}

// [[Rcpp::export]]
vector<double> getProb()
{
	return probs;
}
// [[Rcpp::export]]
string gettimeElapsed()
{
	if(isfirst)
	{
		isfirst = false;
		return string("Application Loaded");
	}
	 string ret = "time elapsed:= ";
	 ret+= to_string(timeelapsed);
	 ret+=" ms";
	 return ret;
}



