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
#include "datasetBuilder.h"


/*! \brief Just build python script in strings
 *
 *  Note that this only build the strings and formats them
 *  Use Submit Script to run the script
 */
class BuildScript{
public:
	BuildScript(const std::string& folderpath): folderpath(folderpath),folderIsSet(true){
		initialize();
	}
	BuildScript():folderIsSet(false){
		initialize();
	}
	BuildScript(const BuildScript& Bs):folderpath(Bs.folderpath), 
													datasourcelines(Bs.datasourcelines), 
													importlines(Bs.importlines), 
													scriptLines(Bs.scriptLines),folderIsSet(Bs.folderIsSet){
	} 
	BuildScript& operator=(const BuildScript& Bs){
		folderpath = Bs.folderpath; 
		datasourcelines = Bs.datasourcelines; 
		importlines = Bs.importlines; 
		scriptLines = Bs.scriptLines;
		folderIsSet = Bs.folderIsSet;
		return *this;
	}

	virtual bstfs::path setUniqueFolder(){
		folderpath=bstfs::unique_path("%%%%-%%%%-%%%%-%%%%");
		return folderpath;
	} 
	virtual bool setFolder(const std::string& path){
		folderpath = path;
		folderIsSet = true;
	}
	virtual bool initialize(){
		std::cout << "BaseBuildScript initialize\n";
		if(folderIsSet)
			bstfs::create_directory( folderpath );

	}

	virtual bool add_DataSourceLoadLines(BaseDataSet* dataset){
		std::cout<< "BuildScript add datasourcelines\n";
	};
	virtual bool add_imports(const std::vector<std::string>& vec_import_lines){
		for(auto v: vec_import_lines)
			importlines.push_back(v);
	}

	virtual bool add_script_line(const std::string& strline){
		scriptLines.push_back(strline);
	}
	virtual bool add_datasrc_line(const std::string& strline){
		datasourcelines.push_back(strline);
	}

	int dump(const std::string& filepath,const std::string& datastr){
		std::ofstream scriptfileObject(filepath,std::ios::out);
		if (!scriptfileObject.is_open()){
			return 0;
		}
		int Nbytes = datastr.size()*sizeof(char);
		scriptfileObject.write(datastr.c_str(), Nbytes);
		return true;

	}

	virtual std::vector<std::string> assembleScriptLines(){
		std::vector<std::string> fullscriptlines;

		// python -m py_compile script.py
		for( const auto & line: importlines)
			fullscriptlines.push_back(line);
		for( const auto & line: datasourcelines)
			fullscriptlines.push_back(line);
		for( const auto & line: scriptLines)
			fullscriptlines.push_back(line);

		return fullscriptlines;
	}
	virtual bool generateScript(){
		auto fullscriptlines = assembleScriptLines();
		dump(getScriptPath(), concatVectorOfStrings(fullscriptlines, "\n" ));

	}
	virtual std::string returnTerminalExecutableScript(){}

	virtual bool run(){}

	virtual std::string getScriptPath()noexcept{
		return folderpath.string()+"/"+"dummy_script.txt";
	}
	virtual std::string getFolderPath()noexcept{
		return folderpath.string();
	}
	

private:
	bstfs::path folderpath;
	std::deque<std::string> datasourcelines, importlines, scriptLines;
	bool folderIsSet = false;
	
};

class BuildMatlabScript: public BuildScript{


};

class BuildPythonScript: public BuildScript{
public:
	BuildPythonScript(const std::string& folderpath): BuildScript(folderpath){
		initialize();
	}
	BuildPythonScript(): BuildScript(){
		initialize();
	}
	BuildPythonScript(const BuildPythonScript& BpyS):BuildScript(BpyS),scriptFileName(BpyS.scriptFileName) {

	}
	BuildPythonScript& operator=(const BuildPythonScript& BpyS){
		BuildScript::operator = (BpyS);
		scriptFileName = BpyS.scriptFileName;
		return *this;
	}
	bool initialize(){
		this->BuildScript::initialize();

		add_imports({	"import pandas as pd",
						"import numpy as np",
						"import matplotlib.pyplot as plt",
						"import json"});
	}
	
	bool add_DataSourceLoadLines(BaseDataSet* dataset){
		std::ifstream readBinPyFile("python_scripts/readBinaryDataSet.py",std::ios::in);
		std::stringstream ss;
		ss << readBinPyFile.rdbuf();
		add_datasrc_line(ss.str());
		add_datasrc_line("ds = readBinaryDataSet('"+dataset->getMetaFilePath()+"','"+dataset->getDataFilePath()+"')");
		
		for(auto const var : (*dataset) ){
			add_datasrc_line("exec(\""+var.variable_name+" = ds['"+var.variable_name+"']\")");
		}
	}
	bool add_DataSourceLoadLines(const std::string& MetaFilePath, const std::string& DataFilePath, const std::vector<std::string>& variableNames){
		std::ifstream readBinPyFile("python_scripts/readBinaryDataSet.py",std::ios::in);
		std::stringstream ss;
		ss << readBinPyFile.rdbuf();
		add_datasrc_line(ss.str());
		add_datasrc_line("ds = readBinaryDataSet('" + MetaFilePath + "','" + DataFilePath + "')");
		
		for(auto const var : variableNames ){
			add_datasrc_line(var + " = ds['" + var + "']");
		}
	}
	virtual std::string returnTerminalExecutableScript(){
		auto fullscriptlines = assembleScriptLines();
		return concatVectorOfStrings(fullscriptlines,";");
	}

	virtual std::string getScriptPath()noexcept{
		return BuildScript::getFolderPath()+"/"+scriptFileName;
	}
	
	virtual bool generateScript(){
		std::cout << "Script path is : " << getScriptPath() << "\n";
 		auto fullscriptlines = assembleScriptLines();
		dump(getScriptPath(), concatVectorOfStrings(fullscriptlines, "\n" ));

	}


	private:
		std::string scriptFileName = "run_script.py";
};
