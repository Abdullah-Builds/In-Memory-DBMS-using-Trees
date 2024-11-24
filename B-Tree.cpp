#include <iostream>
#include "Database_Class.cpp"
using namespace std;

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
            cout << "ID already exists. Cannot Insert Duplicate." << endl;
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
                cout << "ID: " << node->persons[i]->ID << " " << ",Name: " << node->persons[i]->Name << " " << ",Age: " << node->persons[i]->Age << endl;
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

        cout << "ID " << id << " not found." << endl;
    }

   void Update(int key, int age, string name)
{
    if (root == nullptr)
    {
        cout << "Tree is empty!" << endl;
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
            cout << "Found ID: " << key << endl;
            if (node->persons[i] != nullptr)
            {
                node->persons[i]->Age = age;
                node->persons[i]->Name = name;
                cout << "Information updated successfully!" << endl;
                return;
            }
            else
            {
                cout << "Error: No person associated with this ID!" << endl;
                return;
            }
        }

        // Move to the next child node if it's not a leaf, otherwise break.
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

    cout << "ID " << key << " not found." << endl;
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
                cout << "ID " << originalKey << " Deleted." << endl;
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
            cout << "ID " << key << " not found." << endl;
        }
    }

    void Delete(int key)
    {
        if (root == nullptr)
        {
            cout << "Tree is Empty!" << endl;
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

int main()
{
    B_TREE_TABLE btree;

    // Inserting some sample data
    btree.Insert(1, 25, "John Doe");
    btree.Insert(2, 30, "Jane Smith");
    btree.Insert(3, 22, "Sam Green");

    // Searching for an ID
    cout << "Searching for ID 1:" << endl;
    btree.Search(1);  // Should print the details of ID 1

    cout << "\nSearching for ID 4:" << endl;
    btree.Search(4);  // Should indicate that ID 4 was not found

    // Updating an existing ID
    cout << "\nUpdating ID 1's details:" << endl;
    btree.Update(9, 26, "Johnathan Doe");  // Should update successfully

    // Trying to update a non-existing ID
    cout << "\nUpdating ID 4's details:" << endl;
    btree.Update(4, 28, "Unknown Person");  // Should indicate that ID 4 is not found

    return 0;
}
