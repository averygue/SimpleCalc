/*
* Avery, OS 2018
* 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


#define BUFSIZE 100
#define binary 2
#define hex 16
#define octal 8
#define decimal 10

char buf[BUFSIZE];
int bufp = 0;
int total = 0;
//personal method to check for spaces

int myspace(char c) {
    if(c == '\n') {
        return 0;
    }
    else {
        return isspace(c);
    }
}
//methods by C Programming Book for getch and ungetch
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

//checks to see if character is operand
int isOperand(char c) {
    char d;
   
     if(c == '\0') {
         printf("ERROR: unknown operator 0xFFFFFFFF\n");
        exit(EXIT_FAILURE);
    }
   
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '&' || c == '|' || c == '%') {
        return 1;
    }
    else if(c == ' ') {
        printf("ERROR: unknown operator 0xFFFFFFFF\n");
        exit(EXIT_FAILURE);
    }
    else if(c == '<' ) {
        d = getch();
       
        if(d != '<') {
            printf("ERROR: expected number.\n");
            exit(EXIT_FAILURE);
        }
        else {
            return 1;
        }
    } 
    else if(c == '>' ) {
        d = getch();
        if(d != '>') {
            printf("ERROR: expected number.\n");
            exit(EXIT_FAILURE);
        }
        else {
            return 1;
        }
    }
    else
        return 0;
    
}



//getOperand uses a similar method to getInt to return the Operand
char getOperand() {
    char c;
    while(myspace(c = getch())) 
        ;
    if(c == '\n')
        return 0;
    if(c == 0) 
        return 0;
    if(isdigit(c)) {
            printf("ERROR: unknown operator 0xFFFFFFFF\n");
            exit(EXIT_FAILURE);
    }
    if(c == '\0')
        return 0;
    if(isdigit(c) && c!= EOF && !isOperand(c)) {
        ungetch(c);
        return 0;
    }
    else if(isOperand(c)) {
        return c;}
    else if (c != '\n') 
        ungetch(c);
    else if (c != EOF)
        ungetch(c);
    else 
        return 0;
}

//determination determines the base
int determination(char c) {
    int nextChar;
    int nextNextChar;
    int nextNextNextChar;
    nextChar = c;
    if(nextChar == EOF) 
        exit(EXIT_FAILURE);				
    nextNextChar = getch();
    
    if(nextChar == '0')
    {
        if(nextNextChar == 'x') {
            nextNextNextChar = getch();
            if(nextNextNextChar == '\n') {
                  printf("ERROR: expected number.\n");;
                exit(EXIT_FAILURE); }
                
            ungetch(nextNextNextChar);   
            
            return hex;
        }
        else if(nextNextChar == 'b') {
            nextNextNextChar = getch();
            if(nextNextNextChar == '\n') {
                 printf("ERROR: expected number.\n");
                exit(EXIT_FAILURE); }
            ungetch(nextNextNextChar);    
            return binary;
        }
        else {

            ungetch(nextNextChar);
            return octal;   
        }
    }
    if(nextChar < '0' || nextChar > '9') {
        printf("ERROR: expected number.\n");
        exit(EXIT_FAILURE);
    }
       
        ungetch(nextNextChar);
        return decimal;
        
}
int mydigit(char c) {
    if(isalpha(c)) {
        printf("ERROR: expected number.\n");
        exit(EXIT_FAILURE);
    }
    else {
        return isdigit(c);
    }
}

//get int and skip whitespace
int getint(int *pn) {
    int c;
    
    
    while(isspace(c = getch())) 
        ;
    
    int base = determination(c);
    if(base == 0)
        return 0; 
    if(base == hex) {
        c = getch();
        for(*pn = 0; isdigit(c) || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ; c = getch()) {
            if(isdigit(c)) 
                *pn = base * *pn + (c - '0');
            else if((c >= 'A' && c <= 'F'))
                *pn = base * *pn + (c - 55);
            else if((c >= 'a' && c <= 'f')) {
                *pn = base * *pn + (c - 87);
            }
            else {
                 printf("ERROR: expected number.\n");
                exit(EXIT_FAILURE);
            }
        }
        ungetch(c);
        return c;
    }
    if(base == octal) {
        c = getch();
        for(*pn = 0; isdigit(c); c = getch()) {
            if(c < '0' || c >'7') {
                printf("ERROR: unknown operator 0xFFFFFFFF\n");
                exit(EXIT_FAILURE);
            }
            *pn = base * *pn + (c - '0');
    }  
            ungetch(c);
            return c;
    }
    
    if(base == binary) {
        if(!isdigit(c) && c!= EOF) {
        ungetch(c);
        return 0;
        }
        for(*pn = 0; isdigit(c); c = getch()) {
            *pn = base * *pn + (c - '0');
              if(c < '0' || c >'1') {
                printf("ERROR: unknown operator 0xFFFFFFFF\n");
                exit(EXIT_FAILURE);
            }
        }
        if (c != EOF)
            ungetch(c);
        
       
        return c;
    }

     if(!isdigit(c) && c!= EOF) {
        ungetch(c);
        return 0;
    }
    
    for(*pn = 0; mydigit(c); c = getch()) {
        *pn = base * *pn + (c - '0');
        
    }
    if (c != EOF)
        ungetch(c);
    
   
    return c;
}

//final step of get int, get op, get int, etc
int combination() {
    
    int total = 0;
    int total2 = 0;
    getint(&total);
    char c = getOperand();
    if(c == '\n' || c == 0)
        return total;
    
    while(c != '\n') {
        getint(&total2);
       
        switch(c) {
            case '+':
                total = total + total2;
                break;
            case '-':
                total = total - total2;
                break;
            case '*' :
                total = total * total2;
                break;
            case '/' :
                if(total2 == 0) {
                    printf("ERROR: division by zero.\n");
                    exit(EXIT_FAILURE);
                }
                total = total / total2;
                break;
            case '>':
                total  = total >> total2;
                break;
            case'<':
                total = total << total2;
                break;
            case '|':
                total = (total | total2);
                break;
            case '&':
                total = (total & total2);
                break;
	    case '%':
		total = total % total2;
		break;
            case '^':
                total = (int)pow(total,total2);
		break;

	}
        c = getOperand();
        if(c == '\n' || c == 0)
            return total;
    }
    return total;
}
void main()
{
    printf("\n=== Calculator ===\n");
    char c = getch();
    while(c != EOF) {
        ungetch(c);
        printf("= %d\n",  combination());
        c = getch();
    }
    
}

