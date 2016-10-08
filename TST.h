#ifndef _TST_H_
#define _TST_H_
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;




class SVAL
{
	public:
	SVAL(){s="", count = 0;}
	string s;
	double count;
};	

class CompMIN
{
	public:
	bool operator()(const SVAL & p1, const SVAL & p2)const
	{
		return p1.count > p2.count;
	}
};

class CompMAX
{
	public:
	bool operator()(const SVAL & p1, const SVAL & p2)const
	{
		return p1.count < p2.count;
	}
};




typedef priority_queue<SVAL, vector<SVAL>, CompMIN>  PQ_TYPE;
typedef unordered_set<string> HS_TYPE;
typedef unordered_map<string, double> HT_TYPE;

class node
{
	public:
	node(char key, int val = 0);
	char key;
	int val;
	node *left , *mid, *right;
};




class TST
{
	private:
	node * findEnd(const string &str);
	void populateTop5(node * ptr, PQ_TYPE & V, char * buff, int i);
	node * insert(node * ptr, int i, string &str, int val = 0);
	bool prune(node * ptr, int count);
	void serialize(ofstream &outfile, node * ptr);
	node * deserialize(ifstream &infile, node * ptr);


	public:
	TST();
	~TST();
	//Searches all string starting with str and top 5 in vector;
	void populateTop5(const string & str, PQ_TYPE & predQ);
	void insert(string &str, int val = 0);
	int find(string &str);
	void write(const char *filepath, int ngram = -1);
	node * root;
	void read(const char * filepath, int mincount = 1);
	void prune(int count);
	int deletecount;
	void serialize(const char * filepath);
	void deserialize(const char * filepath);
};

#endif

