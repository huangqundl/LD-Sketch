/*
 * sketch_maj.h
 * - heavy hitter random partition
 */
#include "dyn_tbl.hpp"

typedef struct sketch_maj_s * sketch_maj_p_t;

/// Sketch structure
typedef struct sketch_maj_s {
    /// bucket array
	dyn_tbl_p_t* tbl;

    /*********************************************************
     * read only variables
     ********************************************************/
    /// # of rows
	int h;

    /// # of buckets per row
	int w;

    /// # of counters in each tbl
    int l;

    /// total number of buckets
    int size;

    /// length of keys (in bits)
	int n;

    /// detection threshold
    double thresh_abs;

    /// id of detectors (detectors have different hash functions)
    unsigned int tbl_id;
} sketch_maj_t;

/*************************************************************
 * create and destroy
 ************************************************************/

/// init sketch
// @return the pointer to the created sketch
sketch_maj_p_t sketch_maj_init(int w, int h, int l, int n, double thresh_abs, unsigned int tbl_id);

/// free scketch
void sketch_maj_destroy(sketch_maj_p_t sketch_maj);

/*************************************************************
 * read functions
 ************************************************************/

/// print out the sketch to file
// @param sk target sketch
// @param output name of output file
void sketch_maj_write_plaintext(sketch_maj_p_t sk, const char* output);

/// identify heavy keys
// @param sk target sketch
// @param thresh threshold for heavy keys
// @param ret results of detected keys
void sketch_maj_get_heavy_keys(sketch_maj_p_t sk, double thresh, myset& ret);

/// estimate the lower sum of a key
// @param sk target sketch 
// @param key
// @return the estimated lower sum
long long sketch_maj_low_estimate(sketch_maj_p_t sk, dyn_tbl_key_t key);

/// estimate the upper sum of a key
// @param sk target sketch 
// @param key
// @return the estimated upper sum
long long sketch_maj_up_estimate(sketch_maj_p_t sk, dyn_tbl_key_t key);

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
void sketch_maj_update(sketch_maj_p_t sk, unsigned char* key, long long val);

/**
 * copy sketch
 * @param from source sketch
 * @param to target sketch
 */
void sketch_maj_copy(sketch_maj_p_t from, sketch_maj_p_t to);

/**
 * reset sketch
 * @param sk the target sketch
 * @param output filename of the output file
 */
void sketch_maj_reset(sketch_maj_p_t sk);
