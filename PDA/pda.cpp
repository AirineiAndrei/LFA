#include <bits/stdc++.h>
using namespace std;

class IoBase {
private:
public:
    virtual istream &read(istream &is) {
        return is;
    }
    virtual ostream &write(ostream &os) const {
        return os;
    }
    friend ostream &operator<<(ostream &os, const IoBase &base) {
        return base.write(os);
    }
    friend istream &operator>>(istream &is, IoBase &base) {
        return base.read(is);
    }
};



class PDA : public IoBase{
private:
    struct in_edge{
        int state;
        char let;
        char stack_top;
        bool operator < (const in_edge other) const{
            if(state != other.state)
                return state < other.state;
            if(let != other.let)
                return let < other.let;
            return stack_top < other.stack_top;
        }
    };
    struct out_edge{
        int state;
        string stack_add;
    };
    set < int > states;
    set < char > input_alphabet;
    set < char > stack_alphabet;
    int start_state;
    char start_stack_symbol;
    set < int > accepting_states;
    map < in_edge , out_edge > tranz;
public:

    istream &read(istream &is) override {
        IoBase::read(is);
        int n;
        cout << "number of states: ";
        is >> n;
        cout << "states:\n";
        while(n--){
            int x;
            is >> x;
            states.insert(x);
        }

        cout << "size of input alphabet: ";
        is >> n;
        cout << "input_alphabet:\n";
        while(n--){
            char x;
            is >> x;
            input_alphabet.insert(x);
        }

        cout << "size of stack alphabet: ";
        is >> n;
        cout << "stack_alphabet:\n";
        while(n--){
            char x;
            is >> x;
            stack_alphabet.insert(x);
        }

        cout << "start state: ";
        is >> start_state;

        cout << "start stack symbol: ";
        is >> start_stack_symbol;

        cout << "number of final states: ";
        is >> n;
        cout  << "final states:\n";
        while(n--){
            int x;
            is >> x;
            accepting_states.insert(x);
        }

        cout << "number of edges: ";
        is >> n;
        while(n--){
            in_edge in;
            out_edge out;
            is >> in.state >> in.let >> in.stack_top;
            is >> out.state >> out.stack_add;
            tranz[in] = out;
        }

        return is;
    }
    ostream &write(ostream &os) const override {
        IoBase::write(os);

        os << "states: ";
        for(auto it : states)
            os << it << ' ';
        os << '\n';

        os << "input alphabet: ";
        for(auto it : input_alphabet)
            os << it << ' ';
        os << '\n';

        os << "stack alphabet: ";
        for(auto it : stack_alphabet)
            os << it << ' ';
        os << '\n';

        os << "start state: " << start_state << '\n';
        os << "start stack symbol: " << start_stack_symbol << '\n';

        os << "accepting states: ";
        for(auto it : accepting_states)
            os << it << ' ';
        os << '\n';

        os << "edges: \n";
        for(auto it : tranz)
        {
            in_edge in = it.first;
            out_edge out = it.second;
            os << in.state << ' ' << in.let << ' ' << in.stack_top << ' ';
            os << out.state << ' ' << out.stack_add << '\n';
        }
        return os;
    }
    struct PDA_state {
        int indexOfNext;
        int state;
        stack < char > pda_stack;
    };
    void dump_pda_state(PDA_state nod){
        cout << "nod: ";
        cout << nod.indexOfNext << ' ' << nod.state << '\n';
        stack < char > pda_stack_cp = nod.pda_stack;
        while(!pda_stack_cp.empty()){
            cout << pda_stack_cp.top();
            pda_stack_cp.pop();
        }
        cout << "\n\n";
        return;
    }
    bool process(string word){
        //bfs
        queue < PDA_state > c;
        PDA_state start;
        start.state = start_state;
        start.indexOfNext = 0;
        start.pda_stack.push(start_stack_symbol);
        c.push(start);
        while(!c.empty()){
            PDA_state current = c.front();
//            dump_pda_state(current);
            c.pop();

            //find an edge to go somewhere else
            //dont read the next letter from the word use the lambda('#' here) movement
            in_edge no_read;
            no_read.state = current.state;
            no_read.let = '#';
            no_read.stack_top = current.pda_stack.top();
            if(tranz.find(no_read) != tranz.end()){
                out_edge out = tranz[no_read];
                PDA_state nxt = current;
                nxt.state = out.state;
                nxt.pda_stack.pop();
                string add = out.stack_add;
                for(auto it = add.rbegin();it < add.rend();it++)
                    if(*it != '#')
                        nxt.pda_stack.push(*it);
                c.push(nxt);
            }

            if(current.indexOfNext == word.size()){
                if(accepting_states.find(current.state) != accepting_states.end())
                    return true;
                continue;
            }

            //read the next letter from the word
            in_edge read;
            read.state = current.state;
            read.let = word[current.indexOfNext ++];
            read.stack_top = current.pda_stack.top();
            if(tranz.find(read) != tranz.end()){
                out_edge out = tranz[read];
                PDA_state nxt = current;
                nxt.state = out.state;
                nxt.pda_stack.pop();
                string add = out.stack_add;
                for(auto it = add.rbegin();it < add.rend();it++)
                    if(*it != '#')
                        nxt.pda_stack.push(*it);
                c.push(nxt);
            }
        }
        return false;
    }
};


int main() {

    PDA pda;
    cin >> pda;
//    cout << pda;
    vector < string > eval = {"0011" , "0000011111", "0001111","","01010","10","111000"};
    for(auto it : eval){
        cout << it << ' ' << pda.process(it) << '\n';
    }
    return 0;
}
