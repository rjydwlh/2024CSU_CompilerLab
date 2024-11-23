
#ifndef _COMPILER_H

    #define _COMPILER_H

    enum SYS_Token
    {
        SYS_CONST       =1 ,      //          const 
        SYS_END         =2 ,      //          end
        SYS_PROCEDURE   =3 ,      //          procedure
        SYS_IF          =4 ,      //          if
        SYS_THEN        =5 ,      //          then
        SYS_WHILE       =6 ,      //          while
        SYS_DO          =7 ,      //          do
        SYS_BEGIN       =8 ,      //          begin
        SYS_CALL        =9 ,      //          call
        SYS_VAR         =10,      //          var
        SYS_ODD         =11,      //          odd
        SYS_add         =12,      //          +
        SYS_sub         =13,      //          -
        SYS_mul         =14,      //          *      
        SYS_div         =15,      //          /
        SYS_LBRA        =16,      //          (
        SYS_RBRA        =17,      //          )
        SYS_eq          =18,      //          =
        SYS_dou         =19,      //          ,
        SYS_dot         =20,      //          .
        SYS_fen         =21,      //          ;
        SYS_xiaodeng    =22,      //          >
        SYS_dadeng      =23,      //          <
        SYS_dengdeng    =24,      //          <>
        SYS_lhuakuoho   =25,      //          {
        SYS_rh          =26,      //          }
        SYS_fuzhi       =27,      //          :=
        SYS_jiankuohao  =28,      //          <>
        SYS_IDENT       =39,      //  
        SYS_NUMBER      =30      //
    };



    #define MaxLenOfCommand                 1000
    
    #ifndef _IOSTREAM
        #include<iostream>
    #endif


    #ifndef _MAP
        #include<map>
    #endif

    #ifndef _STRING
        #include<string>
    #endif

    #ifndef _VECTOR
        #include<vector>
    #endif

    #ifndef _CSTRING
        #include<cstring>
    #endif

    #ifndef _FSTREAM
        #include<fstream>
    #endif
    
    // compiler.cpp :

    void CreateMap();                       //  编译之前记得先把map建立一下
    void write(int num, std::string x);     // 输出token 到FILE* out里
    inline std::string tr(char s);          // 把字符转化成string
    void compile(FILE *FilePtr);            // 编译FILE* FilePtr 里面的PL/0 代码


    #ifndef _COMPILER_CPP
        #include "compiler.cpp"
    #endif

    class token;                            // token指令
    void trans_token(FILE* in);             // 


    #ifndef _TOKEN_CPP
     #include "token.cpp"
    #endif


#endif
