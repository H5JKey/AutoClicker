
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <thread>

std::vector<std::string> parse(std::string s) {
    std::vector<std::string> args(0);
    std::string command = s.substr(0, s.find(' '));
    std::string args_string = s.substr(s.find(' ') + 1);
    while (args_string.find(',')!=-1) {
        std::string arg = args_string.substr(0, args_string.find(','));
        args.push_back(arg);
        args_string = args_string.substr(args_string.find(',') + 1);
    }
    args.push_back(args_string);
    args.insert(args.begin(), command);

    return args;

}


class Executer {


private:
    std::vector<std::vector<std::string>> commands;
    std::vector<std::string> args;
    bool go;


public:
    Executer(std::string file_path) {
        std::fstream file(file_path);
        std::string s;

        std::getline(file, s);
        args = parse(s);

        while (std::getline(file, s)) {
            commands.push_back(parse(s));
        }
        file.close();
    }

    void run() {
        std::thread hotkeyListening([&]() {
            while (1) {
                if (GetAsyncKeyState(std::stoi(args[1])))
                    go = !go;
                Sleep(100);
            }
        });
        hotkeyListening.detach();

        while (1) {
            while (go)
            {
                for (std::vector<std::string>& command : commands) {
                    if (command[0] == "press") {
                        if (command[1] == "LB") {
                            POINT p;
                            GetCursorPos(&p);
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                        }
                        if (command[1] == "RB") {
                            POINT p;
                            GetCursorPos(&p);
                            mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, 0, 0);
                        }
                    }
                    else if (command[0] == "unpress") {
                        if (command[1] == "LB") {
                            POINT p;
                            GetCursorPos(&p);
                            mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                        }
                        if (command[1] == "RB") {
                            POINT p;
                            GetCursorPos(&p);
                            mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
                        }
                    }
                    else if (command[0] == "move") {
                        SetCursorPos(std::stoi(command[1]), std::stoi(command[2]));
                    }
                    else if (command[0] == "sleep") {
                        Sleep(std::stoi(command[1]));
                    }
                }
                if (std::stoi(args[2]) == 0) {
                    go = false;
                }
            }
        }
    }
    
};

int main(int argc, char* argv[])
{
    std::string path=argv[1];
    Executer executer(path);
    executer.run();
}

