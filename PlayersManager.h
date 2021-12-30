
#ifndef PLAYERS_MANAGER_H
#define PLAYERS_MANAGER_H

#include <iostream>
using namespace std;

#include "AVLTree.h"

class Player_GroupID_Info;
class PlayerLevel_Key;
class Group_Key;
class Group_Info;
class PlayerID_Info;

class PlayersManager {
    AVL<Group_Key, Group_Info> Groups_AVL;
    AVL<int, PlayerID_Info> PlayerID_AVL;
    AVL<PlayerLevel_Key, int> PlayerLevel_AVL; //info doesn't matter
    int numOfPlayers;
    int numOfGroups;

    bool groupAlreadyExists(int groupID);
    bool playerAlreadyExists(int playerID);
    node<Group_Key,Group_Info>* findGroup(int groupID);
    void removeGroup(int groupID);
    node<PlayerLevel_Key,Player_GroupID_Info>* addPlayerToGroup(int groupID, int playerID, int level);
    node<PlayerLevel_Key,int>* addPlayerToPlayerLevelAVL(int playerID, int level);
    
    // in the Player_GroupID_avl inside of the node corresponding with groupID in Groups_AVL, 
    // updates every node's groupNode to point to replacementGroupNode.
    void updateEveryGroupNode(node<PlayerLevel_Key, Player_GroupID_Info>* root, node<Group_Key,Group_Info>* replacementGroupNode); 
    void update_Player_in_group(node<PlayerLevel_Key,Player_GroupID_Info>* root, int oldGroupID);
    void update_Player_in_group(node<PlayerLevel_Key,Player_GroupID_Info>* root);

    void fillArrayWithLevels_PlayerLevel_AVL(node<PlayerLevel_Key,int>* root, int** Players, int* index, int size);
    void fillArrayWithLevels_Player_GroupID_AVL(node<PlayerLevel_Key,Player_GroupID_Info>* root, int** Players, int* index, int size);

    bool checkNumOfNonEmptyGroups(int numOfGroups);
    void numOfNonEmptyGroups(node<Group_Key,Group_Info>* root, int numOfGroups, int* count); // stops when count = numOfGroups.
    void fillArrayWithLevel_Group_AVL(node<Group_Key,Group_Info>* root, int** Players, int numOfGroups, int* index);

    void freeGroup_AVL_trees(node<Group_Key,Group_Info>* root);

public:
    PlayersManager();
    StatusType AddGroup(int groupID);
    StatusType AddPlayer(int PlayerID, int GroupID, int Level);
    StatusType RemovePlayer(int PlayerID);
    StatusType ReplaceGroup(int GroupID, int ReplacementID);
    StatusType IncreaseLevel(int PlayerID, int LevelIncrease);
    StatusType GetHighestLevel(int GroupID, int *PlayerID);
    StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers);
    StatusType GetGroupsHighestLevel(int numOfGroups, int **Players);
    void Quit();

    // for testing:
    void printGroup(int groupID);
};

// PlayerLevel_AVL:
class PlayerLevel_Key {
public:
    int level;
    int ID;
    PlayerLevel_Key(): level(0), ID(0) {};
    PlayerLevel_Key(int level, int ID): level(level), ID(ID) {};
    PlayerLevel_Key(const PlayerLevel_Key& plk) {
        level = plk.level;
        ID = plk.ID;
    }
    friend bool operator==(const PlayerLevel_Key& lhs, const PlayerLevel_Key& rhs) {
        return lhs.level == rhs.level && lhs.ID == rhs.ID;
    }
    friend bool operator<(const PlayerLevel_Key& lhs, const PlayerLevel_Key& rhs) {
        if(lhs.level != rhs.level) {
            return lhs.level < rhs.level;
        } else {
            return !(lhs.ID < rhs.ID);
        }
    }
    friend bool operator>(const PlayerLevel_Key& lhs, const PlayerLevel_Key& rhs) {
        if(lhs == rhs) {
            return false;
        }
        return !(lhs < rhs);
    }
    friend ostream& operator<<(ostream& os, PlayerLevel_Key const & playerLevel_key) {
        return os << "PlayerID: " << playerLevel_key.ID << " level: " << playerLevel_key.level << endl;
    }
};

// Group_AVL:
class Group_Key {
public:
    bool isEmpty;
    int groupID;
    Group_Key(): isEmpty(0), groupID(0) {};
    Group_Key(bool isEmpty, int groupID): isEmpty(isEmpty), groupID(groupID) {};
    friend bool operator==(const Group_Key& lhs, const Group_Key& rhs) {
        return lhs.groupID == rhs.groupID && lhs.isEmpty == rhs.isEmpty;
    }
    friend bool operator<(const Group_Key& lhs, const Group_Key& rhs) {
        if(lhs.isEmpty == false && rhs.isEmpty == true) {
            return false;
        } else if(lhs.isEmpty == true && rhs.isEmpty == true) {
            return lhs.groupID > rhs.groupID;
        } else if (lhs.isEmpty == true && rhs.isEmpty == false) {
            return true;
        } else { //lhs.isEmpty == false && rhs.isEmpty == false
            return lhs.groupID > rhs.groupID;
        }
    }
    friend bool operator>(const Group_Key& lhs, const Group_Key& rhs) {
        if(lhs == rhs) {
            return false;
        }
        return !(lhs < rhs);
    }
    friend ostream& operator<<(ostream& os, Group_Key const & group_key) {
        return os << "GroupID: " << group_key.groupID << " isEmpty: " << group_key.isEmpty << endl;
    }
};
class Group_Info {
public:
    int size;
    AVL<PlayerLevel_Key, Player_GroupID_Info> Player_GroupID_AVL;
    Group_Info(): size(0) {};
    Group_Info(int size): size(size) {};
    void printReverseInOrder() {
        cout << "Player_GroupID_AVL tree, size: " << size << endl;
        Player_GroupID_AVL.printReverseInOrder();
    }
};

// Player_GroupID_AVL (the avl tree inside each node in Group_AVL):
class Player_GroupID_Info {
public:
    int playerID;
    int playerLevel;
    node<Group_Key, Group_Info>* groupNode;
    node<int,PlayerID_Info>* playerNode_in_PlayerID_AVL;
    Player_GroupID_Info(): playerID(0), playerLevel(0), groupNode(nullptr), playerNode_in_PlayerID_AVL(nullptr) {};
    Player_GroupID_Info(int playerID, int playerLevel, node<Group_Key, Group_Info>* groupNode, node<int,PlayerID_Info>* playerNode_in_PlayerID_AVL): 
                        playerID(playerID), playerLevel(playerLevel), groupNode(groupNode), 
                        playerNode_in_PlayerID_AVL(playerNode_in_PlayerID_AVL) {};
    Player_GroupID_Info(const Player_GroupID_Info& i) {
        playerID = i.playerID;
        playerLevel = i.playerLevel;
        groupNode = i.groupNode;
        playerNode_in_PlayerID_AVL = i.playerNode_in_PlayerID_AVL;
    }
};

// PlayerID_AVL:
class PlayerID_Info {
public:
    int level;
    node<PlayerLevel_Key,Player_GroupID_Info>* Player_in_group; 
    node<PlayerLevel_Key,int>* Player_in_level_avl;
    PlayerID_Info(): level(0), Player_in_group(nullptr), Player_in_level_avl(nullptr) {};
    PlayerID_Info(int level): level(level) {};
};



/******** PUBLIC FUNCTIONS ********/
PlayersManager::PlayersManager() {
    numOfGroups = 0;
    numOfPlayers = 0;
}

StatusType PlayersManager::AddGroup(int groupID) {
    if(groupID <= 0) {
        return INVALID_INPUT;
    }

    if(groupAlreadyExists(groupID)) {
        return FAILURE;
    }

    Group_Key newGroupKey(true, groupID);
    Group_Info newGroupInfo(0);
    Groups_AVL.addNode(newGroupKey, newGroupInfo);

    numOfGroups++;

    return SUCCESS;
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level) {
    if(PlayerID <= 0 || GroupID <= 0 || Level < 0) {
        return INVALID_INPUT;
    }
    if(playerAlreadyExists(PlayerID) || !groupAlreadyExists(GroupID)) {
        return FAILURE;
    }
    
    node<PlayerLevel_Key,Player_GroupID_Info>* playerAddedInGroupNode = addPlayerToGroup(GroupID, PlayerID, Level);

    node<PlayerLevel_Key,int>* playerAddedInPlayerLevelAVL = addPlayerToPlayerLevelAVL(PlayerID, Level);

    PlayerID_Info playerID_info(Level);
    playerID_info.Player_in_group = playerAddedInGroupNode;
    playerID_info.Player_in_level_avl = playerAddedInPlayerLevelAVL;

    PlayerID_AVL.addNode(PlayerID, playerID_info);

    playerAddedInGroupNode->info.playerNode_in_PlayerID_AVL = PlayerID_AVL.search(PlayerID);

    numOfPlayers++;
    
    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {
    
    if(PlayerID <= 0) {
        return INVALID_INPUT;
    }
    if(playerAlreadyExists(PlayerID) == false) {
        return FAILURE;
    }

    node<int,PlayerID_Info>* playerNodeInPlayerIDAVL = PlayerID_AVL.search(PlayerID);
    int playerLevel = playerNodeInPlayerIDAVL->info.level;

    // remove player from PlayerLevel_AVL:
    PlayerLevel_Key playerLevelKey(playerLevel, PlayerID);
    PlayerLevel_AVL.removeNode(playerLevelKey);

    // remove player from Player_GroupID_AVL in Group_AVL:
    node<Group_Key,Group_Info>* group = playerNodeInPlayerIDAVL->info.Player_in_group->info.groupNode;
    group->info.Player_GroupID_AVL.removeNode(playerLevelKey);
    group->info.size -= 1;
    if(group->info.size == 0) {
        // cout << "GroupID: " << group->key.groupID << " is now empty" << endl;
        //remove and insert group with isEmpty = true;
        Group_Key group_key(false, group->key.groupID);
        Group_Info group_info(0);
        Groups_AVL.removeNode(group_key);
        group_key.isEmpty = true;
        Groups_AVL.addNode(group_key, group_info);
    }


    // remove player from PlayersID_AVL:
    PlayerID_AVL.removeNode(PlayerID);

    numOfPlayers--;

    return SUCCESS;
}

//TODO: valgrind errors:
StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID) {
    
    if(GroupID <= 0 || ReplacementID <= 0 || GroupID == ReplacementID) {
        return INVALID_INPUT;
    }
    if(groupAlreadyExists(GroupID) == false || groupAlreadyExists(ReplacementID) == false) {
        return FAILURE;
    }
    node<Group_Key,Group_Info>* group = findGroup(GroupID);
    node<Group_Key,Group_Info>* replacementGroup = findGroup(ReplacementID);

    if(group->key.isEmpty == true && replacementGroup->key.isEmpty == true) {
        removeGroup(GroupID);
        return SUCCESS;
    }

    if(replacementGroup->key.isEmpty && group->info.size > 0) {
        // cout << "GroupID: " << replacementGroup->key.groupID << " is now not empty" << endl;
        //remove and insert replacementGroup with isEmpty = false:
        Group_Key group_key(true, replacementGroup->key.groupID);
        Group_Info group_info(0);
        Groups_AVL.removeNode(group_key);
        group_key.isEmpty = false;
        Groups_AVL.addNode(group_key, group_info);
        replacementGroup = findGroup(ReplacementID);
    }

    replacementGroup->info.Player_GroupID_AVL.mergeTree(group->info.Player_GroupID_AVL);

    // update_Player_in_group(replacementGroup->info.Player_GroupID_AVL.root, GroupID);
    update_Player_in_group(replacementGroup->info.Player_GroupID_AVL.root);

    // update the size of replacementGroup in groups_avl:
    replacementGroup->info.size += group->info.size;

    updateEveryGroupNode(replacementGroup->info.Player_GroupID_AVL.root, replacementGroup);

    // remove GroupID from groups_avl:
    group->info.Player_GroupID_AVL.treeClear();
    removeGroup(GroupID);


    return SUCCESS;    
}

StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease) {
    if(PlayerID <= 0 || LevelIncrease <= 0) {
        return INVALID_INPUT;
    }
    if(playerAlreadyExists(PlayerID) == false) {
        return FAILURE;
    }
    node<int,PlayerID_Info>* playerIDNode = PlayerID_AVL.search(PlayerID);
    int prevLevel = playerIDNode->info.level;
    int newLevel = prevLevel + LevelIncrease;
    // update the level of playerID in its node in PlayerID_AVL:
    playerIDNode->info.level = newLevel;

    // update the PlayerLevel_AVL:
    PlayerLevel_Key playerLevelKey(prevLevel, PlayerID);
    PlayerLevel_AVL.removeNode(playerLevelKey);
    playerLevelKey.level = newLevel;
    PlayerLevel_AVL.addNode(playerLevelKey, -1);
    playerIDNode->info.Player_in_level_avl = PlayerLevel_AVL.search(playerLevelKey);

    // update the Player_GroupID_AVL in corresponding group node:
    node<Group_Key,Group_Info>* group = playerIDNode->info.Player_in_group->info.groupNode; //TODO: check if valgrind errors
    PlayerLevel_Key playerLevelKey2(prevLevel, PlayerID);
    group->info.Player_GroupID_AVL.removeNode(playerLevelKey2);
    Player_GroupID_Info playerGroupIDInfo(PlayerID, newLevel, group, playerIDNode);
    playerLevelKey2.level = newLevel;
    group->info.Player_GroupID_AVL.addNode(playerLevelKey2, playerGroupIDInfo);
    playerIDNode->info.Player_in_group = group->info.Player_GroupID_AVL.search(playerLevelKey2);


    return SUCCESS;

}

StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID) {
    if(GroupID == 0) {
        return INVALID_INPUT;
    }
    else if(GroupID < 0) {
        if(numOfPlayers == 0) {
            *PlayerID = -1;
        }
        else {
            *PlayerID = PlayerLevel_AVL.biggest_node->key.ID;
        }
        return SUCCESS;
    } else {
        if(groupAlreadyExists(GroupID) == false) {
            return FAILURE;
        }
        node<Group_Key,Group_Info>* group = findGroup(GroupID);
        if(group->info.size == 0) {
            *PlayerID = -1;
        }
        else {
            *PlayerID = group->info.Player_GroupID_AVL.biggest_node->key.ID;
        }
        return SUCCESS;
    }
   return SUCCESS;
}

StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers) {
    
    if(GroupID == 0) {
        return INVALID_INPUT;
    }
    if(GroupID < 0) {
        int num_of_players = PlayerLevel_AVL.getNumberOfNodes();
        if(num_of_players == 0) {
            *numOfPlayers = 0;
            *Players = nullptr;
            return SUCCESS;
        }
        *numOfPlayers = num_of_players;
        *Players = (int*)malloc(sizeof(int)*num_of_players);
        if(*Players == NULL) {
            return ALLOCATION_ERROR;
        }
        int index = 0;
        fillArrayWithLevels_PlayerLevel_AVL(PlayerLevel_AVL.root, Players, &index, num_of_players);
        return SUCCESS;
    } else {
        // GroupID > 0:
        if(groupAlreadyExists(GroupID) == false) {
            return FAILURE;
        }
        node<Group_Key,Group_Info>* group = findGroup(GroupID);
        int num_of_players = group->info.Player_GroupID_AVL.getNumberOfNodes();
        if(num_of_players == 0) {
            *numOfPlayers = 0;
            *Players = nullptr;
            return SUCCESS;
        }
        *numOfPlayers = num_of_players;
        *Players = (int*)malloc(sizeof(int)*num_of_players);
        if(*Players == NULL) {
            return ALLOCATION_ERROR;
        }
        int index = 0;
        fillArrayWithLevels_Player_GroupID_AVL(group->info.Player_GroupID_AVL.root, Players, &index, num_of_players);
        return SUCCESS;
    }
    
}

StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players) {
    if(numOfGroups < 1) {
        return INVALID_INPUT;
    }
    //check if there are at least numOfGroups non-empty groups:
    if(checkNumOfNonEmptyGroups(numOfGroups) == false) {
        // cout<<"numOfGroups too high"<<endl;
        return FAILURE;
    }
    *Players = (int*)malloc(sizeof(int)*numOfGroups);
    if(*Players == NULL) {
        return ALLOCATION_ERROR;
    }

    int index = 0;
    fillArrayWithLevel_Group_AVL(Groups_AVL.root, Players, numOfGroups, &index);

    return SUCCESS;
}

void PlayersManager::Quit() {
    freeGroup_AVL_trees(Groups_AVL.root);
    Groups_AVL.treeClear();
    PlayerID_AVL.treeClear();
    PlayerLevel_AVL.treeClear();
}


/******** PRIVATE FUNCTIONS ********/
bool PlayersManager::groupAlreadyExists(int groupID) {
    //first search assuming group is empty:
    Group_Key groupKey(true, groupID);
    if(Groups_AVL.search(groupKey)) {
        return true;
    }
    //lastly, search assuming group is not empty:
    groupKey.isEmpty = false;
    if(Groups_AVL.search(groupKey)) {
        return true;
    }

    return false;
}
node<Group_Key,Group_Info>* PlayersManager::findGroup(int groupID) {
    //first search assuming group is empty:
    Group_Key groupKey(true, groupID);
    node<Group_Key,Group_Info>* group = Groups_AVL.search(groupKey);
    if(group != nullptr) {
        return group;
    }
    //lastly, search assuming group is not empty:
    groupKey.isEmpty = false;
    group = Groups_AVL.search(groupKey);
    return group;
}
void PlayersManager::removeGroup(int groupID) {
    //first search assuming group is empty:
    Group_Key groupKey(true, groupID);
    node<Group_Key,Group_Info>* group = Groups_AVL.search(groupKey);
    if(group != nullptr) {
        Groups_AVL.removeNode(groupKey);
        return;
    }
    //lastly, search assuming group is not empty:
    groupKey.isEmpty = false;
    group = Groups_AVL.search(groupKey);
    Groups_AVL.removeNode(groupKey);
}

bool PlayersManager::playerAlreadyExists(int playerID) {
    if(PlayerID_AVL.search(playerID)) {
        return true;
    }
    return false;
}

node<PlayerLevel_Key,Player_GroupID_Info>* PlayersManager::addPlayerToGroup(int groupID, int playerID, int level) {
    node<Group_Key,Group_Info>* group = findGroup(groupID);
    // cout<< "GroupID: " << groupID << " " <<  group->info.size<<endl;
    node<int,PlayerID_Info>* player_in_PlayerID_AVL = PlayerID_AVL.search(playerID);

    //TODO: problem here
    if(group->info.size == 0) {
        // cout << "GroupID: " << group->key.groupID << " is now not empty" << endl;
        //remove and insert with isEmpty = false.
        Group_Key group_key(true, groupID);
        Group_Info group_info(0);
        // cout << "empty" << findGroup(groupID) <<endl;
        Groups_AVL.removeNode(group_key);
        group_key.isEmpty = false;
        Groups_AVL.addNode(group_key, group_info);
        group = findGroup(groupID);
        // cout << "not empty" << group <<endl;
        
    }

    PlayerLevel_Key PlayerLevel(level, playerID);
    Player_GroupID_Info Player_GroupID_info(PlayerLevel.ID, PlayerLevel.level, group, player_in_PlayerID_AVL);
    group->info.Player_GroupID_AVL.addNode(PlayerLevel, Player_GroupID_info);
    group->info.Player_GroupID_AVL.search(PlayerLevel)->info.groupNode = group;
    group->info.size += 1;
    

    return group->info.Player_GroupID_AVL.search(PlayerLevel);
}

node<PlayerLevel_Key,int>* PlayersManager::addPlayerToPlayerLevelAVL(int playerID, int level) {
    PlayerLevel_Key playerLevelKey(level, playerID);
    PlayerLevel_AVL.addNode(playerLevelKey, -1);
    return PlayerLevel_AVL.search(playerLevelKey);
}

void PlayersManager::updateEveryGroupNode(node<PlayerLevel_Key, Player_GroupID_Info>* root, node<Group_Key,Group_Info>* replacementGroupNode) {
    if(root == nullptr) {
        return;
    }
    // updateEveryGroupNode(root->left_son, replacementGroupNode);
    // root->info.groupNode = replacementGroupNode;
    // updateEveryGroupNode(root->right_son, replacementGroupNode);
    updateEveryGroupNode(root->left_son, replacementGroupNode);
    root->info.groupNode = replacementGroupNode;
    updateEveryGroupNode(root->right_son, replacementGroupNode);
}

void PlayersManager::update_Player_in_group(node<PlayerLevel_Key, Player_GroupID_Info>* root, int oldGroupID) {
    if(root == nullptr) {
        return;
    }

    update_Player_in_group(root->left_son, oldGroupID);

    // cout<<root->info.playerNode_in_PlayerID_AVL->info.Player_in_group->info.groupNode->key.groupID<<endl;
    if(root->info.playerNode_in_PlayerID_AVL->info.Player_in_group->info.groupNode->key.groupID == oldGroupID) {
        // cout<<"Old Group ID found!" <<endl;
        root->info.playerNode_in_PlayerID_AVL->info.Player_in_group = root;
    }

    update_Player_in_group(root->right_son, oldGroupID);
}

void PlayersManager::update_Player_in_group(node<PlayerLevel_Key, Player_GroupID_Info>* root) {
    if(root == nullptr) {
        return;
    }

    update_Player_in_group(root->left_son);

    root->info.playerNode_in_PlayerID_AVL->info.Player_in_group = root;

    update_Player_in_group(root->right_son);
}

void PlayersManager::fillArrayWithLevels_PlayerLevel_AVL(node<PlayerLevel_Key,int>* root, int** Players, int* index, int size) {
    if(*index == size || root == nullptr) {
        return;
    }
    // reverse in-order traversal:
    fillArrayWithLevels_PlayerLevel_AVL(root->right_son, Players, index, size);

    (*Players)[*index] = root->key.ID;
    (*index)++;

    fillArrayWithLevels_PlayerLevel_AVL(root->left_son, Players, index, size);

}

void PlayersManager::fillArrayWithLevels_Player_GroupID_AVL(node<PlayerLevel_Key,Player_GroupID_Info>* root, int** Players, int* index, int size) {
    if(*index == size || root == nullptr) {
        return;
    }
    // reverse in-order traversal:
    fillArrayWithLevels_Player_GroupID_AVL(root->right_son, Players, index, size);

    (*Players)[*index] = root->key.ID;
    (*index)++;

    fillArrayWithLevels_Player_GroupID_AVL(root->left_son, Players, index, size);

}

bool PlayersManager::checkNumOfNonEmptyGroups(int numOfGroups) {
    int count = 0;
    numOfNonEmptyGroups(Groups_AVL.root, numOfGroups, &count);
    // cout<<"count: " <<count<<endl;
    if(count == numOfGroups) {
        return true;
    }
    return false;

}

void PlayersManager::numOfNonEmptyGroups(node<Group_Key,Group_Info>* root, int numOfGroups, int* count) {
    if(root == nullptr) {
        return;
    }
    numOfNonEmptyGroups(root->right_son, numOfGroups, count);
    if(*count == numOfGroups) {
        return;
    }
    else if(root->key.isEmpty == false) {
        (*count)++;
    }
    numOfNonEmptyGroups(root->left_son, numOfGroups, count);
}

void PlayersManager::fillArrayWithLevel_Group_AVL(node<Group_Key,Group_Info>* root, int** Players, int numOfGroups, int* index) {
    if(*index == numOfGroups || root == nullptr) {
        return;
    }
    // reverse in-order traversal:
    fillArrayWithLevel_Group_AVL(root->right_son, Players, numOfGroups, index);

    if(root->info.Player_GroupID_AVL.biggest_node == nullptr) {
        return;
    }
    if(*index >= numOfGroups) {
        return;
    }
    (*Players)[*index] = root->info.Player_GroupID_AVL.biggest_node->key.ID;
    (*index)++;

    fillArrayWithLevel_Group_AVL(root->left_son, Players, numOfGroups, index);

}

void PlayersManager::freeGroup_AVL_trees(node<Group_Key,Group_Info>* root) {
    if(root == nullptr) {
        return;
    }

    freeGroup_AVL_trees(root->left_son);

    if(root->info.Player_GroupID_AVL.root != nullptr) {
        root->info.Player_GroupID_AVL.treeClear();
    }

    freeGroup_AVL_trees(root->right_son);
}


// test functions :
void PlayersManager::printGroup(int groupID) {
    node<Group_Key,Group_Info>* group = findGroup(groupID);
    cout << "Group ID: " << group->key.groupID;
    cout << " | Size: " << group->info.size << " | isEmpty: " << group->key.isEmpty;
    cout << " | Players: " << endl;
    group->info.Player_GroupID_AVL.printReverseInOrder();
    cout << endl;
}

#endif /*PLAYERS_MANAGER_H*/


