#pragma once

#define UK_ZERO 48           //unique key 0
#define UK_ONE 49            //unique key 1
#define UK_NINE 57           //unique key 9
#define UK_LOWER_A 97        //unique key a
#define UK_LOWER_Z 122       //unique key z
#define UK_CAPITAL_A 65      //unique key A
#define UK_CAPITAL_Z 90      //unique key Z
#define UK_SYMBOL_START 32   //unique key printable character start
#define UK_SYMBOL_END 126    //unique key printable character end

#define UK_UNDERSCORE 95     //unique key underscore                        
#define UK_SPACE 32          //unique key space 
#define UK_BACKSPACE 8       //unique key backspace
#define UK_COPY 22           //unique key ctrl + v
#define UK_ESCAPE 27         //unique key escape/esc
#define UK_RETURN 13         //unique key enter/return


#define SR_NULL 0            //select range 0
#define SR_ONE 1             //select range 1
#define SR_TWO 2             //select range 2
#define SR_THREE 3           //select range 3
#define SR_FOUR 4            //select range 4
#define SR_FIVE 5            //select range 5
#define SR_SIX 6             //select range 6
#define SR_SEVEN 7           //select range 7
#define SR_EIGHT 8           //select range 8
#define SR_NINE 9            //select range 9

#include <conio.h>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>
#include <ctime>

namespace str {


    void print(const char* str);

    void strcpy(const char* src, char* dest, int maxBuffSize);

    void read(char* buffer, int size);

    class ISTR {

        enum class ASCII_TYPE {
            nums, chars, numsAndChars, printables
        };

        static bool ASCIICheck(const ASCII_TYPE t, char c);
        static void CopiedContent(const ASCII_TYPE t, std::string& mutate, const int max_size);
        static bool IfGoodKeyStr(std::string& mutate, const char c, const short max_len, const ASCII_TYPE restriction);
        static void IfGoodRange(std::string& mutate, const char c, const short max_range, const ASCII_TYPE resrestriction);
        static void BackSpace(std::string& mutate);
    public:

        static std::string InputStr();
        static std::string InputStr(const std::string& msg);
        static int InputNum();
        static int InputNum(const std::string& msg);
        static int InputRange(const unsigned int range);
        static int InputRange(const unsigned int range, const std::string& msg);
    };

    void LogError(const std::string& error);
    void LogMessage(const std::string& message);

    class ERROR_LOG final {

        friend void LogError(const std::string& str);

        static std::vector<std::string>* GetInstance();
        static void Add(const std::string& what);
        static void Add(const char* what);
    public:
        static const std::vector<std::string>& Get();
    private:
        ERROR_LOG() = delete;
        ERROR_LOG(const ERROR_LOG&) = delete;
        ERROR_LOG& operator=(const ERROR_LOG&) = delete;
    };

    class MESSAGE_LOG {

        friend void LogError(const std::string& str);
        friend void LogMessage(const std::string& str);

        static std::vector<std::string>* GetInstance();
        static void Add(const std::string& what);
        static void Add(const char* what);
    public:
        static void Flush();
    private:
        MESSAGE_LOG() = delete;
        MESSAGE_LOG(const MESSAGE_LOG&) = delete;
        MESSAGE_LOG& operator=(const MESSAGE_LOG&) = delete;
    };

    //manip strings
    namespace MSTR {
        void RemoveSymbols(std::string& from, const std::string& symbols);
        void RemoveSymbols(std::string& from, const char symbol);
        void PrintList(const std::vector<std::pair<std::string, std::string>>& list);
        bool ListContains(const std::vector<std::string>& cont, const std::string& compared_to);
        const std::string UnixTime(time_t unix_timestamp);
        std::string GetClipboard();
    }

}