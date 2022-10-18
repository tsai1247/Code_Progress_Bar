#include<sstream>
#include<iostream>
#include<experimental/source_location>
#include<chrono>
#include<atomic>
#include<map>

#ifdef _WIN32
#include<windows.h>
#endif

using std::experimental::source_location;

namespace progress
{
    static std::map<std::string, int> progresses;
    static std::map<std::string, int> last_progress_cnt;
    static std::map<std::string, std::chrono::system_clock::time_point> last_progress_time;
    static std::atomic<int> progressNum;
    int progress_min_step_cnt = 1;
    std::chrono::duration<long long, std::milli>  progress_min_step_time = std::chrono::milliseconds(100);
    
    inline COORD getxy() //移動座標
    {
        COORD ret;
        #if _WIN32
        CONSOLE_SCREEN_BUFFER_INFO cbsi;
        if (GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ), &cbsi))
        {
            ret = cbsi.dwCursorPosition;
        }
        #else

        #endif

        return ret;
    }

    inline void gotoxy(int x,int y) //移動座標
    {
        #if _WIN32
        COORD coord;
        coord.X=x;
        coord.Y=y;
        SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
        #else
        #endif
    }

    void ycgb(int k)//隱藏游標
    {
        #if _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hOut,&cci);
        cci.bVisible=k;//賦1為顯示，賦0為隱藏
        SetConsoleCursorInfo(hOut,&cci);
        #else
        #endif
    }
    auto getWindowSize()
    {
        COORD size;

        #if _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        #else
        size.X = 80;
        size.Y = 20;
        #endif

        return size;
    }

    void clearWindow()
    {
        #ifdef _WIN32
        system("CLS");
        #else
        system("clear");
        #endif
    }

    auto progressbar(int progress) -> std::string
    {
        auto progressbar_size = std::min((int)getWindowSize().X, 100) - 25;
        auto int_progress_base = progress / ((double)100 / progressbar_size);
        std::string bar = "(";
        for(int i=0; i<int_progress_base; i++)
        {
            bar = bar + "*";
        }
        
        for(int i=0; i<progressbar_size - int_progress_base; i++)
        {
            bar = bar + "-";
        }
        
        bar = bar + ")";
        return bar;
    }

    std::string record_go_through = "";
    void record(std::string msg = "", const source_location location = source_location::current())
    {
        std::string msg_through_lines = "You go through line: ";
        if(msg == "")
            msg = std::to_string(location.line());
        record_go_through += msg + " ";

        auto upperbound_limit = std::min((int)getWindowSize().X, 100) - 5 - msg_through_lines.size();
        if(record_go_through.size() > upperbound_limit)
        {
            record_go_through = record_go_through.substr(record_go_through.size() - upperbound_limit, upperbound_limit);
            msg_through_lines += record_go_through;
        }
        else
        {
            msg_through_lines += record_go_through;
            msg_through_lines += std::string(upperbound_limit - record_go_through.size(), ' ');
        }
        
        ycgb(0);
        auto origin_position = getxy();
        gotoxy(0, getWindowSize().Y -1);
        printf("%s", msg_through_lines.c_str());
        gotoxy(origin_position.X, origin_position.Y);
        ycgb(1);
    }

    void record(int cur, int size, const source_location location = source_location::current())
    {
        std::string lineID = location.file_name();
        lineID += ":" + std::to_string(location.line());
        if(progresses.find(lineID) == progresses.end())
        {
            progressNum.fetch_add(1);
            progresses[lineID] = progressNum.load();
            last_progress_cnt[lineID] = -2;
            last_progress_time[lineID] = std::chrono::system_clock::now();
        }
        auto curProgressNum = progresses[lineID];

        auto current_progress = (double) cur*100 / size;
        auto int_progress = (int)current_progress;
        if(current_progress - last_progress_cnt[lineID] > progress_min_step_cnt || current_progress < last_progress_cnt[lineID]
         || std::chrono::system_clock::now() - last_progress_time[lineID] > progress_min_step_time)
        {
            auto bar = progressbar(int_progress);
            ycgb(0);
            auto origin_position = getxy();
            gotoxy(0, getWindowSize().Y - curProgressNum-1);
            printf("line %d: %s %6.2f %%", location.line(), bar.c_str(), current_progress);
            gotoxy(origin_position.X, origin_position.Y);
            ycgb(1);

            last_progress_cnt[lineID] = current_progress;
            last_progress_time[lineID] = std::chrono::system_clock::now();
        }

    }
}