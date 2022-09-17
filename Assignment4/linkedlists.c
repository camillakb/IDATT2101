#include <stdio.h>
#include <stdlib.h>

//Method to find which number is the longest (has most digits).
int max(int a, int b) {
    return a > b ? a : b;
}

//Structure for Digit.
typedef struct Digit {
    char value;
    struct Digit* next;
    struct Digit* previous;
} Digit;

//Structure for Number.
typedef struct {
    Digit* first;
    Digit* last;
    int length;
} Number;

//Structure for Iterator.
typedef struct {
    Digit* position;
} Iterator;

void putLast(Number* number, char value);
int done(Iterator* iter);
void next(Iterator* iter);
Iterator* newHeadIter(Number* number);

//Method to create a new number.
Number* newNumber() {
    Number* number = (Number*)malloc(sizeof(Number));

    number->first = NULL;
    number->last = NULL;
    number->length = 0;

    return number;
}

//Method to create a number using a string.
Number* fromString(char* string) {
    Number* number = newNumber();
    int temp;

    while(1) {
        if (*string == 0) {
            break;
        }

        temp = *string - '0';

        if (0 <= temp && temp <= 9) {
            putLast(number, temp);
        } else {
            printf("Please enter valid number.\n");
            exit(1);
        }

        string++;
    }

    return number;
}

//Method to print a number.
void printNumber(Number* number, int width) {
    Iterator* iter = newHeadIter(number);
    printf("%*s", width - number->length, "");

    while (!done(iter)) {
        printf("%d", iter->position->value);
        next(iter);
    }
}

//Method to create a new digit.
Digit* newDigit(char value, Digit* next, Digit* previous) {
    Digit* res = (Digit*) (malloc(sizeof (Digit)));

    res->value = value; 
    res->next = next;
    res->previous = previous;

    return res;
}

//Method to put a digit first in a number.
void putFirst(Number* number, char value) {
    Digit* digit = newDigit(value, number->first, NULL);

    number->first = digit;

    if(!number->last) {
        number->last = digit;
    } else {
        digit->next->previous = digit;
    }

    number->length++;
}

//Method to put a digit last in a number.
void putLast(Number* number, char value) {
    Digit* digit = newDigit(value, NULL, number->last);

    if (number->last) {
        number->last->next = digit;
    } else { 
        number->first = digit;
    }

    number->last = digit;
    number->length++;
}

//Method to remove a digit.
Digit* removeDigit(Number* number, Digit* digit) {
    if (digit->previous) {
        digit->previous->next = digit->next;
    } else { 
        number->first = digit->next;
    }

    if (digit->next) {
        digit->next->previous = digit->previous;
    } else {
        number->last = digit->previous;
    }

    digit->next = NULL;
    digit->previous = NULL;
    number->length--;
    
    return digit;
}

//Method to check if we have iterated through the entire number.
int done(Iterator* iter) {
    return !iter->position;
}

//Method to see the next digit in a number.
void next(Iterator* iter) {
    if (!done(iter)) {
        iter->position = iter->position->next;
    }
}

//Method to see the previous digit in a number.
void previous(Iterator* iter) {
    if (!done(iter)) {
        iter->position = iter->position->previous;
    }
}

//Nethod to iterate through a number from the start of the number.
Iterator* newHeadIter(Number* number) {
    Iterator* res = (Iterator*)malloc(sizeof(Iterator));

    res->position = number->first;

    return res;
}

//Method to iterate through a number from the end of the number.
Iterator* newTailIter(Number* number) {
    Iterator* res = (Iterator*)malloc(sizeof(Iterator));

    res->position = number->last;

    return res;
}

//Method to add two numbers.
//lhs = number on the left hand side, rhs = number on the right hand side.
Number* add(Number* lhs, Number* rhs) {
    //To store the resulting number.
    Number* sum = newNumber();

    //Initializing iterators for each number (linked list).
    Iterator* lhs_iter = newTailIter(lhs);
    Iterator* rhs_iter = newTailIter(rhs);

    //The digits in the numbers.
    Digit* lhs_digit;
    Digit* rhs_digit;

    int valuesum;
    int carry = 0;

    //While-loop to add the numbers.
    while (1) {
        valuesum = 0;

        //If the left hand side number is not done, find the current digit and add it to valuesum.
        if (!done(lhs_iter)) {
            lhs_digit = lhs_iter->position;
            valuesum += lhs_digit->value;
        }
        
        //If the right hand side number is not done, find the current digit and add it to valuesum.
        if (!done(rhs_iter)) {
            rhs_digit = rhs_iter->position;
            valuesum += rhs_digit->value;
        }
        
        //If both numbers are done, and there is no carry value, break the loop.
        if (done(lhs_iter) && done(rhs_iter) && !carry) {
            break;
        }

        valuesum += carry;
        carry = valuesum / 10;
        
        putFirst(sum, (valuesum % 10));

        previous(lhs_iter);
        previous(rhs_iter);
    }

    return sum;
}

//Method to subtract.
//lhs = number on the left hand side, rhs = number on the right hand side.
Number* subtract(Number* lhs, Number* rhs) {
    Number* difference = newNumber();

    Iterator* lhs_iter = newTailIter(lhs);
    Iterator* rhs_iter = newTailIter(rhs);

    Digit* lhs_digit;
    Digit* rhs_digit;

    int valuediff;
    int borrow = 0;

    while (1) {
        valuediff = 0;

        //If we are not done iterating through the number on the left hand side, add the current digit to the difference value
        //at the current position.
        if (!done(lhs_iter)) {
            lhs_digit = lhs_iter->position;
            valuediff = lhs_digit->value - borrow;

            //If we are not done iterating through the number on the right hand side, subtract the current digit from the 
            //difference value at the current position.
            if (!done(rhs_iter)) {
                rhs_digit = rhs_iter->position;
                valuediff -= rhs_digit->value;
            }

            //If the digit at the current position is a negative value, borrow 10 from the next number and subtract 1 from that
            //same number.
            if (valuediff < 0) {
                valuediff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
        }

        //If both numbers are done, break the loop.
        if (done(lhs_iter) && done(rhs_iter)) {
            break;
        }

        putFirst(difference, valuediff);

        previous(lhs_iter);
        previous(rhs_iter);
    }

    return difference;
}

int main(int argc, char const *argv[]) {
    Number* lhs = fromString(argv[1]);
    Number* rhs = fromString(argv[3]);
    Number* res;

    //Switch statement to see if its a subtraction or an addition.
    switch (*argv[2]) {
    case '+':
        res = add(lhs, rhs);
        break;

    case '-':
        res = subtract(lhs, rhs);
        break;

    default:
        printf("Unknown operator '%c', please use + or - only.", *argv[2]);
    }

    int width = max(res->length, max(lhs->length, rhs->length));

    printf("  ");
    printNumber(lhs, width);
    printf("\n%c ", *argv[2]);
    printNumber(rhs, width);
    printf("\n= ");
    printNumber(res, width);
    printf("\n");

    return 0;
}
