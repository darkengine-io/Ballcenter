#include "olb2D.h"
#ifndef OLB_PRECOMPILED // Unless precompiled version is used,
#include "olb2D.hh"   // include full template code
#endif
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace olb;
using namespace olb::descriptors;
using namespace olb::graphics;
using namespace std;

typedef double T;
#define DESCRIPTOR D2Q9Descriptor

// Parameters for the simulation setup
const int nx = 200;		// 100 recomended, 200 is still ok 400 needs a good processor 800 is similar to 400
const int ny = 100;		// 1/2 * nx
const T uMax = 0.2;	//0.02
const T Re = 100.;


int obst_x = nx / 5 - 1;
int obst_y = ny / 2 - 1;
int obst_r = ny / 10 + 1;


const T nu = uMax * 2.0*obst_r / Re;	// * 2
const T omega = 1. / (3.*nu + 1. / 2.);		// 1/
const int maxT = 40000;	// Maximum number of steps 40,000
const int tSave = 50;		// Steps, 1 render every 20 steps   //50 steps:  200 to 250 ms
const int itStart = 10000;		// suggested 10,000


// Set up the geometry of the simulation
void prepareLattice(BlockStructure2D<T, DESCRIPTOR>& lattice,
	Dynamics<T, DESCRIPTOR>& bulkDynamics,
	OnLatticeBoundaryCondition2D<T, DESCRIPTOR>& boundaryCondition){

	// Set bulk dynamics
	lattice.defineDynamics(0, nx - 1, 0, ny - 1, &bulkDynamics);

	// Set top boundary condition
	boundaryCondition.addVelocityBoundary1P(1, nx - 2, ny - 1, ny - 1, omega);
	// Set bottom boundary condition
	boundaryCondition.addVelocityBoundary1N(1, nx - 2, 0, 0, omega);
	// Set left boundary condition
	boundaryCondition.addVelocityBoundary0N(0, 0, 1, ny - 2, omega);
	// Set right boundary condition
	boundaryCondition.addPressureBoundary0P(nx - 1, nx - 1, 1, ny - 2, omega);

	// Set corner nodes condition
	boundaryCondition.addExternalVelocityCornerNN(0, 0, omega);
	boundaryCondition.addExternalVelocityCornerNP(0, ny - 1, omega);
	boundaryCondition.addExternalVelocityCornerPN(nx - 1, 0, omega);
	boundaryCondition.addExternalVelocityCornerPP(nx - 1, ny - 1, omega);
}




void setBoundaryValues(BlockStructure2D<T, DESCRIPTOR>& lattice,
	int iT){
	OstreamManager clout(std::cout, "setBoundaryValues");

	// Definition of the obstacle (bounce-back nodes) 
	// Set initial condition
	if (iT == 0){
		for (int iX = 0; iX<nx; ++iX) {	// nx
			for (int iY = 0; iY<ny; ++iY) {	// ny
				T u[2] = { 0., 0. };
				T rho = (T)1;
				if ((iX - obst_x)*(iX - obst_x) +
					(iY - obst_y)*(iY - obst_y) <= obst_r*obst_r)
				{//			     , xt   ,    , yt
					lattice.defineDynamics(iY, nx / 4, iY, ny / 2, //iX,iX,iY,iY //iY,nx/4,iY,ny/2
						&instances::getBounceBack<T, DESCRIPTOR>());
				}
				else {
					lattice.get(iX, iY).defineRhoU(rho, u);
					lattice.get(iX, iY).iniEquilibrium(rho, u);
				}
			}
		}
		// Make the lattice ready for simulation
		lattice.initialize();
	}
	////////////////////////////
	if (iT == 6000){
		for (int iX = 0; iX<nx; ++iX) {       // nx
			for (int iY = 0; iY<ny; ++iY) {     // ny
				T u[2] = { 0., 0. };
				T rho = (T)1;
				if ((iX - obst_x)*(iX - obst_x) +
					(iY - obst_y)*(iY - obst_y) <= obst_r*obst_r)
				{//                          , xt   ,    , yt
					lattice.defineDynamics(iX, nx / 4, iY, ny / 2, //iX,iX,iY,iY //iY,nx/4,iY,ny/2
						&instances::getBounceBack<T, DESCRIPTOR>());
				}
				else {
					//lattice.get(iX,iY).defineRhoU(rho, u);
					//lattice.get(iX,iY).iniEquilibrium(rho, u);
				}
			}
		}
		// Make the lattice ready for simulation
		lattice.initialize();
	}

	////////////////////////////
	if (iT <= itStart) {
		const int ny = lattice.getNy();

		// Smooth start curve, sinus
		// SinusStartScale<T,int> nSinusStartScale(itStart, (T)1);
		// T frac = nSinusStartScale(iT);

		// Smooth start curve, polynomial
		PolynomialStartScale<T, int> nPolynomialStartScale(itStart, T(1));
		std::vector<int> help; help.push_back(iT);
		T frac = nPolynomialStartScale(help)[0];

		SquareAnalyticalF<T, T> nPoiseuilleVelocity((ny - 1.) / 2., (ny - 1.) / 2., uMax);
		T p0 = 8.*nu*uMax*(nx - 1) / (2.*(ny - 1)*(ny - 1));
		LinearAnalyticalF1D<T, T> nPoiseuillePressure(0, p0, nx - 1, -p0);

		for (int iY = 0; iY < ny; ++iY) {
			std::vector<T> help2;
			help2.push_back((T)iY);
			T u[2] = { frac * nPoiseuilleVelocity(help2)[0], 0. };
			help2[0] = T();
			T rho = (T)1 + nPoiseuillePressure(help2)[0] * DESCRIPTOR<T>::invCs2;
			lattice.get(0, iY).defineRhoU(rho, u);
		}
	}
}

void getResults(BlockStructure2D<T, DESCRIPTOR>& lattice, int iT){
	if (iT%tSave == 0 && iT != 0) {
		OstreamManager clout(std::cout, "main");
		clout.setMultiOutput(false);
		/// Get statistics
		clout << "step=" << iT /*
							   << "; avEnergy="
							   << lattice.getStatistics().getAverageEnergy()
							   << "; avRho="
							   << lattice.getStatistics().getAverageRho()
							   << "; uMax="
							   << lattice.getStatistics().getMaxU()*/ << endl;

		DataAnalysisBase2D<T, DESCRIPTOR> const& analysis = lattice.getDataAnalysis();
		// Creation of images representing intermediate simulation results
		ImageWriter<T> imageWriter("leeloo");
		// Creation of gif images. This works only on systems on
		// which ImageMagick is installed. If you have the
		// program gifmerge, you can create an animation with the help
		// of a command of the kind "gifmerge -5 u*.gif > anim_u"

		imageWriter.writeScaledGif(createFileName("p", iT, 6),
			//                             analysis.getPressure());
			//    imageWriter.writeScaledGif(createFileName("C", iT, 6),
			analysis.getVelocityNorm());
	}
}


