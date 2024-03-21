#include <fstream>
#include <iostream>

#include <fst.hpp>
#include <vector>
#include <algorithm>
#include <sstream>

std::vector<std::string> read_string_data(std::string fname, size_t n) {
    std::vector<std::string> vec;
    vec.reserve(n);

    std::fstream file;
    file.open(fname, std::ios::in);

    for (size_t i=0; i<n; i++) {
        std::string line;
        if (!std::getline(file, line, '\n')) break;

        std::stringstream ls(line);
        std::string field;

        std::getline(ls, field, '\t');
        std::getline(ls, field, '\n');
        vec.push_back(std::string(field));
    }

    std::sort(vec.begin(), vec.end());

    return vec;
}

int main() {
        /*
    std::vector<std::string> keys = {
        "ACML",  "AISTATS", "DS",    "DSAA",   "ICDM",   "ICML",  //
        "PAKDD", "SDM",     "SIGIR", "SIGKDD", "SIGMOD",
    };
        */

    auto keys = read_string_data("kjv-wordlist.txt", 10000);
    // a trie-index constructed from string keys sorted
    fst::Trie trie(keys);

    // keys are mapped to unique integers in the range [0,#keys)
    std::cout << "[searching]" << std::endl;
    for (size_t i = 0; i < keys.size(); ++i) {
        fst::position_t key_id = trie.exactSearch(keys[i]);
        std::cout << " - " << keys[i] << "(" << i << "): " << key_id << std::endl;
    }

    std::cout << "[statistics]" << std::endl;
    std::cout << " - number of keys: " << trie.getNumKeys() << std::endl;
    std::cout << " - number of nodes: " << trie.getNumNodes() << std::endl;
    std::cout << " - number of suffix bytes: " << trie.getSuffixBytes() << std::endl;
    std::cout << " - memory usage in bytes: " << trie.getMemoryUsage() << std::endl;
    std::cout << " - output file size in bytes: " << trie.getSizeIO() << std::endl;

    std::cout << "[configure]" << std::endl;
    trie.debugPrint(std::cout);

    // write the trie-index to a file
    {
        std::ofstream ofs("fst.idx");
        trie.save(ofs);
    }

    // read the trie-index from a file
    {
        fst::Trie other;
        std::ifstream ifs("fst.idx");
        other.load(ifs);
    }

    std::remove("fst.idx");
    return 0;
}
