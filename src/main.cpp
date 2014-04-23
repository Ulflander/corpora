#include <iostream>
#include <fstream>

// For various tree and maps uses
#include <vector>
#include <map>

// Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

// Benchmark
#include <sys/time.h>
#include <ctime>

// Some utilities functions
#include "utils.hpp"

/* Main namespace */
using namespace std;


int sock_port = 1234;
string sock_host = "localhost";

/* Tried to "define" it like SOCK_HOST
   but can't "cout" it anymore then. */
string VERSION = "0.1";
bool verbose = false;




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
    int i;

    while (curr != NULL) {

        for (i = 0; i < l; i++) {
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
            cRoot = "";
        }
        void root(string r) {
            cRoot = r;
        }
        bool load(string &s);
        Trie* get(string &s);
        bool has(string &s);

    private:
        string cRoot;
        std::map<string, Trie*> corpora;
};

/* Check if a corpus exists */
bool Corpora::has(string &s) {
    return corpora.count(s) > 0;
};

/* Load a corpus */
bool Corpora::load(string &s) {
    int t0 = ulf::get_time();

    ifstream file(cRoot + s + ".txt");
    if (!file) {
        ulf::warning("Corpora cant load file " + cRoot + s + ".txt");
        return false;
    }

    string line;
    Trie* trie = new Trie();
    int n = 0;
    while(getline(file, line)){
        trie->add(line);
        n ++;
    };

    if (verbose) {
        int t1 = ulf::get_time();
        ulf::debug("Loaded " + ulf::to_string(n) + " keywords from " + s + " in " + 
            ulf::to_string(t1 - t0) + "ms");
    }
    corpora.insert(make_pair(s, trie));
    return true;
};

/* Get a corpus */
Trie* Corpora::get(string &s) {
    return corpora.find(s)->second;
};

/* Process a query string */
string process_query(Corpora* corpora, string s) {

    string response = "";

    // First separate corpus list from keyword queried
    string::size_type loc = s.find(" ", 0);
    if( loc == string::npos ) {
        ulf::warning("Separator not found in query " + s);
        return "0";
    }
    string corpus_files = s.substr(0, loc);
    string keyword = s.substr(loc + 1, s.length());
    vector<string> files;


    // Get list of corpuses
    ulf::split(files, corpus_files, ',');
    
    if (files.size() == 0) {
        ulf::warning("No file found");
        return "0";
    }

    int found = 0;
    // And finally query each corpus
    for(vector<int>::size_type i = 0; i != files.size(); i++) {

        // Check if exists or load it
        if (!corpora->has(files[i]) && !corpora->load(files[i])) {
            ulf::warning("Unable to load corpus " + files[i]);
            continue;
        }

        int t0 = ulf::get_microtime();
        // Search and append in response if found
        if (corpora->get(files[i])->search(keyword)) {
            response += "," + files[i];
            found += 1;

            if (verbose) {
                ulf::debug(keyword + " found in corpus '" + files[i]
                    + "' in " + ulf::to_string(ulf::get_microtime() - t0) + " microsecs");
            }
        }
    }

    if (found > 0) {
        return ulf::to_string(found) + response;
    }
    return "0";
}


/* Setup socket server and wait for connections */
void setup_server(Corpora* corpora) {
    /* Prepare address structre and other variables */
    int socket_reusable=1;
    struct sockaddr_in serv_addr;
    int sockfd;

    // clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_port = htons(sock_port);

    /* Create socket */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ulf::error("Socket not created");
    }

    /* Sometimes we can't restart the server immediately if it was stopped,
        the port is retained for a few seconds. Allow reusing address let restart
        corpora immediately */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &socket_reusable,
            sizeof(int)) == -1) {
        ulf::error("Setting socket option SO_REUSEADDR failed");
    }

    /* Bind socket */
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) {
        ulf::error("Socket not binded");
    }

    /* Listen ! Keep 10 connections waiting before refusing connections */
    listen(sockfd, 10);

    if (verbose) {
        ulf::debug("Server started on " + sock_host + ":" + to_string(sock_port));
    }

    /* And loop over requests :) */
    for(;;) {
        socklen_t clilen;
        struct sockaddr_in cli_addr;

        /* Yes! A new message! */
        int newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            ulf::warning("Error occured while accepting a message. Transaction aborted.");
            continue;
        }

        char buffer[256];
        int n;
        /* Let's receive it*/
        /* TODO: accept messages longer than 254*/
        n = recv(newsockfd, buffer, 254, 0);
        if (n < 0) {
            ulf::warning("Error occured while reading a message. Transaction aborted.");
            return;
        }

        /* Delimiter */
        buffer[n] = '\0';

        /* Process the query */
        string r = process_query(corpora, string(buffer));

        /* Send answer */
        send(newsockfd, r.c_str(), r.size(), 0);

        /* And close socket! */
        close(newsockfd);
    }

    // Never reached but looks cleaner ?
    close(sockfd);
}

/*
    Main entry point

    Options:
    - First argument: root of corpora files
*/
int main(int argc, char* argv[]) {

    Corpora* corpora = new Corpora();

    /* Check arguments */
    for(int i = 0; i < argc; i++) {
        /* verbose mode */
        if (strncmp(argv[i], "-v", 2) == 0) {
            verbose = true;
        /* root path for corpora files */
        } else if (strncmp(argv[i], "-r", 2) == 0) {
            i++;
            cout << "Initializing with root " << argv[i] << endl;
            corpora->root(argv[i]); 
        /* Port */
        } else if (strncmp(argv[i], "-p", 2) == 0) {
            i++;
            sock_port = ulf::to_int(argv[i]);
        /* Host */
        } else if (strncmp(argv[i], "-h", 2) == 0) {
            i++;
            sock_host = argv[i];
        /* Help: show how to use */
        } else if (strncmp(argv[i], "--help", 2) == 0) {
            cout << "\nCorpora v" << VERSION << "\n" << endl;
            cout << "Options:" << endl;
            cout << "  --help         Show help and exit" << endl;
            cout << "  -r [root]      Set root path for corpus files" << endl;
            cout << "  -p [port]      Socket port" << endl;
            cout << "  -h [host]      Socket host" << endl;
            cout << "  -v             Activate verbose mode" << endl;
            cout << "\nCreated for Minethat in 2014\n" << endl;
            exit(0);
        }
    }

    setup_server(corpora);
}