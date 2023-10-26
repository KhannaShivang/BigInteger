#ifndef BigInteger_h
#define BigInteger_h
struct Node{
    int data;
    struct Node* next;
};

struct BigInteger{
    struct Node* head;
    int negative;
};
struct BigInteger initialize(char []);
struct BigInteger add(struct BigInteger,struct BigInteger);
struct BigInteger sub(struct BigInteger,struct BigInteger);
struct BigInteger mul(struct BigInteger,struct BigInteger);
struct BigInteger div1(struct BigInteger,struct BigInteger);
struct BigInteger mod(struct BigInteger,struct BigInteger);
struct BigInteger display(struct BigInteger);
#endif
