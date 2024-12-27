#include "trie.h"
#include "user.h"
#include <iostream>
int main() {
    // Create a Trie of strings
    Trie<std::string> trie;

    // Insert some keys into the Trie
    trie.insert("xyz123");
    trie.insert("abc123");
    trie.insert("defabcghi");
    trie.insert("abc1");
    trie.insert("axcf");
    trie.insert("act");
    trie.insert("abc");
    trie.insert("abcmnop");
    trie.insert("abcxyz");
    trie.insert("ab");
    trie.insert("pqrst");
    trie.insert("xyz");
    trie.insert("ahifaifabc");
    

    // Perform wildcard searches
    std::cout << "Wildcard Pattern: **abc*\n";
    std::vector<std::string*> results1;
    trie.wildcardSearch("**abc*", results1);
    for (const auto& result : results1) {
        std::cout << *result << std::endl;
    }

   /* std::cout << "\nWildcard Pattern: a*c?\n";
    std::vector<std::string*> results2;
    trie.wildcardSearch("a*c?", results2);
    for (const auto& result : results2) {
        std::cout << *result << std::endl;
    }

    std::cout << "\nWildcard Pattern: a?c*\n";
    std::vector<std::string*> results3;
    trie.wildcardSearch("a?c*", results3);
    for (const auto& result : results3) {
        std::cout << *result << std::endl;
    }
*/
    return 0;
}
