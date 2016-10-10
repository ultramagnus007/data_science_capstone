#include "TST.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stack>
#include <queue>
#include <cassert>
using namespace std;

node::node(char key, int val)
{
	this->key = key;
	this->val = val;
	left = mid = right = NULL;
}

TST::TST()
{
	root = NULL;
	deletecount = 0;
}

static void inorderDelete(node *ptr)
{
	if(ptr == NULL)
		return;
	inorderDelete(ptr->left);
	inorderDelete(ptr->mid);
	inorderDelete(ptr->right);
	delete ptr;
}
TST::~TST()
{
	inorderDelete(root);
	root = NULL;
}
void TST::insert(string &str, int val)
{	
	root = insert(root, 0, str, val);
}

node * TST::findEnd(const string &str)
{
	int i = 0;
	node * ptr = root;
	node * preptr = NULL;
	while(ptr != NULL && i != str.length())
	{
		if(ptr->key == str[i])
		{
			preptr = ptr;
			ptr = ptr->mid;
			i++;
		}
		else if(str[i] < ptr->key)
			ptr = ptr->left; 
		else
			ptr = ptr->right;
	}
	if(i == str.length()&& preptr != NULL)
		return preptr;
	else
		return NULL;
}

void TST::populateTop5(node * ptr, PQ_TYPE & predQ, char * buff, int i)
{
	if(ptr == NULL)
		return;
	if(i > 0 && ptr->key == ' ')
		return;
	populateTop5(ptr->left, predQ, buff, i);

	buff[i] = ptr->key;
	if(ptr->val > 0)
	{
		buff[i+1] = '\0';
		StringCount sval;
		sval.str = buff;
		sval.count = ptr->val;
		// keep only 5 values in queue
		if(predQ.size() < 5)
		{
			predQ.push(sval);
		}
		else if(predQ.top().count < sval.count)
		{
			predQ.pop();
			predQ.push(sval);
		}
	}
	populateTop5(ptr->mid, predQ, buff, i+1);
	populateTop5(ptr->right, predQ, buff, i);
}


void TST::populateTop5(const string & str, PQ_TYPE & predQ)
{
	string s=str+" ";
	node * endptr = findEnd(s);
	if(endptr != NULL)
	{
		char buff[1000];
		populateTop5(endptr, predQ, buff, 0);
	}
}




void TST::write(ofstream &outfile, node * ptr, char * buff, int i, int spcount, int ngram)
{
	if(ptr == NULL)
		return;
	write(outfile,ptr->left, buff, i , spcount, ngram);
	buff[i] = ptr->key;
	int nextspcount = (ptr->key==' ')? spcount+1:spcount;
	if(ptr->val > 0 && (ngram == -1 || spcount == ngram-1) )
	{
		buff[i+1] = '\0';
		outfile<<buff<<"	"<<ptr->val<<endl;
	}
	write(outfile,ptr->mid, buff, i+1 , nextspcount, ngram);
	write(outfile,ptr->right, buff, i , spcount, ngram);
}
void TST::write(const char *filepath , int ngram)
{
	ofstream outfile;
        outfile.open (filepath);
	char buff[1000];
	write(outfile,root, buff, 0, 0, ngram);
        outfile.close();
}
void TST::read(const char * filepath, int mincount)
{
	string line;
	ifstream myfile (filepath);
	int i = 0;
	while (getline (myfile,line))
        {
		i++;
		stack<int> S;
		int i = line.length()-1;
		while(isdigit(line[i]))	
		{
			S.push(line[i]-'0');
			i--;
		}
		i--;
		string str = line.substr(0, i+1);
		int val = 0;
		while(!S.empty())
		{
			val*=10;
			val+=S.top();
			S.pop();
		}
		if(val >= mincount)
			insert(str, val);
        }
        myfile.close();
}



node * TST::insert(node * ptr, int i, string &str, int val)
{
	if(i == str.length())
		return ptr;
	if(ptr == NULL)
	{
		if(i+1 == str.length())
		{
			if(val != 0)
			ptr = new node(str[i], val);	
			else
			ptr = new node(str[i], 1);
		}
		else
			ptr = new node(str[i]);	
	}
	else if(i+1 == str.length() && ptr->key == str[i])
	{
		 if(val == 0)
		 {
			ptr->val +=1;
		 }
/*******************LOOK HERE WHEN BOTH CORPUS WITH AND WITHOUT STOP WORDS*****************************/
		 //else
			//ptr->val +=val;
	}
	if(ptr->key == str[i])
		ptr->mid = insert(ptr->mid, i+1, str, val);
	else if(ptr->key > str[i])
		ptr->left = insert(ptr->left, i, str, val);
	else
		ptr->right = insert(ptr->right, i, str, val);
	return ptr;
}

int TST::find(string &str)
{
	int i = 0;
	node * ptr = root;
	node * preptr = NULL;
	while(ptr != NULL && i != str.length())
	{
		if(ptr->key == str[i])
		{
			preptr = ptr;
			ptr = ptr->mid;
			i++;
		}
		else if(str[i] < ptr->key)
			ptr = ptr->left; 
		else
			ptr = ptr->right;
	}
	if(i == str.length()&& preptr != NULL)
		return preptr->val;
	else
		return 0;
}

void TST::serialize(ofstream &outfile, node * ptr)
{
	if(ptr == NULL)
	{
		outfile<<")";
		return;
	}
	outfile<<ptr->key;
	if(ptr->val > 0)
		outfile<<ptr->val;
	serialize(outfile,ptr->left);
	serialize(outfile, ptr->mid);
	serialize(outfile, ptr->right);
}


void TST::serialize(const char * filepath)
{
	ofstream outfile;
        outfile.open (filepath);
	serialize(outfile,root);
        outfile.close();
}


node * TST::deserialize(ifstream &infile, node * ptr)
{
	char ch;
	if(infile >> noskipws>>ch)
	{
		if(ch==')')
			return NULL;
		else
			ptr = new node(ch);
	}
	else
		return NULL;
	//cout<<ch;
	queue<int> V;
	bool isEOF = true;
	while(infile >> noskipws>>ch)
	{	
		isEOF = false;
		if(isdigit(ch))
		{
			//ptr->val = ch-'0';
			V.push(ch-'0');
			//cout<<ch;
		}
		else
		{
			infile.putback(ch);
			break;
		}
	}
	
	if(isEOF)
		return ptr;
	if(V.size() > 0)
	{
		int val =0;
		while(V.empty()==false)
		{
			val*=10;
			val+=V.front();
			V.pop();
		}
		ptr->val = val;
	}
	ptr->left = deserialize(infile,ptr->left);
	ptr->mid = deserialize(infile, ptr->mid);
	ptr->right = deserialize(infile, ptr->right);
	return ptr;
}

void TST::deserialize(const char * filepath)
{
	ifstream infile;
        infile.open (filepath);
	root = deserialize(infile,root);
        infile.close();
	cout<<endl;
}


void TST::prune(int count)
{
	if(prune(root, count))
	{
		deletecount++;
		delete root;
		root = NULL;
	}
}

bool TST::prune(node * ptr, int count)
{
	if(ptr == NULL)
		return false;
	bool b1 = true;
	if(ptr->left != NULL)
		b1 = prune(ptr->left, count);
	if(b1)ptr->left = NULL;

	bool b2 = true;
	if(ptr->mid != NULL)
		b2 = prune(ptr->mid, count);
	if(b2)ptr->mid = NULL;


	bool b3 = true;
	if(ptr->right != NULL)   
		b3 = prune(ptr->right, count);
	if(b3)ptr->right = NULL;
	if(ptr->val != 0 && ptr->val < count)
	{
		deletecount++;
		delete ptr;
		return true;
	}
	if(ptr->val == 0 && b1 && b2 && b3)
	{
		deletecount++;
		delete ptr;
		return true;
	}
	return false;
}


#if 0
int main()
{
        TST T;
        vector<string> a = {"amit", "anil", "nittin", "alonso", "kimi", "raikkonen"};
        int n = a.size();
        for(int i = 0; i < n; i++)
                T.insert(a[i], (i+1)*(i+1));
        for(int i = 0; i < n; i++)
                cout<<a[i]<<" "<<T.find(a[i])<<endl;
        cout<<endl;
	string s("unsu");
        cout<<"unsu = "<<T.find(s)<<endl;
	return 0;
}
#endif
