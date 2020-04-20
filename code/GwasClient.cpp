#include "connect.h"
#include "mpc.h"
#include "protocol.h"
#include "util.h"
#include "NTL/ZZ_p.h"
#include "gwasiter.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <bits/stdc++.h>
#include <sys/time.h>

using namespace NTL;
using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cout << "Usage: GwasClient party_id param_file" << endl;
    return 1;
  }

  string pid_str(argv[1]);
  int pid;
  if (!Param::Convert(pid_str, pid, "party_id") || pid < 0 || pid > 2) {
    cout << "Error: party_id should be 0, 1, or 2" << endl;
    return 1;
  }

  // reset the current round
  Param::CUR_ROUND = 0;

  if (!Param::ParseFile(argv[2])) {
    cout << "Could not finish parsing parameter file" << endl;
    return 1;
  }

  if (argc == 4) {
    string num_threads_str(argv[3]);
    int num_threads = stoi(num_threads_str);
    Param::NUM_THREADS = num_threads;
  }
  cout << "Number of threads: " << Param::NUM_THREADS << endl;

  vector< pair<int, int> > pairs;
  pairs.push_back(make_pair(0, 1));
  pairs.push_back(make_pair(0, 2));
  pairs.push_back(make_pair(1, 2));

  /* Initialize MPC environment */
  MPCEnv mpc;
  if (!mpc.Initialize(pid, pairs)) {
    cout << "MPC environment initialization failed" << endl;
    return 1;
  }

  struct timeval start, end;
  double runtime;

  gettimeofday(&start, NULL); 
  ios_base::sync_with_stdio(false);
  bool success = gwas_protocol(mpc, pid);
  gettimeofday(&end, NULL);

  // calculate runtime in seconds
  runtime = (end.tv_sec - start.tv_sec) * 1e6;
  runtime = (runtime + (end.tv_usec - start.tv_usec)) * 1e-6;

  // This is here just to keep P0 online until the end for data transfer
  // In practice, P0 would send data in advance before each phase and go offline
  if (pid == 0) {
    mpc.ReceiveBool(2);
  } else if (pid == 2) {
    mpc.SendBool(true, 0);
  }

  mpc.CleanUp();

  if (success) {
    cout << "Protocol successfully completed" << endl;
    cout << "GWAS Runtime: " << fixed << runtime << setprecision(6); 
    cout << " sec" << endl;
    return 0;
  } else {
    cout << "Protocol abnormally terminated" << endl;
    return 1;
  }
}
