#include <bits/stdc++.h>
using namespace std;

#define SYS_IDENT   10086
#define SYS_NUMBER  10087

map<string, int> MAP;
FILE *out;
const vector<string>SYS_STRING={
    "const","end","procedure","if","then","while","do","begin","call","var","odd","+","-","*","/","(",")","=",",",".",";",">=","<=","==","{","}",":=","<>"
};
const vector<int>SYS_NUM={
    1,2,3,4,5,6,7,8,9,10,11,12,13 ,14 ,15 ,16,17,18,19,20,21,22,23,24,25,26,27,28
};
void CreateMap()
{
    for(int i=0;i<SYS_STRING.size();i++)
    {
        MAP[SYS_STRING[i]]=SYS_NUM[i];
    }
}
void write(int num, string x)
{
    printf("(%-5d,%-10s)\n", num, x.c_str());
    fprintf(out, "(%-5d,%-10s)\n", num, x.c_str());
}
inline string tr(char s)
{
    return string(1, s);
}
void compile(FILE *FilePtr)
{
    int num = 0;
    string name;
    char CharGot;
    CharGot = fgetc(FilePtr);
    while (CharGot != EOF)
    {
        while (CharGot == '\n' || CharGot == '\t' || CharGot == '\r' || CharGot == ' ')
        {
            CharGot = fgetc(FilePtr);
        }
        if (isdigit(CharGot))
        {
            while (isdigit(CharGot))
            {
                num = num * 10 + CharGot - '0';
                CharGot = fgetc(FilePtr);
            }

            write(SYS_NUMBER, string(to_string(num)));
            /*
                对数字num进行处理
            */
            num = 0;
        }
        else if (isalpha(CharGot))
        {
            string name = tr(CharGot);
            CharGot = fgetc(FilePtr);
            while (isdigit(CharGot) || isalpha(CharGot))
            {
                name += CharGot;
                CharGot = fgetc(FilePtr);
            }
            if (MAP.find(name) != MAP.end())
            {
                write(MAP[name], name); // 如果是关键字
            }
            else
            {
                write(SYS_IDENT, name); //  如果是声明变量
            }
        }
        else
        {
            if (CharGot == ':')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '=')
                {
                    write(MAP[":="], ":=");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    printf("\":= \" error!\n");
                }
            }
            else if (CharGot == '+' || CharGot == '-' || CharGot == '*' || CharGot == '/' || CharGot == '(' || CharGot == ')' || CharGot == ',' || CharGot == '.' || CharGot == ';')
            {
                write(MAP[tr(CharGot)], tr(CharGot));
                CharGot = fgetc(FilePtr);
            }
            else if (CharGot == '<')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '>')
                {
                    write(MAP["<>"], "<>");
                    CharGot = fgetc(FilePtr);
                }
                else if (CharGot == '=')
                {
                    write(MAP["<="], "<=");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    write(MAP["<"], "<");
                }
            }
            else if (CharGot == '>')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '=')
                {
                    write(MAP[">="], ">=");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    write(MAP[">"], ">");
                }
            }
            else if (CharGot == '=')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '=')
                {
                    write(MAP["=="], "==");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    write(MAP["="], "=");
                }
            }
            else if (CharGot == '{')
            {
                while (CharGot != '}')
                    CharGot = fgetc(FilePtr);
                CharGot = fgetc(FilePtr);
            }
        }
    }
}
int main()
{
    CreateMap();

    FILE *ptr = fopen("test.pl", "r");
    out = fopen("out.pl", "w");
 
    compile(ptr);
    return 0;
}