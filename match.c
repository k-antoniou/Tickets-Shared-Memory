#include "match.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void initializeMatches(match *football_match) {
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        football_match[i].match_id = i + 1;
        football_match[i].available_tickets = (rand() % 5) + 1;
    }

    char* teams[10][2] = {
        {"panaigialios", "panachaiki"},
        {"pao", "aek"},
        {"paok", "pao"},
        {"pas.giannena", "aek"},
        {"aek", "aris"},
        {"aris", "paok"},
        {"paok", "aek"},
        {"aek", "osfp"},
        {"pao", "osfp"},
        {"osfp", "lamia"}
    };

    float prices[10] = {10.0, 12.4, 50.3, 23.7, 11.1, 21.5, 67.2, 32.1, 72.0, 45.8};

    for (int i = 0; i < 10; i++) {
        football_match[i].team1 = strdup(teams[i][0]);
        football_match[i].team2 = strdup(teams[i][1]);
        football_match[i].price = prices[i];
    }
}
