#include <iostream>
#include "nfa.hpp"

// this file is for testing purposes; it will be discarded during grading

using namespace std;

int main(){     
    NFA M("dfa1.inp");
    cout << "0--" << endl;
    cout << M;

    cout << endl << "1--" << endl;
    M.process("aabba");
    
    cout << endl << "2--" << endl;
    M.process("aabbba");
    
    cout << endl << "3--" << endl;
    M.process("abc");
    

    NFA M2 = M + M;
    cout << endl << "4--" << endl;
    cout << M2;

    NFA M3(!M);
    cout << endl << "5--" << endl;
    M3.process("aabba");
    
/* output:
0--
a b
q0 q1
q0
q0
q0 a q0
q0 b q1
q1 a q1
q1 b q0

1--
(q0, aabba) :- (q0, abba) :- (q0, bba) :- (q1, ba) :- (q0, a) :- (q0, e)
Accept

2--
(q0, aabbba) :- (q0, abbba) :- (q0, bbba) :- (q1, bba) :- (q0, ba) :- (q1, a) :- (q1, e)
Reject

3--
Invalid string

4--
a b
q0 q1 qq0 qq1 s
s
q0 qq0
qq0 a qq0
qq0 b qq1
qq1 a qq1
qq1 b qq0
q0 a q0
q0 b q1
q1 a q1
q1 b q0
s e qq0
s e q0

5--
(q0, aabba) :- (q0, abba) :- (q0, bba) :- (q1, ba) :- (q0, a) :- (q0, e)
Reject
*/
    return 0;
}