#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <limits>

class CountMinSketch {
private:
  const unsigned int PRIME = 2147483647;
  unsigned int d;      // the number of hash tables
  unsigned int w;      // the size of each hash table
  unsigned int **t;    // the Sketch table
  unsigned int **hash; // the values for the hash function
  void genhash(unsigned int **, unsigned int);
  unsigned int hashfunc(unsigned int, unsigned int);

public:
  CountMinSketch(unsigned int, unsigned int);
  bool update(unsigned int, unsigned int);
  bool update(std::string, unsigned int);
  bool update(std::pair<std::string, unsigned int>);
  unsigned int query(unsigned int);
  unsigned int query(std::string);
  ~CountMinSketch();
};

// set d and w for Sketch parameters
CountMinSketch::CountMinSketch(unsigned int din, unsigned int win) {
  d = din;
  w = win;

  // initialize Sketch table and hash functions
  t = new unsigned int *[d];
  hash = new unsigned int *[d];
  for (unsigned int i = 0; i < d; i++) {
    t[i] = new unsigned int[w]();    // set one hash table with 0 initial value
    hash[i] = new unsigned int[2](); // set hashes for various hash functions
    genhash(hash, i);
  }
}

CountMinSketch::~CountMinSketch() {
  for (unsigned int i = 0; i < d; i++) {
    delete[] t[i];
    delete[] hash[i];
  }
  delete[] t;
  delete[] hash;
}

void CountMinSketch::genhash(unsigned int **hash, unsigned int i) {
  hash[i][0] =
      (unsigned int)((float)(PRIME) * (float)(rand()) / (float)(RAND_MAX) + 1);
  hash[i][1] =
      (unsigned int)((float)(PRIME) * (float)(rand()) / (float)(RAND_MAX) + 1);
}

unsigned int CountMinSketch::hashfunc(unsigned int key, unsigned int i) {
  return ((hash[i][0] * key) + hash[i][1]) % PRIME % w;
}

bool CountMinSketch::update(unsigned int key, unsigned int value) {
  for (unsigned int i = 0; i < d; i++) {
    t[i][hashfunc(key, i)] += value;
  }
  return true;
}

bool CountMinSketch::update(std::string key, unsigned int value) {
  unsigned int strhash = std::hash<std::string>()(key);
  for (unsigned int i = 0; i < d; i++) {
    t[i][hashfunc(strhash, i)] += value;
  }
  return true;
}

bool CountMinSketch::update(std::pair<std::string, unsigned int> item) {
  update(item.first, item.second);
  return true;
}

unsigned int CountMinSketch::query(unsigned int key) {
  unsigned int minval = std::numeric_limits<unsigned int>::max();
  for (unsigned int i = 0; i < d; i++) {
    minval = std::min(minval, t[i][hashfunc(key, i)]);
  }
  return minval;
}

unsigned int CountMinSketch::query(std::string key) {
  unsigned int hashval = std::hash<std::string>()(key);
  return query(hashval);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "Usage:./a.out d w \"estimate words\"" << std::endl;
    return 1;
  }
  int d = std::stoi(argv[1]);
  int w = std::stoi(argv[2]);
  std::string argv3 = argv[3];
  std::fstream ifs("data.txt");
  std::string str, key, val;

  CountMinSketch cm(d, w);

  while (getline(ifs, str)) {
    std::istringstream stream(str);
    getline(stream, key, ',');
    getline(stream, val);
    cm.update(key, std::stoi(val));
  }

  std::cout << cm.query(argv3) << std::endl;

  return 0;
}
