#include <iostream>
#include <glog/logging.h>
using namespace std;

int main(int argc, char** argv) { 
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  cout << "hello" << endl;
}
