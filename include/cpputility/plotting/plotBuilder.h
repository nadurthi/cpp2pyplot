
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

#include <boost/filesystem.hpp>
namespace bstfs = boost::filesystem;

#include "cpputility/stringutility.h"
#include "cpputility/datasetBuilder.h"
#include "cpputility/scriptBuilder.h"
#include "cpputility/scriptExecuter.h"



enum class PlotterBackend{
	Python,
	Matlab
};

enum class PlotterExec{
	LocalTerminal,
	Network
};


/*! \brief Plotting via other tools
 *
 * 	we have python plotter and matlab plotter.
 */
class BuildPlotter{

public:
	BuildPlotter(const std::string& folderpath, BaseDataSet* dstr, 
				BuildScript* scptr, ExecuteCode* extr ) : 
												folderpath(folderpath),
												datasetter(dstr),
												scripter(scptr),
												executer(extr), folderIsSet(true){
		initialize();
	}
	// BuildPlotter(const std::string& folderpath, const BaseDataSet& dstr, 
	// 			const BuildScript& scptr, const ExecuteCode& extr ) : 
	// 											folderpath(folderpath),
	// 											datasetter(dstr),
	// 											scripter(scptr),
	// 											executer(extr), folderIsSet(true){
	// 	initialize();
	// }
	// BuildPlotter(std::shared_ptr<BaseDataSet> dstr, std::shared_ptr<BuildScript> scptr, std::shared_ptr<ExecuteCode> extr ) : 
	// 											datasetter(dstr),
	// 											scripter(scptr),
	// 											executer(extr), folderIsSet(false){

	// }

	bool initialize(){
		if (folderIsSet){
			bstfs::remove_all( folderpath );
			bstfs::create_directory( folderpath );

			datasetter->setFolder(folderpath.string());
			datasetter->initialize();
			
			scripter->setFolder(folderpath.string());
			scripter->initialize();
		}
	}
	virtual bool setFolder(const std::string& path){
		folderpath = path;
		folderIsSet = true;
		return true;
	}
	virtual bstfs::path setUniqueFolder(){
		folderpath=bstfs::unique_path("%%%%-%%%%-%%%%-%%%%");
		return folderpath;
	} 

	template <typename T>
	void add_variable(const char* variable_name, T&& M){
		appendBinaryVariable(datasetter,variable_name, std::forward<T>(M));
	}
	bool add_script_line(const std::string& str){
		scripter->add_script_line(str);
	}
	bool run(){
		datasetter->run();
		
		scripter->add_DataSourceLoadLines(datasetter);
		scripter->generateScript();

		executer->run(scripter->getScriptPath());

	}

public:
	BaseDataSet* datasetter ;
	BuildScript* scripter ;
	ExecuteCode* executer ;


private:
	bstfs::path folderpath;
	bool folderIsSet = false;
};

// BuildPlotter plotter<BinaryDataSet,BuildPythonScript,RunPythonTerminalScript>()
// plotter.datasetter->add_varaible();
// plotter.scripter->add_line("plot(1,1,'ro')");
// plotter.run();



// bs.add_dataSource("testdata")
// bs.add_line("plt.pyplot(X[:,1],X[:,0],'ro')")
// bs.submit();
template<typename Datasetter, typename Scripter, typename Executer >
class Plotter: public Datasetter, public Scripter, public Executer{
public:
	Plotter(const std::string& folderpath): Datasetter(), Scripter(), Executer(), folderpath(folderpath), folderIsSet(true){
		initialize();
	}
	bool initialize() noexcept {
		if (folderIsSet){
			bstfs::remove_all( folderpath );
			bstfs::create_directory( folderpath );
		}
		Datasetter::setFolder(folderpath.string());
		Datasetter::initialize();

		Scripter::setFolder(folderpath.string());
		Scripter::initialize();


	}

	bool run(){
		Datasetter::run();
		
		Scripter::add_DataSourceLoadLines(Datasetter::getMetaFilePath(), Datasetter::getDataFilePath(), Datasetter::getVarNames());
		Scripter::generateScript();

		Executer::run(Scripter::getScriptPath());

	}

private:
	bstfs::path folderpath;
	bool folderIsSet = false;
};