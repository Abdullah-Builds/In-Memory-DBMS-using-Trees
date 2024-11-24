#include <iostream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

/***********************************************---Database Class ----************************************************************ */
class Table
{
public:
    int ID;
    string Name;
    int Age;

    Table(int id, int age, string name)
    {
        this->ID = id;
        this->Name = name;
        this->Age = age;
    }

    Table()
    {
        this->ID = 0;
        this->Name = "";
        this->Age = 0;
    }
};

//****************************************----- B TREE -----************************************************************************** */

class BTreeNode
{
public:
    int value[3];
    BTreeNode *children[4];
    int count;
    bool isLeaf;
    Table *persons[3];

    BTreeNode(bool leaf)
    {
        count = 0;
        isLeaf = leaf;
        for (int i = 0; i < 4; i++)
        {
            children[i] = nullptr;
        }
        for (int i = 0; i < 3; i++)
        {
            persons[i] = nullptr;
        }
    }
};

class B_TREE_TABLE
{
private:
    BTreeNode *root;

    bool Check_Unique_ID(BTreeNode *node, int key)
    {
        if (node == nullptr)
        {
            return true;
        }
        int i = 0;
        while (i < node->count && key > node->value[i])
        {
            i++;
        }
        if (i < node->count && key == node->value[i])
        {
            return false;
        }
        if (node->isLeaf)
        {
            return true;
        }
        else
        {
            return Check_Unique_ID(node->children[i], key);
        }
    }

    void splitNode(BTreeNode *parent, int childIndex, BTreeNode *child)
    {
        BTreeNode *newChild = new BTreeNode(child->isLeaf);
        newChild->count = 1;
        newChild->value[0] = child->value[2];
        newChild->persons[0] = child->persons[2];

        if (!child->isLeaf)
        {
            newChild->children[0] = child->children[2];
            newChild->children[1] = child->children[3];
        }

        for (int i = parent->count; i > childIndex; i--)
        {
            parent->children[i + 1] = parent->children[i];
            parent->value[i] = parent->value[i - 1];
            parent->persons[i] = parent->persons[i - 1];
        }

        parent->children[childIndex + 1] = newChild;
        parent->value[childIndex] = child->value[1];
        parent->persons[childIndex] = child->persons[1];
        parent->count++;
        child->count = 1;
    }

    void NonFull(BTreeNode *node, int key, Table *person)
    {
        int i = node->count - 1;

        if (node->isLeaf)
        {
            while (i >= 0 && node->value[i] > key)
            {
                node->value[i + 1] = node->value[i];
                node->persons[i + 1] = node->persons[i];
                i--;
            }
            node->value[i + 1] = key;
            node->persons[i + 1] = person;
            node->count++;
        }
        else
        {
            while (i >= 0 && node->value[i] > key)
            {
                i--;
            }
            i++;
            if (node->children[i]->count == 3)
            {
                splitNode(node, i, node->children[i]);
                if (key > node->value[i])
                {
                    i++;
                }
            }
            NonFull(node->children[i], key, person);
        }
    }

public:
    B_TREE_TABLE()
    {
        root = nullptr;
    }

    void Insert(int id, int age, string name)
    {
        if (!Check_Unique_ID(root, id))
        {
            cout << "In B-Tree ID already exists. Cannot Insert Duplicate." << endl;
            return;
        }
        Table *newPerson = new Table(id, age, name);

        if (root == nullptr)
        {
            root = new BTreeNode(true);
            root->value[0] = id;
            root->persons[0] = newPerson;
            root->count = 1;
        }
        else
        {
            if (root->count == 3)
            {
                BTreeNode *newRoot = new BTreeNode(false);
                newRoot->children[0] = root;
                splitNode(newRoot, 0, root);
                int i = (id > newRoot->value[0]) ? 1 : 0;
                NonFull(newRoot->children[i], id, newPerson);
                root = newRoot;
            }
            else
            {
                NonFull(root, id, newPerson);
            }
        }
    }

    void Search(int id)
    {
        BTreeNode *node = root;

        while (node != nullptr)
        {
            int i = 0;
            while (i < node->count && id > node->value[i])
            {
                i++;
            }

            if (i < node->count && id == node->value[i])
            {
                cout << "In B-Tree ID: " << node->persons[i]->ID << " " << ",Name: " << node->persons[i]->Name << " " << ",Age: " << node->persons[i]->Age << endl;
                return;
            }
            if (node->isLeaf)
            {
                break;
            }
            else
            {
                node = node->children[i];
            }
        }

        cout << "In B-TreeID " << id << " not found." << endl;
    }

    void Update(int key, int age, string name)
    {
        if (root == nullptr)
        {
            cout << "Tree is Empty" << endl;
            return;
        }

        BTreeNode *node = root;
        while (node != nullptr)
        {
            int i = 0;
            while (i < node->count && key > node->value[i])
            {
                i++;
            }

            if (i < node->count && key == node->value[i])
            {
                if (node->persons[i] != nullptr)
                {
                    node->persons[i]->Age = age;
                    node->persons[i]->Name = name;
                    cout << "In B-Tree : Information updated successfully!" << endl;
                    return;
                }
                else
                {
                    return;
                }
            }
            if (node->isLeaf)
            {
                break;
            }
            else
            {
                if (node->children[i] != nullptr)
                {
                    node = node->children[i];
                }
                else
                {
                    break;
                }
            }
        }

        cout << "In B-Tree ID " << key << " not found." << endl;
    }

    void DeleteHelper(BTreeNode *node, int key, int originalKey)
    {
        int i = 0;
        while (i < node->count && key > node->value[i])
        {
            i++;
        }

        if (i < node->count && key == node->value[i])
        {
            if (node->isLeaf)
            {
                for (int j = i; j < node->count - 1; j++)
                {
                    node->value[j] = node->value[j + 1];
                    node->persons[j] = node->persons[j + 1];
                }
                node->count--;
                cout << "In B-Tree ID " << originalKey << " Deleted." << endl;
                return;
            }
            else
            {
                BTreeNode *predNode = node->children[i];
                if (predNode->count >= 2)
                {
                    int predKey = predNode->value[predNode->count - 1];
                    node->value[i] = predKey;
                    node->persons[i] = predNode->persons[predNode->count - 1];
                    DeleteHelper(predNode, predKey, originalKey);
                }
                else
                {
                    BTreeNode *succNode = node->children[i + 1];
                    if (succNode->count >= 2)
                    {
                        int succKey = succNode->value[0];
                        node->value[i] = succKey;
                        node->persons[i] = succNode->persons[0];
                        DeleteHelper(succNode, succKey, originalKey);
                    }
                    else
                    {
                        MergeNodes(predNode, succNode, node, i);
                        DeleteHelper(predNode, key, originalKey);
                    }
                }
            }
        }
        else if (!node->isLeaf)
        {
            DeleteHelper(node->children[i], key, originalKey);
        }
        else
        {
            cout << " In B-Tree ID " << key << " not found." << endl;
        }
    }

    void Delete(int key)
    {
        if (root == nullptr)
        {
            cout << " B-Tree is Empty!" << endl;
            return;
        }

        DeleteHelper(root, key, key);

        if (root->count == 0)
        {
            BTreeNode *oldRoot = root;
            if (root->isLeaf)
            {
                root = nullptr;
            }
            else
            {
                root = root->children[0];
            }
            delete oldRoot;
        }
    }

    void MergeNodes(BTreeNode *left, BTreeNode *right, BTreeNode *parent, int index)
    {

        left->value[left->count] = parent->value[index];
        left->persons[left->count] = parent->persons[index];
        left->count++;

        for (int i = 0; i < right->count; i++)
        {
            left->value[left->count] = right->value[i];
            left->persons[left->count] = right->persons[i];
            left->count++;
        }

        if (!left->isLeaf)
        {
            for (int i = 0; i <= right->count; i++)
            {
                left->children[left->count] = right->children[i];
                left->count++;
            }
        }

        for (int i = index; i < parent->count - 1; i++)
        {
            parent->value[i] = parent->value[i + 1];
            parent->persons[i] = parent->persons[i + 1];
            parent->children[i + 1] = parent->children[i + 2];
        }

        parent->count--;
        delete right;
    }
};
//******************************************----- AVL TREE ----************************************************************* */

class AVL
{
public:
    Table *person;
    AVL *left;
    AVL *right;
    int height;

    AVL(Table p)
    {
        person = new Table(p);
        left = nullptr;
        right = nullptr;
        height = 1;
    }

    AVL()
    {
        person = nullptr;
        left = nullptr;
        right = nullptr;
        height = 0;
    }

    ~AVL()
    {
        delete person;
    }
};

int height(AVL *node)
{
    return (node == nullptr) ? 0 : node->height;
}

int getBalance(AVL *node)
{
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
}

AVL *LeftRotate(AVL *root)
{
    AVL *x = root->left;
    AVL *y = x->right;
    x->right = root;
    root->left = y;
    root->height = max(height(root->left), height(root->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVL *RightRotate(AVL *root)
{
    AVL *x = root->right;
    AVL *y = x->left;
    x->left = root;
    root->right = y;
    root->height = max(height(root->left), height(root->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Table minValueNodePerson(AVL *node)
{
    AVL *current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return *current->person;
}

AVL *balanceNode(AVL *root)
{
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return LeftRotate(root);
    if (balance < -1 && getBalance(root->right) <= 0)
        return RightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = RightRotate(root->left);
        return LeftRotate(root);
    }
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = LeftRotate(root->right);
        return RightRotate(root);
    }
    return root;
}

bool Check_Unique_ID(AVL *root, int id)
{
    if (root == nullptr)
        return true;
    if (root->person->ID == id)
        return false;
    return Check_Unique_ID(root->left, id) && Check_Unique_ID(root->right, id);
}

class AVL_TREE_TABLE
{
private:
    AVL *root;

    AVL *InsertNode(AVL *node, int id, int age, string name)
    {
        if (!Check_Unique_ID(node, id))
        {
            cout << "In AVL-Tree ID already exists. Cannot Insert Duplicates" << endl;
            return node;
        }

        Table person(id, age, name);

        if (node == nullptr)
            return new AVL(person);

        if (id < node->person->ID)
            node->left = InsertNode(node->left, id, age, name);
        else if (id > node->person->ID)
            node->right = InsertNode(node->right, id, age, name);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        return balanceNode(node);
    }

    AVL *DeleteNode(AVL *node, int ID, bool found)
    {
        if (node == nullptr)
        {
            cout << "In AVL-Tree ID " << ID << " not found in the tree." << endl;
            return node;
        }

        if (ID < node->person->ID)
            node->left = DeleteNode(node->left, ID, found);
        else if (ID > node->person->ID)
            node->right = DeleteNode(node->right, ID, found);
        else
        {
            found = true;
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node;
                return nullptr;
            }

            if (node->left == nullptr)
            {
                AVL *temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr)
            {
                AVL *temp = node->left;
                delete node;
                return temp;
            }

            Table successorPerson = minValueNodePerson(node->right);
            node->person->ID = successorPerson.ID;
            node->person->Name = successorPerson.Name;
            node->person->Age = successorPerson.Age;

            node->right = DeleteNode(node->right, successorPerson.ID, found);
        }

        node->height = 1 + max(height(node->left), height(node->right));
        return balanceNode(node);
    }

    void SearchNode(AVL *node, int id) const
    {
        if (node == nullptr)
        {
            cout << "In AVL-Tree ID " << id << " not found." << endl;
            return;
        }
        if (node->person->ID == id)
        {
            cout << "In AVL-Tree ID: " << node->person->ID << "  Name: " << node->person->Name << "  Age: " << node->person->Age << endl;
            return;
        }
        if (id < node->person->ID)
        {
            SearchNode(node->left, id);
        }
        else
        {
            SearchNode(node->right, id);
        }
    }

    void UpdateNode(AVL *node, int key, int age, string name)
    {
        while (node != nullptr)
        {
            if (node->person->ID == key)
            {
                node->person->Name = name;
                node->person->Age = age;
                cout << "In AVL-Tree Information updated successfully!" << endl;
                return;
            }
            if (key < node->person->ID)
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        cout << "In AVL-Tree ID " << key << " not found." << endl;
    }

public:
    AVL_TREE_TABLE()
    {
        root = nullptr;
    }

    void Insert(int id, int age, string name)
    {
        root = InsertNode(root, id, age, name);
    }

    void Delete(int ID)
    {
        bool found = false;
        root = DeleteNode(root, ID, found);
        if (found)
        {
            cout << "In AVL-Tree ID " << ID << " Deleted" << endl;
        }
    }

    void Search(int id) const
    {
        SearchNode(root, id);
    }

    void Update(int key, int age, string name)
    {
        UpdateNode(root, key, age, name);
    }
};

//***********************************-----------BST --------------********************************************************************************** */
class BST
{
public:
    Table *p2;
    BST *left;
    BST *right;

    BST(Table s)
    {
        p2 = new Table(s);
        left = NULL;
        right = NULL;
    }
    BST()
    {
        left = NULL;
        right = NULL;
    }
};

class BST_TREE_TABLE
{
private:
    BST *root;

    BST *Build_Tree(BST *&root, Table *s)
    {
        if (root == NULL)
        {
            return root = new BST(*s);
        }

        if (s->ID < root->p2->ID)
        {
            root->left = Build_Tree(root->left, s);
        }
        else if (s->ID > root->p2->ID)
        {
            root->right = Build_Tree(root->right, s);
        }
        return root;
    }

    bool Check_Unique_ID(BST *root, int key)
    {
        if (root == NULL)
            return true;
        if (root->p2->ID == key)
            return false;
        return Check_Unique_ID(root->left, key) && Check_Unique_ID(root->right, key);
    }

    int MinValue(BST *root)
    {
        BST *temp = root;
        while (temp->left)
            temp = temp->left;
        return temp->p2->ID;
    }

public:
    BST_TREE_TABLE()
    {
        root = NULL;
    }

    void Insert(int id, int age, string name)
    {
        if (!Check_Unique_ID(root, id))
        {
           cout << "In BST ID already exists. Cannot Insert Duplicates" << endl;
            return;
        }

        Table *s = new Table(id, age, name);
        root = Build_Tree(root, s);
    }

    void Search(int key)
    {
        BST *temp = root;
        while (temp)
        {
            if (temp->p2->ID == key)
            {
                cout << "In BST ID: " << temp->p2->ID << ", Name: " << temp->p2->Name << ", Age: " << temp->p2->Age << endl;
                return;
            }
            temp = (key > temp->p2->ID) ? temp->right : temp->left;
        }
        cout << "In BST ID " << key << " not found." << endl;
    }

    void Delete(int key)
    {
        root = Delete_Helper(root, key);
    }

    BST *Delete_Helper(BST *root, int key)
    {
        if (root == NULL)
        {
            cout << "In BST ID " << key << " not found " << endl;
            return root;
        }

        if (key < root->p2->ID)
        {
            root->left = Delete_Helper(root->left, key);
        }
        else if (key > root->p2->ID)
        {
            root->right = Delete_Helper(root->right, key);
        }
        else
        {
            if (root->left == NULL && root->right == NULL)
            {
                cout << "In BST ID " << key << " Deleted " << endl;
                delete root;
                return NULL;
            }
            if (root->left == NULL)
            {
                cout << "In BST ID " << key << " Deleted " << endl;
                BST *temp = root->right;
                delete root;
                return temp;
            }
            if (root->right == NULL)
            {
                cout << "In BST ID " << key << " Deleted " << endl;
                BST *temp = root->left;
                delete root;
                return temp;
            }

            int mini = MinValue(root->right);
            root->p2->ID = mini;
            root->right = Delete_Helper(root->right, mini);
        }
        return root;
    }

    void Update(int key, int age, string name)
    {
        BST *temp = root;
        while (temp != NULL)
        {
            if (temp->p2->ID == key)
            {
                temp->p2->Name = name;
                temp->p2->Age = age;
                cout << "In BST Information updated successfully!" << endl;
                return;
            }
            if (key < temp->p2->ID)
            {

                temp = temp->left;
            }
            if (key > temp->p2->ID)
            {

                temp = temp->right;
            }
            else
            {
                break;
            }
        }
        cout << "In BST ID " << key << " not found." << endl;
    }
};

/*-----------------------------------------------  Helper Functions  ----------------------------------------------------*/

void Data_Generator(int n, vector<int> &ids, vector<int> &ages, vector<string> &names)
{
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < n; ++i)
    {
        ids.push_back(rand() % 9000 + 1000);
        ages.push_back(rand() % 100 + 1);
        string name = "Name" + to_string(rand() % 1000 + 1);
        names.push_back(name);
    }
}

template <typename Func, typename... Args>
double MeasureTimeTaken(Func func, Args &&...args)
{
    auto start = chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    return elapsed.count();
}

void Show_Data_Generator_Data(const vector<int> &ids, const vector<int> &ages, const vector<string> &names)
{
    for (size_t i = 0; i < ids.size(); ++i)
    {
        cout << "ID: " << ids[i] << ", Name: " << names[i] << ", Age: " << ages[i] << endl;
    }
}

/****************************************----  TIME COMPLEXITY ANALYZER -------********************************************************************** */

class Analyzer
{
public:
    void Deletion_Complexity()
    {
        cout << "---------------------------------------" << endl;
        cout << "Deletion Time Complexities: " << endl;
        cout << "BST       : O(log n) (Average), O(n) (Worst)" << endl;
        cout << "AVL Tree  : O(log n)" << endl;
        cout << "B Tree    : O(log n)" << endl;
        cout << "---------------------------------------" << endl;
    }
    void Insertion_Complexity()
    {
        cout << "---------------------------------------" << endl;
        cout << "Insertion Time Complexities: " << endl;
        cout << "BST       : O(log n) (Average), O(n) (Worst)" << endl;
        cout << "AVL Tree  : O(log n)" << endl;
        cout << "B Tree    : O(log n)" << endl;
        cout << "---------------------------------------" << endl;
    }
    void Search_Complexity()
    {
        cout << "---------------------------------------" << endl;
        cout << "Search Time Complexities: " << endl;
        cout << "BST       : O(log n) (Average), O(n) (Worst)" << endl;
        cout << "AVL Tree  : O(log n)" << endl;
        cout << "B Tree    : O(log n)" << endl;
        cout << "---------------------------------------" << endl;
    }
    void Update_Complexity()
    {
        cout << "---------------------------------------" << endl;
        cout << "Updation Time Complexities: " << endl;
        cout << "BST       : O(log n) (Average), O(n) (Worst)" << endl;
        cout << "AVL Tree  : O(log n)" << endl;
        cout << "B Tree    : O(log n)" << endl;
        cout << "---------------------------------------" << endl;
    }
};

/****************************************----   OPERATION CONTROL -------********************************************************************** */

class Operations_Control : public Analyzer
{
private:
    BST_TREE_TABLE t1;
    AVL_TREE_TABLE t2;
    B_TREE_TABLE t3;

public:
    void Insertion(vector<int> &id, vector<int> &age, vector<string> &name)
    {
        int n = id.size();
        double AVL_Time = MeasureTimeTaken([&]()
                                           {
            for (int i = 0; i < n; i++) {
                t1.Insert(id[i], age[i], name[i]);
            } });

        double BST_Time = MeasureTimeTaken([&]()
                                           {
            for (int i = 0; i < n; i++) {
                t2.Insert(id[i], age[i], name[i]);
            } });

        double B_TREE_Time = MeasureTimeTaken([&]()
                                              {
            for (int i = 0; i < n; i++) {
                t3.Insert(id[i], age[i], name[i]);
            } });

        cout << "---------------------------------------" << endl;
        cout << "Insertion Times (ms) " << endl;
        cout << "  AVL Tree: " << AVL_Time << " ms " << endl;
        cout << "  BST: " << BST_Time << " ms " << endl;
        cout << "  B-Tree: " << B_TREE_Time << " ms " << endl;
        cout << "---------------------------------------" << endl;
    }

    void Deletion(int key)
    {
        double AVL_Time = MeasureTimeTaken([&]()
                                           { t1.Delete(key); });
        double BST_Time = MeasureTimeTaken([&]()
                                           { t2.Delete(key); });
        double B_TREE_Time = MeasureTimeTaken([&]()
                                              { t3.Delete(key); });

        cout << "---------------------------------------" << endl;
        cout << "Deletion Times (ms) " << endl;
        cout << "  AVL Tree: " << AVL_Time << " ms " << endl;
        cout << "  BST: " << BST_Time << " ms " << endl;
        cout << "  B-Tree: " << B_TREE_Time << " ms " << endl;
        cout << "---------------------------------------" << endl;
    }

    void Updation(int key, int age, string name)
    {

        double AVL_Time = MeasureTimeTaken([&]()
                                           { t1.Update(key, age, name); });
        double BST_Time = MeasureTimeTaken([&]()
                                           { t2.Update(key, age, name); });
        double B_TREE_Time = MeasureTimeTaken([&]()
                                              { t3.Update(key, age, name); });

        cout << "---------------------------------------" << endl;
        cout << "Updation Times (ms) " << endl;
        cout << "  AVL Tree: " << AVL_Time << " ms " << endl;
        cout << "  BST: " << BST_Time << " ms " << endl;
        cout << "  B-Tree: " << B_TREE_Time << " ms " << endl;
        cout << "---------------------------------------" << endl;
    }

    void Searching(int key)
    {

        double AVL_Time = MeasureTimeTaken([&]()
                                           { t1.Search(key); });
        double BST_Time = MeasureTimeTaken([&]()
                                           { t2.Search(key); });
        double B_TREE_Time = MeasureTimeTaken([&]()
                                              { t3.Search(key); });

        cout << "---------------------------------------" << endl;
        cout << "Searching Times (ms) " << endl;
        cout << "  AVL Tree: " << AVL_Time << " ms " << endl;
        cout << "  BST: " << BST_Time << " ms " << endl;
        cout << "  B-Tree: " << B_TREE_Time << " ms " << endl;
        cout << "---------------------------------------" << endl;
    }
};

class Manage_System : public Operations_Control
{
private:
    vector<int> age;
    vector<int> id;
    vector<string> name;

public:
    Manage_System(vector<int> &id, vector<int> &age, vector<string> &name)
    {
        this->age = age;
        this->id = id;
        this->name = name;
    }

    void Insert_In_Database()
    {
        Insertion_Complexity();
        Insertion(id, age, name);
    }

    void Delete_In_Database()
    {
        Deletion_Complexity();
        int key;
        cout << "Enter ID to Delete: ";
        cin >> key;
        Deletion(key);
    }

    void Update_In_Database()
    {
        Update_Complexity();
        int key, age_;
        string name_;

        cout << "Enter ID to Update: ";
        cin >> key;
        cout << "Enter New Name: ";
        cin >> name_;
        cout << "Enter New Age: ";
        cin >> age_;

        Updation(key, age_, name_);
    }

    void Search_In_Database()
    {
        Search_Complexity();
        int choice;
        cout << "Do you want a custom search : " << endl;
        cout << "0 for Custom Search " << endl;
        cout << "1 for User Choice Search";
        cin >> choice;
        if (choice == 0)
        {
            for (int i = 0; i < id.size(); i++)
            {
                Searching(id[i]);
            }
        }
        if (choice == 1)
        {
            int key;
            cout << "Enter ID to Search : ";
            cin >> key;
            Searching(key);
        }
        else
        {
            cout << "Invalid Choice" << endl;
        }
    }
};

int main()
{
    int choice;
    vector<int> ids, ages;
    vector<string> names;

    int n;
    cout << "Enter the Size of Dummy data You Want : ";
    cin >> n;

    Data_Generator(n, ids, ages, names);
    Manage_System obj(ids, ages, names);
    Show_Data_Generator_Data(ids, ages, names);
    do
    {
        cout << endl
             << "**----- Database Management System ------**" << endl;
             cout << "ROLL NO : K23-0607, NAME : ABDULLAH KHAN " << endl;
             cout << "1. Insert into Database" << endl;
             cout<< "2. Delete from Database" << endl;
             cout<< "3. Update in Database" << endl;
             cout<< "4. Search in Database" << endl;
             cout<< "5. Exit" << endl;
             cout<< "---------------------------------------" << endl;
             cout<< "Enter your choice: " << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            cout << "---------------------------------------" << endl;
            obj.Insert_In_Database();
            break;
        case 2:
            system("cls");
            cout << "---------------------------------------" << endl;
            obj.Delete_In_Database();
            break;
        case 3:
            system("cls");
            cout << "---------------------------------------" << endl;
            obj.Update_In_Database();
            break;
        case 4:
            system("cls");
            cout << "---------------------------------------" << endl;
            obj.Search_In_Database();
            break;
        case 5:
            cout << "---------------------------------------" << endl;
            cout << "Exiting the system " << endl;
            break;
        default:
            cout << "Invalid choice. Please try again" << endl;
        }
    } while (choice != 5);
}