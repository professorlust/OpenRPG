/*
character-generator - character-generator.cpp
Created on: Jan 24, 2017

OpenRPG Software License - Version 1.0 - February 10th, 2017 <https://openrpg.io/about/license/>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
*/
#include <vector>
#include <string>

#include "openrpg.h"
#include "character.h"

using namespace std;
using namespace ORPG;

/* Option parser - parse_args(argc, argv)
    This function parses all cla's passed to argv. */
int parse_args(int argc, char* argv[]) {
    int status = EXIT_SUCCESS;

    /* getopt_long stores the option and option index here */
    int opt, opt_ind;

    /* disables getopt printing to now be handled in '?' case */
    Core::opterr = 0;

    /* these are the long cla's and their corresponding chars */
    static struct Core::option long_opts[] = {
        {"help",    no_argument,        0,  'h'},
        {"random",  no_argument,        0,  'r'},
        {"verbose", no_argument,        0,  'v'},
        {"version", no_argument,        0,  'V'},
        /* NULL row to terminate struct */
        {0,         0,                  0,   0}
    };

    while ((opt = Core::getopt_long(argc, argv, "rhvV",
                               long_opts, &opt_ind)) != EOF &&
                               status != EXIT_FAILURE) {

        switch (opt) {
        /* -h --help */
        case 'h': {
            Characters::print_help_flag();
        } break;

        /* -r --random */
        case 'r': {
            // TODO skip character creator and generate fully random character
        } break;

        /* -V --verbose */
        case 'v': {
            Core::VB_FLAG = true;
            Core::QUIET_FLAG = false;
        } break;

        /* -v --version */
        case 'V': {
            Characters::print_version_flag();
        } break;
            
        /* parsing error */
        case ':':
        case '?': {
            Characters::print_help_flag();
        } break;

        /* if we get here something very bad happened */
        default: {
            printf("Aborting...\n");
            status = EXIT_FAILURE;
        }
        }
    }

    return status;
}

int request_selection(CharacterFactory factory) {
    int index = -1;
    string input;

    vector<string> list;
    
    if(factory.has_options())
        list = factory.current_options();
    
    while(index < 0 || index > (signed)list.size()) {
        
        int tick = 0;

        for(string str : list) {
            cout << "\t" << (tick++) << ") " << str;
            
            if(tick % 3 == 0) cout << endl;
        }
        
        tick = 0;
        
        cout << "\n#? ";
        cin >> input;
    
        index = stoi(input);
    }

    return index;
}

Ability request_scores() {    
    printf("\n");

    Ability ret;
    string input;
    vector<int> stats = ability_vector();

    printf("You generated the following ability scores: \n");

    for(int num : stats) printf("%i (%i)\n", num, modifier(num));

    printf("\n");

    for(size_t i = 0; i < stats.size(); i++) {
        switch(i) {
        case 0: {
            printf("Set Strength\t (STR): ");

            cin >> input;
            
            ret.STR = stoi(input);
        } break;

        case 1: {
            printf("Set Dexterity\t (DEX): ");

            cin >> input;
            
            ret.DEX = stoi(input);
        } break;

        case 2: {
            printf("Set Constitution (CON): ");

            cin >> input;
            
            ret.CON = stoi(input);
        } break;

        case 3: {
            printf("Set Intelligence (INT): ");

            cin >> input;
            
            ret.INT = stoi(input);
        } break;

        case 4: {
            printf("Set Wisdom\t (WIS): ");

            cin >> input;
            
            ret.WIS = stoi(input);
        } break;

        case 5: {
            printf("Set Charisma\t (CHA): ");

            cin >> input;
            
            ret.CHA = stoi(input);
        } break;

        default: {
            printf("should not have gotten here");
            exit(EXIT_FAILURE);
        }
        }
    }

    printf("\n");
    
    return ret;
}

int main(int argc, char* argv[]) {
    int status = parse_args(argc, argv); // may exit
    
    /* begin creating the character here */
    printf("Use character creator (Y/n)\n");   // TODO character creator switch ('-r' argv should ALSO handle this)

    CharacterFactory factory;
    
    printf("Choose Race:\n");
    
    factory.select_option(request_selection(factory));

    if(factory.has_options()) {
        printf("Choose Subrace:\n");
        factory.select_option(request_selection(factory)); 
    }
    
    printf("Class\n");                         // TODO class menu.
    printf("Background\n");                    // TODO background menu

    Ability abil = request_scores();
    
    printf("Skill select based on class\n");   // TODO Skill select based on class
    printf("Hit points\n");                    // TODO hit points max, avg, or roll + con mod
    printf("Equipment\n\n");                   // TODO select equipment based on class and background

    printf("(leave blank for random name)\n");
    printf("Name: ");

    string name;

    cin.ignore();
    getline(cin, name);

    printf("\n");

    Character* character = name.empty() ? factory.NewCharacter(abil) : factory.NewCharacter(abil, name);
    
    printf("%s", character->to_string().c_str());

    return status;
}
