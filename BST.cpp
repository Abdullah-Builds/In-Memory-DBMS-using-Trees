#include<iostream>
#include "Database_Class.cpp"
using namespace std;
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
            cout << "ID already exists. Cannot Insert Duplicates" << endl;
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
                cout << "ID: " << temp->p2->ID << ", Name: " << temp->p2->Name << ", Age: " << temp->p2->Age << endl;
                return;
            }
            temp = (key > temp->p2->ID) ? temp->right : temp->left;
        }
        cout << "ID " << key << " not found." << endl;
    }

    void Delete(int key)
    {
        root = Delete_Helper(root, key);
    }

    BST *Delete_Helper(BST *root, int key)
    {
        if (root == NULL)
        {
            cout << "ID " << key << " not found " << endl;
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
                cout << "ID " << key << " Deleted " << endl;
                delete root;
                return NULL;
            }
            if (root->left == NULL)
            {
                cout << "ID " << key << " Deleted " << endl;
                BST *temp = root->right;
                delete root;
                return temp;
            }
            if (root->right == NULL)
            {
                cout << "ID " << key << " Deleted " << endl;
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
        while (temp)
        {
            if (temp->p2->ID == key)
            {
                temp->p2->Name = name;
                temp->p2->Age = age;
                cout << "Information updated successfully!" << endl;
                return;
            }
            if (key > temp->p2->ID)
            {
                temp = temp->right;
            }
            else
            {
                temp = temp->left;
            }
        }
        cout << " ID " << key << " not found." << endl;
    }
};
