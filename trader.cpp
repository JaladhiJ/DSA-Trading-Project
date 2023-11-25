#include "receiver.h"
#include <vector>
#include <iostream>
#include <string>
#include<set>
using namespace std;

// Custom Map Class
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
};
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
    int value;
    Map* mapPtr;
    
    MyData(int v, Map* ptr) : value(v), mapPtr(ptr) {}
};

int main() {
    Receiver rcv;
    Map map;
    Map highestsell;
    Map lowestbuy;
   std:: string message="";
   while(true)
   {
     message+=rcv.readIML();
     if(message[message.size()-1]=='$')break;
   }
    std::istringstream iss(message);
    std::string input;
    while (std::getline(iss, input, '#')) {
        std::istringstream lineStream(input);
        std::string name, value, sb;
        lineStream>>name>>value>>sb;
        if(name[0]=='$')return 0;
        int n=stringToInt(value);
        if(!map.count(name))
        {
                map.insert(name,n);cout<<name<<" "<<n<<" ";
                if(sb[0]=='s')cout<<'b'<<endl;
                if(sb[0]=='b')cout<<'s'<<endl;
        }
        else
        {
            if(sb[0]=='s')
            {
                    if(n<map[name])
                    {
                        if((lowestbuy[name]==0||n<lowestbuy[name]) && (n!=highestsell[name] || !highestsell[name]))
                        { 
                            cout<<name<<" "<<n<<" "<<'b'<<endl;
                            lowestbuy.update(name,n);
                            map.update(name,n);
                        }
                        else
                        {
                            if(n==highestsell[name])
                            {
                                highestsell[name]=0;
                            }
                            cout<<"No Trade"<<endl;
                        }
                               
                    }
                    else
                    {
                        if(lowestbuy[name]==0)
                            {
                                lowestbuy.insert(name,n);
                            }
                        else if(n<lowestbuy[name])
                        {
                            lowestbuy.update(name,n);
                        }
                         cout<<"No Trade"<<endl;
                    }
            }
            else
            {
                    if(n>map[name])
                    {

                        if((highestsell[name]==0||n>highestsell[name]) && (n!=lowestbuy[name] || !lowestbuy[name]))
                        { 
                            cout<<name<<" "<<n<<" "<<'s'<<endl;
                            map.update(name,n);
                            highestsell.update(name,n);
                        }
                        else
                        {
                            if(n==lowestbuy[name])
                            {
                                lowestbuy[name]=100000;
                            }
                            cout<<"No Trade"<<endl;
                        }
                                
                    }
                    else
                    {
                        if(highestsell[name]==0)
                        {
                            highestsell.insert(name,n);
                        }
                        else if(n>highestsell[name])
                        {
                            highestsell.update(name,n);
                           
                        }
                         cout<<"No Trade"<<endl;
                    }
            }
        }
}
    return 0;
}
