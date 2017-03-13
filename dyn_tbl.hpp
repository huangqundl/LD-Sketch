#ifndef DYN_MAJ_TBL_HPP
#define DYN_MAJ_TBL_HPP

#include <string.h>
#include <unordered_set>
#include <unordered_map>
//#include "hash.h"
#include "Hash.hpp"

/**********************************************************
 * 
 * Basic objects
 *
 **********************************************************/

/**
 * Structure of key
 */
typedef struct dyn_tbl_key_s {
    /// 16-byte key
    unsigned char key[16];
} dyn_tbl_key_t;

/**
 * Object for hash
 */
typedef struct {
    /// overloaded operation
    long operator() (const dyn_tbl_key_t &k) const { return AwareHash(k.key, 16); }
} dyn_tbl_key_hash;

/**
 * Object for equality
 */
typedef struct {
    /// overloaded operation
    bool operator() (const dyn_tbl_key_t &x, const dyn_tbl_key_t &y) const {
        return memcmp(x.key, y.key, 16)==0;
    }
} dyn_tbl_key_eq;


typedef std::unordered_set<dyn_tbl_key_t, dyn_tbl_key_hash, dyn_tbl_key_eq> myset;

/**********************************************************
 * 
 * Buckets
 *
 **********************************************************/

typedef struct dyn_tbl_s * dyn_tbl_p_t;

/**
 * Bucket structure
 */
typedef struct dyn_tbl_s {

    /// associative array: A(i,j)
    std::unordered_map<dyn_tbl_key_t, long long, dyn_tbl_key_hash, dyn_tbl_key_eq> data;

    /// total sum: V(i,j)
    long long total;

    /// maximum length of counters allowed, exceeding this value would trigger expansion: l(i, j)
    unsigned int max_len;

    /// total number of decrement: e(i, j)
    unsigned int decrement;

    /// maximum sum among keys, to speed up detection
    long long max_value;

    /***********************
     * read only members
     ***********************/
    /// length of keys
    unsigned int n;
} dyn_tbl_t;

/*************************************************************
 * create and destroy
 ************************************************************/

/// init bucket
// @param l length of associative array
// @param n length of key
// @return the pointer to bucket
dyn_tbl_p_t dyn_tbl_init(unsigned int l, int n);

/// free the bucket
// @param dyn_tbl the target associative array
void dyn_tbl_destroy(dyn_tbl_p_t dyn_tbl);

/*************************************************************
 * read functions
 ************************************************************/

/// identify heavy keys
// @param dyn_tbl target bucket
// @param thresh threshold for heavy keys
// @param ret results of detected keys
void dyn_tbl_get_heavy_key(dyn_tbl_p_t dyn_tbl, double thresh, myset& ret);

/// estimate the lower sum of a key
// @param dyn_tbl target bucket
// @param key
// @return the estimated lower sum
long long dyn_tbl_low_estimate(dyn_tbl_p_t dyn_tbl, dyn_tbl_key_t& key);

/// estimate the higher sum of a ket
// @param dyn_tbl target bucket
// @param key
// @return the estimated higher sum
long long dyn_tbl_up_estimate(dyn_tbl_p_t dyn_tbl, dyn_tbl_key_t& key);

/// print out the sketch to file
// @param dyn_tbl target bucket
// @param output name of output file
void dyn_tbl_print(dyn_tbl_p_t dyn_tbl, const char* output);

/*************************************************************
 * write functions
 ************************************************************/

/**
 * update the bucket with an data item
 * @param dyn_tbl the target bucket
 * @param key key of the data item
 * @param val value of the data item
 * @param T expansion parameter
 */
void dyn_tbl_update(dyn_tbl_p_t dyn_tbl, unsigned char* key, int val, double T);

/**
 * copy bucket
 * @param from source bucket
 * @param to target bucket
 */
void dyn_tbl_copy(dyn_tbl_p_t from, dyn_tbl_p_t to);

/**
 * reset bucket
 * @param dyn_tbl the target bucket
 * @param output filename of the output file
 */
void dyn_tbl_reset(dyn_tbl_p_t dyn_tbl);

#endif
