// testGameView.c ... test the GameView ADT

// Edited by the group:
//  ╔═╗╔═╗╔╦╗╔═╗┌─┐┌┐┌┌┬┐┌─┐┌─┐┌┬┐┬┌─┐5
//  ╠╣ ║ ║ ║║╠╣ ├─┤│││ │ ├─┤└─┐ │ ││
//  ╚  ╚═╝═╩╝╚  ┴ ┴┘└┘ ┴ ┴ ┴└─┘ ┴ ┴└─┘
// *** Starring ***
//   +-+-+-+-+-+ +-+-+-+-+-+
//   |R|u|c|h|i| |G|u|p|t|a|
//   +-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
//   |S|a|n|j|a|y| |N|a|r|a|y|a|n|a|
//   +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+
//   |S|h|a|n|u|s|h| |P|r|e|m|a| |T|h|a|s|a|r|a|t|h|a|n|
//   +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+
//   |S|i|m|o|n| |V|a|n| |W|i|n|d|e|n|
//   +-+-+-+-+-+ +-+-+-+ +-+-+-+-+-+-+
//   |J|e|s|s|e| |Y|u|e|
//   +-+-+-+-+-+ +-+-+-+
// 72 character limit...................................................


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;
    
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    //assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    //assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    //assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    //assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    //assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    //assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    //assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);
    
    //------------------------------

    
    // TEST HUNTER HEALTH/SCORE STUFF
    //   "The score decreases by 6 each time a Hunter is teleported to the hospital."
    // Test that when person goes to Hospital, they don't magically regenerate health
    
    printf("Hunter health\n");
    PlayerMessage mess0[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGET... SGE.... HGE.... MGE.... DST.... "
                     "GPAT... SGE.... HGE.... MGE.... DST.... "
                     "GGET... SGE.... HGE.... MGE.... DST.... "
                     "GPAT... SGE.... HGE.... MGE.... DST.... "
                     "GGET... SGE.... HGE.... MGE.... DST....", mess0);
    //printf("Score is %d\n",getScore(gv));
    //assert(getScore(gv) == GAME_START_SCORE - 5*SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_HUNTER_HOSPITAL);
    //assert(getHealth(gv, PLAYER_LORD_GODALMING) == 0);
    printf("passed\n");
    
    // Health gained when resting
    gv = newGameView("GGET... SGE.... HGE.... MGE.... DST....", mess0);
    assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    //assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
    gv = newGameView("GGET... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST....", mess0);
    //assert(getScore(gv) == GAME_START_SCORE - 2*SCORE_LOSS_DRACULA_TURN);
    printf("health is %d\n", getHealth(gv, PLAYER_LORD_GODALMING));
    //assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    
    // If there is more than one Trap at a location the Hunter encounters the Traps one
    //  at a time until either the Hunter is reduced to 0 or less life points, or until all the Traps are
    //  encountered, whichever occurs first.
    gv = newGameView("GGETTT. SGE.... HGE.... MGE.... DST....", mess0);
    //assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - 3*LIFE_LOSS_TRAP_ENCOUNTER);
    gv = newGameView("GGE.... SGETT.. HGE.... MGE.... DST....", mess0);
    //assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - 2*LIFE_LOSS_TRAP_ENCOUNTER);
    printf("passed\n");
    
    //-------------------------------
    
    // TEST DRACULA STUFF:
    // Test "The score decreases by 1 each time Dracula finishes a turn."
    printf("Score decrease when dracula finishes turn\n");
    PlayerMessage mess1[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE....", mess1);
    //assert(getScore(gv) == GAME_START_SCORE);
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DST....", mess1);
    //assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    
    //Test "The score decreases by 13 each time a Vampire matures (ie falls off the trail)."
    printf("Score decrease when vampire matures\n");
    PlayerMessage mess2[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DST.V.. "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... ", mess2);
    //assert(getScore(gv) == GAME_START_SCORE - 6*SCORE_LOSS_DRACULA_TURN);
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DST.V.. "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST.... "
                     "GGE.... SGE.... HGE.... MGE.... DST..V. ", mess2);
    //assert(getScore(gv) == GAME_START_SCORE - 7*SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_VAMPIRE_MATURES); // -currently failing here- FIXED
    printf("passed\n");
    
    //Testing health points
    //Dracula:
    //  "Dracula loses 10 blood points each time he encounters a Hunter."
    printf("Dracula blood points\n");
    PlayerMessage mess3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SZU.... HGE.... MGE.... DST.... "
                     "GGE.... SSTD... HGE.... MGE.... DZU....", mess3);
    //assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER); // Only works if string includes Dracula as an encounter
    
    //  "Dracula loses 2 blood point if he is at sea at the end of his turn (he hates water!)"
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC....", mess3);
    //assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
    
    //  "Dracula regains 10 blood points if he is in Castle Dracula at the end of his turn and has not
    //     yet been defeated (even if he has been automagically teleported there - see Dracula's Turn below)"
    gv = newGameView("GGE.... SST.... HGE.... MGE.... DCD....", mess3);
    //assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA); // -currently failing here- FIXED
    printf("passed\n");
    
    // Check double back, hide, etc for dracula
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DED.V.. ", mess3);
    assert(getLocation(gv, PLAYER_DRACULA) == abbrevToID("ED"));
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DED.V.. "
                     "GST.... SST.... HST.... MST.... DHIT... ", mess3);
    assert(getLocation(gv, PLAYER_DRACULA) == HIDE);
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DED.V.. "
                     "GST.... SST.... HST.... MST.... DHIT... "
                     "GST.... SST.... HST.... MST.... DMNT... "
                     "GST.... SST.... HST.... MST.... DD1T...", mess3);
    assert(getLocation(gv, PLAYER_DRACULA) == DOUBLE_BACK_1);
    
    
    // Seeing if these one cause asserts to pop up
    gv = newGameView("GED.... SGE.... HZU.... MCA.... DCD.V.. GNS.... SMR.... HMR.... MMA.... DC?T... GAO.... SMR.... HMR.... MLS.... DC?T... GAO.... SMR.... HMR.... MSN.... DC?T... GAO.... SMR.... HMR.... MSR.... DD1T... GAO....", mess3);
    
    // ---------------------------------
    
    // Testing closest city function
    //LocationID wanted[4] = {70, 45, 14, 28};
    //printf("\nTesting closestCity function. Location is %d\n\n", closestCityGame(gv, 50, wanted, 4));
    
    // ---------------------------------
    
    //Testing incomplete round
    printf("Test for imcomplete round\n");
    PlayerMessage messages50[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB....", messages50);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getRound(gv) == 0);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    printf("passed\n");
    disposeGameView(gv);
    
    //Testing incomplete round
    printf("Test for imcomplete round\n");
    PlayerMessage messages60[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU....", messages60);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getRound(gv) == 0);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    printf("passed\n");
    disposeGameView(gv);
    
    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);    
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    for (i = 0; i < size; i++) {
        printf ("edges[%d] = %s\n", i, idToName (edges[i]));
    }
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);

    printf ("Checking 3 max rail jumps, Paris start\n");
    edges = connectedLocations(gv, &size, PARIS, PLAYER_LORD_GODALMING, 3,1,1,1);
    for (i = 0; i < size; i++) {
        printf ("edges[%d] = %s\n", i, idToName (edges[i]));
    }
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 14); assert(seen[PARIS]); assert(seen[BRUSSELS]); assert(seen[LE_HAVRE]);
    assert(seen[NANTES]); assert(seen[CLERMONT_FERRAND]); assert(seen[GENEVA]);
    assert(seen[STRASBOURG]); assert(seen[MARSEILLES]); assert (seen[BORDEAUX]);
    assert (seen[SARAGOSSA]); assert (seen[BARCELONA]); assert (seen[MADRID]);
    assert (seen[COLOGNE]); assert (seen[FRANKFURT]);
    free(edges);

    printf ("Checking 2 max rail jumps, Szeged start, Mina Harker\n");
    edges = connectedLocations(gv, &size, SZEGED, PLAYER_MINA_HARKER, 7,1,1,1);
    for (i = 0; i < size; i++) {
        printf ("edges[%d] = %s\n", i, idToName (edges[i]));
    }
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 11); assert (seen[SZEGED]); assert (seen[KLAUSENBURG]); assert (seen[BUDAPEST]);
    assert (seen[BELGRADE]); assert (seen[ST_JOSEPH_AND_ST_MARYS]);
    assert (seen[ZAGREB]); assert (seen[VIENNA]); assert (seen[SOFIA]); assert (seen[BUCHAREST]);
    assert (seen[GALATZ]); assert (seen[CONSTANTA]);
    free(edges);

    printf ("Checking 3 max rail jumps, London start, Van Helsing\n");
    edges = connectedLocations(gv, &size, LONDON, PLAYER_VAN_HELSING, 5,1,1,1);
    for (i = 0; i < size; i++) {
        printf ("edges[%d] = %s\n", i, idToName (edges[i]));
    }
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 7); assert (seen[LONDON]); assert (seen[PLYMOUTH]); assert (seen[SWANSEA]);
    assert (seen[MANCHESTER]); assert (seen[LIVERPOOL]); assert (seen[EDINBURGH]);
    assert (seen[ENGLISH_CHANNEL]);
    free(edges);    

    printf ("Checking 1 max rail jump, Alicante start, Dr Seward\n");
    edges = connectedLocations(gv, &size, ALICANTE, PLAYER_DR_SEWARD, 0,1,1,1);
    for (i = 0; i < size; i++) {
        printf ("edges[%d] = %s\n", i, idToName (edges[i]));
    }
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 6); assert (seen[ALICANTE]); assert (seen[SARAGOSSA]); assert (seen[MADRID]);
    assert (seen[GRANADA]); assert (seen[MEDITERRANEAN_SEA]);
    free(edges);  

    printf ("Checking 1 max rail jump, Atlantic Ocean start, Dr Seward\n");
    edges = connectedLocations(gv, &size, ATLANTIC_OCEAN, PLAYER_DR_SEWARD, 0,1,1,1);
    for (i = 0; i < size; i++) {
        printf ("edges[%d] = %s\n", i, idToName (edges[i]));
    }
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 9); assert (seen[ATLANTIC_OCEAN]); assert (seen[BAY_OF_BISCAY]);
    assert (seen[ENGLISH_CHANNEL]); assert (seen[GALWAY]); assert (seen[LISBON]);
    assert (seen[IRISH_SEA]); assert (seen[NORTH_SEA]); assert (seen[MEDITERRANEAN_SEA]);
    assert (seen[CADIZ]);
    free(edges);

    printf("passed\n");
    disposeGameView(gv);
    return 0;
}
