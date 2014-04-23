Corpora
=======

C++ application for fast search in multiple huge text corpora, 
using one [Trie](http://en.wikipedia.org/wiki/Trie) per corpus.

Offers a socket API in order to submit queries and get a response.

Corpus files are loaded in memory the first time they're required. If you load huge corpus files (more than 1 million elements) then the socket request may timeout before the file is entirely loaded.
In this case two strategies:
- Extend timeout in client
- Make a dummy query at initialization of your app so corpora preloads files

## Run

```
./corpora [-r root-path] [-p socket-port] [-h socket-host]
```

Options:
- `-r [root-path]` Root path of the corpus files
- `-h [socket-host]` Socket host
- `-p [socket-port]` Socket port
- `-v` Activate verbose mode (display various benchmarks)


## Querying

To query the service, you have to send a list of corpuses you want to check 
(without the `.txt` extension), along with the word or expression to send, 
separated with a space.

For example, to search term `Hello` in corpus `[root]/en/some_corpus.txt`:
```
en/some_corpus Hello
```

### Multiple corpus files query

If you need to search many corpus files, separate them with a comma `,`:

```
en/some_corpus,en/other Hello
```

will try to load both `[root]/en/some_corpus.txt` 
and `[root]/en/other.txt` corpus files.


## Error handling

Corpora gracefully handles errors, and trying to search an unexisting corpus won't block current or subsequent queries.


## Responses

Response is formatted with a "header" that is the amount of corpus 
where the keyword was found, followed by the list of corpus files.

```
[length],[corpus_file],[other_corpus_file]
```

So sending `en/some_corpus,en/other Hello` may return:
- `0` if no result has been found
- `1,en/some_corpus` if a result has been found in `some_corpus` file
- `2,en/some_corpus,en/other` if a result has been found in both corpus files.

## Dictionaries

Dictionary files must follow this strict:

```
[root]
    |
    |- [language]
    |   |
    |   |- [corpus-name].txt
    |   |- [corpus-name].txt
    |
    |- [language]
    |   |
    |   |- [corpus-name].txt
    |   |- [corpus-name].txt
```

Per convention, language folders are two code letters of the language: `en`, `fr`... and
exceptionally 5 letters codes for specific languages subsets: `zh-tw`.

In dictionaries, you should have only one term or expression per line:

```
some
word
another
one
and
so
on
```

## Build

The make command should work on most UNIX platforms:

```
make
```

### Install tests

Tests relies on node.js and mocha. Install node and npm if you need to run tests.

```
cd test && npm install .
```

### Run tests

```
# Start corpora with test dataset root
./corpora -r test/corpus/

# Then in another terminal, run test suite
cd test && mocha
```

# Examples of use

### Example of querying corpora using core Node.js net connections

```js
var net = require("net"),
    socket;

// Create a socket connection
socket = net.createConnection(1234, "localhost", function (){

socket.on ('data', function (d){
    var res = d.toString();
    console.log(res);
    // 2,en/names,en/places
});

socket.write('en/words,en/names,en/places,en/companies paris');
```