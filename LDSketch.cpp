#include "../HeavyHitterCommon.h"
#include "LDSketch.hpp"

/****************************
 * Private functions
 * *************************/

// reset all internal data structures
void LDSketch::reset() {
    if (is_heavy_changer) {
        sketch_maj_copy(sk_cur, sk_old);
    }
    sketch_maj_reset(sk_cur);
}
    
unsigned int LDSketch::fill_in_cand_list(double thresh) {
    long long old_low, old_up;
    long long new_low, new_up;
    long long diff;

    myset res;
    sketch_maj_get_heavy_keys(sk_cur, thresh, res);
    if (is_heavy_changer) {
        sketch_maj_get_heavy_keys(sk_old, thresh, res);
    }

    //LOG_MSG("Res size %u\n", res.size());

    unsigned int no_cand = 0;
    for (std::unordered_set<dyn_tbl_key_t>::iterator it = res.begin();
            it != res.end(); ++it) {

        old_low = 0;
        old_up = 0;
        if (is_heavy_changer) {
            old_low = sketch_maj_low_estimate(sk_old, *it);
            old_up = sketch_maj_up_estimate(sk_old, *it);
        }
        new_low = sketch_maj_low_estimate(sk_cur, *it);
        new_up = sketch_maj_up_estimate(sk_cur, *it);

        if (new_low > new_up || old_low > old_up) {
            LOG_MSG("Warning: estimator error\n");
        }

        diff = MAX(new_up - old_low, old_up - new_low);
        //printf("old: %lld %lld, new: %lld, %lld, diff: %lld\n", old_low, old_up, new_low, new_up, diff);

        if (diff >= thresh) {
		    memcpy(cand_list[no_cand], it->key, sizeof(dyn_tbl_key_t));
            no_cand++;
        }
        
    }
    return no_cand;
}


void LDSketch::print_parameter() {
    BasicDetector::print_parameter();
    unsigned int real_size = width * depth + length * (1+(double)n/64);
    LOG_MSG("LDSketch: config width %u, depth %u, length %u, actual size %u\n", width, depth, length, real_size);
}

void LDSketch::calculate_diff() {
}

/****************************
 * Public functions
 * *************************/

LDSketch::LDSketch(Config* config, unsigned int id): BasicDetector(config, id) {
    name = "New Detector";

    // new detector:
    width = config->getint("width", 1);
    depth = config->getint("depth", 1);
    length = config->getint("length", 1);
}

void LDSketch::init() {
    Config *config = Config::getInstance();
    double thresh = config->getdouble("thresh", 0);
    sk_cur = sketch_maj_init(width, depth, length, n, thresh*MB, id);
    sketch_maj_reset(sk_cur);
    if (is_heavy_changer) {
        sk_old = sketch_maj_init(width, depth, length, n, thresh*MB, id);
        sketch_maj_reset(sk_old);
    }
    else {
        sk_old = NULL;
    }


    BasicDetector::init();
}

void LDSketch::update(unsigned char* key_str, long long value) {

    sketch_maj_update(sk_cur, key_str, value);
    //dyn_tbl_update(dyn_tbl_cur, key_str, value);
}
    
void LDSketch::print_internal_state(std::string output_file) {
    sketch_maj_write_plaintext(sk_cur, ("sk_cur" + output_file).c_str());
    //dyn_tbl_print(dyn_tbl_cur, ("count_cur" + output_file).c_str());
    if (is_heavy_changer) {
        sketch_maj_write_plaintext(sk_old, ("sk_old" + output_file).c_str());
        //dyn_tbl_print(dyn_tbl_old, ("count_old" + output_file).c_str());
    }
}

void LDSketch::clean() {
    sketch_maj_destroy(sk_cur);
    //dyn_tbl_destroy(dyn_tbl_cur);
    if (is_heavy_changer) {
        sketch_maj_destroy(sk_old);
        //dyn_tbl_destroy(dyn_tbl_old);
    }
    BasicDetector::clean();
}
