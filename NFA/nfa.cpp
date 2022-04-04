#include <bits/stdc++.h>
using namespace std;
ifstream f("input.txt");
ofstream g("output.txt");
class NFA{
private:
	set < int > current;
	int initial;
	vector < int > states;
	map < pair < int , char > , vector < int > > tranz;
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
		map < pair < int , int > , int > parent;
		//parent[{i,j}] -> cum ajunge in starea i la nivelul j de procesare
		int level = -1;
		for(auto let : word)
		{
			level ++;
			set < int > nextstate;
			for(auto cstate : current)
				if(tranz.find({cstate,let}) != tranz.end())
					for(auto nstate : tranz[{cstate,let}])
					{
						if(parent.find({nstate,level}) == parent.end())
							parent[{nstate,level}] = cstate; 
						nextstate.insert(nstate);
					}
			if(nextstate.empty())
			{
				//nu avem continuare
				return false;
			}
			current = nextstate;
			//tin toate starile la nivelul current de procesare
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
		if(ok)
		{
			g<<"Drumul de acceptare este\n";
			vector < int > processing_road;
			for(int node = lstate; level >= -1;node = parent[{node,level--}])
				processing_road.push_back(node);
			reverse(processing_road.begin(), processing_road.end());
			for(auto it : processing_road)
				g<<it<<' ';
			g<<'\n';
		}
		return ok;
	}
};


int32_t main()
{
	NFA automata;
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
