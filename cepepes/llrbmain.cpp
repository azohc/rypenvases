#include "LLRBTree.h"
#include <iostream>
#include <sys/time.h>       // Measure execution time of code fragments
#include <vector>           // Key generation control
#include <unordered_map>    // Key generation control    
#include <fstream>          // Read example trees from files
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::vector;

const int SEARCH = 1;
const int INSERT = 2;
const int DELETE = 3;
 

// To measure the time elapsed during the execution of a section of code
typedef unsigned long long utime_t;
static utime_t get_time ()      
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return  now.tv_usec + (utime_t)now.tv_sec * 1000000;
}

template <class K, class V>
void printtree(LLRBTree<K,V> t)
{
    std::list<K> kl = t.get_keys_inorder();
    if(kl.empty())
    {
        cout << "tree is empty" << endl;
        return;
    }
    auto kit = kl.cbegin();
    std::list<V> vl = t.get_values_inorder();
    auto vit = vl.cbegin();

    cout << "(" << *kit++ << ", " << *vit++ << ")"; 
    while(kit != kl.cend())
        cout << ", (" << *kit++ << ", " << *vit++ << ")"; 
    cout << endl;
}

template <class K, class V>
LLRBTree<K,V> readtree(const std::string path)
{
    LLRBTree<int,char> t;
    ifstream f;
    f.open(path);

    if(f.is_open())
        cout << "Opening example tree " << path << "." << endl;
    else
    { 
        cout << "Failed to open file " << path << "." << endl 
            << "Returning empty tree..." << endl;
        return t;
    }

    K key;
    V value;
    while (!f.eof())
    {
        f >> key >> value;
        t.insert(key, value);
    }
    
    f.close();
    
    cout << endl << "loaded tree (in order walk): ";
    printtree(t);
    return t;
}


LLRBTree<int,char> opt_readtree()
{
    std::string path;
    cout << "enter a file path, 'casos/c1.txt', for example (don't include apostrophes!)." << endl
        << "file path: ";
    cin >> path;

    return readtree<int,char>(path);
}

double test_insert(int n)
{
    //make tree 
    LLRBTree<int,char> tree;
    std::unordered_map<int, int> keymap;    // Stores keys to ensure that every key in keys vector is unique
    vector<int> keys;
    char *vals;
    int k;
    double t;
    utime_t t0, t1;
    tree = LLRBTree<int,char>();
    vals = new char[n];
    for (int i = 0; i < n; i++)
    {
        k = rand() % n;
        if(!keymap[k])         // Only push keys that are not in the tree
        {
            keys.push_back(k);    
            keymap[k]++;
        }
        vals[k] = rand();    
    }

    //test insertions
    t = 0;
    k = keys.back();
    while(!keys.empty())
    {
        t0 = get_time();
        tree.insert(k, vals[k]);
        t1 = get_time();
        t += (t1 - t0)/1000.0L;
        keys.pop_back();
        k = keys.back();
    }
    cout << "time elapsed for " << n << " insertions: " << t << " ms." << endl;
    delete[]vals;
    return t;
}

void opt_tinsert()
{
    int n = -1;
    cout << "test_insert: performs a number of insertions on the tree" << endl; 
    
    while(n < 1)
    {
        cout << "enter the number of elements to test the tree with: ";
        cin >> n;
    }
    cout << endl;
    test_insert(n);
}

double test_search(int n)
{
    //make tree 
    LLRBTree<int,char> tree;
    std::unordered_map<int, int> keymap;    // Stores keys to ensure that every key in keys vector is unique
    vector<int> keys;
    char *vals;
    int k;
    double t;
    utime_t t0, t1;
    tree = LLRBTree<int,char>();
    vals = new char[n];
    for (int i = 0; i < n; i++)
    {
        k = rand() % n;
        if(!keymap[k])         // Only push keys that are not in the tree
        {
            keys.push_back(k);    
            keymap[k]++;
        }
        vals[k] = rand();    
        tree.insert(k, vals[k]);
    }

    //test searches
    t = 0;
    k = keys.back();
    while(!keys.empty())
    {
        t0 = get_time();
        if (tree.get(k) != vals[k])
            throw inexistent_key();
        t1 = get_time();
        t += (t1 - t0)/1000.0L;
        keys.pop_back();
        k = keys.back();
    }
    cout << "time elapsed for " << n << " searches: " << t << " ms." << endl;
    delete[]vals;
    return t;
}

void opt_tsearch()
{
    int n = -1;
    cout << "test_search: performs a number of searches on the tree" << endl; 
    
    while(n < 1)
    {
        cout << "enter the number of elements to test the tree with: ";
        cin >> n;
    }
    cout << endl;
    test_search(n);
}


double test_delete(int n)
{
    //make tree 
    LLRBTree<int,char> tree;
    std::unordered_map<int, int> keymap;    // Stores keys to ensure that every key in keys vector is unique
    vector<int> keys;
    char *vals;
    int k;
    double t;
    utime_t t0, t1;
    tree = LLRBTree<int,char>();
    vals = new char[n];
    for (int i = 0; i < n; i++)
    {
        k = rand() % n;
        if(!keymap[k])         // Only push keys that are not in the tree
        {
            keys.push_back(k);    
            keymap[k]++;
        }
        vals[k] = rand();    
        tree.insert(k, vals[k]);
    }

    //test searches
    t = 0;
    k = keys.back();
    while(!keys.empty())
    {
        t0 = get_time();
        tree.remove(k);
        t1 = get_time();
        t += (t1 - t0)/1000.0L;
        keys.pop_back();
        k = keys.back();
    }
    cout << "time elapsed for " << n << " deletions: " << t << " ms." << endl;
    delete[]vals;
    return t;
}

void opt_tdelete()
{
    int n = -1;
    cout << "test_delete: performs a number of deletions on the tree" << endl; 
    
    while(n < 1)
    {
        cout << "enter the number of elements to test the tree with: ";
        cin >> n;
    }
    cout << endl;
    test_delete(n);
}

void opt_insert(LLRBTree<int,char> &t)
{
    int n;
    char c;
    cout << "enter a key value pair (separated by space) to add to the tree: ";
    cin >> n >> c;
    
    t.insert(n, c);

    cout << endl << "resulting tree (in order walk): ";
    printtree(t);
}

void opt_search(LLRBTree<int,char> &t)
{
    int n;
    char c;
    cout << "enter a key to search in the tree: ";
    cin >> n;
    try
    {
        c = t.get(n);
    } catch (inexistent_key &e) {
        std::cerr << e.what() << endl;
        return;
    }

    cout << endl << "value associated to " << n << " = " << c << endl;
}

void opt_delete(LLRBTree<int,char> &t)
{
    int n;
    cout << "enter a key to delete from the tree: ";
    cin >> n;
    
    
    try
    {
        t.get(n);
    } catch (inexistent_key &e) {
        std::cerr << e.what() << endl;
        return;
    }

    t.remove(n);

    cout << endl << "resulting tree (in order walk): ";
    printtree(t);
}

void test_times(int tests, int elems, int op)
{
    ofstream ofspertest;
    ofspertest << std::fixed;
    ofstream ofsperop;
    ofsperop << std::fixed;
    double acc = 0;
    switch(op)
        {
            case INSERT :
                ofspertest.open("plots/testinsert.txt", std::ofstream::out | std::ofstream::app);
                ofsperop.open("plots/opinsert.txt", std::ofstream::out | std::ofstream::app);

                for (int i = 0; i < tests; i++)
                    acc += test_insert(elems); 
                 
                cout << "total time for " << tests << "x" << elems << " insertions: " << acc << endl;
                cout << "average time per " << elems << " insertions: " << acc/tests << " ms."<< endl;
                cout << "average time per single insertion: " << acc/(tests*elems) << " ms." << endl;
                break;
                
            case SEARCH :
                ofspertest.open("plots/testsearch.txt", std::ofstream::out | std::ofstream::app);       
                ofsperop.open("plots/opsearch.txt", std::ofstream::out | std::ofstream::app);       
                for (int i = 0; i < tests; i++)
                    acc += test_search(elems);

                cout << "total time for " << tests << "x" << elems << " searches: " << acc << endl;
                cout << "average time per " << elems << " searches: " << acc/tests << " ms."<< endl;
                cout << "average time per single search: " << acc/(tests*elems) << " ms." << endl;
                break;

            case DELETE :
                ofspertest.open("plots/testdelete.txt", std::ofstream::out | std::ofstream::app);
                ofsperop.open("plots/opdelete.txt", std::ofstream::out | std::ofstream::app);
                for (int i = 0; i < tests; i++)
                    acc += test_delete(elems);

                cout << "total time for " << tests << "x" << elems << " deletions: " << acc << endl;              
                cout << "average time per " << elems << " deletions: " << acc/tests << " ms." << endl;
                cout << "average time per single deletion: " << acc/(tests*elems) << " ms." << endl;
                break;
    }

    ofspertest << elems/1000 << "\t" << acc/tests << endl; 
    ofsperop << elems/1000 << "\t" << acc/(tests*elems) << endl; 
    
    ofspertest.close();
    ofsperop.close();
}


void opt_help()
{
    cout << "1. -> read tree from file" << endl
        << "\t load a tree from a text file. there are files containing trees in the folder test_trees." << endl
        << "\t you must provide the path to a file that has a format similar to the test_trees files." << endl << endl

        << "2. -> insert element" << endl
        << "\t provide an integer key and character value to insert into the tree." << endl << endl

        << "3. -> search element" << endl
        << "\t provide an integer key to search for its value in the tree." << endl << endl

        << "4. -> delete element" << endl
        << "\t provide an integer key to delete it from the tree." << endl << endl

        << "5. -> test insertion" << endl
        << "\t measure the execution time of insert operation by inserting key value pairs into a tree." << endl << endl

        << "6. -> test search" << endl
        << "\t measure the execution time of search operation by getting key value pairs from a tree." << endl << endl

        << "7. -> test deletion" << endl
        << "\t measure the execution time of delete operation by deleting key value pairs from a tree." << endl << endl

        << "8. -> print tree" << endl
        << "\t print current state of tree in order." << endl << endl

        << "9. -> print help" << endl
        << "\t print this message." << endl << endl;
}

void printmenu()
{
    cout << endl << "left leaning red black tree test menu" << endl 
        << "\t1. \t->\t read tree from file" << endl
        << "\t2. \t->\t insert element" << endl
        << "\t3. \t->\t search element" << endl
        << "\t4. \t->\t delete element" << endl
        << "\t5. \t->\t test insertion" << endl
        << "\t6. \t->\t test search" << endl
        << "\t7. \t->\t test deletion" << endl
        << "\t8. \t->\t print tree" << endl
        << "\t9. \t->\t print help" << endl
        << "\t0. \t->\t exit" << endl;
}
int getchoice()
{
    int choice = -1;

    cout << "enter your option: ";
    cin >> choice;
    while (choice < 0 || choice > 9)
    {
        cout << "enter your option (0 to 9): ";
        cin >> choice;
    }
    cout << endl;
    return choice;
}


int main()
{
    srand(time(NULL));
    cout << std::fixed;
    
    printmenu();
    int choice = getchoice();
    LLRBTree<int,char> tree;
    while(choice)
    {
        switch(choice)
        {
            case 1:
                tree = opt_readtree();
                break;
            case 2:
                opt_insert(tree);
                break;
            case 3:
                opt_search(tree);
                break;
            case 4:
                opt_delete(tree);
                break;
            case 5:
                opt_tinsert();
                break;
            case 6:
                opt_tsearch();
                break;
            case 7:
                opt_tdelete();
                break;
            case 8: 
                printtree(tree);
                break;
            case 9: 
                opt_help();
                break;
        }
        if(choice != 9)
            printmenu();
        choice = getchoice();
    }
    
    
    // int elems = 1000000;
    // for(int i = 0; i < 20; i++)
    // {
    //     elems += 500000;
    //     test_times(3, elems, INSERT);
    //     test_times(3, elems, SEARCH);
    //     test_times(3, elems, DELETE);
    // }

    return 0;
}