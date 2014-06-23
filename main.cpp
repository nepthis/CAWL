/*
 * main.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cawl-server
 */
#include <stdio.h>
#include "Ground_control/Ground.h"
#include "Major_Tom/Major_Tom.h"

int main(int argc, char * args[]){
	for(int i = 0; i < argc; i++){
		if (not ((args[0] == "ground") or (args[0] == "major"))){
			throw std::logic_error("Invalid Arguments\n");
		}
	}
	if((std::string)args[0] == "ground"){
		Ground gc =  Ground();

	}
	else if((std::string)args[0] =="major"){
		//Todo, gör om major tom till ett objekt/klass.
		Major_Tom::Mobile mobile = Major_Tom::Mobile();

	}


}
