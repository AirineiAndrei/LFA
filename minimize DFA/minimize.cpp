#include <bits/stdc++.h>
using namespace std;
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
void dump_set(set < int > s){
    for(auto it : s)cout<<it<<' ';
    cout<<'\n';
}

set < int > intersection(set < int > s1, set < int > s2)
{
    set < int > ans;
    for(auto it : s1)
        if(s2.find(it) != s2.end())
            ans.insert(it);
    return ans;
}
set < int > difference(set < int > s1, set < int > s2)
{
    set < int > ans = s1;
    for(auto it : s2)
        if(ans.find(it) != ans.end())
            ans.erase(it);
    return ans;
}


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

    void minimize() {
        //modifies the class
        set<int> nofinal;
        set<char> sigma;
        for (auto it: tranz) {
            nofinal.insert(it.first.first);
            nofinal.insert(it.second);
            sigma.insert(it.first.second);
        }
        for (auto it: final_states)
            if (nofinal.find(it) != nofinal.end())
                nofinal.erase(it);
        set< set < int > > W = {nofinal, final_states};
        set< set < int > > P = {nofinal, final_states};



        while (!W.empty()) {
            set<int> A = *W.begin();
            W.erase(A);

            for (auto c: sigma) {
                set<int> X;
                for (auto muchie: tranz)
                    if (muchie.first.second == c and A.find(muchie.second) != A.end())
                        X.insert(muchie.first.first);


                set<set<int> > newP = P;
                for (auto Y: P) {
                    set<int> intersect = intersection(X, Y);
                    set<int> differ = difference(Y, X);


                    if (!intersect.empty() and !differ.empty()) {

                        newP.erase(Y);
                        newP.insert(intersect);
                        newP.insert(differ);
                        if (W.find(Y) != W.end()) {
                            W.erase(Y);
                            W.insert(intersect);
                            W.insert(differ);
                        } else {
                            if (intersect.size() <= differ.size())
                                W.insert(intersect);
                            else
                                W.insert(differ);
                        }
                    }
                }
                P = newP;
            }
        }
        for (auto it: P) {
            it.erase(*it.begin());
            for(auto tt : it)
            {
                map < pair < int , char > , int > ntranz ;
                for(auto muchie : tranz)
                    if(!(muchie.first.first == tt or muchie.second == tt))
                        ntranz.insert(muchie);
                tranz = ntranz;
            }
        }
    }
};


int main() {
    DFA test;
    cin >> test;
    cout << test;
    test.minimize();
    cout << test;
    return 0;
}
