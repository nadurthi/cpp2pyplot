// #include "datasetBuilder.h"
#include "cpputility/plotBuilder.h"

namespace bstfs = boost::filesystem;

int main(){

	// create an empty structure (null)
json j;

// add a number that is stored as double (note the implicit conversion of j to an object)
j["pi"] = 3.141;

// add a Boolean that is stored as bool
j["happy"]["hh"] = true;


Eigen::Matrix<double,6,6> M1;
M1(0,0)=1;
M1(0,5)=2;
M1(5,0)=3;
M1(5,5)=4;

Eigen::Matrix<double,4,2> M2;
Eigen::Matrix<int,3,3> Mint;


std::vector<int> Vi={1,2,3,4};
std::vector<double> Vd={1.1,2.2,3.3};



std::cout << "ok" <<"\n";
std::cout << j << "\n";
std::cout << typeid(Eigen::Matrix<double,6,6>).name() << "\n";

// double dd = 33.33;

// auto ds = BinaryDataSet("testdata");
// // ds.clearpaths();
// // ds.initialize();
// appendBinaryVariable(&ds,"X0", M1);
// appendBinaryVariable(&ds,"KalmanGain",M2);
// appendBinaryVariable(&ds,"Xpara",Mint);
// appendBinaryVariable(&ds,"statevec",Vd);
// appendBinaryVariable(&ds,"paravec",Vi);
// appendBinaryVariable(&ds,"para",dd));

// ds.add_variable("X0", M1);
// ds.add_variable("KalmanGain",M2);
// ds.add_variable("Xpara",Mint);

// ds.add_variable("statevec",Vd);
// ds.add_variable("paravec",Vi);
// ds.add_variable("para",dd);
// ds.print_meta();
// ds.generate_metadata_json('test.json');

// ds.close();

// bstfs::remove_all("foobar");

// writeBinary("testdata/data.bin",M1);
// M1.data();
// print_inv_cond(M1);
// bstfs::create_directory( bstfs::unique_path("%%%%-%%%%-%%%%-%%%%") );


 
auto plotter = BuildPlotter("newtestdata",new BinaryDataSet(), new BuildPythonScript(), new ExecutePythonTerminalScript() );

plotter.add_variable("X0", M1);
plotter.add_variable("KalmanGain",M2);
plotter.add_variable("Xpara",Mint);
plotter.add_variable("statevec",Vd);
plotter.add_variable("paravec",Vi);

plotter.add_script_line(
R"V0G0N(

for i in range(2):
	plt.figure()
	plt.plot(X0[:,0],X0[:,5],'ro-')
plt.show()

)V0G0N"
);


plotter.run();

Plotter<BinaryDataSet,BuildPythonScript,ExecutePythonTerminalScript> myplotter("newtestdata2222");
myplotter.add_variable("X0", M1);
myplotter.add_variable("KalmanGain",M2);
myplotter.add_variable("Xpara",Mint);
myplotter.add_variable("statevec",Vd);
myplotter.add_variable("paravec",Vi);

myplotter.add_script_line(
R"V0G0N(

for i in range(2):
	plt.figure()
	plt.plot(X0[:,0],X0[:,5],'ro-')
plt.show()

)V0G0N"
);
myplotter.run();

}
