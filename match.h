#ifndef MATCH_H
#define MATCH_H

typedef struct match {
    int match_id;
    char* team1;
    char* team2;
    int available_tickets;
    float price;
} match;

void initializeMatches(match *football_match);

#endif /* MATCH_H */
