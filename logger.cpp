
#include "logger.h"



std::string logLevel = "error";
std::mutex m_logError;
void logError( const std::string &text )
{
	m_logError.lock();
	std::time_t result = std::time(NULL);
	std::string t(std::ctime(&result));

	std::ofstream log_file(LOGFILE, std::ios_base::out | std::ios_base::app );
	if(logLevel == "verbose"){
		std::cout << "error: "<< text << std::endl;
	}
	log_file << t.substr(0, t.length()-1)  << ": error: "<< text << std::endl;
	m_logError.unlock();
}

void logWarning( const std::string &text )
{

	if( (logLevel == "warning") ||( logLevel == "verbose") || (logLevel == "info")){
		m_logError.lock();
		std::time_t result = std::time(NULL);
		std::string t(std::ctime(&result));
		std::ofstream log_file(LOGFILE, std::ios_base::out | std::ios_base::app );
		if(logLevel == "verbose"){
			std::cout << "warning: "<< text << std::endl;
		}
		log_file << t.substr(0, t.length()-1)  << ": warning: "<< text << std::endl;
		m_logError.unlock();
	}

}
void logVerbose( const std::string &text )
{
	if((logLevel == "verbose") || (logLevel == "info")){
		m_logError.lock();
		std::time_t result = std::time(NULL);
		std::string t(std::ctime(&result));

		std::ofstream log_file(
				LOGFILE, std::ios_base::out | std::ios_base::app );
		if(logLevel == "verbose"){
			std::cout << logLevel <<": "<< text << std::endl;
		}
		log_file << t.substr(0, t.length()-1)  << ": "<<logLevel << ": "<< text << std::endl;
		m_logError.unlock();
	}

}
