#include <bits/stdc++.h>
using namespace std;
ifstream f("input.txt");
ofstream g("output.txt");
class DFA{
private:
	int current;
	int initial;
	vector < int > states;
	map < pair < int , char > , int > tranz;
	//nod + litera ne duce intr-o alta stare
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
			tranz[{n1,let}] = n2;
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
		current = initial;
		vector < int > processing_road = {initial};
		for(auto let : word)
		{
			if(tranz.find({current,let}) == tranz.end())
				return false;
			current = tranz[{current,let}];
			processing_road.push_back(current);
		}
		if(final.find(current) != final.end())
		{
			g<<"Drumu de procesare pana la acceptare\n";
			for(auto it : processing_road)
				g<<it<<' ';
			g<<'\n';
		}
		return (final.find(current) != final.end());
	}
};


int32_t main()
{
	DFA automata;
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
