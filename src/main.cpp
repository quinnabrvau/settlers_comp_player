//
//  main.cpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#include <iostream>
#include "Game.hpp"

class CmdLineParser
{
    int argc;const char** argv;public:
    CmdLineParser(int _argc, const char** _argv): argc(_argc), argv(_argv){}
    //is the param?
    bool operator[](std::string param)
    {int idx = -1;   for (int i = 0; i < argc && idx == -1; i++)if (std::string(argv[i]) == param) idx = i;return (idx != -1); }
    std::string operator[](int param)
    {return (std::string) argv[param%argc];}
    //return the value of a param using a default value if it is not present
    std::string operator()(std::string param,std::string defvalue = "-1")
    {int idx = -1;for (int i = 0; i < argc && idx == -1; i++)if (std::string(argv[i]) == param) idx = i;if (idx == -1) return defvalue;else return (argv[idx + 1]);}
};

int main(int argc, const char * argv[]) {
    CmdLineParser cmd = CmdLineParser(argc, argv);
    if (cmd["-t"]) {
        Game game0;
        int w = game0.play_game();
        Game game1(3,10);
        w = game1.play_game();
        return w; // should be 0
    }
    return 0;
}
