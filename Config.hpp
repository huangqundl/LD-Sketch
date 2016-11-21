#ifndef SAND_CONFIG_HPP_
#define SAND_CONFIG_HPP_

#include <stdint.h>

#include <iniparser.h>

#include "Util.hpp"

#define MAX_LINE_SIZE 1024

const char* strconcat(const char* a, const char* b);

class Config {
private:
    static Config *instance_;

    dictionary *dict_;
    char section_name_[MAX_LINE_SIZE];

	uint32_t num_sub_analyzers_;		// number of sub_analyzers
	uint32_t flow_table_size_;			// flow table size
	double flow_table_timeout_;			// flow table expire timeout
    /*
	uint32_t pair_port_table_size_;		// pair-port table size
	double pair_port_table_timeout_;	// pair-port table expire timeout
	uint32_t pair_table_size_;			// pair table size
	double pair_table_timeout_;			// pair table expire timeout
	uint32_t ipport_table_size_;		// ipport table size
	double ipport_table_timeout_;		// ipport table expire timeout
    */
	bool is_event_print_;				// true if event sent to stdout

    Config(const char* ininame = NULL, const char* section_name = "");

public:
	~Config();

    static Config *getInstance(const char* ininame = NULL, const char* section_name = "") {
        if (!instance_) {
            instance_ = new Config(ininame, section_name);
        }

        return instance_;
    }

	inline uint32_t get_num_sub_analyzers() { 
		return num_sub_analyzers_; 
	}
	inline uint32_t get_flow_table_size() { 
		return flow_table_size_; 
	} 
	inline double get_flow_table_timeout() { 
		return flow_table_timeout_; 
	}
    /*
	inline uint32_t get_pair_port_table_size() { 
		return pair_port_table_size_; 
	} 
	inline double get_pair_port_table_timeout() { 
		return pair_port_table_timeout_; 
	}
	inline uint32_t get_pair_table_size() { 
		return pair_table_size_; 
	} 
	inline double get_pair_table_timeout() { 
		return pair_table_timeout_; 
	}
	inline uint32_t get_ipport_table_size() { 
		return ipport_table_size_; 
	} 
	inline double get_ipport_table_timeout() { 
		return ipport_table_timeout_; 
	}
    */
	inline bool is_event_print() {
		return is_event_print_;
	}

    inline char* getstring(const char* key, char* def) {
        return iniparser_getstring(dict_, strconcat(section_name_, key), def);
    }
    inline int getint(const char* key, int notfound) {
        return iniparser_getint(dict_, strconcat(section_name_, key), notfound);
    }
    inline unsigned long long int get_ull(const char* key, unsigned long long int notfound) {
        char* str = iniparser_getstring(dict_, strconcat(section_name_, key), NULL);
        if (str == NULL) return notfound;
        return strtoull(str, NULL, 10);
    }
    inline double getdouble(const char* key, double notfound) {
        return iniparser_getdouble(dict_, strconcat(section_name_, key), notfound);
    }
    inline int getboolean(const char* key, int notfound) {
        return iniparser_getboolean(dict_, strconcat(section_name_, key), notfound);
    }

    inline const char *get_section_name() {
        return section_name_;
    }
};

#endif // SAND_CONFIG_HPP_
