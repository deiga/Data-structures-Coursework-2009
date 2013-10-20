#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

// ------------- Data types ----------

struct node {
  
  // ------------- Variables ------------
  
  node* parent;
  node* left;
  node* right;
  int weight;
  int index;
  unsigned char val;
  
  // ------------- Constructors ---------

  node() : parent(NULL), left(NULL), right(NULL), weight(0), val(0) {}
  node(int weig, int ind) : parent(NULL), left(NULL), right(NULL), weight(weig), index(ind), val(0) {}
  node(int weig, int ind, unsigned char mark) : parent(NULL), left(NULL), right(NULL), weight(weig), index(ind), val(mark) {}
  
  ~node() {
    parent = left = right = 0;
    weight = index = 0;
    val = 0;
  }
  
  // -------------- Operators ----------
  
  bool operator==(const node&);
  
};

struct btree {
  
  // ------------- Variables ------------
  
  node* root;
  node** nodetbl; // Taulukko, jossa viitteet kaikkii puussa oleviin lehtisolmuihin.
  int* weighttbl; // Taulukko, jossa kaikkien solmujen weight-arvot.
  
  // ------------- Constructors ---------

  btree() : root(new node(0,513)), nodetbl(new node*[257]), weighttbl(new int[513]) {
    for (int i = 0; i < 257; i++) {
      nodetbl[i] = NULL;
    }
    for (int i = 0; i < 513; i++) {
      weighttbl[i] = -1;
    }
    weighttbl[512] = 0; 
    nodetbl[256] = root;
  }
  
  ~btree();
  
  // ------------- Class functions -----------
  void incWeight(node*);
  void updateWeight(node*);
  void siblingChk(node*);
  void giveBirth(node*, unsigned char);
  void nodeSwitch(node*, node*);
  node* search(int, node*);
  void searchPath(node*, node*);
  node* followPath(char*);
  node* followPath(char, node*);
  bool isParent(node*, node*);
  
};


// ------------- General functions ----------

void insert(btree*, unsigned char);
void deltree(node*);
void openFile(char*);
std::string inorderTreeWalk(node*);
std::string postorderTreeWalk(node*);
std::string preorderTreeWalk(node*);
std::string buildStr(node*);
std::ostream& operator<<(std::ostream&, const btree&);
std::ostream& operator<<(std::ostream&, const node&); 
void bytobi(int);
int bitoby(char*);
void readFile(btree*,char*);
void writeToBuffer(char);

// ------------- Variables ------------------

std::ifstream ifilestr;
std::ofstream ofilestr;
char* cBuffer;                // GLoobaali puskuri, tiedostoon tallennettavalle datalle.
int buff_cnt = 0;             // Laskuri, kuinka monta merkkia on jo puskuriin tallennettu.
char code = 0;                // Muuttuja, joka kertoo onko kyseessä purku vaiko pakkaus operaatio.
