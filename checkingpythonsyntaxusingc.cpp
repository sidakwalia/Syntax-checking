#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//assignment statements
//if
//else
//elif
//"if" before "elif" and "else"
//for loop
//while loop
//print statement
//expression check
//colon after loop
//variable use before initializing
//parentheses open and close
//double quotes open and close
//no operator on left hand side of =

char keywords[33][10] = {"False", "None", "True", "and", "as", "assert", "break", "class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"};
char operators[] = {"+-*/%^"};
char assignment_operators[6][3] = {"=", "-=", "+=", "/=", "*=", "%="};
char initialized_ids[][30] = {"dummy"};
int no_of_ids = 0;
int no_of_ifs = 0; //Number of if statements; incremented on encountering "if", decremented on encountering "else".

bool isKeyword(char ch[])
{
    int i;
    for(i=0; i<33; i++)
        if(strcmp(keywords[i], ch) == 0)
            return true;
    return false;
}

bool isOperator(char ch)
{
    int i;
    for(i=0; i<6; i++)
        if(ch == operators[i])
            return true;
    return false;
}

bool isInitialized(char ch[])
{
    if(no_of_ids == 0)
        return false;
    for(int i=0; i<no_of_ids; i++)
        if(strcmp(initialized_ids[i], ch) == 0)
            return true;
    return false;
}

bool contains(char line[], char ch[])
{
    if(strstr(line, ch) == NULL)
        return false;
    else
        return true;
}

int isNumeric(char ch[])
{
    // -1 means not numeric
    // 1 means numeric
    // 0 means more than 1 decimal

    int no_of_decimal = 0;
    for(int i=0; i<strlen(ch); i++)
    {
        if(ch[i] == '.' && no_of_decimal < 2)
        {
            no_of_decimal++;
            if(no_of_decimal == 2)
                return 0;
        }
        else if(ch[i]<48 || ch[i]>57)
            return -1;
    }
    return 1;
}

bool parentheses(char line[])
{
    int i, opened = 0;
    for(i=0; i<strlen(line); i++)
    {
        if(line[i] == '(')
            opened++;
        else if(line[i] == ')')
            opened--;
    }
    if(opened == 0)
        return true;
    return false;
}

bool doubleQuotes(char line[])
{
    int i, opened = 0;
    for(i=0; i<strlen(line); i++)
    {
        if(line[i] == '"')
            opened++;
    }
    if(opened%2 == 0)
        return true;
    return false;
}

int assignment(char line[], int line_number)
{
    int i, number_ids_left = 0, j = 0, index, k, m;
    char buffer[20], buffer1[20], ch;

    //Identifying the index of "=" operator
    for(i=0; i<strlen(line); i++)
        if(line[i] == '=')
        {
            index = i;
            break;
        }

    //If any operator appears before "=" and the next character is not "=", then there is an error.
    for(i=0; i<index-1; i++)
        if(isOperator(line[i]))
        {
            printf("SyntaxError in line %d: can't assign to operator\n", line_number);
            return -1;
        }

    i = index;
    for(; i<strlen(line); i++)
    {
        ch = line[i];
   		if(isalnum(ch) || ch=='_')
   			buffer[j++] = ch;

   		else if((ch == ' ' || ch == '\n' || isOperator(ch)) && (j != 0))
        {
   			buffer[j] = '\0';
   			j = 0;

            if(isKeyword(buffer))
            {
                printf("Error in line %d: Didn't expect keyword %s\n", line_number, buffer);
                return -1;
            }
            else
            {
                int temp = isNumeric(buffer);
                if(temp == 0)
                {
                    printf("SyntaxError in line %d: invalid syntax\n", line_number);
                    return -1;
                }
                if(temp == -1)
                {
                    if(!isInitialized(buffer))
                    {
                        printf("Error in line %d: name '%s' is not defined\n", line_number, buffer);
                        return -1;
                    }
                    else //When RHS is not a numeric and the identifier in RHS is also being initialized/declared already, we need to store the LHS identifier in the initialized_ids array
                    {
                        m = 0;
                        for(k = 0; k<index; k++)
                        {
                            ch = line[k];
                            if(isalnum(ch) || ch=='_')
                                buffer1[m++] = ch;
                            else if((ch == ' ' || ch == '\n' || isOperator(ch)) && (m != 0))
                            {
                                buffer1[m] = '\0';
                                m = 0;
                                break;
                            }
                        }
                        if(isKeyword(buffer1))
                        {
                            printf("Error in line %d: Didn't expect keyword %s\n", line_number, buffer);
                            return -1;
                        }
                        else
                        {
                            if(!isInitialized(buffer1))
                            {
                                strcpy(initialized_ids[no_of_ids], buffer1);
                                no_of_ids++;
                            }
                        }
                    }
                }
                else  //When RHS is numeric, we need to store the LHS identifier in the initialized_ids array
                {
                    m = 0;
                    for(k = 0; k<index; k++)
                    {
                        ch = line[k];
                        if(isalnum(ch) || ch=='_')
                            buffer1[m++] = ch;
                        else if((ch == ' ' || ch == '\n' || isOperator(ch)) && (m != 0))
                        {
                            buffer1[m] = '\0';
                            m = 0;
                            break;
                        }
                    }
                    if(isKeyword(buffer1))
                    {
                        printf("Error in line %d: Didn't expect keyword %s\n", line_number, buffer);
                        return -1;
                    }
                    else
                    {
                        if(!isInitialized(buffer1))
                        {
                            strcpy(initialized_ids[no_of_ids], buffer1);
                            no_of_ids++;
                        }
                    }
                }
            }
   		}
    }
    return 0;
}

int ifStatement(char line[], int line_number)
{
    char buffer[30] = "if";
    int i=0, j=0, flag=0;

    while(line[i] == ' ')
        i++;
    for(; j<2; i++, j++)
    {
        if(buffer[j] != line[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("Error in line %d: invalid syntax\n", line_number);
        return -1;
    }

    if(line[i]!=' ' && line[i]!='(')
    {
        printf("Error in line %d: invalid syntax\n", line_number);
        return -1;
    }

    j = 0;
    for(; i<strlen(line); i++)
        buffer[j++] = line[i];
    if(contains(buffer, "<") || contains(buffer, ">") || contains(buffer, "==") || contains(buffer, "<=") || contains(buffer, ">=") || contains(buffer, "!=") || contains(buffer, "in"))
        no_of_ifs++;
    else
    {
        printf("Error in line %d: no condition after if\n", line_number);
        return -1;
    }
    if(line[strlen(line)-2] != ':')
    {
        printf("Error in line %d: expected ':' at the end\n", line_number);
        return -1;
    }
    return 0;
}

int elifStatement(char line[], int line_number)
{
    if(no_of_ifs == 0)
    {
        printf("SyntaxError in line %d: no 'if' statement before 'elif'\n", line_number);
        return -1;
    }
    char buffer[30] = "elif";
    int i=0, j=0, flag=0;

    while(line[i] == ' ')
        i++;
    for(; j<4; i++, j++)
    {
        if(buffer[j] != line[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    if(line[i]!=' ' && line[i]!='(')
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }

    j = 0;
    for(; i<strlen(line); i++)
        buffer[j++] = line[i];

    if(!(contains(buffer, "<") || contains(buffer, ">") || contains(buffer, "==") || contains(buffer, "<=") || contains(buffer, ">=") || contains(buffer, "!=") || contains(buffer, "in")))
    {
        printf("SyntaxError in line %d: no condition after elif\n", line_number);
        return -1;
    }
    if(line[strlen(line)-2] != ':')
    {
        printf("SyntaxError in line %d: expected ':' at the end\n", line_number);
        return -1;
    }
    return 0;
}

int elseStatement(char line[], int line_number)
{
    if(no_of_ifs == 0)
    {
        printf("SyntaxError in line %d: no 'if' statement before 'else'\n", line_number);
        return -1;
    }
    no_of_ifs--;
    char buffer[] = "else";
    int i=0, j=0, flag=0;

    while(line[i] == ' ')
        i++;
    for(; j<4; i++, j++)
    {
        if(buffer[j] != line[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    if(line[strlen(line)-2] != ':')
    {
        printf("SyntaxError in line %d: expected ':' at the end\n", line_number);
        return -1;
    }
    while(line[i] == ' ')
        i++;
    if(line[i]!=':' || i!=(strlen(line)-2))
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    return 0;
}

int forStatement(char line[], int line_number)
{
    char buffer[] = "for";
    int i=0, j=0, flag=0;

    while(line[i] == ' ')
        i++;
    for(; j<3; i++, j++)
    {
        if(buffer[j] != line[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    if(line[i]!=' ')
    {
        printf("Error in line %d: invalid syntax\n", line_number);
        return -1;
    }
    if(line[strlen(line)-2] != ':')
    {
        printf("SyntaxError in line %d: expected ':' at the end\n", line_number);
        return -1;
    }
    return 0;
}

int whileStatement(char line[], int line_number)
{
    char buffer[] = "while";
    int i=0, j=0, flag=0;

    while(line[i] == ' ')
        i++;
    for(; j<5; i++, j++)
    {
        if(buffer[j] != line[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    if(line[i]!=' ' && line[i]!='(')
    {
        printf("Error in line %d: invalid syntax\n", line_number);
        return -1;
    }
    j = 0;
    for(; i<strlen(line); i++)
        buffer[j++] = line[i];

    if(!(contains(buffer, "<") || contains(buffer, ">") || contains(buffer, "==") || contains(buffer, "<=") || contains(buffer, ">=") || contains(buffer, "!=")))
    {
        printf("Error in line %d: no condition after while\n", line_number);
        return -1;
    }
    if(line[strlen(line)-2] != ':')
    {
        printf("SyntaxError in line %d: expected ':' at the end\n", line_number);
        return -1;
    }
    return 0;
}

int printStatement(char line[], int line_number)
{
    char buffer[] = "print";
    int i=0, j=0, flag=0;

    while(line[i] == ' ')
        i++;
    for(; j<5; i++, j++)
    {
        if(buffer[j] != line[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    while(line[i] == ' ')
        i++;
    if(line[i] != '(')
    {
        printf("SyntaxError in line %d: invalid syntax\n", line_number);
        return -1;
    }
    return 0;
}

int typeOfStatement(char line[])
{
    //print statement
    if(contains(line, "print"))
        return 7;

    //assignment statement
    if(contains(line, "="))
        if(!contains(line, "=="))
            return 1;

    //elif statement
    if(contains(line, "elif"))
        return 4;

    //if statement
    if(contains(line, "if"))
        return 2;

    //else statement
    if(contains(line, "else"))
        return 3;

    //for loop statement
    if(contains(line, "for"))
        return 5;

    //while loop statement
    if(contains(line, "while"))
        return 6;

    return -1;
}

int main()
{
    FILE *fptr = fopen("python_program.txt", "r");
    char line[1000];
    int x, line_number = 0, y;

    if(fptr == NULL)
    {
        printf("Error while opening the file!");
        exit(1);
    }

    while (fgets(line, sizeof(line), fptr))
    {
        line_number++;
        x = typeOfStatement(line);
        switch(x)
        {
            case 1: y = assignment(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            case 2: y = ifStatement(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            case 3: y = elseStatement(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            case 4: y = elifStatement(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            case 5: y = forStatement(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            case 6: y = whileStatement(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            case 7: y = printStatement(line, line_number);
                    if(y == -1)
                        exit(0);
                    break;
            default: printf("Line %d: Unrecognized statement\n", line_number);
                     exit(0);
        }
        if(!parentheses(line))
        {
            printf("SyntaxError in line %d: invalid syntax\n", line_number);
            exit(0);
        }
        if(!doubleQuotes(line))
        {
            printf("SyntaxError in line %d: invalid syntax\n", line_number);
            exit(0);
        }

        //printf("Line %d: %s", line_number, line);
    }
    printf("No syntax errors in the program!\n");

    fclose(fptr);
    return 0;
}