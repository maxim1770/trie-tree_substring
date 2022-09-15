#include <iostream>
#include <cctype>
#include <vector>

#define ALPHABET_SIZE 26
#define ROOT_LETTER '\0'

using namespace std;



struct ValueContainer
{
private:
    int value;

public:
    ValueContainer(int value)
    {
        this->value = value;
    }

    int getValue()
    {
        return this->value;
    }
};

struct TrieNode
{
private:
    char letter;
    ValueContainer* value;
    TrieNode* parent;
    TrieNode** children;

    int findChildIndex(char letter) //найти индекс ребенка
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode* node = this->children[i];
            if (node != nullptr && node->letter == letter)
                return i;
        }

        return -1;
    }

public:
    TrieNode(char letter, ValueContainer* value = nullptr) //констр
    {
        this->letter = letter;
        this->value = value;
        this->parent = nullptr;
        this->children = new TrieNode * [ALPHABET_SIZE];

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->children[i] = nullptr;
        }
    }

    TrieNode* findChild(char letter) //указатель на искомую букву
    {
        int index = this->findChildIndex(letter);

        return index > -1 ? this->children[index] : nullptr;
    }

    TrieNode* addChild(char letter, ValueContainer* value = nullptr)  //добавлдение буквы
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (!this->children[i])
            {
                TrieNode* node = new TrieNode(letter, value);
                node->parent = this;

                this->children[i] = node;

                return node;
            }
        }
        return nullptr;
    }

    void deleteChild(char letter) //удаление буквы
    {
        int index = this->findChildIndex(letter);
        if (index == -1)
            return;

        delete this->children[index];
        this->children[index] = nullptr;
    }

    void deleteSelf() //
    {
        if (this->parent)
            this->parent->deleteChild(this->letter);

        // call destructor
    }

    bool hasChildren() //проверка, есть ли у родителя дети
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode* node = this->children[i];
            if (node != nullptr)
                return true;
        }

        return false;
    }

    void print(TrieNode* node, string str)
    {
        TrieNode* current;
        current = node;
        if (node != nullptr)
        {
            str = str + current->letter;
            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (current->children[i] != nullptr)
                {
                    print(current->children[i], str);
                }
            }
            if (current->getValue() != nullptr)
                cout << "\n" << str;
        }
    }


    void setValue(ValueContainer* value) //установить значение
    {
        this->value = value;
    }

    ValueContainer* getValue() //получить указатель значения
    {
        return this->value;
    }


    TrieNode* getParent()
    {
        return this->parent;
    }

    char getLetter()
    {
        return this->letter;
    }

};

struct TrieTree
{
private:
    TrieNode* root;

    TrieNode* getNode(string key)
    {
        TrieNode* node = this->root;

        for (int i = 0; i < key.size(); i++)
        {
            char letter = key[i];
            node = node->findChild(letter);
            if (!node)
                return nullptr;
        }

        return node;
    }
    void printVector(vector<string> a)
    {
        for (int i = 0; i < a.size(); i++)
            cout << a[i] << endl;
    }
public:
    TrieTree()
    {
        this->root = new TrieNode(ROOT_LETTER);
    }

    ValueContainer* get(string key)
    {
        TrieNode* node = this->getNode(key);

        return node ? node->getValue() : nullptr;
    }

    bool set(string key, ValueContainer* value)
    {
        TrieNode* node = this->root;

        bool isNewBranch = false;
        for (int i = 0; i < key.size(); i++)
        {
            char letter = key[i];
            TrieNode* next = isNewBranch ? nullptr : node->findChild(letter);
            if (next)
                node = next;
            else
            {
                node = node->addChild(letter);
                isNewBranch = true;
            }
        }

        if (node)
            node->setValue(value);

        return node != nullptr;
    }

    bool remove(string key) //удалить слово
    {

        TrieNode* node = this->getNode(key);
        if (!node)
            return false;
        bool flague = false;
        TrieNode* current = node;

        while (current && current->getLetter() != ROOT_LETTER /*current->getValue() != nullptr*/)
        {
            TrieNode* parent = current->getParent();

            if (current->hasChildren())
            {
                if (current == node)
                    current->setValue(nullptr);
                return true;
            }
            if (current->getValue() != nullptr && current != node) break;
            current->deleteSelf();
            current = parent;
            flague = true;
        }

        return true;
    }

    void print()
    {
        string str = "";
        root->print(root, str);
    }

    bool check_substring(string substring, TrieNode* node = nullptr) {
        int l = substring.length();

        if (l == 0) {
            cout << "Length substring is zero.";
            return false;
        }

        if (!node->hasChildren()) {
            return false;
        }

        //TrieNode* T = nullptr;

        for (int i = 0; i < l; i++) {
            node = node->findChild(substring[i]);
            if (node == nullptr) {
                break;
            }

            //T = node->findChild(ROOT_LETTER);

        }

        if (node != nullptr) {
            return true;
        }

        //if (T != nullptr) {
        //    return true;
        //}


        return false;
    }


    bool search_words_with_substring(string substring) {
        int l = substring.length();

        if (l == 0) {
            cout << "Length substring is zero.";
            return false;
        }

        TrieNode* node = this->root;

        if (!node->hasChildren()) {
            return false;
        }

        if (this->check_substring(substring, node))
            return true;

        //for (int i = 0; i < l; i++) {
        //}

    }

};

int main()
{
    setlocale(LC_ALL, "rus");
    TrieTree tree;

    string current = ".";
    int count = 0;
    cout << "Input '0', if you want to stop adding words\n";
    cout << "Input words...\n";
    cin >> current;
    while (current != "0")
    {
        ValueContainer val(count);
        tree.set(current, &val);
        count++;
        cin >> current;
    }

    cout << "\nYour trie tree\n";
    cout << "//////////////";
    tree.print();
    cout << "\n//////////////";

    string substring_;
    cout << "\nInput substring...\n";
    cin >> substring_;
    if (tree.search_words_with_substring(substring_)) {
        cout << "найдено!" << endl;
    }


    return 0;
}
