#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "dyn_tbl.hpp"

void dyn_tbl_get_heavy_key(dyn_tbl_t* dyn_tbl, long long thresh, unsigned char* keys, int* num_key) {
    long long bias = dyn_tbl->decrement;
    thresh = thresh - bias;
    //for(std::unordered_map<dyn_tbl_key_t, long long>::iterator it = dyn_tbl->array.begin(); it != dyn_tbl->array.end(); ++it) {
    int n = 0;
    for(auto it = dyn_tbl->array.begin(); it != dyn_tbl->array.end(); ++it) {
        if (abs(it->second) >= thresh) {
            memcpy(keys+n*dyn_tbl->lgn/8, it->first.key, dyn_tbl->lgn/8);
            n++;
            // dyn_tbl_key_t key = it->first;
            // ret.insert(key);
        }
    }
    
    *num_key = n;
}

long long dyn_tbl_low_estimate(dyn_tbl_t* dyn_tbl, unsigned char* key_str) {
    dyn_tbl_key_t key;
    memcpy(key.key, key_str, dyn_tbl->lgn/8);

    if (dyn_tbl->array.find(key) != dyn_tbl->array.end()) {
        return dyn_tbl->array[key];
    }
    else {
        return 0;
    }
}

long long dyn_tbl_up_estimate(dyn_tbl_t* dyn_tbl, unsigned char* key_str) {
    long long ret = 0;
    dyn_tbl_key_t key;
    memcpy(key.key, key_str, dyn_tbl->lgn/8);

    if (dyn_tbl->array.find(key) != dyn_tbl->array.end()) {
        ret = dyn_tbl->array[key];
    }
    double bias = dyn_tbl->decrement;
    return ret + bias;
}

void dyn_tbl_print(dyn_tbl_t* dyn_tbl, const char* output) {
    FILE* fp;

	// open a file
	if ((fp = fopen(output, "w")) == NULL) {
		fprintf(stderr, "ERR: cannot open %s\n", output);
		exit(-1);
	}

    unsigned int len = dyn_tbl->array.size();
    fprintf(fp, "length: %u\n", len);
    // for(std::unordered_map<dyn_tbl_key_t, long long>::iterator it = dyn_tbl->array.begin(); it != dyn_tbl->array.end(); ++it) {
    for(auto it = dyn_tbl->array.begin(); it != dyn_tbl->array.end(); ++it) {
        dyn_tbl_key_t key = it->first;
        if (dyn_tbl->lgn == 32) {
            char addr1[30];
            unsigned int* ptr = (unsigned int*) key.key;
            fprintf(fp, "%s %lld\n", ip2a(*ptr, addr1), it->second);
        }
        else {
            char addr1[30];
            char addr2[30];
            unsigned int* ptr = (unsigned int*) key.key;
            fprintf(fp, "%s %s %lld\n", ip2a(*ptr, addr1), ip2a(*(ptr+1), addr2), it->second);
        }
    }

	// close the file
	fclose(fp);
}

int dyn_tbl_length(dyn_tbl_t* dyn_tbl) {
    return dyn_tbl->array.size();
}

void dyn_tbl_copy(dyn_tbl_t* dyn_tbl_from, dyn_tbl_t* dyn_tbl_to) {
    dyn_tbl_to->array = dyn_tbl_from->array;
    dyn_tbl_to->decrement = dyn_tbl_from->decrement;
    dyn_tbl_to->total = dyn_tbl_from->total;
    dyn_tbl_to->max_len = dyn_tbl_from->max_len;
    dyn_tbl_to->max_value = dyn_tbl_from->max_value;
}

void dyn_tbl_reset(dyn_tbl_t* dyn_tbl) {
    dyn_tbl->array.clear();
    dyn_tbl->decrement = 0;
    dyn_tbl->total = 0;
    dyn_tbl->max_value = 0;
}

void dyn_tbl_update(dyn_tbl_t* dyn_tbl, unsigned char* key_str, int val) {

    dyn_tbl_key_t key;
    memcpy(key.key, key_str, dyn_tbl->lgn/8);
    dyn_tbl->total += val;

    if (dyn_tbl->array.find(key) != dyn_tbl->array.end()) {
        dyn_tbl->array[key] += val;
    }
    else {
        unsigned int frac = dyn_tbl->total / dyn_tbl->T;
        if (dyn_tbl->array.size() < dyn_tbl->max_len) {
            dyn_tbl->array[key] = val;
        }
        else if (dyn_tbl->max_len < (frac+1)*(frac+2)-1) {
            dyn_tbl->max_len = (frac+1)*(frac+2)-1;
            dyn_tbl->array[key] = val;
        }
        else {
            // std::unordered_map<dyn_tbl_key_t, long long>::iterator it;
            long long min = val;
            for(auto it = dyn_tbl->array.begin(); it != dyn_tbl->array.end(); ++it) {
                if (it->second < min) {
                    min = it->second;
                }
            }
            dyn_tbl->decrement += min;
            for(auto it = dyn_tbl->array.begin(); it != dyn_tbl->array.end(); ) {
                it->second -= min;
                if (it->second <= 0) {
                    dyn_tbl->array.erase(it++);
                }
                else {
                    it++;
                }
            }
            if (min < val) {
                if (dyn_tbl->array.size() >= dyn_tbl->max_len) {
                    fprintf(stderr, "Warning: maj tbl update error\n");
                }
                dyn_tbl->array[key] = val - min;
            }
        }
    }
    long long value = 0;
    if (dyn_tbl->array.find(key) != dyn_tbl->array.end()) {
        value = dyn_tbl->array[key] + dyn_tbl->decrement;
    }
    else {
        value = dyn_tbl->decrement;
    }
    if (value > dyn_tbl->max_value)
        dyn_tbl->max_value = value;
}

dyn_tbl_t* dyn_tbl_init(unsigned int length, int lgn, long long T) {
    dyn_tbl_t* ret = (dyn_tbl_t*)calloc(1, sizeof(dyn_tbl_t));
    ret->lgn = lgn;
    ret->max_len = length;
    ret->decrement = 0;
    ret->total = 0;
    ret->T = T;
    ret->max_value = 0;
    return ret;
}

void dyn_tbl_destroy(dyn_tbl_t* dyn_tbl) {
    free(dyn_tbl);
}
