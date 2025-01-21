#include "strings2.h"


namespace str {

    bool ISTR::ASCIICheck(const ASCII_TYPE t, char c) {
        switch (t) {
        case ASCII_TYPE::nums:           return (c >= UK_ZERO && c <= UK_NINE);
        case ASCII_TYPE::chars:          return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || c == UK_UNDERSCORE;
        case ASCII_TYPE::numsAndChars:   return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || (c >= UK_ZERO && c <= UK_NINE) || c == UK_UNDERSCORE;
        case ASCII_TYPE::printables:     return (c >= UK_SYMBOL_START && c <= UK_SYMBOL_END);
        }
        throw "Invalid argument";
    }
    void ISTR::CopiedContent(const ASCII_TYPE t, std::string& mutate, const int max_size) {
        std::string copy = MSTR::GetClipboard();//get all data in text form from clipboard

        copy.erase(std::remove_if(copy.begin(), copy.end(), [&t](const char c) { //remove every symbol which does not pass the check
            return !ASCIICheck(t, c);
            }), copy.end());


        for (char c : copy) {
            if (mutate.size() >= max_size) {
                break;
            }

            mutate += c;
            _putch(c);
        }
    }
    bool ISTR::IfGoodKeyStr(std::string& mutate, const char c, const short max_len, const ASCII_TYPE restriction) {
        if (ASCIICheck(restriction, c) && mutate.size() < max_len) {
            mutate += c;
            _putch(c);
            return true;
        }
        return false;
    }
    void ISTR::IfGoodRange(std::string& mutate, const char c, const short max_range, const ASCII_TYPE restriction) {
        if (ASCIICheck(restriction, c) && !(mutate.empty() && c == UK_ZERO)) {

            mutate += c;
            _putch(c);

            if (std::stoi(mutate) > max_range) {

                for (const char each : mutate) {
                    _putch('\b');
                    _putch(' ');
                    _putch('\b');
                }


                mutate = std::to_string(max_range);

                for (const char each : mutate) {
                    _putch(each);
                }

            }
        }
    }
    void ISTR::BackSpace(std::string& mutate) {
        _putch('\b');
        _putch(' ');
        _putch('\b');
        mutate.pop_back();
    }
    std::string ISTR::InputStr() {
        static constexpr ASCII_TYPE restriction = ASCII_TYPE::numsAndChars;//shorthand to pass restructions for inputs (may be depricated in the future)
        static constexpr short max_input_len = 50;
        std::string str;

        while (true) {
            const char c = _getch();

            if (c == UK_RETURN && !str.empty()) {
                break;
            }
            if (IfGoodKeyStr(str, c, max_input_len, restriction)) {}

            else if (c == UK_BACKSPACE && !str.empty()) {
                BackSpace(str);
            }
            else if (c == UK_COPY) {
                CopiedContent(restriction, str, max_input_len);
            }
            else if (c == UK_ESCAPE) {
                return "";
            }
        }

        return str;
    }
    int ISTR::InputNum() {
        static constexpr ASCII_TYPE restriction = ASCII_TYPE::nums;//shorthand to pass restructions for inputs (may be depricated in the future)
        static constexpr short max_num_digits = 9;
        std::string str;

        while (true) {
            const char c = _getch();

            if (c == UK_RETURN && !str.empty()) {
                break;
            }
            if (IfGoodKeyStr(str, c, max_num_digits, restriction)) {}

            else if (c == UK_BACKSPACE && !str.empty()) {
                BackSpace(str);
            }
            else if (c == UK_COPY) {
                CopiedContent(restriction, str, max_num_digits);
            }
            else if (c == UK_ESCAPE) {
                return -1;
            }
        }

        return std::stoi(str);
    }
    int ISTR::InputRange(const unsigned int range) {
        static constexpr ASCII_TYPE restriction = ASCII_TYPE::nums;//shorthand to pass restructions for inputs (may be depricated in the future)
        std::string str;


        while (true) {
            const char c = _getch();

            if (c == UK_RETURN && !str.empty()) {
                break;
            }
            IfGoodRange(str, c, range, restriction);
            if (c == UK_BACKSPACE && str.size() > 0) {
                BackSpace(str);
            }
            else if (c == UK_ESCAPE) {
                return -1;
            }
        }

        return std::stoi(str);
    }
    std::string ISTR::InputStr(const std::string& msg) {
        printf("\n%s", msg.c_str());
        return InputStr();
    }
    int ISTR::InputNum(const std::string& msg) {
        printf("\n%s", msg.c_str());
        return InputNum();
    }
    int ISTR::InputRange(const unsigned int range, const std::string& msg) {
        printf("\n%s", msg.c_str());
        return InputRange(range);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   logging stuff

    void LogError(const std::string& error) {
        ERROR_LOG::Add(error);
        MESSAGE_LOG::Add(error);
    }
    void LogMessage(const std::string& message) {
        MESSAGE_LOG::Add(message);
    }
    std::vector<std::string>* ERROR_LOG::GetInstance() {
        static std::unique_ptr<std::vector<std::string>> error_log = std::make_unique<std::vector<std::string>>();
        return error_log.get();
    }
    void ERROR_LOG::Add(const std::string& what) {
        GetInstance()->push_back(what);
    }
    void ERROR_LOG::Add(const char* what) {
        GetInstance()->push_back(what);
    }
    const std::vector<std::string>& ERROR_LOG::Get() {
        return *GetInstance();
    }
    std::vector<std::string>* MESSAGE_LOG::GetInstance() {
        static std::unique_ptr<std::vector<std::string>> message_log = std::make_unique<std::vector<std::string>>();
        return message_log.get();
    }
    void MESSAGE_LOG::Add(const std::string& what) {
        GetInstance()->push_back(what);
    }
    void MESSAGE_LOG::Add(const char* what) {
        GetInstance()->push_back(what);
    }
    void MESSAGE_LOG::Flush() {
        for (const auto& each : *GetInstance()) {
            printf("\n%s", each.c_str());
        }
        GetInstance()->clear();
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   string manipulation stuff

    void MSTR::RemoveSymbols(std::string& from, const std::string& symbols) {
        from.erase(std::remove_if(from.begin(), from.end(), [&symbols](const char c) {
            return symbols.find(c) != std::string::npos;
            }), from.end());
    }
    void MSTR::RemoveSymbols(std::string& from, const char symbol) {
        from.erase(std::remove_if(from.begin(), from.end(), [symbol](const char c) {
            return symbol == c;
            }), from.end());
    }
    void MSTR::PrintList(const std::vector<std::pair<std::string, std::string>>& list) {
        size_t i = 1;

        for (const auto& each : list) {
            printf("\n[%zu] %s", i++, each.second.c_str());
        }
    }
    bool MSTR::ListContains(const std::vector<std::string>& cont, const std::string& compared_to) {
        return std::find(cont.begin(), cont.end(), compared_to) != cont.end();
    }
    const std::string MSTR::UnixTime(time_t unix_timestamp) {

        char time_buf[80];
        struct tm ts;
        localtime_s(&ts, &unix_timestamp);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &ts);

        return std::string(time_buf);
    }
    std::string MSTR::GetClipboard() {
        if (!OpenClipboard(nullptr)) {
            return "";
        }
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == nullptr) {
            CloseClipboard();
            return "";
        }

        char* copy_stream = static_cast<char*>(GlobalLock(hData));
        std::string text;

        if (copy_stream) {
            text = copy_stream;
        }

        GlobalUnlock(hData);
        CloseClipboard();

        return text;
    }

    ///
    void print(const char* str) {
        for (; *str != 0; str++) {
            _putch(*str);
        }
    }

    void strcpy(const char* src, char* dest, int maxBuffSize) {
        int n = 0;
        for (; *src != 0 && (n + 1 < maxBuffSize); src++, dest++, n++) {
            *dest = *src;
        }
        *dest = 0;
    }

    void read(char* buffer, int size) {

        const char* const pEnd = buffer + size;

        for (char c = _getch(); c != 13 && (buffer + 1 < pEnd); c = _getch(), buffer++) {
            _putch(c);
            *buffer = c;
        }
        *buffer = 0;
    }
    ///

}