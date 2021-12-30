#include "library1.h"
#include "PlayersManager.h"
#include <iostream>

void* Init() {
    PlayersManager *DS = NULL;
    try {
        DS = new PlayersManager(); 
    } catch(std::bad_alloc const&) {
        (*DS).Quit(); 
    }
    return (void*)DS;
}


StatusType AddGroup(void *DS, int GroupID){
    if(DS == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> AddGroup(GroupID);
}


StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level) {
    if(DS == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> AddPlayer(PlayerID, GroupID, Level);
}


StatusType RemovePlayer(void *DS, int PlayerID) {
    if(DS == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> RemovePlayer(PlayerID);
}


StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID) {
    if(DS == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> ReplaceGroup(GroupID, ReplacementID);
}


StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID) {
    if(DS == nullptr || PlayerID == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> GetHighestLevel(GroupID, PlayerID);
}


StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers) {
    if(DS == nullptr || numOfPlayers == nullptr || Players == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> GetAllPlayersByLevel(GroupID, Players, numOfPlayers);
}


StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players) {
    if(DS == nullptr) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> GetGroupsHighestLevel(numOfGroups, Players);
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease) {
    if(DS == nullptr ) {
        return INVALID_INPUT;
    }
    return ((PlayersManager*)DS)-> IncreaseLevel(PlayerID, LevelIncrease);
}

void Quit(void** DS) {
    ((PlayersManager*)*DS)-> Quit();
    delete ((PlayersManager*)*DS);
    *DS = nullptr;
}


// // tester functions:
// void printGroup(void* DS, int groupID) {
//     return ((PlayersManager*)DS)-> printGroup(groupID);
// }




