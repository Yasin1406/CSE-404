#include<bits/stdc++.h>
using namespace std;

int numPointer,key;
int hopsize=0;

struct Node{
    int totalKeys;
    Node *parentNode;
    Node *nextNode;
    Node **nodePointers;
    bool isLeaf;
    string *word;
    string *meaning;
};

Node *root;

Node *createNewNode(){
    Node *newnode=new Node();

    newnode->totalKeys = 0;
    newnode->parentNode = NULL;
    newnode->nextNode = NULL;
    newnode->nodePointers = new Node *[numPointer + 1];
    for (int i = 0; i <= numPointer; i++){
        newnode->nodePointers[i] = nullptr;
    }
    newnode->isLeaf = true;
    newnode->word = new string[key + 1];
    newnode->meaning = new string[key + 1];
    return newnode;
}

void createNewRoot(Node *node, string value, Node *leftChild, Node *rightChild){
    node = createNewNode();
    node->isLeaf = false;
    node->word[0] = value;
    node->totalKeys++;
    node->nodePointers[0] = leftChild;
    node->nodePointers[1] = rightChild;
    leftChild->parentNode = node;
    rightChild->parentNode = node;
    root = node;
}

Node *searchPlace(Node *node, string word){
    while(!node->isLeaf){
        int i;
        for(i = 0; i < node->totalKeys; i++){
            if(word < node->word[i]){
                break;
            }
        }
        node = node->nodePointers[i];
        hopsize++;
    }

    return node;
}


void addWithParent(Node *parent, string value, Node *rightChild){
    int track = parent->totalKeys;
    track--;

    if(track > -1){
        for(; track > -1; track--){
            if (parent->word[track] <= value){
                break;
            }
            else{
                parent->word[track + 1] = parent->word[track];
                parent->nodePointers[track + 2] = parent->nodePointers[track + 1];
            }
        }
    }
    parent->word[track + 1] = value;
    parent->nodePointers[track + 2] = rightChild;
    parent->totalKeys++;
}

void moveUp(Node *parent, string value, Node *leftChild, Node *rightChild){
    if(parent == NULL){
        createNewRoot(parent, value, leftChild, rightChild);    // The splitted node was the root
        return;
    }

    // parent exists, then add to the parent
    rightChild->parentNode = parent;
    addWithParent(parent, value, rightChild);

    // balancing in the parent node
    if(parent->totalKeys == numPointer){
        Node *secondNode = createNewNode();    // second node
        secondNode->isLeaf = false;

        int mark = parent->totalKeys, j = 0;

        for(int i = mark - (numPointer / 2); i < numPointer; i++){
            secondNode->word[j] = parent->word[i];
            if(j == 0){
                secondNode->nodePointers[0] = parent->nodePointers[i];   // look up later
                secondNode->nodePointers[0]->parentNode = secondNode;
            }

            secondNode->nodePointers[j + 1] = parent->nodePointers[i + 1];
            secondNode->nodePointers[j + 1]->parentNode = secondNode;
            j++;
        }

        parent->totalKeys -= (mark / 2 + 1);
        // parent->totalKeys -= (mark / 2);
        secondNode->totalKeys = (mark / 2);

        // move up recursively
        moveUp(parent->parentNode, parent->word[parent->totalKeys], parent, secondNode);
        // moveUp(parent->parentNode, secondNode->word[0], parent, secondNode);
    }
}

void addToLeaf(string word, string meaning){
    // searching for an appropriate place
    Node *leaf = searchPlace(root, word);

    // sorting and insertion
    int track = leaf->totalKeys;
    track--;

    if(track > -1){
        for(; track >- 1; track--){
            if(word < leaf->word[track]){
                leaf->word[track + 1] = leaf->word[track];
                leaf->meaning[track + 1] = leaf->meaning[track];
            }
            else break;
        }
    }
    leaf->word[track + 1] = word;
    leaf->meaning[track + 1] = meaning;
    leaf->totalKeys++;

    // balancing
    if(leaf->totalKeys == numPointer){
        Node *secondNode = createNewNode();    // second node

        int mark = leaf->totalKeys, j = 0;
        for(int i = mark - (numPointer / 2); i <numPointer; i++){
            secondNode->word[j] = leaf->word[i];
            secondNode->meaning[j] = leaf->meaning[i];
            j++;
        }

        //int v=leaf->totalKeys;
        leaf->totalKeys -= (numPointer / 2);
        secondNode->totalKeys =(numPointer / 2);
        secondNode->nextNode = leaf->nextNode;
        leaf->nextNode = secondNode;

        // move up
        moveUp(leaf->parentNode, secondNode->word[0], leaf, secondNode);
    }
}

void searchMeaning(){
    int i;
    string word;
    cout << "Enter 'x' to exit" << endl << endl;
    while(1){
        cout << "Enter the word: ";
        cin >> word;
        if(word == "x"){
            break;
        }
        for(i = 0; i < word.size(); i++){
            word[i] = tolower(word[i]);
        }
        hopsize = 0;
        Node *leaf = searchPlace(root, word);
        for(i = 0; i < leaf->totalKeys; i++){
            if(leaf->word[i] == word){
                break;
            } 
        }
        if(i == leaf->totalKeys){
            cout << "Sorry. Couldn't find the word" << endl;
        }
        else{
            cout << word << " means" << leaf->meaning[i] << endl;
        }
        cout << "Hop Size: " << hopsize << endl << endl;
    }
}

bool buildBPlusTree(){
    string word, meaning;
    root = createNewNode();

    ifstream filePtr;
    filePtr.open("dictionary.txt");

    if(!filePtr){
        cout << "Couldn't open the file!" << endl;
        return false;
    }
    else{
        while(filePtr >> word){
            getline(filePtr, meaning);
            addToLeaf(word, meaning);
        }
        filePtr.close();
    }

    return true;
}

int main(){
    cout << "Enter the number of pointers: ";
    cin >> numPointer;
    key = numPointer - 1;
    cout << endl;
    if(buildBPlusTree()){
        cout << "Tree has been built successfully." << endl << endl;
        // cout << "Hopsize: " << hopsize << endl <<endl;
        searchMeaning();
    }
    else{
        cout << "Sorry. Tree couldn't be built" << endl;
    }
}