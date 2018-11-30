#pragma once

#include <iostream>
#include <thread>
#include <vector>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <array>
#include <deque>
#include <thread>

#include <boost/filesystem.hpp>
namespace bstfs = boost::filesystem;

#include "cpputility/stringutility.h"



std::string exec(const std::string& cmd) noexcept {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (pipe){
	    while (!feof(pipe.get())) {
	        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
	            result += buffer.data();
	    }
	}
	else
		result = "Error in opening pipe";
    return result;
}

/*! \brief Run the python code
 *
 *  Two options: 
 *	1) Run from terminal
 *  2) Run over network
 *  	- Deal with data transfer
 *		- Send the script over the network and run there 
 */
class ExecuteCode{
public:
	// ExecuteCode(const std::string& folderpath): folderpath(folderpath){}
	// ExecuteCode(): folderpath( bstfs::unique_path("%%%%-%%%%-%%%%-%%%%") ){}
	ExecuteCode(){}
	
	virtual bool setFolder(const std::string& path){
		folderpath = path;
		folderIsSet = true;
		return true;
	}

	virtual bool runTerminalCode(std::string terminalstring){
	}
	virtual bool runScript(std::string ScriptName,std::string ScriptArgs){
	}
	virtual bool run(std::string scriptpath){

	}
	virtual bool runPythonTerminalCode(std::string terminalstring){
		// std::cout << exec("python -c \"import pandas as pd;import numpy as np;df=pd.DataFrame();print df;import matplotlib.pyplot as plt;plt.plot([1,2],[1,2]);plt.show();\"");
		std::cout << exec("python -c \"" + terminalstring + "\"") << "\n";
	}
	// virtual bool runPythonTerminalScript(std::string ScriptPathArgs){
	// 	// std::cout << exec("python -c \"import pandas as pd;import numpy as np;df=pd.DataFrame();print df;import matplotlib.pyplot as plt;plt.plot([1,2],[1,2]);plt.show();\"");
		
	// 	std::cout << exec("python " + ScriptPathArgs) << "\n";
	// }

private:
	bstfs::path folderpath;
	bool folderIsSet = false;
};

class ExecutePythonTerminalScript: public ExecuteCode{
public:

	bool run(std::string ScriptPathArgs){
		std::cout << "running : " << ScriptPathArgs << "\n";
		std::cout << exec("python " + ScriptPathArgs) << "\n";
		// runPythonTerminalScript(ScriptPathArgs);
	}


};