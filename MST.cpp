//author: Sarah Amick

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<set>
#include<algorithm>
using namespace std;

//method to split a string by a specified delimiter
vector<string> split(const string& s, char delimiter){
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)){
    tokens.push_back(token);
  }
  return tokens;
}

bool sortCol(const vector<long>& v1, const vector<long>& v2){
  return v1[2] < v2[2];
}

long find(long* uf, long v){
  if(uf[v]!=v){
    uf[v] = find(uf, uf[v]);
  }
  return uf[v];
}

void unionVertices(long* uf, long* rank, long v1, long v2){
  long v1Parent = find(uf, v1);
  long v2Parent = find(uf, v2);
  if(rank[v1Parent] < rank[v2Parent]) {
    uf[v1Parent] = v2Parent;
    rank[v2Parent] = rank[v2Parent] + 1;
  }
  else {
    uf[v2Parent] = v1Parent;
    rank[v1Parent] = rank[v1Parent] + 1;
  }
}

bool connected(long* uf, long v1, long v2){
  return find(uf, v1) == find(uf, v2);
}

//method implementing the Kruskal's MST algorithm
long mst(vector< vector<long> >* graph, int numE, int& numV){
  //1. Initialize the UnionFind array to be size of number vertices with each V the parent of itself
   long* uf = new long[numV];
   long* rank = new long[numV];
   for(int i = 0; i != numV; ++i) {
     uf[i] = i;
     rank[i] = 0;
   }
  //2. Sort the edges from least -> greatest weight
  sort(graph->begin(), graph->end(), sortCol);
  //3. Pick an edge to add to the MST one at a time, if the two vertices it connects aren't already connected
  long weight = 0;
  long v1;
  long v2;
  for(int i = 0; i != numE; ++i){
    v1 = graph->at(i)[0];
    v2 = graph->at(i)[1];
    if(!connected(uf, v1, v2)){
      weight += graph->at(i)[2];
      unionVertices(uf, rank, v1, v2);
    }
  }
  delete[] uf;
  delete[] rank;
  return weight;
}

//method to convert a text graph representation into a 2D array
vector< vector<long> >* read_file(int n, string filename, int& numV){

  vector< vector<long> >* graph = new vector< vector<long> >(n, vector<long>(3)); //create a pointer to a 2D array in memory:

  int largestNode = 0;
  long fromNode;
  long toNode;
  long weight;
  string line;
  ifstream myfile(filename);
  if (myfile.is_open()){
    int count = 0;
    while(getline(myfile, line)){
      vector<string> edge = split(line, ' ');
      fromNode = stol(edge[0]);
      toNode = stol(edge[1]);
      weight = stol(edge[2]);
      if(fromNode > largestNode) largestNode = fromNode;
      if(toNode > largestNode) largestNode = toNode;

      graph->at(count)[0] = fromNode;
      graph->at(count)[1] = toNode;
      graph->at(count)[2] = weight;

      count = count + 1;
    }
    myfile.close();
  }
  else cout << "Unable to open file \n";
  numV = largestNode + 1;

  return graph;
}

int main(int argc, char** args){
  int numV;
  vector< vector<long> >* graph = read_file(atoi(args[1]), args[2], numV);
  long minWeight = mst(graph, atoi(args[1]), numV);
  delete graph;
  cout << minWeight << endl;
}
