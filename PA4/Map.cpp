#include "Map.h"

bool create = false;
int balanceCounter = 0;
std::vector<Isle*> deleteIsle;

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }

    while(!nodes.empty()) {
        const auto last = nodes.front();

        if(nodes.front()->left !=nullptr) {
            nodes.push(nodes.front()->left);
        }
        if(nodes.front()->right !=nullptr) {
            nodes.push(nodes.front()->right);
        }
        delete last;
        nodes.pop();
    }

}

void Map::initializeMap(std::vector<Isle *> isles)
{
    for(const auto& s:isles) {
        insert(s);
    }
    populateWithItems();
    create = true;
}


MapNode *Map::rotateRight(MapNode *current)
{
    if (current == nullptr || current->left == nullptr) {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode* x = current->left;
    MapNode* y = x->right;
    x->right = current;
    current->left = y;

    const int leftHeight = height(current->left);
    const int rightHeight = height(current->right);
    current->height = 1 + std::max(leftHeight, rightHeight);

    const int newLeftHeight = height(x->left);
    const int newRightHeight = height(x->right);
    x->height = 1 + std::max(newLeftHeight, newRightHeight);

    return x;
}
MapNode *Map::rotateLeft(MapNode *current)
{
    if (current == nullptr || current->right == nullptr) {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode* x = current->right;
    MapNode* y = x->left;
    x->left = current;
    current->right= y;

    const int leftHeight = height(current->left);
    const int rightHeight = height(current->right);
    current->height = 1 + std::max(leftHeight, rightHeight);

    const int newLeftHeight = height(x->left);
    const int newRightHeight = height(x->right);
    x->height = 1 + std::max(newLeftHeight, newRightHeight);

    return x;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    return (node != nullptr) ? node->height : 0;
}

void Map::dist() {
    if(create) {
        populateWithItems();
        dropItemBFS();
    }

}
MapNode *Map::insert(MapNode *node, Isle *isle)
{
    if(node == nullptr) {
        return new MapNode(isle);
    }
    if (isle->getName() == node->isle->getName()) {
        return node;
    }

    if (isle->getName() > node->isle->getName()) {
        node->right = insert(node->right,isle);
    }
    else {
        node->left = insert(node->left,isle);
    }

    const int leftHeight = height(node->left);
    const int rightHeight = height(node->right);

    node->height = 1 + std::max(leftHeight, rightHeight);

    const int balance = leftHeight - rightHeight;
    if(balance>1 && height(node->left->left) >= height(node->left->right)) {
        if(create)
            balanceCounter++;
        return rotateRight(node);
    }
    if(balance>1 && height(node->left->left) < height(node->left->right)) {
        if(create)
            balanceCounter++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if(balance<-1 && height(node->right->left) <= height(node->right->right)) {
        if(create)
            balanceCounter++;
        return rotateLeft(node);
    }
    if(balance<-1 && height(node->right->left) > height(node->right->right)) {
        if(create)
            balanceCounter++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}   

void Map::insert(Isle *isle)
{
    if(isle != nullptr) {
        root = insert((root), isle);
        if(balanceCounter % 3 == 0 && balanceCounter>0) {
            dist();
        }
    }
}

MapNode* findSuccessor(MapNode* node) {
    MapNode* current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}

MapNode *findParent(MapNode *node, Isle *isle) {
    if(node == nullptr || node->isle->getName() == isle->getName()) {
        return nullptr;
    }
    if ((node->left && node->left->isle->getName() == isle->getName()) || (node->right && node->right->isle->getName() == isle->getName()))
        return node;

    if (isle->getName() < node->isle->getName())
        return findParent(node->left, isle);

    return findParent(node->right, isle);
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    if(node == nullptr) {
        return node;
    }
    if (isle->getName() == node->isle->getName()) {
        if ((node->left == nullptr) || (node->right == nullptr)) {
            MapNode* temp = node->left ? node->left : node->right;
            const auto parentNode = findParent(root,isle);
            if(parentNode != nullptr) {
                if(parentNode->left == node) {
                    parentNode->left = nullptr;
                }else {
                    parentNode->right = nullptr;
                }
            }
            delete node;
            return temp;
        }
        const MapNode* temp= findSuccessor(node->left);
        node->isle = temp->isle;
        node->right = remove(node->left, temp->isle);

    }else if (isle->getName() > node->isle->getName()) {
        node->right = remove(node->right,isle);
    }
    else {
        node->left = remove(node->left,isle);
    }

    const int leftHeight = height(node->left);
    const int rightHeight = height(node->right);

    const int balance = leftHeight-rightHeight;
    node->height = 1 + std::max(leftHeight, rightHeight);

    if(balance>1 && height(node->left->left) > height(node->left->right)) {
        if(create)
            balanceCounter++;
        return rotateRight(node);
    }
    if(balance>1 && height(node->left->left) < height(node->left->right)) {
        if(create)
            balanceCounter++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if(balance<-1 && height(node->right->left) < height(node->right->right)) {
        if(create)
            balanceCounter++;
        return rotateLeft(node);
    }
    if(balance<-1 && height(node->right->left) > height(node->right->right)) {
        if(create)
            balanceCounter++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;

}

void Map::remove(Isle *isle)
{
    if(isle != nullptr) {
        if(root== nullptr) {
            std::cout << "[Remove] " << "Tree is Empty" << std::endl;
        }else {
            root = remove((root), isle);
            if(balanceCounter%3 == 0 && balanceCounter>0) {
                dist();
            }
        }
    }
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if(current != nullptr) {
        count++;
        if(count%5 ==0) {
            current->isle->setItem(EINSTEINIUM);

            std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
        }
        preOrderItemDrop(current->left,count);
        preOrderItemDrop(current->right,count);
    }
}


// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    if(current != nullptr) {
        postOrderItemDrop(current->left,count);
        postOrderItemDrop(current->right,count);
        count++;
        if(count%3 == 0) {
            current->isle->setItem(GOLDIUM);
            std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
        }
    }
}


MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }

    while(!nodes.empty()) {
        const auto last = nodes.front();

        if(nodes.front()->left !=nullptr) {
            nodes.push(nodes.front()->left);
        }
        if(nodes.front()->right !=nullptr) {
            nodes.push(nodes.front()->right);
        }
        nodes.pop();
        if(last->isle->getItem() == EMPTY) {
            return last;
        }
    }
    return nullptr;
}

void Map::dropItemBFS()
{
    const auto node = findFirstEmptyIsle(root);
    if(node != nullptr) {
        node->isle->setItem(AMAZONITE);
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << node->isle->getName() << std::endl;
    }else {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    if (node != nullptr) {
        return std::max(getDepth(node->left),getDepth(node->right))+1;
    }
    return -1;
}

int Map::getIsleDepth(Isle *isle)
{
    const MapNode *current = root;
    int depth = 0;

    while (current != nullptr) {
        if (current->isle == isle) {
            return depth;
        }
        if (isle->getName() < current->isle->getName()) {
            current = current->left;
        } else if (isle->getName() > current->isle->getName()) {
            current = current->right;
        }
        depth++;
    }
    return -1;
}

int Map::getDepth()
{
    return getDepth(root);
}

void Map::populateWithItems()
{
    int count = 0;
    postOrderItemDrop(root,count);
    count = 0;
    preOrderItemDrop(root,count);
}

Isle *Map::findIsle(Isle isle)
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }
    else
        return nullptr;

    while(!nodes.empty()) {
        const auto last = nodes.front();
        if(last->isle->getName() == isle.getName()) {
            return last->isle;
        }
        if(nodes.front()->left !=nullptr) {
            nodes.push(nodes.front()->left);
        }
        if(nodes.front()->right !=nullptr) {
            nodes.push(nodes.front()->right);
        }
        nodes.pop();
    }
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }
    else
        return nullptr;

    while(!nodes.empty()) {
        const auto last = nodes.front();
        if(last->isle->getName() == name) {
            return last->isle;
        }
        if(nodes.front()->left !=nullptr) {
            nodes.push(nodes.front()->left);
        }
        if(nodes.front()->right !=nullptr) {
            nodes.push(nodes.front()->right);
        }
        nodes.pop();
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }
    else
        return nullptr;

    while(!nodes.empty()) {
        const auto last = nodes.front();
        if(last->isle == &isle) {
            return last;
        }
        if(nodes.front()->left !=nullptr) {
            nodes.push(nodes.front()->left);
        }
        if(nodes.front()->right !=nullptr) {
            nodes.push(nodes.front()->right);
        }
        nodes.pop();
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }
    else
        return nullptr;

    while(!nodes.empty()) {
        const auto last = nodes.front();
        if(last->isle->getName() == name) {
            return last;
        }
        if(nodes.front()->left !=nullptr) {
            nodes.push(nodes.front()->left);
        }
        if(nodes.front()->right !=nullptr) {
            nodes.push(nodes.front()->right);
        }
        nodes.pop();
    }

    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}



void Map::inOrderTraversal(MapNode* current, std::ofstream &outFile)
{
    if(current != nullptr) {
        inOrderTraversal(current->left,outFile);
        outFile << current->isle->getName() << std::endl;
        inOrderTraversal(current->right,outFile);
    }
}

void Map::breadthFirstTraversal(std::ofstream &outFile)
{
    std::queue<MapNode*> nodes;
    if(root != nullptr) {
        nodes.push(root);
    }

    int counter = 0;
    int power = 2;
    while(!nodes.empty()) {
        counter++;
        const auto last = nodes.front();
        if(last != nullptr) {
            nodes.push(nodes.front()->left);
            nodes.push(nodes.front()->right);
            outFile << last->isle->getName();
        }else {
            outFile << "NULL";
        }
        if(counter == power-1){
            outFile <<std::endl;
            power *=2;
            if(log2(power) == getDepth()+2) {
                break;
            }
        }
        else
            outFile <<" ";
        nodes.pop();
    }
}

void Map::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    breadthFirstTraversal(file);
    std::cout << "[Output] " << "Isles have been written to " << filename << " according to rankings." << std::endl;
}

void Map::writeIslesToFile(const std::string &filename)
{
    std::ofstream file(filename);
    inOrderTraversal(root,file);
    std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}