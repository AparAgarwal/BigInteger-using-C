#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInteger.h"

struct BigInteger initialize(char *str)
{
    struct BigInteger b;
    b.head = b.tail = NULL;
    b.size = 0;
    b.sign = 1;
    int len = strlen(str);
    if (len > 0 && str[0] == '-')
    {
        b.sign = -1;
        str++; // Move the pointer past the '-' sign
        len--; // Decrement the length to exclude the '-' sign
    }
    if (len > 0 && str[len - 1] == '\n')
    {
        len--; // Decrement the length to exclude the newline character
    }
    for (int i = 0; i < len; i++)
    {
        int digit = str[i] - '0'; // Convert character to integer
        addAtEnd(&b, digit);      // Add the digit to the big integer
    }
    return b;
}

void freeBigInteger(struct BigInteger *b)
{
    digit *current = b->head;
    while (current != NULL)
    {
        digit *temp = current;
        current = current->next;
        free(temp);
    }
}

void addAtEnd(struct BigInteger *b, int val)
{
    digit *newNode = (digit *)malloc(sizeof(digit));
    newNode->data = val;
    newNode->next = NULL;

    if (b->tail == NULL)
    {
        b->head = b->tail = newNode;
        newNode->prev = NULL;
    }
    else
    {
        newNode->prev = b->tail;
        b->tail->next = newNode;
        b->tail = newNode;
    }
    b->size++;
}

void addAtBeginning(struct BigInteger *b, int val)
{
    digit *newDigit = (digit *)malloc(sizeof(digit));
    newDigit->data = val;
    newDigit->prev = NULL;
    newDigit->next = b->head;
    if (b->head == NULL)
    {
        b->head = b->tail = newDigit;
        b->size++;
        return;
    }
    b->size++;
    b->head->prev = newDigit;
    b->head = newDigit;
}

// Function to remove leading zeros e.g. 00123 --> 123
void removeZeros(struct BigInteger *b)
{
    if (b->head == NULL)
    {
        return;
    }
    while (b->head->data == 0 && b->head->next != NULL)
    {
        digit *temp = b->head;
        b->head = b->head->next;
        b->head->prev = NULL;
        free(temp);
        b->size--;
    }
}

// Compairing Big Integers (returns -1 for b2, 1 for b1 and 0 for equal)
int compareBigINT(struct BigInteger b1, struct BigInteger b2)
{
    if (b1.size < b2.size)
        return -1;
    if (b1.size > b2.size)
        return 1;
    digit *curr1 = b1.head;
    digit *curr2 = b2.head;
    while (curr1 != NULL)
    {
        if (curr1->data < curr2->data)
            return -1;
        if (curr1->data > curr2->data)
            return 1;
        curr1 = curr1->next;
        curr2 = curr2->next;
    }
    return 0;
}

// Display
void display(struct BigInteger b)
{
    struct BigInteger zero = initialize("0");
    if (compareBigINT(b, zero) == 0)
    {
        // if b = 0 then remove - sign (if any)
        b.sign = 1;
    }
    freeBigInteger(&zero);
    digit *current = b.head;
    if (current == NULL)
    {
        printf("NULL\n");
        return;
    }
    if (b.sign == -1)
    {
        printf("-");
    }
    while (current != NULL)
    {
        printf("%d", current->data);
        current = current->next;
    }
    printf("\n");
}

// Addition
struct BigInteger add(struct BigInteger b1, struct BigInteger b2)
{
    struct BigInteger result;
    result.head = result.tail = NULL;
    result.size = 0;
    result.sign = 1;
    if (b1.sign == 1 && b2.sign == -1)
    {
        // If b1 is positive and b2 is negative, we perform subtraction by changing the sign
        // b1 + (-b2)  =  b1 - b2
        b2.sign = 1;
        return sub(b1, b2);
    }
    else if (b1.sign == -1 && b2.sign == 1)
    {
        // If b1 is negative and b2 is positive, we perform subtraction by changing the sign
        // (-b1) + b2  =  b2 - b1
        b1.sign = 1;
        return sub(b2, b1);
    }
    else
    {
        // Otherwise, both numbers have the same sign (either positive or negative)
        int carry = 0;
        digit *current1 = b1.tail;
        digit *current2 = b2.tail;

        while (current1 || current2 || carry)
        {
            int val1 = (current1) ? current1->data : 0; // current1 != NULL
            int val2 = (current2) ? current2->data : 0; // current2 != NULL
            int sum = val1 + val2 + carry;
            carry = sum / 10;
            sum = sum % 10;
            addAtBeginning(&result, sum);

            if (current1)
                current1 = current1->prev;
            if (current2)
                current2 = current2->prev;
        }

        result.sign = b1.sign; // Set the sign of the result to match b1 or b2
    }

    return result;
}

// Subtraction
struct BigInteger sub(struct BigInteger b1, struct BigInteger b2)
{
    struct BigInteger result;
    result.head = result.tail = NULL;
    result.size = 0;
    result.sign = 1;
    if (b1.sign == 1 && b2.sign == -1)
    {
        // If b1 is positive and b2 is negative, we perform addition by changing the sign of b2
        b2.sign = 1;
        return add(b1, b2);
    }
    else if (b1.sign == -1 && b2.sign == 1)
    {
        // If b1 is negative and b2 is positive, we perform addition by changing the sign of b2
        b2.sign = -1;
        return add(b1, b2);
    }
    else
    {
        // Both numbers have the same sign
        int swap = 1;
        if (compareBigINT(b1, b2) == -1)
        {
            // If b2 is larger, we swap b1 and b2 and adjust the sign
            struct BigInteger temp = b1;
            b1 = b2;
            b2 = temp;
            swap = -1;
        }

        int borrow = 0;
        digit *current1 = b1.tail;
        digit *current2 = b2.tail;

        while (current1 || current2)
        {
            int val1 = (current1) ? current1->data : 0;
            int val2 = (current2) ? current2->data : 0;
            int diff = val1 - val2 - borrow;

            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }

            addAtBeginning(&result, diff);

            if (current1)
                current1 = current1->prev;
            if (current2)
                current2 = current2->prev;
        }

        // Adjust the sign based on the swap
        result.sign = swap * b1.sign;
    }
    // removing any leading zeroes in result E.g. 00111 --> 111
    removeZeros(&result);
    return result;
}

// Multiplication
struct BigInteger mul(struct BigInteger b1, struct BigInteger b2)
{
    struct BigInteger result;
    result.head = result.tail = NULL;
    result.size = 0;

    struct BigInteger temp;

    int count = 0;
    while (b2.tail != NULL)
    {
        int carry = 0;
        temp.head = temp.tail = NULL;
        temp.size = 0;
        temp.sign = 1;
        for (int i = 0; i < count; i++)
        {
            addAtEnd(&temp, 0);
        }
        struct BigInteger ptr = b1;
        while (ptr.tail != NULL)
        {
            int product = (ptr.tail->data * b2.tail->data + carry);
            carry = product / 10;
            int digit = product % 10;
            addAtBeginning(&temp, digit);
            ptr.tail = ptr.tail->prev;
        }
        if (carry != 0)
        {
            addAtBeginning(&temp, carry);
        }
        result = add(result, temp);
        count++;
        b2.tail = b2.tail->prev;
    }
    removeZeros(&result);
    result.sign = (b1.sign == b2.sign) ? 1 : -1; // Determine the sign of the result
    return result;
}

struct BigInteger divideBy2(struct BigInteger num)
{
    struct BigInteger result;
    result.head = result.tail = NULL;
    result.size = 0;
    result.sign = num.sign;
    int carry = 0;
    digit *current = num.head;
    while (current)
    {
        int value = current->data + carry * 10;
        int quotient = value / 2;
        carry = value % 2;
        addAtEnd(&result, quotient);
        current = current->next;
    }
    // Remove leading zeros in the result
    removeZeros(&result);
    return result;
}

struct BigInteger div1(struct BigInteger b1, struct BigInteger b2)
{
    struct BigInteger quotient = initialize("0");
    struct BigInteger one = initialize("1");
    struct BigInteger null = initialize("");

    int sign = (b1.sign == b2.sign) ? 1 : -1;
    b1.sign = b2.sign = 1;
    if (compareBigINT(b1, b2) == -1)
    {
        return quotient;
    }
    else if (compareBigINT(b1, b2) == 0)
    {
        one.sign = sign;
        return one;
    }
    else if (b2.size == 1 && b2.head->data == 0)
    {
        return null;
    }
    else if (b2.size == 1 && b2.head->data == 1)
    {
        b1.sign = sign;
        return b1;
    }
    quotient = add(quotient, one);
    struct BigInteger dividend = b1;
    struct BigInteger divisor = b2;
    struct BigInteger multiple = divisor;
    // Exponential Search ---->
    while (compareBigINT(dividend, multiple) == 1 || compareBigINT(dividend, multiple) == 0)
    {
        multiple = add(multiple, multiple); // Exponentially increase the multiple
        quotient = add(quotient, quotient); // Double the quotient
    }
    struct BigInteger result = initialize("0");
    while (compareBigINT(dividend, divisor) == 1 || compareBigINT(dividend, divisor) == 0)
    {
        if (compareBigINT(dividend, multiple) == 1 || compareBigINT(dividend, multiple) == 0)
        {
            dividend = sub(dividend, multiple);
            result = add(result, quotient);
        }
        multiple = divideBy2(multiple); // Divide the multiple by 2
        quotient = divideBy2(quotient); // Divide the quotient by 2
    }
    result.sign = sign;
    freeBigInteger(&multiple);
    freeBigInteger(&quotient);
    freeBigInteger(&one);
    return result;
}

// Modulus
struct BigInteger mod(struct BigInteger b1, struct BigInteger b2)
{
    int sign = b1.sign;
    // Remove signs for calculation
    b1.sign = b2.sign = 1;
    struct BigInteger quotient = div1(b1, b2);
    struct BigInteger multiple = mul(b2, quotient);
    struct BigInteger remainder = sub(b1, multiple);
    // Restore the sign of b1 to match the original b1
    remainder.sign = sign;
    return remainder;
}
