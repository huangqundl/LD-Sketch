#ifndef DYN_MAJ_TBL_HPP
#define DYN_MAJ_TBL_HPP

#include <string.h>
#include <unordered_set>
#include <unordered_map>
#include "hash.h"
//#include "Hash.hpp"

/**********************************************************
 * 
 * Basic objects
 *
 **********************************************************/

#define MAX_KEYLEN 13

/**
 * Structure of key
 */
typedef struct dyn_tbl_key_s {
    /// 13-byte key
    unsigned char key[MAX_KEYLEN];
} dyn_tbl_key_t;

/**
 * Object for hash
 */
typedef struct {
    /// overloaded operation
    long operator() (const dyn_tbl_key_t &k) const { return AwareHash((unsigned char*)k.key, MAX_KEYLEN); }
} dyn_tbl_key_hash;

/**
 * Object for equality
 */
typedef struct {
    /// overloaded operation
    bool operator() (const dyn_tbl_key_t &x, const dyn_tbl_key_t &y) const {
        return memcmp(x.key, y.key, MAX_KEYLEN)==0;
    }
} dyn_tbl_key_eq;


// typedef std::unordered_set<dyn_tbl_key_t, dyn_tbl_key_hash, dyn_tbl_key_eq> myset;

/**********************************************************
 * 
 * Buckets
 *
 **********************************************************/

// typedef struct dyn_tbl_s * dyn_tbl_p_t;

/**
 * Bucket structure
 */
typedef struct dyn_tbl_s {

    /// associative array: A(i,j)
    std::unordered_map<dyn_tbl_key_t, long long, dyn_tbl_key_hash, dyn_tbl_key_eq> array;

    /// total sum: V(i,j)
    long long total;

    /// maximum length of counters allowed, exceeding this value would trigger expansion: l(i, j)
    unsigned int max_len;

    /// total number of decrement: e(i, j)
    unsigned int decrement;

    /// expansion parameter: T
    long long T;

    /// maximum sum among keys, to speed up detection
    long long max_value;

    /***********************
     * read only members
     ***********************/
    /// length of keys
    unsigned int lgn;
} dyn_tbl_t;

/*************************************************************
 * create and destroy
 ************************************************************/

/// init bucket
// @param l length of associative array
// @param lgn length of key
// @param T expansion parameter
// @return the pointer to bucket
dyn_tbl_t* dyn_tbl_init(unsigned int l, int lgn, long long T);

/// free the bucket
// @param dyn_tbl the target associative array
void dyn_tbl_destroy(dyn_tbl_t* dyn_tbl);

/*************************************************************
 * read functions
 ************************************************************/

/// identify heavy keys
// @param dyn_tbl target bucket
// @param thresh threshold for heavy keys
// @param keys results of detected keys
// @param num_key number of detected keys
void dyn_tbl_get_heavy_key(dyn_tbl_t* dyn_tbl, long long thresh, unsigned char* keys, int* num_key);

/// estimate the lower sum of a key
// @param dyn_tbl target bucket
// @param key
// @return the estimated lower sum
long long dyn_tbl_low_estimate(dyn_tbl_t* dyn_tbl, unsigned char* key);

/// estimate the higher sum of a ket
// @param dyn_tbl target bucket
// @param key
// @return the estimated higher sum
long long dyn_tbl_up_estimate(dyn_tbl_t* dyn_tbl, unsigned char* key);

/// print out the sketch to file
// @param dyn_tbl target bucket
// @param output name of output file
void dyn_tbl_print(dyn_tbl_t* dyn_tbl, const char* output);

/// number of tracked elements in associative array
// @param dyn_tbl target bucket
// @return number of elements in associative array
int dyn_tbl_length(dyn_tbl_t* dyn_tbl);

/*************************************************************
 * write functions
 ************************************************************/

/**
 * update the bucket with an data item
 * @param dyn_tbl the target bucket
 * @param key key of the data item
 * @param val value of the data item
 */
void dyn_tbl_update(dyn_tbl_t* dyn_tbl, unsigned char* key, int val);

/**
 * copy bucket
 * @param from source bucket
 * @param to target bucket
 */
void dyn_tbl_copy(dyn_tbl_t* from, dyn_tbl_t* to);

/**
 * reset bucket
 * @param dyn_tbl the target bucket
 * @param output filename of the output file
 */
void dyn_tbl_reset(dyn_tbl_t* dyn_tbl);

#endif
