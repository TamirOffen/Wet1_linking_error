
#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
using namespace std;

#include <cmath>
#include <memory>

template<class KeyType, class InfoType> 
struct node
{
    KeyType key;
    InfoType info;
    int height;
    node<KeyType, InfoType>* left_son;
    node<KeyType, InfoType>* right_son;
    node<KeyType, InfoType>* parent;

    node(): key(), info(), height(0), left_son(nullptr), right_son(nullptr), parent(nullptr) {};
    node(const KeyType new_key, const InfoType new_info) :key(new_key), info(new_info), height(0), left_son(nullptr),
        right_son(nullptr), parent(nullptr) {}

};

template<class KeyType, class InfoType>
class AVL
{
private:
    void destroyTree(node<KeyType, InfoType>* treeNode);
    void copyTree(node<KeyType, InfoType>* src, node<KeyType, InfoType>* dst);
    static node<KeyType, InfoType>* bstSearch(node<KeyType, InfoType>* treeNode, KeyType key);
    static node<KeyType, InfoType>* bstAddNode(AVL<KeyType, InfoType>* tree, KeyType key, InfoType info); 
    static node<KeyType, InfoType>* bstRemoveNode(AVL<KeyType, InfoType>* tree, KeyType key); 
    void populateAlmostPerfectAVL(node<KeyType,InfoType>* treeRoot, node<KeyType,InfoType>** mergedAB, int length, int* index);
    static void updateHeight(node<KeyType, InfoType>* treeNode);
    int getUpdatedHeightOfNode(node<KeyType,InfoType>* treeNode);
    static int bf(node<KeyType, InfoType>* treeNode);
    void fillArray(node<KeyType,InfoType>* treeRoot, node<KeyType,InfoType>** array, int* index);
    void rotateRight(node<KeyType, InfoType>* treeNode);
    void buildPerfectBinaryTree(node<KeyType,InfoType>* treeRoot, int perfectTreeSize);
    void rotateLeft(node<KeyType, InfoType>* treeNode);
    int getNumberOfNodes(node<KeyType,InfoType>* treeRoot);
    void printTree(node<KeyType,InfoType>* treeNode);
    void buildAlmostPerfectBinaryTree(node<KeyType,InfoType>** treeRoot, int numOfNodes);
    int balance(node<KeyType, InfoType>* treeNode);
    static node<KeyType, InfoType>* getNextNodeInOrder(node<KeyType, InfoType>* treeNode);
    void merge(node<KeyType,InfoType>** dst, node<KeyType,InfoType>** arr1, int length_arr1, node<KeyType,InfoType>** arr2, int length_arr2);
    static void swapNodes(node<KeyType, InfoType>* treeNode1, node<KeyType, InfoType>* treeNode2);
    node<KeyType,InfoType>* removeLastLeafs(node<KeyType,InfoType>* treeRoot, int* numOfLeafsToDelete);
    node<KeyType, InfoType>* getBiggestNode(node<KeyType, InfoType>* root);
    void printReverseInOrder(node<KeyType,InfoType>* treeNode);

public:
    node<KeyType, InfoType>* root; //root of the tree.
    node<KeyType, InfoType>* biggest_node; //keeps track of the biggest_node in the tree.
    
    AVL(); //c'tor
    ~AVL(); //d'tor
    AVL(const AVL& tree); //copy c'tor

    AVL& operator=(const AVL& tree); //assignment operator

    node<KeyType, InfoType>* search(const KeyType key) const;
    int addNode(const KeyType key, const InfoType info); 
    int removeNode(KeyType key); 

    void printTree() { printTree(root); }  
    void treeClear() { destroyTree(root); root = nullptr; }
    void printReverseInOrder() { printReverseInOrder(root); } 
    int getNumberOfNodes() { return getNumberOfNodes(root); }

    void mergeTree(const AVL& otherTree); //merges otherTree with this tree.


};

// public funcs:

template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::printTree(node<KeyType,InfoType>* treeNode) {
    if(treeNode == nullptr) return;
    printTree(treeNode->left_son);
    cout << treeNode->key << " BF: " << bf(treeNode) << " Height: " << treeNode->height << endl;//<<"|"<<height(treeNode)<< endl;
    printTree(treeNode->right_son);
}

template<class KeyType, class InfoType>
AVL<KeyType, InfoType>::AVL() : root(nullptr) , biggest_node(nullptr)
{
    // root = nullptr;
    // biggest_node = nullptr;
}

template<class KeyType, class InfoType>
AVL<KeyType, InfoType>::~AVL()
{
    destroyTree(root);
}

template<class KeyType, class InfoType>
AVL<KeyType, InfoType>::AVL(const AVL& tree)
{
    if (tree.root != nullptr)
    {
        root = new node<KeyType, InfoType>(tree.root->key, tree.root->info);
        root->height = tree.root->height;
        copyTree(tree.root, root);
    }
    else
    {
        root = nullptr;
    }
    biggest_node = getBiggestNode(root);
}

template<class KeyType, class InfoType>
AVL<KeyType, InfoType>& AVL<KeyType, InfoType>::operator=(const AVL<KeyType, InfoType>& tree)
{
    if (this == &tree)
    {
        return *this;
    }

    node<KeyType, InfoType>* temp = root;
    if (tree.root != nullptr)
    {
        node<KeyType, InfoType>* temp = new node<KeyType, InfoType>(tree.root->key, tree.root->info);
        copyTree(tree.root, temp);
    }
    else
    {
        node<KeyType, InfoType>* temp = nullptr;
    }

    destroyTree(root);
    root = temp;
    biggest_node = getBiggestNode(root);
    return *this;
}

template<class KeyType, class InfoType>
node<KeyType, InfoType>* AVL<KeyType, InfoType>::search(const KeyType key) const
{
    node<KeyType, InfoType>* found = bstSearch(root, key);
    if (found == nullptr || !(found->key == key))
    {
        return nullptr;
    }
    return found;
}


template<class KeyType, class InfoType>
int AVL<KeyType, InfoType>::addNode(const KeyType key, const InfoType info)
{
    node<KeyType, InfoType>* new_node = bstAddNode(this, key, info);
    if (new_node == nullptr) //if a node with the key is already in the tree
    {
        return -1;
    }

    while (new_node != root)  //search the node where the balance was broken
    {
        node<KeyType, InfoType>* new_node_father = new_node->parent;
        if (new_node_father->height >= (new_node->height + 1))
        {
            break;
        }

        new_node_father->height = new_node->height + 1;

        if (balance(new_node_father))
        {
            break;
        }

        new_node = new_node_father;
    }

    biggest_node = getBiggestNode(root);

    return 1;
}

template<class KeyType, class InfoType>
int AVL<KeyType, InfoType>::removeNode(KeyType key)
{
    if (root == nullptr)
    {
        return -1;
    }
    node<KeyType, InfoType>* father_of_deleted = bstRemoveNode(this, key);
    if (father_of_deleted == nullptr && root != nullptr) //the key was not found
    {
        return -1;
    }

    bool is_current_left_of_next;
    while (father_of_deleted != nullptr)
    {
        node<KeyType, InfoType>* next_node_in_path = father_of_deleted->parent;
        if (next_node_in_path != nullptr)
        {
            is_current_left_of_next = (next_node_in_path->left_son == father_of_deleted);
        }

        int initial_height = father_of_deleted->height;
        updateHeight(father_of_deleted);
        balance(father_of_deleted);
        if (next_node_in_path != nullptr)
        {
            if (is_current_left_of_next)
            {
                if (next_node_in_path->left_son->height == initial_height)
                {
                    break;
                }
            }
            else if (next_node_in_path->right_son->height == initial_height)
            {
                break;
            }
        }

        father_of_deleted = next_node_in_path;
    }

    biggest_node = getBiggestNode(root);

    return 1;
}

template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::mergeTree(const AVL& otherTree) {
    // get the number of nodes in each tree:
    int this_tree_size = getNumberOfNodes(root);
    int other_tree_size = getNumberOfNodes(otherTree.root);

    // sorted array of the nodes in both trees:
    int index_A = 0, index_B = 0;
    node<KeyType,InfoType>** A = new node<KeyType,InfoType>*[this_tree_size];
    node<KeyType,InfoType>** B = new node<KeyType,InfoType>*[other_tree_size];
    fillArray(root, A, &index_A);
    fillArray(otherTree.root, B, &index_B);

    // merge the arrays into 1 sorted array:
    node<KeyType,InfoType>** mergedAB = new node<KeyType,InfoType>*[this_tree_size + other_tree_size];
    merge(mergedAB, A, this_tree_size, B, other_tree_size);

    // build an empty AVL tree with 2n nodes:
    AVL<KeyType,InfoType> mergedAVL;
    buildAlmostPerfectBinaryTree(&(mergedAVL.root), this_tree_size + other_tree_size);

    // populate mergedAVL with values from mergedAB, in in-order traversal:
    int index_mergedAB = 0;
    populateAlmostPerfectAVL(mergedAVL.root, mergedAB, this_tree_size + other_tree_size, &index_mergedAB);
    // mergedAVL.printTree();

    // free and copy mergedAVL to this tree:
    treeClear();
    if(mergedAVL.root == nullptr) {
        root = nullptr;
    } else {
        root = new node<KeyType,InfoType>(mergedAVL.root->key, mergedAVL.root->info);
        // root = createNewNode(mergedAVL.root->key, mergedAVL.root->info);
        root->height = mergedAVL.root->height;
        // copyTree(mergedAVL.root, root);
        copyTree(mergedAVL.root, root);
    }

    biggest_node = getBiggestNode(root);

    // free the memory used:
    for(int i = 0; i < this_tree_size; i++) {
        delete A[i];
    }
    delete[] A;

    for(int i = 0; i < other_tree_size; i++) {
        delete B[i];
    }
    delete[] B;

    for(int i = 0; i < this_tree_size + other_tree_size; i++) {
        delete mergedAB[i];
    }
    delete[] mergedAB;

}



/***** private funcs *****/

template<class KeyType, class InfoType>
void AVL<KeyType, InfoType>::destroyTree(node<KeyType, InfoType>* treeNode)
{
    if (treeNode == nullptr)
    {
        return;
    }

    destroyTree(treeNode->left_son);
    destroyTree(treeNode->right_son);
    delete treeNode;
}

template<class KeyType, class InfoType>
void AVL<KeyType, InfoType>::copyTree(node<KeyType, InfoType>* src, node<KeyType, InfoType>* dst)
{
    if (src->left_son != nullptr)
    {
        dst->left_son = new node<KeyType, InfoType>(src->left_son->key, src->left_son->info);
        dst->left_son->height = src->left_son->height;
        dst->left_son->parent = dst;
        copyTree(src->left_son, dst->left_son);
    }
    if (src->right_son != nullptr)
    {
        dst->right_son = new node<KeyType, InfoType>(src->right_son->key, src->right_son->info);
        dst->right_son->height = src->right_son->height;
        dst->right_son->parent = dst;
        copyTree(src->right_son, dst->right_son);
    }
}


template<class KeyType, class InfoType>
node<KeyType, InfoType>* AVL<KeyType, InfoType>::bstSearch(node<KeyType, InfoType>* treeNode, KeyType key)
{
    node<KeyType, InfoType>* new_node = treeNode;
    if (new_node == nullptr) return nullptr;

    if (new_node->key == key) return new_node;
    if (key < new_node->key)
    {
        if (new_node->left_son == nullptr) return new_node;
        return bstSearch(new_node->left_son, key);
    }
    
    if (new_node->right_son == nullptr) return new_node;
    return bstSearch(new_node->right_son, key);
}

template<class KeyType, class InfoType>
node<KeyType, InfoType>* AVL<KeyType, InfoType>::bstAddNode(AVL<KeyType, InfoType>* tree, KeyType key, InfoType info)
{
    node<KeyType, InfoType>* father_of_new_node = bstSearch(tree->root, key);
    if (father_of_new_node == nullptr)
    {
        tree->root = new node<KeyType, InfoType>(key, info);
        return tree->root;
    }
    if (father_of_new_node->key == key) return nullptr;
    node<KeyType, InfoType>* new_node = new node<KeyType, InfoType>(key, info);
    new_node->parent = father_of_new_node;
    if (key < father_of_new_node->key) father_of_new_node->left_son = new_node;
    else father_of_new_node->right_son = new_node;

    return new_node;
}


template<class KeyType, class InfoType>
node<KeyType, InfoType>* AVL<KeyType, InfoType>::bstRemoveNode(AVL<KeyType, InfoType>* tree, KeyType key)
{
    node<KeyType, InfoType>* node_to_remove = bstSearch(tree->root, key);
    if (!(node_to_remove->key == key)) return nullptr;

    while (true)
    {
        node<KeyType, InfoType>* node_to_remove_father = node_to_remove->parent;
        if (node_to_remove->left_son == nullptr && node_to_remove->right_son == nullptr) //the node is a leaf
        {
            if (tree->root == node_to_remove) //the leaf is the root of the tree
            {
                tree->root = nullptr;
                delete node_to_remove;
                return nullptr;
            }

            if (node_to_remove_father->left_son == node_to_remove) node_to_remove_father->left_son = nullptr;
            else node_to_remove_father->right_son = nullptr;
            delete node_to_remove;
            return node_to_remove_father;
        }

        else if (node_to_remove->left_son != nullptr && node_to_remove->right_son != nullptr)
        {
            node<KeyType, InfoType>* next_node_in_order = getNextNodeInOrder(node_to_remove);
            swapNodes(next_node_in_order, node_to_remove);
            if (node_to_remove == tree->root) tree->root = next_node_in_order;
            continue;
        }

        else if (node_to_remove->left_son != nullptr)
        {
            if (node_to_remove_father == nullptr)
            {
                tree->root = node_to_remove->left_son;
                tree->root->parent = nullptr;
                delete node_to_remove;
                return tree->root;
            }
            else
            {
                if (node_to_remove_father->left_son == node_to_remove)
                {
                    node_to_remove_father->left_son = node_to_remove->left_son;
                    node_to_remove_father->left_son->parent = node_to_remove_father;
                }
                else
                {
                    node_to_remove_father->right_son = node_to_remove->left_son;
                    node_to_remove_father->right_son->parent = node_to_remove_father;
                }
                delete node_to_remove;
                return node_to_remove_father;
            }
        }
        else
        {
            if (node_to_remove_father == nullptr)
            {
                tree->root = node_to_remove->right_son;
                tree->root->parent = nullptr;
                delete node_to_remove;
                return tree->root;
            }
            else
            {
                if (node_to_remove_father->left_son == node_to_remove)
                {
                    node_to_remove_father->left_son = node_to_remove->right_son;
                    node_to_remove_father->left_son->parent = node_to_remove_father;
                }
                else
                {
                    node_to_remove_father->right_son = node_to_remove->right_son;
                    node_to_remove_father->right_son->parent = node_to_remove_father;
                }
                delete node_to_remove;
                return node_to_remove_father;
            }
        }
    }
}


template<class KeyType, class InfoType>
void AVL<KeyType, InfoType>::updateHeight(node<KeyType, InfoType>* treeNode)
{
    if (treeNode->left_son == nullptr && treeNode->right_son == nullptr)
    {
        treeNode->height = 0;
        return;
    }
    if (treeNode->left_son == nullptr)
    {
        treeNode->height = 1 + treeNode->right_son->height;
        return;
    }
    if (treeNode->right_son == nullptr)
    {
        treeNode->height = 1 + treeNode->left_son->height;
        return;
    }
    if (treeNode->left_son->height < treeNode->right_son->height)
    {
        treeNode->height = 1 + treeNode->right_son->height;
        return;
    }
    treeNode->height = 1 + treeNode->left_son->height;
}


template<class KeyType, class InfoType>
int AVL<KeyType, InfoType>::bf(node<KeyType, InfoType>* treeNode)
{   
    // balance factor = left_son - right_son (height)
    if (treeNode->left_son == nullptr && treeNode->right_son == nullptr)
        return 0;
    if (treeNode->left_son == nullptr)
        return -(treeNode->right_son->height + 1);
    if (treeNode->right_son == nullptr)
        return treeNode->left_son->height + 1;
    return treeNode->left_son->height - treeNode->right_son->height;
}


template<class KeyType, class InfoType>
void AVL<KeyType, InfoType>::rotateRight(node<KeyType, InfoType>* treeNode)
{
    node<KeyType, InfoType>* new_left_son = treeNode->left_son;
    new_left_son->parent = treeNode->parent;
    if (treeNode->parent != nullptr)
    {
        if (treeNode->parent->left_son == treeNode)
        {
            treeNode->parent->left_son = new_left_son;
        }
        else
        {
            treeNode->parent->right_son = new_left_son;
        }
    }
    else
    {
        root = new_left_son;
    }
    treeNode->left_son = new_left_son->right_son;
    if (treeNode->left_son != nullptr)
    {
        treeNode->left_son->parent = treeNode;
    }
    new_left_son->right_son = treeNode;
    treeNode->parent = new_left_son;
    updateHeight(treeNode);
    updateHeight(new_left_son);
}


template<class KeyType, class InfoType>
void AVL<KeyType, InfoType>::rotateLeft(node<KeyType, InfoType>* treeNode)
{
    node<KeyType, InfoType>* new_right_son = treeNode->right_son;
    new_right_son->parent = treeNode->parent;
    if (treeNode->parent != nullptr)
    {
        if (treeNode->parent->right_son == treeNode)
        {
            treeNode->parent->right_son = new_right_son;
        }
        else
        {
            treeNode->parent->left_son = new_right_son;
        }
    }
    else
    {
        root = new_right_son;
    }

    treeNode->right_son = new_right_son->left_son;
    if (treeNode->right_son != nullptr)
    {
        treeNode->right_son->parent = treeNode;
    }
    new_right_son->left_son = treeNode;
    treeNode->parent = new_right_son;

    updateHeight(treeNode);
    updateHeight(new_right_son);
}


template<class KeyType, class InfoType>
node<KeyType, InfoType>* AVL<KeyType, InfoType>::getNextNodeInOrder(node<KeyType, InfoType>* treeNode)
{   
    // go right and all the way left to find the next inorder
    node<KeyType, InfoType>* next_in_order = treeNode->right_son;
    while (next_in_order->left_son != nullptr)
    {
        next_in_order = next_in_order->left_son;
    }
    return next_in_order;
}


template<class KeyType, class InfoType>
void AVL<KeyType, InfoType>::swapNodes(node<KeyType, InfoType>* treeNode1, node<KeyType, InfoType>* treeNode2)
{   
    int node1_height = treeNode1->height;
    node<KeyType, InfoType>* node1_right = treeNode1->right_son;
    node<KeyType, InfoType>* node1_father = treeNode1->parent;
    node<KeyType, InfoType>* node1_left = treeNode1->left_son;

    treeNode1->parent = treeNode2->parent;
    treeNode1->right_son = treeNode2->right_son;
    treeNode1->height = treeNode2->height;
    treeNode1->left_son = treeNode2->left_son;

    treeNode2->left_son = node1_left;
    treeNode2->height = node1_height;
    treeNode2->parent = node1_father;
    treeNode2->right_son = node1_right;

    if (treeNode2->parent == treeNode2) //if node1 is the left_son of treeNode2 this happens
    {
        treeNode1->right_son = treeNode2;
        treeNode2->parent = treeNode1;
    }

    if (treeNode1->left_son != nullptr)
    {
        treeNode1->left_son->parent = treeNode1;
    }
    if (treeNode1->right_son != nullptr)
    {
        treeNode1->right_son->parent = treeNode1;
    }
    if (treeNode2->left_son != nullptr)
    {
        treeNode2->left_son->parent = treeNode2;
    }
    if (treeNode2->right_son != nullptr)
    {
        treeNode2->right_son->parent = treeNode2;
    }

    if (treeNode1->parent != nullptr)
    {
        if (treeNode1->parent->left_son == treeNode2)
        {
            treeNode1->parent->left_son = treeNode1;
        }
        else
        {
            treeNode1->parent->right_son = treeNode1;
        }
    }
    if (treeNode2->parent != nullptr)
    {
        if (treeNode2->parent->left_son == treeNode1)
        {
            treeNode2->parent->left_son = treeNode2;
        }
        else
        {
            treeNode2->parent->right_son = treeNode2;
        }
    }

}


template<class KeyType, class InfoType>
int AVL<KeyType, InfoType>::balance(node<KeyType, InfoType>* treeNode)
{   
    // if not balanced - rotate. balanced == |bf| <= 1
    int balance_factor = bf(treeNode);
    if (balance_factor == 2)
    {
        //int left_son_balance = bf(treeNode->left_son);
        if (bf(treeNode->left_son) >= 0)  //Rotate LL
        {
            rotateRight(treeNode);
        }
        else if (bf(treeNode->left_son) == -1) //Rotate LR
        {
            rotateLeft(treeNode->left_son);
            rotateRight(treeNode);
        }
        return 1;
    }
    if (balance_factor == -2)
    {
        //int right_balance = bf(treeNode->right_son);
        if (bf(treeNode->right_son) <= 0) //Rotate RR
        {
            rotateLeft(treeNode);
        }
        else if (bf(treeNode->right_son) == 1) //Rotate RL
        {
            rotateRight(treeNode->right_son);
            rotateLeft(treeNode);
        }
        return 1;
    }
    return 0;

}


template<class KeyType, class InfoType>
node<KeyType, InfoType>* AVL<KeyType, InfoType>::getBiggestNode(node<KeyType, InfoType>* treeNode)
{   
    // all the way right
    if (treeNode == nullptr) return treeNode;
    node<KeyType, InfoType>* current = treeNode;
    while (current->right_son != nullptr) current = current->right_son;
    return current;
}


template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::printReverseInOrder(node<KeyType,InfoType>* treeNode) {
    if(treeNode == nullptr) return;
    
    printReverseInOrder(treeNode->right_son);
    cout << treeNode->key << " BF: " << bf(treeNode) << " Height: " << treeNode->height << endl;
    printReverseInOrder(treeNode->left_son);
    
}

template<class KeyType, class InfoType>
int AVL<KeyType,InfoType>::getUpdatedHeightOfNode(node<KeyType,InfoType>* treeNode) {
    if(treeNode->left_son == nullptr && treeNode->right_son == nullptr) return 0;
    else if(treeNode->left_son == nullptr) return treeNode->right_son->height + 1;
    else if(treeNode->right_son == nullptr) return treeNode->left_son->height + 1;
    else {
        return max(treeNode->left_son->height, treeNode->right_son->height) + 1;
    }
}

template<class KeyType, class InfoType>
int AVL<KeyType,InfoType>::getNumberOfNodes(node<KeyType,InfoType>* treeRoot) {
    //postorder traversal
    if(treeRoot == nullptr) {
        return 0;
    }
    return 1 + getNumberOfNodes(treeRoot->left_son) + getNumberOfNodes(treeRoot->right_son);
}

// mergeTree helper funcs:
template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::fillArray(node<KeyType,InfoType>* treeRoot, node<KeyType,InfoType>** array, int* index) {
    if(treeRoot == nullptr) {
        return;
    }

    fillArray(treeRoot->left_son, array, index);
    array[*index] = new node<KeyType,InfoType>(treeRoot->key, treeRoot->info); 
    (*index)++;
    fillArray(treeRoot->right_son, array, index);
}

template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::merge(node<KeyType,InfoType>** dst, node<KeyType,InfoType>** arr1, int length_arr1, node<KeyType,InfoType>** arr2, int length_arr2) {
    int dst_index = 0, arr1_index = 0, arr2_index = 0;
    while(arr1_index < length_arr1 && arr2_index < length_arr2) {
        if(arr1[arr1_index]->key < arr2[arr2_index]->key) {
            dst[dst_index] = new node<KeyType,InfoType>(arr1[arr1_index]->key, arr1[arr1_index]->info);
            arr1_index++;
        } else {
            dst[dst_index] = new node<KeyType,InfoType>(arr2[arr2_index]->key, arr2[arr2_index]->info);
            arr2_index++;
        }
        dst_index++;
    }

    while(arr1_index < length_arr1) {
        dst[dst_index] = new node<KeyType,InfoType>(arr1[arr1_index]->key, arr1[arr1_index]->info);
        arr1_index++;
        dst_index++;
    }

    while(arr2_index < length_arr2) {
        dst[dst_index] = new node<KeyType,InfoType>(arr2[arr2_index]->key, arr2[arr2_index]->info);
        arr2_index++;
        dst_index++;
    }

}

template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::buildPerfectBinaryTree(node<KeyType,InfoType>* treeRoot, int height) {
    if(height == 0) {
        return;
    }

    // treeRoot->left_son = createNewNode(0,0);
    treeRoot->left_son = new node<KeyType,InfoType>;
    treeRoot->left_son->height = height-1;
    treeRoot->left_son->parent = treeRoot;
    // treeRoot->right_son = createNewNode(0,0);
    treeRoot->right_son = new node<KeyType,InfoType>;
    treeRoot->right_son->height = height-1;
    treeRoot->right_son->parent = treeRoot;

    buildPerfectBinaryTree(treeRoot->left_son, height-1);
    buildPerfectBinaryTree(treeRoot->right_son, height-1);
}

template<class KeyType, class InfoType>
node<KeyType,InfoType>* AVL<KeyType,InfoType>::removeLastLeafs(node<KeyType,InfoType>* treeRoot, int* numOfLeafsToDelete) {
    // we are doing backwards in-order traversal
    if(treeRoot == nullptr) {
        return nullptr;
    }
    if(*numOfLeafsToDelete == 0) {
        return treeRoot;
    }
    treeRoot->right_son = removeLastLeafs(treeRoot->right_son, numOfLeafsToDelete);
    if(treeRoot->height == 0) {
        //node is a leaf
        if(treeRoot->parent != nullptr) {
            if(treeRoot->parent->right_son == treeRoot) {
                treeRoot->parent->right_son = nullptr;
            } else {
                treeRoot->parent->left_son = nullptr;
            }
            treeRoot->parent->height = getUpdatedHeightOfNode(treeRoot->parent);
        }
        delete treeRoot;
        treeRoot = nullptr; 
        *numOfLeafsToDelete -= 1;
        return nullptr;
    }
    treeRoot->left_son = removeLastLeafs(treeRoot->left_son, numOfLeafsToDelete);
    return treeRoot;

}

template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::buildAlmostPerfectBinaryTree(node<KeyType,InfoType>** treeRoot, int numOfNodes) {
    int perfectTreeHeight = ceil(log2(1+numOfNodes)) - 1;
    int numOfNodesInPerfectTree = pow(2, perfectTreeHeight + 1) - 1;

    *treeRoot = new node<KeyType,InfoType>;
    (*treeRoot)->height = perfectTreeHeight;
    buildPerfectBinaryTree(*treeRoot, perfectTreeHeight);

    int numOfLeafsToDelete = numOfNodesInPerfectTree - numOfNodes;
    *treeRoot = removeLastLeafs(*treeRoot, &numOfLeafsToDelete);

}

template<class KeyType, class InfoType>
void AVL<KeyType,InfoType>::populateAlmostPerfectAVL(node<KeyType,InfoType>* treeRoot, node<KeyType,InfoType>** mergedAB, int length, int* index) {
    // in-order traversal:
    if(*index == length) {
        return;
    }
    if(treeRoot == nullptr) {
        return;
    }
    populateAlmostPerfectAVL(treeRoot->left_son, mergedAB, length, index);
    treeRoot->key = mergedAB[*index]->key;
    treeRoot->info = mergedAB[*index]->info;
    (*index)++;
    populateAlmostPerfectAVL(treeRoot->right_son, mergedAB, length, index);
}



#endif //AVL_TREE_H

