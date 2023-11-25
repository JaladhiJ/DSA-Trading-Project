#include "receiver.h"
#include <vector>
#include <iostream>
#include <string>
#include<set>
using namespace std;
class Map {
private:
    Map* iterator(const string& first) {
        Map* temp = root;
        while (temp != nullptr && temp->first != first) {
            if (first < temp->first) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return temp;
    }

    const Map* iterator(const string& first) const {
        Map* temp = root;
        while (temp != nullptr && temp->first != first) {
            if (first < temp->first) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return temp;
    }

    const int search(const string& first) const {
        const Map* temp = iterator(first);
        if (temp != nullptr) {
            return temp->second;
        }
        return 0;
    }

    Map* create(const string& first) {
        Map* newnode = new Map;
        newnode->first = first;
        newnode->second = 0;
        newnode->left = nullptr;
        newnode->right = nullptr;
        newnode->par = nullptr;
        newnode->depth = 1;
        return newnode;
    }

    void right_rotation(Map* x) {
        Map* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->par = x;
        }
        if (x->par != nullptr && x->par->right == x) {
            x->par->right = y;
        }
        else if (x->par != nullptr && x->par->left == x) {
            x->par->left = y;
        }
        y->par = x->par;
        y->right = x;
        x->par = y;
    }

    void left_rotation(Map* x) {
        Map* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->par = x;
        }
        if (x->par != nullptr && x->par->left == x) {
            x->par->left = y;
        }
        else if (x->par != nullptr && x->par->right == x) {
            x->par->right = y;
        }
        y->par = x->par;
        y->left = x;
        x->par = y;
    }

    void helper(Map* node) {
        if (depthf(node->left) - depthf(node->right) > 1) {
            if (depthf(node->left->left) > depthf(node->left->right)) {
                node->depth = max(depthf(node->right) + 1, depthf(node->left->right) + 1);
                node->left->depth = max(depthf(node->left->left) + 1, depthf(node) + 1);
                right_rotation(node);
            }
            else {
                node->left->depth = max(depthf(node->left->left) + 1, depthf(node->left->right->left) + 1);
                node->depth = max(depthf(node->right) + 1, depthf(node->left->right->right) + 1);
                node->left->right->depth = max(depthf(node) + 1, depthf(node->left) + 1);
                left_rotation(node->left);
                right_rotation(node);
            }
        }
        else if (depthf(node->left) - depthf(node->right) < -1) {
            if (depthf(node->right->right) > depthf(node->right->left)) {
                node->depth = max(depthf(node->left) + 1, depthf(node->right->left) + 1);
                node->right->depth = max(depthf(node->right->right) + 1, depthf(node) + 1);
                left_rotation(node);
            }
            else {
                node->right->depth = max(depthf(node->right->right) + 1, depthf(node->right->left->right) + 1);
                node->depth = max(depthf(node->left) + 1, depthf(node->right->left->left) + 1);
                node->right->left->depth = max(depthf(node) + 1, depthf(node->right) + 1);
                right_rotation(node->right);
                left_rotation(node);
            }
        }
    }

    void balance(Map* node) {
        while (node != root) {
            int d = node->depth;
            node = node->par;
            if (node->depth < d + 1) {
                node->depth = d + 1;
            }
            if (node == root && depthf(node->left) - depthf(node->right) > 1) {
                if (depthf(node->left->left) > depthf(node->left->right)) {
                    root = node->left;
                }
                else {
                    root = node->left->right;
                }
                helper(node);
                break;
            }
            else if (node == root && depthf(node->left) - depthf(node->right) < -1) {
                if (depthf(node->right->right) > depthf(node->right->left)) {
                    root = node->right;
                }
                else {
                    root = node->right->left;
                }
                helper(node);
                break;
            }
            helper(node);
        }
    }

    int depthf(Map* node) {
        if (node == nullptr)
            return 0;
        return node->depth;
    }

    Map* insert(const string& first) {
        cnt++;
        Map* newnode = create(first);
        if (root == nullptr) {
            root = newnode;
            return root;
        }
        Map* temp = root, *prev = nullptr;
        while (temp != nullptr) {
            prev = temp;
            if (first < temp->first) {
                temp = temp->left;
            }
            else if (first > temp->first) {
                temp = temp->right;
            }
            else {
                delete newnode;
                cnt--;
                return temp;
            }
        }
        if (first < prev->first) {
            prev->left = newnode;
        }
        else {
            prev->right = newnode;
        }
        newnode->par = prev;
        balance(newnode);
        return newnode;
    }

    Map* inorderPredecessor(Map* head) {
        if (head == nullptr)
            return head;
        while (head->right != nullptr) {
            head = head->right;
        }
        return head;
    }

    Map* inorderSuccessor(Map* head) {
        if (head == nullptr)
            return head;
        while (head->left != nullptr) {
            head = head->left;
        }
        return head;
    }


public:
    class Map* root;
    int cnt;

    Map* left, *right, *par;
    string first;
    int second, depth;
    Map(){
        root = NULL;
        cnt = 0;
    }

    int& operator[](const string& key) {
        return insert(key)->second;
    }

    const int operator[](const string& key) const {
        return search(key);
    }

    int count(const string& key) {
        Map* temp = iterator(key);
        if (temp != nullptr) {
            return 1;
        }
        return 0;
    }

    int size(void) {
        return cnt;
    }

    void erase(const string& key) {
        Map* prev = nullptr;
        Map* temp = root;
        cnt--;
        while (temp != nullptr && temp->first != key) {
            prev = temp;
            if (key < temp->first) {
                temp = temp->left;
            }
            else if (key > temp->first) {
                temp = temp->right;
            }
        }
        if (temp == nullptr) {
            cnt++;
            return;
        }
        if (cnt == 0 && temp == root) {
            delete temp;
            root = nullptr;
            return;
        }
        Map* l = inorderPredecessor(temp->left);
        Map* r = inorderSuccessor(temp->right);
        if (l == nullptr && r == nullptr) {
            if (prev == nullptr) {
                root = nullptr;
            }
            else {
                if (prev->left == temp) {
                    prev->left = nullptr;
                }
                else {
                    prev->right = nullptr;
                }
                delete temp;
                balance(prev);
            }
            return;
        }
        Map* start;
        if (l != nullptr) {
            if (l == temp->left) {
                l->right = temp->right;
                if (l->right != nullptr) {
                    l->right->par = l;
                }
                start = l;
            }
            else {
                if (l->left != nullptr) {
                    l->left->par = l->par;
                }
                start = l->par;
                l->par->right = l->left;
                l->right = temp->right;
                l->par = nullptr;
                if (l->right != nullptr) {
                    l->right->par = l;
                }
                l->left = temp->left;
                temp->left->par = l;
            }
            if (prev == nullptr) {
                root = l;
            }
            else {
                if (prev->left == temp) {
                    prev->left = l;
                    l->par = prev;
                }
                else {
                    prev->right = l;
                    l->par = prev;
                }
                delete temp;
            }
            balance(start);
            return;
        }
        else {
            if (r == temp->right) {
                r->left = temp->left;
                if (r->left != nullptr) {
                    r->left->par = r;
                }
                start = r;
            }
            else {
                if (r->right != nullptr) {
                    r->right->par = r->par;
                }
                start = r->par;
                r->par->left = r->right;
                r->left = temp->left;
                r->par = nullptr;
                if (r->left != nullptr) {
                    r->left->par = r;
                }
                r->right = temp->right;
                temp->right->par = r;
            }
            if (prev == nullptr) {
                root = r;
            }
            else {
                if (prev->right == temp) {
                    prev->right = r;
                    r->par = prev;
                }
                else {
                    prev->left = r;
                    r->par = prev;
                }
                delete temp;
            }
            balance(start);
            return;
        }
    }
Map* getIthElement(int i) {
    return getIthElementUtil(Minimum(root), i);
}

Map* getIthElementUtil(Map* node, int &i) {
    if (node == nullptr)
        return nullptr;

    int position=i;
    if(position==0)return node;
    else{
        return getIthElementUtil(Successor(node),--position);
    }
}
    bool empty(void) {
        if (root == nullptr)
            return true;
        return false;
    }

    void update(const string& key, int value) {
        Map* temp = iterator(key);
        if (temp != nullptr) {
            temp->second = value;
        }
    }

    void clear(void) {
        while (root != nullptr) {
            erase(root->first);
        }
    }
    void iterate(){
        iterate2(root);
    }
    void iterate2(Map* head) {
        if (root == nullptr)
            return;
        if (head->left != nullptr) {
            iterate2(head->left);
        }
        cout << "Key: " << head->first << ", Value: " << head->second << ' ';
        if (head->right != nullptr) {
            iterate2(head->right);
        }
    }

    Map* find(const string& key) {
        return iterator(key);
    }

    void insert(const string& key, int value) {
        Map* temp = iterator(key);
        if (temp == nullptr) {
            insert(key)->second = value;
        }
        else {
            temp->second = value;
        }
    }
    Map* Minimum(Map* node) 
    {
        if (node == nullptr)
            return nullptr;

        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Map* Successor(Map* node)
     {
            if (node == nullptr)
                return nullptr;
            if (node->right != nullptr) {
                node = node->right;
                while (node->left != nullptr) {
                    node = node->left;
                }
                return node;
            }
            Map* parent = node->par;
            while (parent != nullptr && node == parent->right) {
                node = parent;
                parent = parent->par;
            }

        return parent;
    }
};

int pow(int x, int y) {
    if (y == 0) {
        return 1; 
    } else if (y > 0) {
        return x * pow(x, y - 1);
    } else {
        return 1 / pow(x, -y);
    }
}

int stringToInt(string str) {
    int result = 0;
    int sign = 1; 
    if (!str.empty() && str[0] == '-') {
        sign = -1;
        str = str.substr(1); 
    }

    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
        }
    }
    return result * sign;
}

struct MyData {
    char whatsit;
    int value;
    Map* mapPtr;
    
    MyData(char s, int v, Map* ptr) : whatsit(s), value(v), mapPtr(ptr) {}
};

std::string decimalToBinary(int n) {
    std::string binary = "";
    if (n == 0) {
        return "0";
    }

    while (n > 0) {
        binary = std::to_string(n % 2) + binary;
        n /= 2;
    }

    return binary;
}


int main()
{
    // Simulating the received message from your code
    Receiver rcv;
    std:: string message="";
   while(true)
    {
        message+=rcv.readIML();
        if(message[message.size()-1]=='$')break;
    }
    std::istringstream iss(message);
    std::string input;
    char delimiter = '#';
    std::vector<MyData*> dataVector;
    Map* firstone;
    firstone = new Map();
    dataVector.push_back(new MyData('s', 0, firstone));
    std::vector<int> deleter;
    int total=0;
    int maxval=0;
    int goodindex=-1;
    while (std::getline(iss, input, delimiter)) 
    {
        std::istringstream lineStream(input);
        std::string name, value;
        std::vector<std::pair<std::string, int>> nameValuePairs;
        
        while (lineStream >> name >> value) 
        {
            if (value[0] != 's' && value[0] != 'b') 
            {
                int n = std::stoi(value);
                nameValuePairs.push_back(std::make_pair(name, n));
            }
        }
        if (name[0] == '$') break;
        std::string price, sb;
        price = name;
        sb = value;

            Map* myMap;
            myMap = new Map();
        if(sb[0]=='s')
        {
            for (size_t i = 0; i < nameValuePairs.size(); i++) 
            {
                nameValuePairs[i].second = -1*nameValuePairs[i].second; 
            }
        }
        for (size_t i = 0; i < nameValuePairs.size(); i++) 
        {
            (*myMap)[nameValuePairs[i].first] = nameValuePairs[i].second;
        }
        int x = stringToInt(price);
        if(sb[0]=='s'){sb[0]='b';x=-1*x;}
        else if(sb[0]=='b'){sb[0]='s';}

        int checkc=1;
        string maxi=decimalToBinary(dataVector.size());
        for (size_t l = 0; l < maxi.size()-1; l++)
        {
            int t=pow(2,l);
            if(sb[0]==dataVector[t]->whatsit)
            {
                checkc=1;
                for(int i=0;i<myMap->size();i++)
                    {
                         auto it = dataVector[t]->mapPtr->find(nameValuePairs[i].first);
                                if (it != NULL)
                                {
                                   if(it->second!=nameValuePairs[i].second)
                                    {
                                        checkc=0;break;
                                    }
                                } 
                                else
                                {
                                    checkc=0;break;
                                }
                    }
                    if(sb[0]=='s' && checkc==1)
                    {
                        if(x>=dataVector[t]->value)
                        {
                            checkc=10+t;
                        }
                        else
                        {
                            checkc=2;
                        }
                    }
                    else if(sb[0]=='b' && checkc==1)
                    {
                        if(x>dataVector[t]->value){checkc=2;}
                        else
                        {
                            checkc=10+t;
                        }
                    }
                if(checkc>0)
                {
                    for (size_t i = 0; i < deleter.size(); i++)
                    {
                        if((t & deleter[i])!=0)
                        { 
                            checkc=0;
                        }
                    }
                }
                if(checkc>0)break;  
            }
        }
        if(checkc==2)
        {
            continue;
        }  
        else if(checkc>9)
        {
            deleter.push_back(checkc-10);
        }

        dataVector.push_back(new MyData(sb[0],x, myMap)); 
        int k=dataVector.size();
        for(int j=1;j<k-1;j++)
        {
                    Map* mine;
                    mine = new Map();
                    bool chec=0;
                    for (size_t i = 0; i < deleter.size(); i++)
                    {
                        if((j & deleter[i])!=0)
                        { 
                            dataVector.push_back(new MyData('s', 0, mine));chec=1;
                            break;
                        }
                    }
                    if(chec==1)continue;
                    
                    for(int i=0;i<myMap->size();i++)
                    {
                         auto it = dataVector[j]->mapPtr->find(nameValuePairs[i].first);
                                if (it != NULL)
                                {
                                   (*mine)[nameValuePairs[i].first]=it->second+nameValuePairs[i].second;
                                } 
                                else
                                {
                                    (*mine)[nameValuePairs[i].first]=nameValuePairs[i].second;
                                }
                    }
                    for (size_t i = 0; i < dataVector[j]->mapPtr->size(); i++)
                    {
                        auto it = mine->find(dataVector[j]->mapPtr->getIthElement(i)->first);
                        if (it != NULL)
                                {
                                   continue;
                                } 
                                else
                                {
                                    (*mine)[dataVector[j]->mapPtr->getIthElement(i)->first]=dataVector[j]->mapPtr->getIthElement(i)->second;
                                }
                    }
                    int valuedat=0;
                    valuedat=dataVector[j]->value+dataVector[k-1]->value;
                    dataVector.push_back(new MyData('s', valuedat, mine));
                    
                    if(valuedat>0)
                    {
                           for (size_t z = 0; z < mine->size(); z++)
                            {
                                int j=0;
                                bool checks=0;
                                while(mine->getIthElement(j)!=nullptr)
                                {
                                    if(mine->getIthElement(j)->second!=0)checks=1;
                                    j++;
                                }
                                if(maxval<valuedat && checks==0)
                                {
                                    maxval=valuedat;
                                    goodindex=dataVector.size()-1;
                                }
                            }
                    }
        }
        if(maxval==0)cout<<"No Trade"<<endl;
        else if(maxval>0)
        {
            string printer=decimalToBinary(goodindex);
            for(int i=0; i<printer.size(); i++)
            {
                if(printer[i]=='0'){continue;}
                int r=pow(2,printer.size()-1-i);
                deleter.push_back(r);
                int j=0;
                if(dataVector[r]->whatsit=='b')
                {
                    while(dataVector[r]->mapPtr->getIthElement(j)!=nullptr)
                    {
                        cout<<dataVector[r]->mapPtr->getIthElement(j)->first<<" "<<-1*dataVector[r]->mapPtr->getIthElement(j)->second<<" ";
                        j++;
                    }
                    cout<<-1*dataVector[r]->value<<" "<<dataVector[r]->whatsit<<"#"<<endl;
                }
                if(dataVector[r]->whatsit=='s')
                {
                    while(dataVector[r]->mapPtr->getIthElement(j)!=nullptr)
                    {
                        cout<<dataVector[r]->mapPtr->getIthElement(j)->first<<" "<<dataVector[r]->mapPtr->getIthElement(j)->second<<" ";
                        j++;
                    }
                    cout<<dataVector[r]->value<<" "<<dataVector[r]->whatsit<<"#"<<endl;
                }
            }
            total+=maxval;
            maxval=0;
            goodindex=-1;
        }
        auto it = dataVector.back()->mapPtr->find("X");
    }
    cout<<total<<endl;
    for (MyData* item : dataVector) {
        delete item;
    }

    return 0;
}
