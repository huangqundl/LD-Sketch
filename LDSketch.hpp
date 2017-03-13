/*
 * LDSketch.h
 * - heavy hitter random partition
 */
#include "dyn_tbl.hpp"

/// Sketch structure
typedef struct LDSketch_s {

    /// bucket array
	dyn_tbl_t** tbl;

    /*********************************************************
     * read only variables
     ********************************************************/
    /// # of rows
	int h;

    /// # of buckets per row
	int w;

    /// init # of counters in each tbl (will grow later)
    int l;

    /// total number of buckets
    // int size;

    /// length of keys (in bits)
	int lgn;

    /// detection threshold
    double thresh_abs;

    /// id of detectors (detectors have different hash functions)
    unsigned int tbl_id;
} LDSketch_t;

/*************************************************************
 * create and destroy
 ************************************************************/

/// init sketch
// @return the pointer to the created sketch
LDSketch_t* LDSketch_init(int w, int h, int l, int lgn, double thresh_abs, unsigned int tbl_id);

/// free scketch
void LDSketch_destroy(LDSketch_t* LDSketch);

/*************************************************************
 * read functions
 ************************************************************/

/// print out the sketch to file
// @param sk target sketch
// @param output name of output file
void LDSketch_write_plaintext(LDSketch_t* sk, const char* output);

/// identify heavy keys
// @param sk target sketch
// @param thresh threshold for heavy keys
// @param ret results of detected keys
void LDSketch_get_heavy_keys(LDSketch_t* sk, double thresh, myset& ret);

/// estimate the lower sum of a key
// @param sk target sketch 
// @param key
// @return the estimated lower sum
long long LDSketch_low_estimate(LDSketch_t* sk, dyn_tbl_key_t key);

/// estimate the upper sum of a key
// @param sk target sketch 
// @param key
// @return the estimated upper sum
long long LDSketch_up_estimate(LDSketch_t* sk, dyn_tbl_key_t key);

/*************************************************************
 * write functions
 ************************************************************/

/**
 * update the sketch with an data item
 * @param sk the target sketch
 * @param key key of the data item
 * @param val value of the data item
 * @param T expansion parameter
 */
void LDSketch_update(LDSketch_t* sk, unsigned char* key, long long val);

/**
 * copy sketch
 * @param from source sketch
 * @param to target sketch
 */
void LDSketch_copy(LDSketch_t* from, LDSketch_t* to);

/**
 * reset sketch
 * @param sk the target sketch
 * @param output filename of the output file
 */
void LDSketch_reset(LDSketch_t* sk);
