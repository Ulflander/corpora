
#include <iostream>
#include <fstream>

#include <vector>
#include <map>

/* Main namespace */
using namespace std;


#define SOCK_PORT 1234
#define SOCK_HOST "localhost"

/* Tried to "define" it like SOCK_HOST
   but can't "cout" it anymore then. */
string VERSION = "0.1";

/*
    Node class for Trie
*/
class Node {
    public:
        Node() { 
            mContent = ' '; 
            mMarker = false; 
        }
        ~Node() {}
        /* Get node content */
        char get() { 
            return mContent; 
        }
        /* Set node content */
        void set(char c) { 
            mContent = c; 
        }
        /* Check if node is a marker */
        bool isMarker() { 
            return mMarker; 
        }
        /* Set node as a marker */
        void setMarker() { 
            mMarker = true; 
        }
        /* Append child node */
        void append(Node* child) { 
            mChildren.push_back(child); 
        }
        /* Get all children */
        vector<Node*> children() { 
            return mChildren; 
        }
        /* Find a child */
        Node* find(char c);

    private:
        char mContent;
        bool mMarker;
        vector<Node*> mChildren;
};

/* Node find child impl */
Node* Node::find(char c) {
    int l = mChildren.size();
    Node* tmp;
    int i;

    for (i = 0; i < l; i++) {
        tmp = mChildren.at(i);
        if (tmp->get() == c) {
            return tmp;
        }
    }

    return NULL;
};

/*
    Trie class
*/
class Trie {
    public:
        Trie();
        ~Trie();
        /* Add nodes given a string */
        void add(string s);
        /* Search for a string in trie */
        bool search(string s);
    private:
        Node* root;
};

/* Constructor: creates the root node */
Trie::Trie() {
    root = new Node();
}

Trie::~Trie() {
    // Free memory
    // Should I do something here ??
}

/* Add nodes given a string */
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

/* Search for a string in trie */
bool Trie::search(string s) {
    Node* curr = root;
    int l = s.length();
    Node* tmp;

    while (curr != NULL) {

        for (int i = 0; i < l; i++) {
            tmp = curr->find(s[i]);
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

/* Contain tries for corpora */
class Corpora {
    public:
        Corpora() { 
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
        std::map<string, Trie*> corpora;
};

bool Corpora::has(string s) {
    return false;
};

bool Corpora::load(string s) {
    ifstream file(cRoot+s);
    if (!file) {
        cerr << "Corpora cant load file " << s << endl;
        return false;
    }

    string line;
    Trie* trie = new Trie();
    while(getline(file, line)){
        trie->add(line);
    };
    corpora.insert(make_pair(s, trie));
    return true;
};

Trie* Corpora::get(string s) {
    return NULL;
};

/*
    Main entry point

    Requires:
    - First argument: root of corpora files
*/
int main(int argc, char* argv[]) {

    Corpora* corpora = new Corpora();

    for(int i = 0; i < argc; i++) {
        /* root path for corpora files */
        if (strncmp(argv[i], "-r", 2) == 0) {
            i++;
            cout << "Initializing with root " << argv[i] << endl;
            corpora->root(argv[i]); 
        /* Help: show how to use */
        } else if (strncmp(argv[i], "-h", 2) == 0) {
            cout << "\nCorpora v" << VERSION << "\n" << endl;
            cout << "Options:" << endl;
            cout << "    -h : Show help" << endl;
            cout << "    -r : Set root path for corpus files" << endl;
            cout << "\nCreated for Minethat in 2014\n" << endl;
            exit(0);
        }
    }
    


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