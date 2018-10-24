#include<iostream>
#include <fstream>
#include <string>
#include<vector>
#include<sstream>
#include<set>
#include<algorithm>
using namespace std;

bool sortCol(const vector<long>& v1, const vector<long>& v2){
  return v1[2] < v2[2];
}

void unionVertices(long* uf, long v1, long v2){
  uf[v1] = v2;
}

long getParent(long* uf, long v){
  if(uf[v]!=v){
    uf[v] = getParent(uf, uf[v]);
  }
  return uf[v];
}

bool find(long* uf, long v1, long v2){
  return getParent(uf, v1) == getParent(uf, v2);
}

//method implementing the Kruskal's MST algorithm
long mst(vector< vector<long> >* graph, int numE){

  //1. Count # of verticies in graph
  set<long> vertices;
  for(int i = 0; i != numE; ++i){
    for(int j = 0; j != 2; ++j){
      vertices.insert(graph->at(i)[j]);
    }
  }
  int numV = vertices.size();

  //2. Initialize the UF array with each V the parent of itself
   long* uf = new long[numV];
   for(int i = 0; i != numV; ++i) uf[i] = i;

  //3. Sort the edges from least -> greatest weight
  sort(graph->begin(), graph->end(), sortCol);
  for(int i = 0; i < numE; i++) cout << graph->at(i)[0] << " " << graph->at(i)[1] << " " << graph->at(i)[2] << endl;

  //4. Pick an edge to add to the MST one at a time, if the two vertices it connects aren't already connected
  long weight = 0;
  for(int i = 0; i != numE; ++i){
    if(!(find(uf, graph->at(i)[0], graph->at(i)[1]))){
      weight += graph->at(i)[2];
      unionVertices(uf, graph->at(i)[0], graph->at(i)[1]);
    }
  }
  delete[] uf;
  return weight;
}


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

//method to convert a text graph representation into a 2D array
vector< vector<long> >* read_file(int n, string filename){

  vector< vector<long> >* graph = new vector< vector<long> >(n, vector<long>(3)); //create a pointer to a 2D array in memory:

  string line; //read from the file
  ifstream myfile(filename);
  if (myfile.is_open()){
    int count = 0;
    while(getline(myfile, line)){
      vector<string> edge = split(line, ' ');
      graph->at(count)[0] = stol(edge[0]); //stol converts string to long
      graph->at(count)[1] = stol(edge[1]);
      graph->at(count)[2] = stol(edge[2]);
      count = count + 1;
    }
    myfile.close();
  }
  else cout << "Unable to open file \n";
  return graph;
}

//method to call functions from
int main(int argc, char** args){
  vector< vector<long> >* graph = read_file(atoi(args[1]), args[2]);
  long minWeight = mst(graph, atoi(args[1]));
  delete graph;
  cout << minWeight << endl;
}
