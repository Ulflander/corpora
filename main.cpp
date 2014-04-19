#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Node {
    public:
        Node() { 
            mContent = ' '; 
            mMarker = false; 
        }
        ~Node() {}
        char get() { 
            return mContent; 
        }
        void set(char c) { 
            mContent = c; 
        }
        bool isMarker() { 
            return mMarker; 
        }
        void setMarker() { 
            mMarker = true; 
        }
        void append(Node* child) { 
            mChildren.push_back(child); 
        }
        vector<Node*> children() { 
            return mChildren; 
        }
        Node* find(char c);

    private:
        char mContent;
        bool mMarker;
        vector<Node*> mChildren;
};

Node* Node::find(char c) {
    int l = mChildren.size();

    for (int i = 0; i < l; i++) {
        Node* tmp = mChildren.at(i);
        if (tmp->get() == c) {
            return tmp;
        }
    }

    return NULL;
};


class Trie {
    public:
        Trie();
        ~Trie();
        void add(string s);
        bool search(string s);
        void rem(string s);
    private:
        Node* root;
};

Trie::Trie() {
    root = new Node();
}

Trie::~Trie() {
    // Free memory
}

void Trie::add(string s) {
    Node* curr = root;
    int l = s.length();

    if (l == 0) {
        curr->setMarker(); // an empty word
        return;
    }

    for (int i = 0; i < l; i++) { 
        Node* child = curr->find(s[i]);

        if (child != NULL) {
            curr = child;
        } else {
            Node* tmp = new Node();
            tmp->set(s[i]);
            curr->append(tmp);
            curr = tmp;
        }

        if (i == l - 1) {
            curr->setMarker();
        }
    }
}


bool Trie::search(string s) {
    Node* curr = root;
    int l = s.length();

    while (curr != NULL) {

        for (int i = 0; i < l; i++) {
            Node* tmp = curr->find(s[i]);
            if ( tmp == NULL ) {
                return false;
            }
            curr = tmp;
        }

        if (curr->isMarker()) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}


class Corpus {
    public:
        Corpus() { 
            cRoot = ' ';
        }
        void root(string r) {
            cRoot = r;
        }
        bool load(string s);
        Trie* get(string s);
        bool has(string s);

    private:
        string cRoot;
        typedef struct _Data {
            Trie t;
            string n;
        } Data ;
        vector<Data> stack;
};

bool Corpus::has(string s) {
    return false;
};

bool Corpus::load(string s) {
    ifstream file(cRoot+s);
    if (!file) {
        return false;
    }

    Trie* trie = new Trie();
    while(getline(*file, line)){
        trie->add(line)
    };
    return true;
};

Trie* Corpus::get(string s) {
    return NULL;
};

/*
    Main entry point

    Requires:
    - First argument: root of corpus files
*/
int main(int argc, char* argv[]) {

    Corpus* corpus = new Corpus();

    cout << "argc = " << argc << endl; 
    for(int i = 0; i < argc; i++) {

    }
    cout << "argv[" << i << "] = " << argv[i] << endl; 


    Trie* trie = new Trie();
    trie->add("Hello");
    trie->add("Balloon");
    trie->add("Ball");

    if ( trie->search("Hell") )
        cout << "Found Hell" << endl;

    if ( trie->search("Hello") )
        cout << "Found Hello" << endl;

    if ( trie->search("Helloo") )
        cout << "Found Helloo" << endl;

    if ( trie->search("Ball") )
        cout << "Found Ball" << endl;

    if ( trie->search("Balloon") )
        cout << "Found Balloon" << endl;

    delete trie;
}