#include<iostream>
#include "compiler.h"
// tree
struct node
{
    std::string name;
    std::vector<node*>son;
    std::string data;
    node(std::string Name="",std::vector<node*>Son=std::vector<node*>()):name(Name),son(Son){}

};


std::string Naming(std::string a,int& cnt,std::string data="")
{
    cnt+=1;
    return a+std::to_string(cnt-1)+"((\""+data+"\"))";
}
node* Top;

bool fail;//      代表是否出错
int varcnt,factorcnt,expcnt,constcnt,bodycnt,lexpcnt,statementcnt,blockcnt,Scnt,fucnt,termcnt;


std::ofstream err("Error",std::ios::out);// 输出错误信息
std::ofstream tree("Tree",std::ios::out);

std::map<std::string ,int>Words;
std::map<std::string ,int>ConstWords;



void var1(int&i,node* father);


void factor(int&i,node* father);

void Exp(int&i,node* father);

void term(int&i,node* father);

void vardeclation(int& i,node* father);

void constdeclation(int& i,node* father);

void body(int &i,node* father);

void lexp(int&i,node* father);

void statement(int&i,node* father);

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
    std::cerr<<"Error\n";
}

void Exp(int& i,node* father)
{
    if(Tokens[i].id == SYS_add ||Tokens[i].id == SYS_sub )
    {
        node* son=new node(Naming("fu",fucnt,Tokens[i].name));
        father->son.push_back(son);    
        gettoken(i);
        node* son1=new node(Naming("term",termcnt,"term"));
        father->son.push_back(son1);    
        term(i,son1);
    }
    else 
    {
        node* son1=new node(Naming("term",termcnt,"term"));
        father->son.push_back(son1);
        term(i,son1);
    }
    while(Tokens[i].id == SYS_add ||Tokens[i].id == SYS_sub)
    {
        node* son1=new node(Naming("fu",fucnt,Tokens[i].name));
        father->son.push_back(son1);
        gettoken(i);
        
        node* son2=new node(Naming("term",termcnt,"term"));
        father->son.push_back(son2);

        term(i,son2);
    }
}

void term(int&i,node* father)
{
    node* son=new node(Naming("factor",factorcnt,"factor"));
    father->son.push_back(son);    
    factor(i,son);
    while(Tokens[i].id == SYS_mul ||Tokens[i].id == SYS_div )
    {
        node* son1=new node(Naming("fu",fucnt,Tokens[i].name));
        father->son.push_back(son1);
        gettoken(i);
        node* son2=new node(Naming("factor",factorcnt,"factor"));
        father->son.push_back(son2);
        factor(i,son2);
    }
}

void factor(int&i,node* father)
{
    if(Tokens[i].id == SYS_IDENT)
    {
        node* son1=new node(Naming("ident",fucnt,Tokens[i].name));
        father->son.push_back(son1);
        gettoken(i);
    }
    else if(Tokens[i].id == SYS_NUMBER)
    {
        node* son1=new node(Naming("number",fucnt,Tokens[i].name));
        father->son.push_back(son1);
        gettoken(i);
    }
    else if(Tokens[i].id == SYS_LBRA)
    {
        node* son1=new node(Naming("LBRA",fucnt,Tokens[i].name));
        father->son.push_back(son1);
        gettoken(i);
        node* son2=new node(Naming("exp",expcnt,"exp"));
        father->son.push_back(son2);
        Exp(i,son2);
        if(Tokens[i].id != SYS_RBRA)
        {
            error("缺少右括号!");
        }
        node* son3=new node(Naming("fu",fucnt,")"));
        father->son.push_back(son3);
        gettoken(i);
    }
}

void body(int &i,node* father)
{
    if(Tokens[i].id == SYS_BEGIN)
    {
        
        node* son=new node(Naming("begin",fucnt,"begin"));
        father->son.push_back(son);
        gettoken(i);
        node* ___son=new node(Naming("statement",statementcnt,"statement"));
        father->son.push_back(___son);
        statement(i,___son);
        bool ED=0;
        while(Tokens[i].id == SYS_fen)
        {
            node* __son=new node(Naming("fen",fucnt,";"));
            father->son.push_back(__son);
            gettoken(i);
            if(Tokens[i].id == SYS_END)
            {
                node* _son =new node(Naming("end",fucnt,"end"));
                father->son.push_back(_son);
                break;
            }
            node* _son3=new node(Naming("statement",statementcnt,"statement"));
            father->son.push_back(_son3);
            statement(i,_son3);
        }
      
    }
    else error("缺少begin!");

}



void vardeclation(int& i,node* father)
{
    if(Words.find(Tokens[i].name)==Words.end())
    {
        node* son=new node(Naming("ident",fucnt,Tokens[i].name));
        father->son.push_back(son);
        // Words[Tokens[i].name]=0;
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
        node* son=new node(Naming("ODD",fucnt,"ODD"));
        father->son.push_back(son);
        gettoken(i);
        node* __son=new node(Naming("exp",expcnt,"exp"));
        father->son.push_back(__son);
        Exp(i,__son);
    }
    else 
    {
        node* __son=new node(Naming("exp",expcnt,"exp"));
        father->son.push_back(__son);
        Exp(i,__son);
        if(Tokens[i].id == SYS_dengdeng 
        || Tokens[i].id == SYS_dadeng 
        || Tokens[i].id == SYS_xiaodeng
        || Tokens[i].id == SYS_jiankuohao)
        {
                
            node* son1=new node(Naming("lexp",lexpcnt,Tokens[i].name));
            father->son.push_back(son1);
            node* son2=new node(Naming("exp",expcnt,"exp"));
            father->son.push_back(son2);
            gettoken(i);
            Exp(i,son2);
        }
    }
}

void constdeclation(int& i,node* father)
{
    if(Tokens[i].id == SYS_IDENT)
    {
        node* son=new node(Naming("ident",fucnt,Tokens[i].name));
        father->son.push_back(son);
        ConstWords[Tokens[i].name]=0;// 这里可能还有赋值的操作就先不写了
        gettoken(i);

        if(Tokens[i].id==SYS_fuzhi)
        {
            node* _son=new node(Naming("fuzhi",fucnt,":="));
            father->son.push_back(_son);
            gettoken(i);
            if(Tokens[i].id == SYS_IDENT)
            {
                node* __son=new node(Naming("ident",fucnt,Tokens[i].name));
                father->son.push_back(__son);
                gettoken(i);
            }
            else if(Tokens[i].id == SYS_NUMBER)
            {
                node* __son=new node(Naming("number",fucnt,Tokens[i].name));
                father->son.push_back(__son);
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
            node* son =new node(Naming("const",constcnt,"const"));
            node* son1 =new node(Naming("const",constcnt,"const"));
            son->son.push_back(son1);
            node* nowson=son;

            father->son.push_back(son);
            int k=father->son.size()-1;
            constdeclation(i,nowson);
            while(Tokens[i].id == SYS_dou)
            {
                node* son2=new node(Naming("const",constcnt,"const"));
                father->son[k]=son2;
                son2->son.push_back(nowson);
                node* son3=new node(Naming("fu",fucnt,","));
                nowson->son.push_back(son3);
                nowson=son2;
                gettoken(i);
                constdeclation(i,nowson);
            }
            if(Tokens[i].id == SYS_fen)
            {
                node* son=new node(Naming("fen",fucnt,";"));
                father->son.push_back(son);
                gettoken(i);
            }
            else error("缺少;符号!");
        }
        else if(Tokens[i].id == SYS_VAR)
        {
            gettoken(i);
            node* son =new node(Naming("var",varcnt,"var"));
            node* son1 =new node(Naming("var",varcnt,"var"));
            son->son.push_back(son1);
            node* nowson=son;

            father->son.push_back(son);
            int k=father->son.size()-1;
            vardeclation(i,nowson);
            while(Tokens[i].id == SYS_dou)
            {
                node* son2=new node(Naming("var",varcnt,"var"));
                father->son[k]=son2;
                son2->son.push_back(nowson);
                node* son3=new node(Naming("fu",fucnt,","));
                nowson->son.push_back(son3);
                nowson=son2;
                gettoken(i);
                vardeclation(i,nowson);
            }
             if(Tokens[i].id == SYS_fen)
            {
                node* son=new node(Naming("fen",fucnt,";"));
                father->son.push_back(son);
                gettoken(i);
            }
            else error("缺少;符号!");
        }
        else if(Tokens[i].id == SYS_BEGIN)
        {
            node* son=new node(Naming("body",bodycnt,"body"));
            father->son.push_back(son);
            body(i,son);
            if(Tokens[i].id != SYS_END)
            {
                error("缺少end!!");
            }
            gettoken(i);
        }
        else if(Tokens[i].id == SYS_PROCEDURE)
        {
            node* son=new node(Naming("procedure",fucnt,"procedure"));
            father->son.push_back(son);
            gettoken(i);
            if(Tokens[i].id == SYS_IDENT)
            {
                node* son1=new node(Naming("ident",fucnt,Tokens[i].name));
                father->son.push_back(son1);
                gettoken(i);
                if(Tokens[i].id == SYS_fen)
                {
                    node* son2=new node(Naming("fen",fucnt,";"));
                    father->son.push_back(son2);
                    gettoken(i);
                    node* son3=new node(Naming("block",blockcnt,"block"));
                    father->son.push_back(son3);
                    block(i,son3);
                }
                else error("缺少分号!");
            }
            else error("缺少标识符!");
            
        }
        else if(Tokens[i].id == SYS_dot)
        {
            node* son3=new node(Naming("dot",fucnt,"."));
            father->son.push_back(son3);
            break;
        }
        else error("未定义符号!");
    }
}

void statement(int& i,node* father)
{
    if(Tokens[i].id == SYS_IDENT)
    {
        node* son=new node(Naming("ident",fucnt,Tokens[i].name));
        father->son.push_back(son);
        gettoken(i);
        if(Tokens[i].id == SYS_fuzhi)
        {
            node* son0=new node(Naming("fuzhi",fucnt,":="));
            father->son.push_back(son0);    
            gettoken(i);
            node* son1=new node(Naming("exp",expcnt,"exp"));
            father->son.push_back(son1);    
            Exp(i,son1);
        }
        else error("缺少赋值符号!");
    }
    else if(Tokens[i].id == SYS_IF)
    {
        node* son=new node(Naming("if",fucnt,"if"));
        father->son.push_back(son);    
        gettoken(i);
        node* son1=new node(Naming("lexp",lexpcnt,"lexp"));
        father->son.push_back(son1);    
        lexp(i,son1);
        if(Tokens[i].id == SYS_THEN)
        {
            node* son2=new node(Naming("then",fucnt,"then"));
            father->son.push_back(son2);    
            gettoken(i);
            node* son3=new node(Naming("statement",statementcnt,"statement"));
            father->son.push_back(son3);    
            statement(i,son3);
        }
        else error("缺少then!");
    }
    else if(Tokens[i].id == SYS_BEGIN)
    {
        node* son=new node(Naming("body",bodycnt,"body"));
        father->son.push_back(son);    
        body(i,son);
        if(Tokens[i].id != SYS_END)
        {
            error("缺少end符号!!");
        }
        gettoken(i);
    }
    else if(Tokens[i].id == SYS_WHILE)
    {
        node* son=new node(Naming("while",fucnt,"while"));
        father->son.push_back(son);    
        gettoken(i);
        node* son1=new node(Naming("lexp",lexpcnt,"lexp"));
        father->son.push_back(son1);    
        lexp(i,son1);
        if(Tokens[i].id == SYS_DO)
        {
            node* son2=new node(Naming("do",fucnt,"do"));
            father->son.push_back(son2);    
            gettoken(i);
            node* son3=new node(Naming("statement",statementcnt,"statement"));
            father->son.push_back(son3);    
            statement(i,son3);
        }
        else error("缺少do!");
    }
    else if(Tokens[i].id == SYS_CALL)
    {
        node* son=new node(Naming("call",fucnt,"call"));
        father->son.push_back(son);    
        gettoken(i);
        if(Tokens[i].id == SYS_IDENT)
        {
            node* son=new node(Naming("ident",fucnt,Tokens[i].name));
            father->son.push_back(son);    
            gettoken(i);
        }
        else error("缺少标识符!");
    }
    else if(Tokens[i].id == SYS_fen)
    {
        return ;
    }
    else error("识别到未定义符号!");
}



void processtokens()
{
    int i=0;
    // Tree("<S>","<block>");
    Top=new node(Naming("S",Scnt,"S"));
    
    node* Block=new node(Naming("block",blockcnt,"block"));
    Top->son.push_back(Block);
    block(i,Block);
    if(fail)
    {
        error("编译失败!");
    }
}

void query(node* top)
{
    for(int i=0;i<top->son.size();i++)
    {
        tree<<top->name<<"---->"<<top->son[i]->name<<'\n';
        query(top->son[i]);
    }
}


int main()
{
    CreateMap();

    in =fopen("in","r");
    out =fopen("out","w");

    compile(in);
    
    fclose(in);
    fclose(out);
    
    in=fopen("out","r");
    trans_token(in);
    processtokens();
    query(Top);
    fclose(in);

    return 0;
}

