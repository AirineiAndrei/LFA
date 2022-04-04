#include <bits/stdc++.h>
using namespace std;
ifstream f("input.txt");
ofstream g("output.txt");
class LNFA{
private:
	set < int > current;
	int initial;
	vector < int > states;
	map < pair < int , char > , vector < int > > tranz;
	map < int , vector < int > > lambda;
	//nod + litera ne duce intr-o multime de stari
	set < int > final;
public:
	void read()
	{
		int n;
		f>>n;
		states.resize(n);
		for(auto &st : states)
			f>>st;
		int m;
		f>>m;
		for(int i = 1;i <= m;i++)
		{
			int n1,n2;
			char let;
			f>>n1>>n2>>let;
			if(tranz.find({n1,let}) == tranz.end())
				tranz[{n1,let}] = {n2};
			else
				tranz[{n1,let}].push_back(n2);
		}
		int L;
		f>>L;
		for(int i=1;i<=L;i++)
		{
			//lambda muchii
			int n1,n2;
			f>>n1>>n2;
			if(lambda.find(n1) == lambda.end())
				lambda[n1] = {n2};
			else
				lambda[n1].push_back(n2);
		}
		f>>initial;
		int finalstates;
		f>>finalstates;
		while(finalstates--)
		{
			int x;
			f>>x;
			final.insert(x);
		}
	}
	bool process(string word)
	{
		current.clear();
		current.insert(initial);
		//imi tin de fiecare data starile in care sunt in pasul curent
		//complexitate finala n*log(n)*m*log(m)*p, (p nr de litere),log(n) pt ca folosesc set
		//alternativ putem scapa de log folosind un unordered_set
		//log m din mapul in care tin tranzitiile, aceiasi schimbare posibila pt a scapa de el
		int level = -1;
		for(auto let : word)
		{
			while(true)
			{
				vector < int > newlambda;
				for(auto it : current)
					if(lambda.find(it) != lambda.end())
					{
						for(auto nlam : lambda[it])
							if(current.find(nlam) == current.end())
								newlambda.push_back(nlam);
					}
				if(newlambda.empty())break;
				for(auto it : newlambda)
					current.insert(it);
			}
			set < int > nextstate;
			for(auto cstate : current)
				if(tranz.find({cstate,let}) != tranz.end())
					for(auto nstate : tranz[{cstate,let}])
						nextstate.insert(nstate);
			if(nextstate.empty())
			{
				//nu avem continuare
				return false;
			}
			current = nextstate;			
		}
		bool ok = false;
		int lstate = -1;
		for(auto it : current)
			if(final.find(it) != final.end())
			{
				lstate = it;
				ok = true;
				break;//am gasit o stare
			}
		return ok;
	}
};


int32_t main()
{
	LNFA automata;
	automata.read();
	int wordcnt;
	f>>wordcnt;
	while(wordcnt--)
	{
		string word;
		f>>word;
		g << (automata.process(word) ? "DA" : "NU") << '\n';
	}
    return 0;
}