#include <bits/stdc++.h>
#include <ostream>

using namespace std;

//ifstream f("input.txt");
//ofstream g("output.txt");

const bool PRINT_ROAD = false;

class automat{
protected:
    int initial_state;
    set < int > final_states;
public:
    automat(int initialState, const set<int> &finalStates) :
    initial_state(initialState), final_states(finalStates) {}
    automat() {}

    friend ostream &operator<<(ostream &os, const automat &automat) {
        os << "initial_state: " << automat.initial_state << '\n';
        os << " final_states: ";
        for(auto it : automat.final_states)
            os << it << ' ';
        os << '\n';
        return os;
    }
    friend istream &operator>>(istream &is,automat &automat) {
        is >> automat.initial_state;
        int n;
        is >> n;
        while(n--){
            int state;
            is >> state;
            automat.final_states.insert(state);
        }
        return is;
    }

    void setInitialState(int initialState) {
        initial_state = initialState;
    }

    void setFinalStates(const set<int> &finalStates) {
        final_states = finalStates;
    }

    int getInitialState() const {
        return initial_state;
    }
};

class DFA : public automat{
private:
    map < pair < int , char > , int > tranz;
public:
    DFA(int initialState, const set<int> &finalStates, const map<pair<int, char>, int> &tranz) :
        automat(initialState,finalStates),tranz(tranz) {}
    DFA() {}

    friend ostream &operator<<(ostream &os, const DFA &dfa) {
        os << static_cast<const automat &>(dfa) << " tranz: ";
        for(auto tr : dfa.tranz)
            os << "Muchie " << tr.first.first << ' ' << tr.second << ' ' << tr.first.second <<'\n';
        return os;
    }

    friend istream &operator>>(istream &is,DFA &dfa) {
        is >> static_cast< automat &>(dfa);
        int m;
        is >> m;
        while(m --)
        {
            int n1,n2;
            char let;
            is >> n1 >> n2 >> let;
            dfa.tranz.insert({{n1,let},n2});
        }
        return is;
    }
    bool process(string word)
    {
        int current;
        current = initial_state;
        vector < int > processing_road = {initial_state};
        for(auto let : word)
        {
            if(tranz.find({current,let}) == tranz.end())
                return false;
            current = tranz[{current,let}];
            processing_road.push_back(current);
        }
        if(final_states.find(current) != final_states.end() and PRINT_ROAD)
        {
            cout<<"Drumu de procesare pana la acceptare\n";
            for(auto it : processing_road)
                cout<<it<<' ';
            cout<<'\n';
        }
        return (final_states.find(current) != final_states.end());
    }

    void setTranz(const map<pair<int, char>, int> &tranz) {
        DFA::tranz = tranz;
    }
};

class NFA : public automat{
private:
    map < pair < int , char > , vector < int > > tranz;
    map < int , set < char > > alphaconnection;
    void generate_alphaconnection(){
        for(auto it : tranz)
        if(!it.second.empty())
        {
            int n1 = it.first.first;
            char let = it.first.second;
            if(alphaconnection.find(n1) == alphaconnection.end())
                alphaconnection[n1] = {let};
            else
                alphaconnection[n1].insert(let);
        }

    }
public:
    NFA(int initialState, const set<int> &finalStates, const map<pair<int, char>, vector<int>> &tranz) : automat(
            initialState, finalStates), tranz(tranz) {generate_alphaconnection();}
    NFA() {}



    friend ostream &operator<<(ostream &os, const NFA &nfa) {
        os << static_cast<const automat &>(nfa) << " tranz: ";
        for(auto tr : nfa.tranz)
        {
            os << "Muchie " << tr.first.first << ' ' << tr.first.second <<'\n';
            os << "Merge in: ";
            for(auto node : tr.second)
                os << node << ' ';
            os << '\n';
        }
        return os;
    }

    friend istream &operator>>(istream &is,NFA &nfa) {
        is >> static_cast< automat &>(nfa);
        int m;
        is >> m;
        while(m --)
        {
            int n1,n2;
            char let;
            is >> n1 >> n2 >> let;
            if(nfa.tranz.find({n1,let}) == nfa.tranz.end())
                nfa.tranz[{n1,let}] = {n2};
            else
                nfa.tranz[{n1,let}].push_back(n2);
        }
        nfa.generate_alphaconnection();
        return is;
    }
    bool process(string word)
    {
        set < int > current;
        current.insert(initial_state);
        map < pair < int , int > , int > parent;
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
                return false;
            current = nextstate;
        }
        bool ok = false;
        int lstate = -1;
        for(auto it : current)
            if(final_states.find(it) != final_states.end())
            {
                lstate = it;
                ok = true;
                break;//am gasit o stare
            }
        if(ok and PRINT_ROAD)
        {
            cout << "Drumul de acceptare este\n";
            vector < int > processing_road;
            for(int node = lstate; level >= -1;node = parent[{node,level--}])
                processing_road.push_back(node);
            reverse(processing_road.begin(), processing_road.end());
            for(auto it : processing_road)
                cout <<it<<' ';
            cout <<'\n';
        }
        return ok;
    }
    set < int > next_subset(set < int > current, char let)
    {
        //returneaza subsetu accesibil print let din subsetu curent
        set < int > ans;
        for(auto it : current)
            for(auto node : tranz[{it,let}])
                ans.insert(node);
        return ans;
    }
    set < char > get_alphabet_subset(const set < int > current)
    {
        set < char > ans;
        for(auto it : current)
            for(auto let : alphaconnection[it])
                ans.insert(let);
        return ans;
    }
    bool is_final(const int node)
    {
        return (final_states.find(node) != final_states.end());
    }
};

DFA convert (NFA nfa)
{
    DFA output;
    //subset construction
    map < set < int > , int > subsets;
    int id = 0; // le aloc un id
    map < pair < int , char > , int > edges;
    queue < set < int > > c; // for BFS
    c.push({nfa.getInitialState()});
    subsets.insert({{nfa.getInitialState()},id++});
    while(!c.empty())
    {
        set < int > step = c.front();
        int stepid = subsets[step];
        c.pop();
        set < char > alphabetlocal = nfa.get_alphabet_subset(step);
        for(auto let : alphabetlocal){
            set < int > nextstep = nfa.next_subset(step,let);
            if(subsets.find(nextstep) != subsets.end()){
                //already processed
                edges[{stepid,let}] = subsets[nextstep];
                continue;
            }
            edges[{stepid,let}] = id;
            subsets.insert({nextstep,id++});
            c.push(nextstep);
        }
    }
    output.setInitialState(0);
    output.setTranz(edges);
    set < int > finalstates;
    for(auto it : subsets){
        bool has_final = false;
        for(auto tt : it.first)
            if(nfa.is_final(tt)){
                has_final = true;
                break;
            }
        if(has_final)
            finalstates.insert(it.second);
    }
    output.setFinalStates(finalstates);

//    for(auto it : subsets)
//    {
//        for(auto tt : it.first)
//            cout<<tt<<' ';
//        cout<<"is: ";
//        cout<<it.second<<'\n';
//    }
    return output;
}


int main() {
    NFA automata;
    cin >> automata;

    DFA out = convert(automata);
//    cout<<automata<<'\n';
//    cout<< out;

    int q;
    cin >> q;
    while(q --)
    {
        string word;
        cin >> word;
//        cout << word << '\n';
        cout << automata.process(word) <<' '<<out.process(word)<< '\n';
    }

    return 0;
}
