#ifndef __PARAM_H__
#define __PARAM_H__

#include <iostream>
#include <vector> 
using namespace std;

class Param {
  public:
    /* IP addresses */
    static string IP_ADDR_P0;
    static string IP_ADDR_P1;
    static string IP_ADDR_P2;

    /* Ports */
    // The party with smaller ID listens on the port 
    // and the other connects to it. Make sure the firewall 
    // setting of the listener allows these ports.
    static int PORT_P0_P1;
    static int PORT_P0_P2;
    static int PORT_P1_P2;
    static int PORT_P1_P3;
    static int PORT_P2_P3;
    
    /* Directories and files */
    static string KEY_PATH; // encryption keys for secure channels
    static string SNP_POS_FILE; // genomic positions for SNPs in the data
    static string LOG_FILE; // runtime/communication profiler output file
    static string OUTPUT_FILE_PREFIX; // prefix for GWAS output (P2 only)
    static vector<string> CACHE_FILE_PREFIX; // prefix for each cache files; includes
                                     // input shares, which can be large

    /* Secret sharing parameters (see Supplementary Information) */
    // Require:
    //   NBIT_F < NBIT_K
    //   Both NBIT_K and NBIT_K - NBIT_F are even
    //   log2(BASE_P) > NBIT_K + NBIT_F + NBIT_V + 2
    // Useful resource:
    //   https://primes.utm.edu/lists/2small/200bit.html
    static int NBIT_K; // total bit length of a data value
    static int NBIT_F; // bit length of fractional range
    static int NBIT_V; // security parameter
    static string BASE_P; // base prime for finite field
    
    /* Quality control filters */
    // UB: upper-bound, LB: lower-bound
    static double IMISS_UB; // individual missing rate 
    static double GMISS_UB; // genotype missing rate
    static double HET_LB; // individual heterozygosity
    static double HET_UB;
    static double MAF_LB; // minor allele frequency
    static double MAF_UB;
    static double HWE_UB; // hardy-weinberg equilibrium test statistic

    /* Algorithm parameters */
    static int NUM_DIM_TO_REMOVE; // number of PCs to correct for
    static int NUM_OVERSAMPLE; // oversampling parameter for rand PCA
    static int NUM_POWER_ITER; // number of power iterations for rand PCA
    static int ITER_PER_EVAL; // number of QR iterations per eigenvalue in
                              // eigendecomposition
    static long LD_DIST_THRES; // genomic distance threshold
                               // for selecting SNPs for PCA
    
    /* This variable is used to signify which 'round' of data sharing we are in.
    When multiple parties are contributing data, this variable is used as an index
    into array-type parameters (such as NUM_INDS and CACHE_FILE_PREFIX) to determine
    which specific cache file we are currently updating.*/
    static int CUR_ROUND;

    /* Data dimensions */
    static vector<long> NUM_INDS; // number of individuals in each input dataset
    static long NUM_SNPS; // number of SNPs in every input dataset
    static long NUM_COVS; // number of covariate features in every input dataset

    /* Software parameters */
    static uint64_t MPC_BUF_SIZE; // size of data buffer in bytes
    static long DIV_MAX_N; // maximum number of parallel divisions/sqrts
    static long PITER_BATCH_SIZE; // size of batch processing of genomes
    static long PAR_THRES; // minimum threshold for NTL thread-boosting
    static long NTL_NUM_THREADS; // number of threads for NTL thread-boosting
    static long NUM_THREADS; // number of threads for custom multi-threading
    static vector<long> NUM_CHUNKS; // number of chunks per dataset for chunked data-sharing

    /* Global control */
    static bool SKIP_QC; // skip quality control and use all individuals/SNPs
    static bool PROFILER; // turn on/off profiler for runtime/communication
    static bool DEBUG; // debug flag
    
    /* Helper functions */
    template<class T>
    static bool Convert(string s, T &var, string name);
    template<class T>
    static bool ConvertVector(string s, vector<T> &var, string name);
    template<class T>
    static bool ConvertPort(string s, T &var, string name);
    static bool ParseFile(const char *param_file);
};

#endif 
