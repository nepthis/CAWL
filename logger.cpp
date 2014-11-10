#include <fstream>

#define LOGFILE "cawl_logfile.txt"

void write_text_to_log_file( const std::string &text )
{
    std::ofstream log_file(
        LOGFILE, std::ios_base::out | std::ios_base::app );
    log_file << text << std::endl;
}
