#include <iostream>
#include <glog/logging.h>

#define GV_JS
#include "gv.hpp"
using namespace std;

int main(int argc, char** argv) { 
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  g_gvDefaultAlpha = 180;

  for (int i = 0; i < 100; ++i) {
    gvLine(0, i, 1, 1, gvColor(i));
    gvLine(0, 0, i, 1);
    gvLine(4, 2, 2, 3);
    gvNewTime();
  }
  cout << "hello" << endl;
}
