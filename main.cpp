#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

//input:
//AAA
//AAB
//BBB
//
//rec:
//###
//#BB
//#BB
//
//vwork:
//AAA
//AAA
//AAA

ostream& operator<<(ostream& out, vector<string> vin){
  for(auto s : vin){
    out << s << endl;
  }
  return out;
}

void getInput(vector<string>& vin){
  string line;
  while(getline(cin,line)){
    vin.push_back(line);
  }
}

void initRec(
  size_t ymax,
  size_t xmax,
  vector<string>& vw
){
  for(size_t i = 0; i < ymax; ++i){
    string s(xmax, '#');
    vw.push_back(s);
  }
}

void getLetters(
  const vector<string>& vin,
  set<char>& letters
){
  for(auto s : vin){
    for(auto l : s){
      letters.insert(l);
    }
  }
}

vector<string>& createRec(
  char letter, 
  size_t ystart,
  size_t xstart,
  size_t ylen,
  size_t xlen,
  vector<string>& rec
){
  for(size_t y = ystart; y < (ystart + ylen); ++y){
    for(size_t x = xstart; x < (xstart + xlen); ++x){
      rec.at(y).at(x) = letter;
    }
  }
  return rec;
}

size_t matchCount(
  const vector<string>& rec,
  const vector<string>& vin,
  const vector<string>& vwork
){
  size_t ymax = rec.size();
  size_t xmax = rec[0].size();
  size_t count = 0;
  for(size_t y = 0; y < ymax; ++y){
    for(size_t x = 0; x < xmax; ++x){
      // Ignore if not planning to overwrite
      if(rec[y][x] == '#'){continue;}

      // Exit if overwriting valid letter with an invalid letter
      if(rec[y][x] != vin[y][x] && vwork[y][x] == vin[y][x]){
        return 0;
      }

      // Only count if adding a new letter
      if(rec[y][x] == vin[y][x] && vwork[y][x] != vin[y][x]){
        ++count;
      }
    }
  }
  return count;
}

void updateWorking(
  const vector<string>& rec,
  vector<string>& vwork
){
  size_t ymax = rec.size();
  size_t xmax = rec[0].size();
  for(size_t y = 0; y < ymax; ++y){
    for(size_t x = 0; x < xmax; ++x){
      if(rec[y][x] == '#'){continue;}
      vwork[y][x] = rec[y][x];
    }
  }
}

void addRectangle(
  const vector<string>& vin, 
  const set<char>& letters,
  vector<string>& vwork
){
  size_t xmax = vin[0].size();
  size_t ymax = vin.size();
  size_t bestMatchCount = 0;
  vector<string> bestMatch;
  for(auto letter: letters){
    for(size_t xstart = 0; xstart < xmax; ++xstart){
      for(size_t ystart = 0; ystart < ymax; ++ystart){
        for(size_t xlen = 1; xlen <= (xmax-xstart); ++xlen){
          for(size_t ylen = 1; ylen <= (ymax-ystart); ++ylen){
            vector<string> rec;
            initRec(ymax, xmax, rec);
            createRec(letter, ystart, xstart, ylen, xlen, rec);
            size_t count = matchCount(rec, vin, vwork);
            if(count > bestMatchCount){
              bestMatchCount = count;
              bestMatch = move(rec);
            }
          }
        }
      }
    }
  }
  updateWorking(bestMatch, vwork);
}

bool testSolution(
  const vector<string>& vin,
  const vector<string>& vwork
){
  size_t ymax = vin.size();
  for(size_t y = 0; y < ymax; ++y){
    if(vin[y] != vwork[y])
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////
struct bfsStruct{
  vector<string> vwork;
  size_t depth = 0;
  shared_ptr<bfsStruct> parent = nullptr;
};

///////////////////////////////////////////////////////
shared_ptr<bfsStruct> recursiveAddRectangle(
  const vector<string>& vin, 
  const set<char>& letters,
  const vector<string>& vwork,
  size_t maxDepth
){
  // queue holding upcoming working arrays for bfs
  queue<shared_ptr<bfsStruct>> queue;
  shared_ptr<bfsStruct> bfs(new bfsStruct);
  bfs->vwork = vwork;
  queue.push(bfs);

  size_t xmax = vin[0].size();
  size_t ymax = vin.size();
  while(queue.front()->depth < maxDepth){
    for(auto letter: letters){
      for(size_t xstart = 0; xstart < xmax; ++xstart){
        for(size_t ystart = 0; ystart < ymax; ++ystart){
          for(size_t xlen = 1; xlen <= (xmax-xstart); ++xlen){
            for(size_t ylen = 1; ylen <= (ymax-ystart); ++ylen){
              vector<string> rec;
              initRec(ymax, xmax, rec);
              createRec(letter, ystart, xstart, ylen, xlen, rec);
              
              auto newWork = queue.front()->vwork;
              updateWorking(rec, newWork);
              //cout << "trying:" << endl << newWork << endl;

              if(testSolution(vin, newWork)){
                cout << "solved! " << queue.front()->depth+1 
                  << endl << newWork << endl;
                return queue.front();
              }else{
                shared_ptr<bfsStruct> newBfs(new bfsStruct);
                newBfs->vwork = newWork;
                newBfs->depth = queue.front()->depth+1;
                newBfs->parent = queue.front();
                queue.push(newBfs);
              }
            }
          }
        }
      }
    }
    queue.pop();
  }
  return nullptr;
}

///////////////////////////////////////////////////////
int main(){
  vector<string> gVin;
  vector<string> gVwork;
  set<char> gLetters;

  getInput(gVin);
  initRec(gVin.size(), gVin[0].size(), gVwork);
  getLetters(gVin, gLetters);
  cout << "input:" << endl << gVin << endl;

  size_t numRectangles = 0;
  while(!testSolution(gVin, gVwork)){
    addRectangle(gVin, gLetters, gVwork);
    cout << gVwork << endl;
    numRectangles++;
  }
  cout << "# of rectangles: " << numRectangles << endl;

  // Try to find a solution with fewer rectangles
  vector<string> work;
  initRec(gVin.size(), gVin[0].size(), work);
  shared_ptr<bfsStruct> bfs = 
    recursiveAddRectangle(gVin, gLetters, work, numRectangles);

  // print bfs result
  while(bfs->parent != nullptr){
    cout << bfs->vwork << endl;
    bfs = bfs->parent;
  }
}
