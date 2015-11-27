#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

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

vector<string>& updateRec(
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
  const vector<string>& vin
){
  size_t ymax = rec.size();
  size_t xmax = rec[0].size();
  size_t count = 0;
  for(size_t y = 0; y < ymax; ++y){
    for(size_t x = 0; x < xmax; ++x){
      if(rec[y][x] == vin[y][x]){
        ++count;
      }
    }
  }
  return count;
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

size_t addRectangle(
  const vector<string>& vin, 
  const set<char>& letters,
  const vector<string>& work
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
            vector<string> rec = work;
            updateRec(letter, ystart, xstart, ylen, xlen, rec);
            size_t count = matchCount(rec, vin);
            if(count > bestMatchCount){
              bestMatchCount = count;
              bestMatch = move(rec);
            }
          }
        }
      }
    }
  }

  cout << bestMatch;
  cout << "count: " << bestMatchCount << endl << endl;

  if(testSolution(vin, bestMatch)){
    cout << "solved!!" << endl;
    return 1;
  }

  return addRectangle(vin, letters, bestMatch) + 1;
}

///////////////////////////////////////////////////////
int main(){
  vector<string> gVin;
  vector<string> gVwork;
  set<char> gLetters;

  getInput(gVin);
  initRec(gVin.size(), gVin[0].size(), gVwork);
  getLetters(gVin, gLetters);
  cout << "input:" << endl << "==========" << endl << gVin << endl;

  size_t numRecangles = addRectangle(gVin, gLetters, gVwork);
  cout << "# of rectangles: " << numRecangles << endl;
}
