#pragma once

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "cpputility/stringutility.h"

#include <boost/filesystem.hpp>
namespace bstfs = boost::filesystem;


// for convenience
using json = nlohmann::json;

/*
//## TODO
# check pos_type of tellp
*/


std::string getType2String(int& a) noexcept {return "int";}
std::string getType2String(float& a) noexcept {return "float";}
std::string getType2String(double& a) noexcept {return "double";}
std::string getType2String(long double& a) noexcept {return "longdouble";}
std::string getType2String(const char* a) noexcept {return "string";}

template <typename Derived>
std::string getType2String(const Eigen::MatrixBase<Derived>& M){
	typename Derived::Scalar a;
	return "matrix<"+getType2String( a ) +">" ;
}

struct VariableMeta{
	explicit VariableMeta(){};

	std::string variable_name;
	std::string vartype;
	std::string internalVartype;
	long unsigned int startBytePosition=0;
	long unsigned int NBytes=0;
	long unsigned int count=0; // like nrows*ncols
 	long unsigned int NBytes_per_unit=0;
 	int nrows=0;
	int ncols=0;

	bool status =false;

	static std::vector<std::string> headers;
};
std::vector<std::string> VariableMeta::headers = {"variable_name","vartype","internalVartype","startBytePosition","NBytes","count","NBytes_per_unit","nrows","ncols"};


//  ---------- print VariableMeta --------------------
std::ostream& operator<<(std::ostream& outstream, const VariableMeta& Vm){
	outstream << Vm.variable_name << "," << Vm.vartype 
				<< "," << Vm.internalVartype 
				<< "," << Vm.startBytePosition 
				<< "," << Vm.NBytes 
				<< "," << Vm.count 
				<< "," << Vm.NBytes_per_unit 
				<< "," << Vm.nrows 
				<< "," << Vm.ncols << "\n";
	return outstream;
}

// 
VariableMeta appendBinaryFile(const std::string& filepath,const std::string& datastr) noexcept {
	VariableMeta Varmeta;
	std::ofstream outFileStream(filepath.c_str(), std::ios::binary |  std::ios::out | std::ios::app );
	
	if (!outFileStream.is_open()){
		Varmeta.status = false;
		return Varmeta;
	}

	int Nbytes = datastr.size()*sizeof(char);
	outFileStream.write(datastr.c_str(),Nbytes);

	Varmeta.variable_name = "";
	Varmeta.vartype = "string";
	Varmeta.internalVartype = "char";
	Varmeta.startBytePosition = outFileStream.tellp();
	Varmeta.NBytes = Nbytes;
	Varmeta.count = datastr.size(); // like nrows*ncols
 	Varmeta.NBytes_per_unit = sizeof(char);
 	Varmeta.nrows = 0;
	Varmeta.ncols = 0;
	Varmeta.status = true;

	return Varmeta;
}

// template <typename T, typename S= typename std::enable_if< std::is_pod<T>::value,T >::type  >
// int appendBinaryFile(const std::string& filepath,const T& M)noexcept{
// 	std::ofstream outFileStream(filepath.c_str(), std::ios::binary |  std::ios::out | std::ios::app );
// 	if (!outFileStream.is_open())
// 		return 0;
// 	int Nbytes = sizeof(T);
// 	outFileStream.write(reinterpret_cast<char*>(const_cast<std::remove_cv<T>::type>(M)),Nbytes);
// 	return Nbytes;
// }


/*! \brief convertappendBinaryFile overriding function
 *         first convert and then write to file
 *
 *  In this override: Eigen matrix is written to binary file at the end
 *  Written row-wise
 */
template <typename Derived>
VariableMeta appendBinaryFile(const std::string& filepath,const Eigen::MatrixBase<Derived>& M) noexcept {
	VariableMeta Varmeta;

	typedef typename Derived::Scalar Scalar; 
	Eigen::Matrix<Scalar,Eigen::Dynamic,Eigen::Dynamic,Eigen::ColMajor> M_ColWise = M;

	std::ofstream outFileStream(filepath.c_str(), std::ios::binary |  std::ios::out | std::ios::app );
	if (!outFileStream.is_open()){
		Varmeta.status = false;
		return Varmeta;
	}
	auto startBytePosition = outFileStream.tellp();
	int Nbytes = (M_ColWise.rows()*M_ColWise.cols())*sizeof(Scalar);
	outFileStream.write(reinterpret_cast<char*>(M_ColWise.data()),Nbytes);
	
	Scalar dummy;
	Varmeta.variable_name = "";
	Varmeta.vartype = "matrix";
	Varmeta.internalVartype = getType2String(dummy);
	Varmeta.startBytePosition = startBytePosition;
	Varmeta.NBytes = Nbytes;
	Varmeta.count = (M_ColWise.rows()*M_ColWise.cols()); // like nrows*ncols
 	Varmeta.NBytes_per_unit = sizeof(Scalar);
 	Varmeta.nrows = M_ColWise.rows();
	Varmeta.ncols = M_ColWise.cols();
	Varmeta.status = true;

	return Varmeta;
}	

template <typename T>
VariableMeta appendBinaryFile(const std::string& filepath,std::vector<T>& V) noexcept {
	VariableMeta Varmeta;

	std::ofstream outFileStream(filepath.c_str(), std::ios::binary |  std::ios::out | std::ios::app );
	if (!outFileStream.is_open()){
		Varmeta.status = false;
		return Varmeta;
	}
	auto startBytePosition = outFileStream.tellp();
	int Nbytes = V.size()*sizeof(T);
	outFileStream.write(reinterpret_cast<char*>(V.data()),Nbytes);
	
	T dummy;
	Varmeta.variable_name = "";
	Varmeta.vartype = "matrix";
	Varmeta.internalVartype = getType2String(dummy);
	Varmeta.startBytePosition = startBytePosition;
	Varmeta.NBytes = Nbytes;
	Varmeta.count = V.size(); // like nrows*ncols
 	Varmeta.NBytes_per_unit = sizeof(T);
 	Varmeta.nrows = V.size();
	Varmeta.ncols = 1;
	Varmeta.status = true;

	return Varmeta;

}	

//  ---------- print vector --------------------
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& V){
	for(auto const& v: V){
		out << v << " ";
	}
	return out;
}

//  ---------- ------------ --------------------
class BaseDataSet{
public:
	BaseDataSet() noexcept{
		std::cout << "BaseDataSet ctor \n";
	}
	BaseDataSet(const char* folderpath) noexcept : _folderpath(folderpath) {
		std::cout << "BaseDataSet ctor folderpath\n";
	}
	//copy ctor
	BaseDataSet(const BaseDataSet& ds) noexcept:
	_metadata(ds._metadata),_folderpath(ds._folderpath),binDataPath(ds.binDataPath), metaPath(ds.metaPath){
		std::cout << "BaseDataSet copy ctor\n";

	}
	//copy ass
	BaseDataSet& operator=(const BaseDataSet& ds)noexcept{
		_metadata = ds._metadata;
		_folderpath = ds._folderpath;
		binDataPath = ds.binDataPath;
		metaPath = ds.metaPath;
		std::cout << "BaseDataSet copy ass\n";

		return *this;
	}
	//move ctor
	BaseDataSet(BaseDataSet&& ds) noexcept: _metadata(std::move(ds._metadata)),
											_folderpath(std::move(ds._folderpath)),
											binDataPath(std::move(ds.binDataPath)), 
											metaPath(std::move(ds.metaPath)){
		std::cout << "BaseDataSet move ctor\n";

	} 
	//move assignment
	BaseDataSet& operator=(BaseDataSet&& ds)noexcept{
		_metadata = std::move(ds._metadata);
		_folderpath = std::move(ds._folderpath);
		binDataPath = std::move(ds.binDataPath);
		metaPath = std::move(ds.metaPath);
		std::cout << "BaseDataSet move ass\n";

		return *this;
	}
	virtual bstfs::path setUniqueFolder(){
		_folderpath=bstfs::unique_path("%%%%-%%%%-%%%%-%%%%");
		return _folderpath;
	} 
	virtual std::string getFolderPath()noexcept{
		return _folderpath.string();
	}
	virtual std::string getBinaryPath()noexcept{
		return _folderpath.string()+"/"+binDataPath;
	}
	virtual std::string getMetaPath()noexcept{
		return _folderpath.string()+"/"+metaPath;
	}
	virtual bool clearFoldersIfExists(){
		bstfs::remove_all( _folderpath );
		bstfs::create_directory( _folderpath );		
	}
	virtual	bool initialize() noexcept {
		std::cout << "BaseDataSet initialize\n";

		// bstfs::remove_all( _folderpath );
		// bstfs::create_directory( _folderpath );

		std::ofstream metaStream(getMetaPath().c_str(), std::ios::out | std::ios::app);
		
		for(int i=0;i<VariableMeta::headers.size();++i){
			if(i==VariableMeta::headers.size()-1)
				metaStream << VariableMeta::headers[i] << "\n";
			else
				metaStream << VariableMeta::headers[i] << ",";
		}	
		return true;
	}

	// virtual void add_variable(const char* variable_name)noexcept{}

	virtual std::vector<VariableMeta>::iterator begin() {
		return _metadata.begin();
	}
	virtual std::vector<VariableMeta>::iterator end() {
		return _metadata.end();
	}
	virtual void print_meta(){
		std::cout << "-------\n " << _metadata << "\n";
	}
	virtual bool updateMeta(VariableMeta Vm){
		std::cout << "BaseDataSet updateMeta\n";
		_metadata.push_back(Vm) ;
		std::ofstream metaStream(getMetaPath().c_str(), std::ios::out | std::ios::app);
		metaStream << Vm;
	}

	virtual bool run(){

	}
private:
	std::vector<VariableMeta> _metadata;
	bstfs::path _folderpath = "";
	std::string binDataPath = "data.bin"; 
	std::string metaPath = "metadata.txt";

};

class BinaryDataSet: public BaseDataSet{
public:

	//ctor
	BinaryDataSet(const char* folderpath) noexcept : BaseDataSet(folderpath) {
		// initialize();
	}
	//ctor
	BinaryDataSet()noexcept: BaseDataSet() {
		// initialize();
	}

	//copy-ctor
	BinaryDataSet(const BinaryDataSet& ds)noexcept:BaseDataSet(ds){

	}
	//copy-ass
	BinaryDataSet& operator=(const BinaryDataSet& ds) noexcept {
		BaseDataSet::operator=(ds);
		return *this;
	}

	//move-ctor
	BinaryDataSet(BinaryDataSet&& ds)noexcept:BaseDataSet(std::move(ds)) {
	}
	//move-ass
	BinaryDataSet& operator=(BinaryDataSet&& ds){
		BaseDataSet::operator=(std::move(ds));
		return *this;
	}
	bool initialize() noexcept {
		std::cout << "BinaryDataSet initialize\n";
		this->BaseDataSet::initialize();
	}
	template <typename T>
	void add_variable(const char* variable_name,T&& M)noexcept{
		auto Vm = appendBinaryFile( getBinaryPath() ,std::forward<T>(M) );
		Vm.variable_name = variable_name;
		updateMeta(Vm);	
	}

};

























