#include <vector>
#include <iostream>

using namespace std;


class B{
 public:
   int nr;
   B(int a) : nr(a){
    }
   int getNr(){return nr;}
};


class A{
  std::vector<B*> myvector;
 
  public:

  A(){
    myvector.push_back( new B(1) );
    myvector.push_back( new B(2) );
    myvector.push_back( new B(3) );
    for(int i = 0; i < 3; i++){
       cout << myvector[i]->getNr() << endl;
       }
  }

  const B& operator[] (size_t i) const {return *myvector[i];}	
  const vector<B*> &getVector() const { return &myvector; }

};

int main(){

A a;

std::vector<B*> avector;
&avector = a.getVector();

std::vector<B*>::iterator it;

for(it = avector.begin(); it != avector.end(); ++it)
   cout << (*it)->getNr() << endl;


return 0;
}

