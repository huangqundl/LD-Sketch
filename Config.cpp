#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Config.hpp"

Config* Config::instance_ = NULL;

const char* strconcat(const char* a, const char* b) {
    static char l[MAX_LINE_SIZE+1];

    if (a == NULL)
        return b;
    if (b == NULL)
        return NULL;

    strcpy(l, a);
    strcat(l, b);

    return l;
}

/*
 * Constructor
 */
Config::Config(const char* ininame, const char* section_name) {
	// set default parameters
	num_sub_analyzers_ = 2;
	flow_table_size_ = 100000;
	flow_table_timeout_ = 600;
    /*
	pair_port_table_size_ = 1000000;
	pair_port_table_timeout_ = 120;
	pair_table_size_ = 1000000;
	pair_table_timeout_ = 7200;
	ipport_table_size_ = 1000000;
	ipport_table_timeout_ = 7200;
    */
	is_event_print_ = true;

    if (ininame == 0) {
        fprintf(stderr, "didn't specify configuration file\n");
        return;
    }

    dict_ = iniparser_load(ininame);
    if (dict_ == 0) {
        fprintf(stderr, "error: iniparser_load()\n");
        return;
    }

    if (strlen(section_name) + 2 > MAX_LINE_SIZE) {
        fprintf(stderr, "error: section name is too long\n");
        return;
    }
    strcpy(section_name_, section_name);
    section_name_[strlen(section_name_)] = ':';
    section_name_[strlen(section_name_) + 1] = 0;

    num_sub_analyzers_ = getint("numSubAnalyzers", num_sub_analyzers_);
    flow_table_size_ = getint("flowTableSize", flow_table_size_);
    flow_table_timeout_ = getdouble("flowTableTimeout", flow_table_timeout_);
    /*
    pair_port_table_size_ = getint("pairPortTableSize", pair_port_table_size_);
    pair_port_table_timeout_ = getdouble("pairPortTableTimeout", pair_port_table_timeout_);
    pair_table_size_ = getint("pairTableSize", pair_table_size_);
    pair_table_timeout_ = getdouble("pairTableTimeout", pair_table_timeout_);
    ipport_table_size_ = getint("ipportTableSize", ipport_table_size_);
    ipport_table_timeout_ = getdouble("ipportTableTimeout", ipport_table_timeout_);
    */
    is_event_print_ = getboolean("isEventPrint", is_event_print_);
}

Config::~Config() {
    iniparser_freedict(dict_);
}
