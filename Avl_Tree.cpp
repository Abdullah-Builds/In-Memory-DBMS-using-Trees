#include<iostream>
#include "Database_Class.cpp"
using namespace std;
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
            cout << "ID already exists. Cannot Insert Duplicates" << endl;
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
            cout << "ID " << ID << " not found in the tree." << endl;
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
            cout << "ID " << id << " not found." << endl;
            return;
        }
        if (node->person->ID == id)
        {
            cout << "ID: " << node->person->ID << "  Name: " << node->person->Name << "  Age: " << node->person->Age << endl;
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
        if (node == nullptr)
        {
            cout << "ID " << key << " not found." << endl;
        }
        if (node->person->ID == key)
        {

            node->person->Name = name;
            node->person->Age = age;
            cout << "Information updated successfully!" << endl;
            return;
        }
        if (key < node->person->ID)
        {
            UpdateNode(node->left, key, age, name);
        }
        else
        {
            UpdateNode(node->right, key, age, name);
        }
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
            cout << "ID " << ID << " Deleted" << endl;
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