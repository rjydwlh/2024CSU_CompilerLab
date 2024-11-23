
std::map<std::string, int> O_to_S;


FILE *out;
FILE *in;

std::vector<std::string>SYS_STRING={
    "const","end","procedure","if","then","while","do",
    "begin","call","var","odd","+","-","*","/","(",")",
    "=",",",".",";",">","<","==","{","}",":=","<>","ident","number"
};
std::map<std::string,int>INT;//        记录声明的常量

// std::map<std::string,>Cal;// 循环指令



char ERRORSET[][100]={
    ""
};

std::vector<int>SYS_NUM={
    1,2,3,4,5,6,7,8,9,10,11,12,13 ,14 
    ,15 ,16,17,18,19,20,21,22,23,24,25,
    26,27,28,29,30
};

void CreateMap()
{
    for(int i=0;i<SYS_STRING.size();i++)
    {
        O_to_S[SYS_STRING[i]]=SYS_NUM[i];
    }
}

void write(int num, std::string x)
{
    printf("(%-5d,%-10s)\n", num, x.c_str());
    fprintf(out, "(%-5d,%-10s)\n", num, x.c_str());
}

inline std::string tr(char s)
{
    return std::string(1, s);
}


void compile(FILE *FilePtr)
{
    int num = 0;
    std::string name;
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

            write(SYS_NUMBER, std::string(std::to_string(num)));
            /*
                对数字num进行处理
            */
            num = 0;
        }
        else if (isalpha(CharGot))
        {
            std::string name = tr(CharGot);
            CharGot = fgetc(FilePtr);
            while (isdigit(CharGot) || isalpha(CharGot))
            {
                name += CharGot;
                CharGot = fgetc(FilePtr);
            }
            if (O_to_S.find(name) != O_to_S.end())
            {
                write(O_to_S[name], name); // 如果是关键字
            }
            else
            {
                write(SYS_IDENT, name); //  如果是声明变量
                // words.emplace_back(name);
            }
        }
        else
        {
            if (CharGot == ':')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '=')
                {
                    write(O_to_S[":="], ":=");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    printf("\":= \" error!\n");
                }
            }
            else if (CharGot == '+' || CharGot == '-' || CharGot == '*' || CharGot == '/' || CharGot == '(' || CharGot == ')' || CharGot == ',' || CharGot == '.' || CharGot == ';')
            {
                write(O_to_S[tr(CharGot)], tr(CharGot));
                CharGot = fgetc(FilePtr);
            }
            else if (CharGot == '<')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '>')
                {
                    write(O_to_S["<>"], "<>");
                    CharGot = fgetc(FilePtr);
                }
                else if (CharGot == '=')
                {
                    write(O_to_S["<="], "<=");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    write(O_to_S["<"], "<");
                }
            }
            else if (CharGot == '>')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '=')
                {
                    write(O_to_S[">="], ">=");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    write(O_to_S[">"], ">");
                }
            }
            else if (CharGot == '=')
            {
                CharGot = fgetc(FilePtr);
                if (CharGot == '=')
                {
                    write(O_to_S["=="], "==");
                    CharGot = fgetc(FilePtr);
                }
                else
                {
                    write(O_to_S["="], "=");
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
