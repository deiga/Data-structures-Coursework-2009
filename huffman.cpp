#include "huffman.hpp"

// -------------- Constructors -----------------

/**
 * Destructor of the btree-struct. Calls deltree, which recursively goes
 * through the tree and deletes all nodes. Then delete both arrays in the struct.
**/
btree::~btree() {
   //delete[] nodetbl; // Kommentoitu pois, koska aiheuttaa segfaultin ohjelman ajossa.
   nodetbl = NULL;
   for (int i = 0; i < 513; i++ ) {
      weighttbl[i] = 0;
   }
   //delete[] weighttbl; // Kommentoitu pois, koska aiheuttaa segfaultin ohjelman ajossa.
   weighttbl = NULL;
   deltree(this->root);
}

// --------------- Member functions -------------

/**
 * Function incWeight, sets all weights of startingPoints ancestors to the values of its children.
 * the function recursively calls itself, until it reaches the root of the tree.
**/
void btree::incWeight(node* startPoint) {

   node* startParent;

   if (startPoint == 0) {
      std::cerr << "incWeight: Invalid pointer" << std::endl;
      return;
   }

   if (startPoint->parent != 0) {
      startParent = startPoint->parent;

      startParent->weight = startParent->left->weight;
      startParent->weight += startParent->right->weight;

      incWeight(startParent);
   }
   this->updateWeight(this->root);
}

/**
 * The function starts at the given paramter, and updates all weights of the nodes,
 * into corresponding cells of the weighttbl array. it works itself rekursively through the tree
 * using inorder-tree-walk.
**/
void btree::updateWeight(node* start) {

   int tmpindx = 0;

   if ( start != 0) {
      this->updateWeight(start->left);
      tmpindx = start->index;
      if ( start->weight != this->weighttbl[tmpindx] ) {
         this->weighttbl[tmpindx] = start->weight;
      }
      this->updateWeight(start->right);
   }

}

/**
 * The function beginns with a node and checks if it's the highest ordered of it's siblings.
 * If it is not, then it will change places with the highest ordered one.
 * The function runs recursively until it gets to the root node.
**/
void btree::siblingChk(node* toChange) {

   node* tmp;
   int tmpindx = 0;

   if ( toChange->parent != 0) {
      for (int i = 0; i < 513; i++) {

         if (this->weighttbl[i] == toChange->weight) {
            tmpindx = i;
         }
      }

      if ( tmpindx != 0  && tmpindx != toChange->index) {
         do {
            tmp = this->search(tmpindx, this->root);
            for (int i = tmpindx; i >= 0; i--) {
               if (this->weighttbl[i] == toChange->weight) {
                  tmpindx = i;
               }
            }
         } while (tmp == 0);
         this->nodeSwitch(toChange, tmp);
      }
      this->siblingChk(toChange->parent);
   }
   toChange->weight++;
}

/**
 * The function searches for a node with the given ind-parameter.
 * It runs form the root node all the way down to all leaf nodes.
 * It implements a form of post-order-tree-walk.
 * The returned node-pointer can be used to access the node that was searched for.
**/
node* btree::search(int ind, node* start) {

   node* tmp = NULL;
   node* found = NULL;

   if (ind > 513 || ind < 1) {
      std::cerr << "Bad index value!" << std::endl;
      return NULL;
   }
   if (start != 0) {
      tmp = this->search(ind, start->left);
      if ( tmp != 0 ) {
         found = tmp;
      }
      tmp = this->search(ind, start->right);
      if ( tmp != 0 ) {
         found = tmp;
      }
      if (start->index == ind) {
         return start;
      }

   }
   return found;
}

/**
 * The function takes 2 parameters, both are node-pointers. Parent is the parent of the node start.
 * It's use is to find the binary path to the given start-node.
**/
void btree::searchPath(node* parent, node* start) {

   if (parent != 0) {
      if (parent->parent != 0) {
         this->searchPath(parent->parent, parent);
      }

      if (start != 0) {
         if (start == parent->right) { writeToBuffer('1'); } 
         else if(start == parent->left) { writeToBuffer('0'); }
      }
   } 
}

/**
 * The followPath function, takes a char array as a parameter, the array is a binary path to some node.
 * Uses of the function is to find a node at the end of such a path.
 * It returns the node at the end of the path
**/
node* btree::followPath(char* path) {

   node* tmp = this->root;

   if (tmp->left == 0) { return NULL; }
   
   for (unsigned int i = 0; i < 8; i++) {
      if (path[i] == '1') { tmp = tmp->right; } 
      else if (path[i] == '0') { tmp = tmp->left; } 
   }
   return tmp;
}

/**
 * The followPath function, takes a char and a node-pointer as a parameter, the char tells the direction in the tree we need to go.
 * The node is the starting point of the search.
 * Uses of the function is to find a node at the end of 1 direction
 * It returns the node at the end of the direction
**/
node* btree::followPath(char direction, node* start) {

   node* tmp = 0;
   
   if (direction != '0' && direction != '1') { return tmp; }
   if (start->left == 0) { return tmp; }

   if (direction == '1') { tmp = start->right; } 
   else if (direction == '0') { tmp = start->left; }

   return tmp;
}

/**
 * Funktio isParent, tarkistaa onko parametri to, parametrin from vanhempi, jollain tasolla.
 * Jos ei ole, funktio palauttaa TRUE, muuten FALSE.
**/
bool btree::isParent(node* from, node* to) {

   node* tmp = from;

   if (from == to) { return false; } 
   else if (to->index < from->index) { return false; } 
   else if (to == this->root || from == this->root) { return true; }

  /**
   * While-silmukka kiertää, kunnes tmp-muuttuja saa puun juuren arvokseen.
   * Jos tmp-muuttujan vanhempi on yhtäkuin to-muutuja, palautetaan true.
   * Jos ei, jatketaan silmukkaa, kunnes silmukka loppuu tai vanhemmuus varmistetaan.
  **/
   while (tmp != this->root) {
      if (tmp->parent == to) { return true; }
      else { tmp = tmp->parent; }
   }
   return false;
}

/**
 * The function attempts to switch nodes form and to, so that the sibling property will hold again.
 * Both parameters are node-pointers.
**/
void btree::nodeSwitch(node* from, node* to) {

   if (from == 0 || to == 0) {
      std::cerr << "nodeSwitch: Invalid pointer!" << std::endl;
      return;
   }

   if (isParent(from, to)) { return; }

   int tmpind = 0;
   node* tmp;

   tmp = from->parent;
   from->parent = to->parent;
   to->parent = tmp;
   tmpind = from->index;
   from->index = to->index;
   to->index = tmpind;

   if (from->index%2 == 0) { from->parent->right = from; }
   else { from->parent->left = from; }
   
   if (to->index%2 == 0) { to->parent->right = to; }
   else { to->parent->left = to; }
}

/**
 * Function giveBirth() takes one node-pointer argument.
 * It doesn't return any values.
 * It creates two nodes as children of the argument node,
 * the left node becomes the new NYA node and the right node
 * is the new character inserted into the tree.
 * It also updates the places in the nodetbl of the tree.
**/

void btree::giveBirth(node* NYA, unsigned char toIns) {

   int mark = (int) toIns;
   node* newNYA;
   node* newChar;
   newNYA = new node(0, (NYA->index)-2);
   newNYA->parent = NYA;
   NYA->left = newNYA;
   newChar = new node(0, (NYA->index)-1, toIns);
   NYA->right = newChar;
   newChar->parent = NYA;
   this->nodetbl[mark] = newChar;
   this->nodetbl[256] = newNYA;
}

// --------------- Tree functions ---------------



/**
 * The insert function takes a btree and a unsigned char as parameters. The tree indicates, into which tree
 * it should insert. And the char is the sign of a character to be inserted into the tree.
 * It's use is to insert new nodes into the tree.
**/
void insert(btree* tree, unsigned char toIns) {

   int mark = (int) toIns;
   node* nodeToIns = tree->nodetbl[mark];
   node* NYA = tree->nodetbl[256];

   if ( nodeToIns == NULL ) {
      if ( code == 'e' ) { tree->searchPath(NYA->parent, NYA); }

      tree->giveBirth(NYA, toIns);
      nodeToIns = tree->nodetbl[mark];
      tree->siblingChk(nodeToIns);

      if (code == 'e') {
         bytobi(mark);           // Tämä kirjoittaa kirjaimen "bittiesityksen" tiedostoon.
         //writeToBuffer(toIns);   // Tämä kirjoittaa kirjaimet tiedostoon.
      } 
      else if (code == 'd') { writeToBuffer(toIns); }
   } else {

      if (code == 'e') { tree->searchPath(nodeToIns->parent, nodeToIns); }
      else if (code == 'd') { writeToBuffer(toIns); }

      tree->siblingChk(nodeToIns);
   }
   nodeToIns = tree->nodetbl[mark];
   tree->incWeight(nodeToIns);
}


/**
 * The function is a helper to the destructor of the btree struct.
 * it will empty the tree before deleting the tree.
 * It's 1 argument is supposed to be the root of a tree.
**/
void deltree(node* root) {

   if (root != 0) {
      deltree(root->left);
      deltree(root->right);
   }
   delete root;
   root = NULL;

}

// ------------ General functions ------------

/**
 * The openFile function, opens the file for reading anda new file fro writing. 
 * If there occurred errors it will ask the user for a new filename.
 *
**/
void openFile(char* file) {
   int len = strlen(file);
   char* binFile = new char[len+1];
   
   strncat(binFile, file,(strlen(file)-4));
   if ( strstr(file, ".bin") == NULL) { strcat(binFile,".bin"); } 
   else { strcat(binFile, "1.txt"); }

   do {
      ifilestr.open(file, std::fstream::in | std::fstream::binary);
      if (ifilestr.fail()) {
         std::cerr << "Unable to open file " << file << std::endl;
         std::cerr << "Please provide full path to file: " << std::endl;
         std::cin >> file;
      }
   } while (ifilestr.fail());
   ofilestr.open(binFile, std::fstream::out | std::fstream::binary);
}

/**
 * The function print's out the help message for the program.
**/
void printHelp() {

   std::cout << "You execute the program by runnign the shell script huffman.sh, "
      << "the script will automatically build the program." << std::endl;
   std::cout << "    huffman.sh [options]|filename|filepath" << std::endl << std::endl;
   std::cout << "    -d or --debug     Enters gdb to debug the program." << std::endl;
   std::cout << "    -h or --help      Prints this helptext." << std::endl;
   std::cout << "    filename or path  Runs the program with the given argument." << std::endl;
}

/**
 * The function doas an inorder-tree-walk on the tree and prints out all nodes.
**/
std::string inorderTreeWalk(node* startN) {

   node* l;
   node* r;
   std::string temp;

   if ( startN != 0) {
      l = startN->left;
      r = startN->right;

      temp += inorderTreeWalk(l);
      temp += buildStr(startN);
      temp += inorderTreeWalk(r);
   }
   return temp;
}

/**
 * The function doas an preorder-tree-walk on the tree and prints out all nodes.
**/

std::string preorderTreeWalk(node* startN) {

   std::string temp;
   node* l;
   node* r;

   if ( startN != 0) {
      l = startN->left;
      r = startN->right;

      temp += buildStr(startN);
      temp += preorderTreeWalk(l);
      temp += preorderTreeWalk(r);
   }
   return temp;
}

/**
 * The function doas an postorder-tree-walk on the tree and prints out all nodes.
**/

std::string postorderTreeWalk(node* startN) {

   std::string temp;
   node* l;
   node* r;

   if ( startN != 0) {
      l = startN->left;
      r = startN->right;

      temp += postorderTreeWalk(l);
      temp += postorderTreeWalk(r);
      temp += buildStr(startN);
   }

   return temp;
}

/**
 * The function builds a string from the nodes, so that it is easier to read.
**/
std::string buildStr(node* startN) {

   std::string temp;
   std::stringstream os;
   std::string ind_str;
   std::string wei_str;

   os << startN->index;
   ind_str = os.str();
   os.str("");
   os << startN->weight;
   wei_str = os.str();
   temp.append("( '");
   temp += startN->val;
   temp.append("' | ");
   temp.append(ind_str);
   temp.append(" | ");
   temp.append(wei_str);
   temp.append(" ) ");

   return temp;
}

/**
 * The function converts bytes to bits. Its parameter is an int whih depicts an ascii character.
**/
void bytobi(int data) {

   if (data < 0 || 255 < data) {
      std::cerr << "Inavlid integer!" << std::endl;
      return;
   }

   for (int i=0; i < 8; i++) {
      if (( (data & (1 << (7-i)) ) != 0 )) { writeToBuffer('1'); } else { writeToBuffer('0'); }
   }

}

/**
 * The function converts bits to bytes. Its parameter is a char array, which holds a bitqeue. 
 * The function returns the converted number as an int.
**/
int bitoby(char* bits) {

   int data = 0;

   if (bits == 0) { return 0; }

   for (int i = 0; i < 8; i++) {
      if (bits[i] == '1') 
         data += (1 << (7-i));
   }
   return data;
}

/**
 * Function readFile reads contents of file and gives it forward for coding.
 * Its parameters are the inot wich the coding will happen and a char array, depicting the filename to be read.
 * It performs different operations for encoding and decoding a file.
**/
void readFile(btree* tree, char* filen) {

   char bBuffer[8];
   char helpBuff[8];
   int help_cnt = 0;
   unsigned char merkki = 0;
   node* tmp = NULL;
   node* tmpN = NULL;
   node* hold = NULL;
   int read_cnt = 0;
   int left = 0;

   if (strstr(filen,".bin")) { // Jos tiedoston pääte on ".bin", niin tiedoston sisältö puretaan.
      code = 'd';
      while ( ifilestr.read(bBuffer, 8) ) { 
         merkki = 0;
         left = 8;
         
         if (read_cnt == 0) {  // Jos on ensimmmäinen lukukerta, niin ekat 8 merkkia on kirjain.
            merkki = bitoby(bBuffer); 
            for (int i = 0; i < 8; i++) { bBuffer[i] = 0;}
            left = 0;
         }
         else if (help_cnt != 0) { // Jos helpBufferiin on tallennettu jotain, mennään tänne.
            left = 8;
            for (int i = 0; i < 8; i++) {
               if (bBuffer[i] != 0 && help_cnt < 8) {
                  helpBuff[help_cnt] = bBuffer[i];
                  bBuffer[i] = 0;
                  help_cnt++;
                  left--;
               }
            }
            if (help_cnt == 8) {    // Kun helpBufferi on täysi, otetaan siitä merkki ylös.
               merkki = bitoby(helpBuff);
               help_cnt = 0;
            }
         }
         left:
         if (left != 0) { 
            if (merkki != 0) { 
               insert(tree, merkki); 
               merkki = 0; 
               read_cnt++;
            }
            if ( hold == 0) { tmp = tree->root; }
            else { tmp = hold; hold = 0;}
            for (int i = 0; i < 8; i++) {
               if (bBuffer[i] != 0) {
                  tmpN = tree->followPath(bBuffer[i],tmp);
                  if (tmpN != 0) { tmp = tmpN; bBuffer[i] = 0;  }   
               }
            }

            if ( tmp->weight == 0) { // Jos löydetty solmu on NYT-solmu, seuraavat 8 "bittiä" ovat seuraava kirjain.
               if ( bBuffer[7] == 0 ) {
                  ifilestr.read(bBuffer, 8);
               }
               for (int i = 0; i < 8; i++) {
                  if (bBuffer[i] != 0) {
                     helpBuff[help_cnt] = bBuffer[i];
                     bBuffer[i] = 0;
                     help_cnt++;
                     left--;
                  }
               }
               merkki = 0;
            } 
            else if (tmp->left == 0) { merkki = tmp->val; } // Jos löydetty solmu ei ole NYT-solmu, lisätään löydetyn solmun merkki puuhun.
            else { hold = tmp; }
         }
         if (merkki != 0) {
            insert(tree, merkki); 
            merkki = 0; 
            read_cnt++;
         }
         if (bBuffer[7] != 0) {
            goto left;
         }
      }

   } 
   else { // Jos tiedoston pääte ei ole ".bin", niin pakataan tiedoston sisältö.
      code = 'e';
      char c = 0;
      
      while ((ifilestr.get(c))) {
         merkki = c;
         insert(tree, merkki);
         merkki = 0;
      }
   }
   writeToBuffer(0);
}


/**
 * The function takes care of the global buffer. Whe the buffer is full it flushes the content to file.
 * The parameter defines what cahr to add to the buffer, or is it's null, it will fill the rst of the buffer with zero's.
**/
void writeToBuffer (char val) {

   if (buff_cnt == 8 || val == 0) {
      while (buff_cnt < 8) {
         if ( code == 'e' ) { cBuffer[(buff_cnt)] = '0'; }
         else if ( code == 'd' ) { cBuffer[(buff_cnt)] = ' '; }   
         buff_cnt++;
      }
      for (int i = 0; i < 8; i++) {
         ofilestr.put(cBuffer[i]);
      }
      buff_cnt = 0;
   }
   cBuffer[buff_cnt] = val;
   if (val != 0) { buff_cnt++; }
}

// ------------- Operators -----------

/**
 * The operater compares to nodes to each other.
 * It returns TRRUE if they are equal, otherwise FALSE.
**/
bool node::operator== (const node& other) {
   if (this->index == other.index && this->weight == other.weight && this->parent->index == other.parent->index) {
      return true;
   }
   return false;
}

/**
 * The operator allows the tree structure to be printed.
**/
std::ostream& operator<<(std::ostream& out, const btree& tree) {

   out << "Inorder-Tree-Walk:   " << inorderTreeWalk(tree.root) << std::endl;
   out << "Postorder-Tree-Walk: " << postorderTreeWalk(tree.root) << std::endl;
   out << "Preorder-Tree-Walk:  " << preorderTreeWalk(tree.root) << std::endl;
   return out;
}

/**
 * The operator allows the node structure to be printed.
**/
std::ostream& operator<<(std::ostream& out, const node& node) {

   if (&node == 0) {
      std::cerr << "node operator<<: Invalid pointer!" << std::endl;
      return out;
   }
   out << "Node: " << node.index << " ";
   if (node.parent == 0) {
      out << "Root" << std::endl;
      out << "    Parent: " << node.parent << std::endl;
      if (node.left != 0) {
         out << "    Left:   " << node.left->index << std::endl;
      } else {
         out << "    Left:   " << node.left << std::endl;
      }
      if (node.right != 0) {
         out << "    Right:  " << node.right->index << std::endl;
      }   else {
         out << "    Right:  " << node.right << std::endl;
      }
   } else if (node.left != 0) {
      out << "Internal" << std::endl;
      out << "    Parent: " << node.parent->index << std::endl;
      out << "    Left:   " << node.left->index << std::endl;
      out << "    Right:  " << node.right->index << std::endl;
   } else {
      out << "Leaf" << std::endl;
      out << "    Parent: " << node.parent->index << std::endl;
      out << "    Left:   " << node.left << std::endl;
      out << "    Right:  " << node.right << std::endl;
   }
   out << "    Weight: " << node.weight << std::endl;
   out << "    Index:  " << node.index << std::endl;
   out << "    Val:    " << node.val << std::endl;

   return out;
}

// ------------- Main -------------------------

int main(int argc, char *argv[]) {

   cBuffer = new char[8];
   std::string help = "-h";
   btree* huffTree = new btree();

   if ( argc > 2) {
      std::cerr << "Too many parameters for program, please try again!" << argv;
      exit(1);
   } else {
      if (argc == 1 || argv[1] == help) {
         printHelp();
         exit(0);
      }

      char* fname = 0;

      if (strlen(argv[1]) > 2)
         fname = argv[1];

      openFile(fname);
      readFile(huffTree,fname);

      // std::cout << *huffTree; // This prints out the tree.

      delete huffTree;
      ifilestr.close();
      ofilestr.close();
      delete[] cBuffer;
   }
   return 0;
}
