#ifndef BIGINTEGER_H
#define BIGINTEGER_H

struct digit_node
{
    int data;
    struct digit_node *prev, *next;
};

typedef struct digit_node digit;

struct BigInteger {
    digit* head, *tail;
    int size, sign;
};
void removeZeros(struct BigInteger* b);
struct BigInteger initialize(char *str);
void freeBigInteger(struct BigInteger* b);
void addAtBeginning(struct BigInteger *b, int val);
void addAtEnd(struct BigInteger* b, int val);
int compareBigINT(struct BigInteger b1, struct BigInteger b2);
struct BigInteger divideBy2(struct BigInteger num);
void display(struct BigInteger b);
struct BigInteger add(struct BigInteger b1, struct BigInteger b2);
struct BigInteger sub(struct BigInteger b1, struct BigInteger b2);
struct BigInteger mul(struct BigInteger b1, struct BigInteger b2);
struct BigInteger div1(struct BigInteger b1, struct BigInteger b2);
struct BigInteger mod(struct BigInteger b1, struct BigInteger b2);
struct BigInteger inputBigINT();

#endif
