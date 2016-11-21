#ifndef LD_SKETCH_HPP
#define LD_SKETCH_HPP

#include <set>

#include "sketch_maj.hpp"

/// LD-Sketch detector
class LDSketch // : public BasicDetector 
{

private:
    /// Sketch for current epoch
    LDSketch_t* sk_cur;
    
    /// Sketch for last epoch
    LDSketch_t* sk_old;

    /// number of rows
    unsigned int depth;
    
    /// number of buckets in a row
    unsigned int width;

    /// initial length for associative array
    unsigned int length;

    bool is_heavy_changer;

private:
    void reset();

    unsigned int fill_in_cand_list(double thresh);

    void print_parameter();

    void calculate_diff();

public:
    /// Constructor
    LDSketch(Config* config, unsigned int id);

    void init();

    void update(unsigned char* key, long long value);

    void print_internal_state(std::string);

    void clean();
};

#endif
