#include<iostream>
#include<fstream>
#include<stack>
#include "compiler.h"
// 储存错误信息的文件

bool fail=0;//      代表是否出错



std::ofstream err("Error",std::ios::out);// 输出错误信息
std::ofstream tree("Tree",std::ios::out);

std::map<std::string ,int>Words;
std::map<std::string ,int>ConstWords;




std::stack<int>num;
std::stack<char>stk;
void var1(int&i);


void factor(int&i);

void Exp(int&i);

void term(int&i);

void vardeclation(int& i);

void constdeclation(int& i);

void body(int &i);

void lexp(int&i);

void statement(int&i);

inline void gettoken(int &i)
{
    i++;
}   


inline void Tree(std::string a,std::string b)
{
    tree<< a<<"-->"<<b<<'\n';
}

void error(std::string x)
{
    fail=1;
    err<<x<<'\n';
}

void Exp(int& i,node* father)
{
    if(Tokens[i].id == SYS_add ||Tokens[i].id == SYS_sub )
    {
        Tree("<Exp>",Tokens[i].name);
        gettoken(i);
        term(i);
    }
    else 
    {
        Tree("<Exp>","<term>");
        term(i);
    }
    while(Tokens[i].id == SYS_add ||Tokens[i].id == SYS_sub)
    {
        Tree("<Exp>","<"+Tokens[i].name+">");
        gettoken(i);
        Tree("<Exp>","<term>");

        term(i);
    }
}

void term(int&i,node* father)
{
    Tree("<term>","<factor>");
    factor(i);
    while(Tokens[i].id == SYS_mul ||Tokens[i].id == SYS_div )
    {
        Tree("<term>",Tokens[i].name);
        gettoken(i);
        Tree("term","<factor>");
        factor(i);
    }
}

void factor(int&i,node* father)
{
    if(Tokens[i].id == SYS_IDENT)
    {
        Tree("<factor>","<ident>");
        gettoken(i);
    }
    else if(Tokens[i].id == SYS_NUMBER)
    {
        Tree("<factor>","<number>");
        gettoken(i);
    }
    else if(Tokens[i].id == SYS_LBRA)
    {
        Tree("<factor>","<(>");
        gettoken(i);
        Tree("<factor>","<exp>");
        Exp(i);
        if(Tokens[i].id != SYS_END)
        {
            error("缺少右括号!");
        }
        gettoken(i);
    }
}

void body(int &i,node* father)
{
    if(Tokens[i].id == SYS_BEGIN)
    {
        gettoken(i);
        Tree("<body>","<begin>");
        Tree("<body>","<statement>");
        statement(i);
        while(Tokens[i].id == SYS_fen)
        {
            gettoken(i);
            Tree("<body>","<;>");
            Tree("<body>","<statement>");
            statement(i);
        }
        if(Tokens[i].id == SYS_END)
        {
            Tree("<body>","<end>");
            gettoken(i);
        }
        else error("缺少end!");
    }
    else error("缺少begin!");

}



void vardeclation(int& i,node* father)
{
    if(Words.find(Tokens[i].name)==Words.end())
    {
        Tree("<var>","<ident>");
        Words[Tokens[i].name]=0;
        gettoken(i);
    }
    else{
        error(std::string("\"")+Tokens[i].name+'\"'+"redefined!!!");
        fail=true;
    }
}

void lexp(int&i,node* father)
{
    if(Tokens[i].id == SYS_ODD)
    {
        gettoken(i);
        Tree("<lexp>","<odd>");
        Exp(i);
    }
    else 
    {
        Tree("<lexp>","<Exp>");
        Exp(i);
        if(Tokens[i].id == SYS_dengdeng 
        || Tokens[i].id == SYS_dadeng 
        || Tokens[i].id == SYS_xiaodeng
        || Tokens[i].id == SYS_jiankuohao)
        {
            Tree("<lexp>","<"+Tokens[i].name+">");
            gettoken(i);
        }
    }
}
//      const id := number ;
void constdeclation(int& i,node* father)
{
    if(Tokens[i].id == SYS_IDENT)
    {
        Tree("<const>","<ident>");
        ConstWords[Tokens[i].name]=0;// 这里可能还有赋值的操作就先不写了
        gettoken(i);

        if(Tokens[i].id==SYS_fuzhi)
        {
            Tree("<const>","<:=>");
            gettoken(i);
            if(Tokens[i].id == SYS_IDENT)
            {
                Tree("<const>","<ident>");
                gettoken(i);
            }
            else if(Tokens[i].id == SYS_NUMBER)
            {
                Tree("<const>","<number>");
                gettoken(i);
            }
            else
            {
                error("Tokens"+std::to_string(i+1)+"后缺少标识符");
            }
        }
        else 
        {
            error("Error! Expected an equal sign.");
        }
    }
    else 
    {
        error("Compilation error: an identifier token was expected.");
    }
}




void block(int& i,node* father)
{

    while(i<Tokens.size()&&!fail)
    {
        if(Tokens[i].id == SYS_CONST)
        {
            gettoken(i);
            Tree("<block>","<const>");
            constdeclation(i);
            while(Tokens[i].id == SYS_dou)
            {
                Tree("<const>","<,>");
                gettoken(i);
                Tree("<block>","<const>");
                constdeclation(i);
            }
        }
        else if(Tokens[i].id == SYS_VAR)
        {
            Tree("<block>","<var>");
            gettoken(i);
            vardeclation(i);
            while(Tokens[i].id == SYS_dou)
            {
                Tree("<var>","<,>");
                gettoken(i);
                Tree("<block>","<var>");
                vardeclation(i);
            }
        }
        else if(Tokens[i].id == SYS_fen)
        {
            Tree("<block>","<;>");
            gettoken(i);
        }
        else if(Tokens[i].id == SYS_BEGIN)
        {
            Tree("<block>","<body>");
            body(i);
        }
        else if(Tokens[i].id == SYS_PROCEDURE)
        {
            Tree("<block>","<procedure>");
            gettoken(i);
            if(Tokens[i].id == SYS_IDENT)
            {
                Tree("<block>","<ident>");
                gettoken(i);
                if(Tokens[i].id == SYS_fen)
                {
                    Tree("<block>","<;>");
                    gettoken(i);
                    Tree("<block>","<block>");
                    block(i);
                    gettoken(i);
                    if(Tokens[i].id == SYS_END)
                    {
                    Tree("<block>","<end>");
                        gettoken(i);
                    }
                    else error("缺少end");
                    
                }
                else error("缺少分号!");
            }
            else error("缺少标识符!");
            
        }
        else if(Tokens[i].id == SYS_END)
        {
            Tree("<block>","<end>");
            gettoken(i);
            if(Tokens[i].id == SYS_dot)
            {
                Tree("<block>","<dot>");
                break;
            }
            else error("缺少\".\"号");
        }
        else if(Tokens[i].id == SYS_dot)
        {
            Tree("<block>","<dot>");
            gettoken(i);
            break;
        }
        else error("未定义符号!");
    }
    if(i>=Tokens.size())
    {
        error("缺少end");
    }
}

void statement(int& i,node* father)
{
    if(Tokens[i].id == SYS_IDENT)
    {
        Tree("<statement>","<ident>");
        gettoken(i);
        if(Tokens[i].id == SYS_fuzhi)
        {
            Tree("<statement>","<:=>");
            gettoken(i);
            Tree("<statement>","<Exp>");
            Exp(i);
        }
        else error("缺少赋值符号!");
    }
    else if(Tokens[i].id == SYS_IF)
    {
        Tree("<statement>","<if>");
        gettoken(i);
        Tree("<statement>","<lexp>");
        lexp(i);
        if(Tokens[i].id == SYS_THEN)
        {
            Tree("<statement>","<then>");
            gettoken(i);
            Tree("<statement>","<statement>");
            statement(i);
        }
        else error("缺少then!");
    }
    else if(Tokens[i].id == SYS_WHILE)
    {
        Tree("<statement>","<while>");
        gettoken(i);
        Tree("<statement>","<lexp>");
        lexp(i);
        if(Tokens[i].id == SYS_DO)
        {
            Tree("<statement>","<do>");
            gettoken(i);
            Tree("<statement>","<statement>");
            statement(i);
        }
    }
    else if(Tokens[i].id == SYS_CALL)
    {
        Tree("<statement>","<call>");
        gettoken(i);
        if(Tokens[i].id == SYS_IDENT)
        {
            Tree("<statement>","<call>");
            gettoken(i);
        }
        else error("缺少标识符!");
    }
    else if(Tokens[i].id == SYS_fen)
    {
        Tree("<statement>","<;>");
        return ;
    }
}



void processtokens()
{
    int i=0;
    Tree("<S>","<block>");
    block(i);
    if(fail)
    {
        error("编译失败!");
    }
}



