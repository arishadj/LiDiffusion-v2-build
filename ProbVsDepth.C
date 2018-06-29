#include <fstream>  
#include <stdio.h>      /* printf */
#include <math.h>       /* exp */
#include <iostream>

#include "TSystem.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TList.h"
#include "TString.h"
#include<cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>  
#include <cmath> 
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
//#include<string.h>
//#include<fstream>
//#include<sstream>
//#include<dirent.h>
//#include <sys/types.h>

using namespace std;
double defineThresholdEnergy(string fnameLandau, double thresholdProb)
{
 std::vector<double> tempLandauE;
 std::vector<double> tempLandauN;
  double tempE=0;
  double tempN=0;
  int nPointsLandau = 0;
double totalLandau=0;
  std::ifstream fieldLandau;
  fieldLandau.open(fnameLandau) ;
  if (fieldLandau.is_open())
  {
	std::string lineLandau;
	std::istringstream inStreamLandau; 
	while(!fieldLandau.eof())
	{			
		getline(fieldLandau, lineLandau) ;
		inStreamLandau.clear();
		inStreamLandau.str(lineLandau); 
		inStreamLandau >> tempE >> tempN ;
		tempLandauE.push_back(tempE);
		tempLandauN.push_back(tempN);
		totalLandau+=tempN;
		nPointsLandau++;
	}
	nPointsLandau--;
  }
  fieldLandau.close();

  //copy arrays in std::vector and compute fMax
  //

double sumLandau=0;
int j=nPointsLandau;
  while (sumLandau<thresholdProb*totalLandau) {
	j--;
    sumLandau += tempLandauN[j];
   
  };
thresholdEnergy=tempLandauE[j];

   tempLandauE.clear();
   tempLandauN.clear();

cout<<"Threshold Energy: "<<thresholdEnergy<<" keV, corresponding to the highest "<<thresholdProb*
100<<" % of counts"<<endl;

return thresholdEnergy;
}


int ProbVsDepth(){
 gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

////////////////// defining threshold in energy /////////////////////
double thresholdProb=0.3;
string fnameLandau = "DiffusionProfiles/EnergyHistograms/AlphaEnergySpectrum.txt";
double UPthreshold=0;
UPthreshold=defineThresholdEnergy(fnameLandau, thresholdProb);


 

  string fname = "ProbVsD"+to_string(static_cast<int>(thresholdProb*100))+"ps.txt";
  int tempD=0;
  int tempCounts=0;
  int tempDecays=0;
  double tempNorm=0.0;
  double tempErr=0.0;

std::vector<double> InitDepth;
std::vector<int> InitCounts;
std::vector<int> InitDecays;
std::vector<double> InitNorm;
std::vector<double> InitErr;
std::vector<double> InitZeros;

  std::ifstream field;
    int counter=0;
    if (std::ifstream(fname.c_str()))
    {
        // file exists. //
    field.open(fname) ;
    if (field.is_open())
    {
    std::string line;
    std::istringstream inStream; 
    while(!field.eof()) 
    {	
	counter++;
		
	getline(field, line) ;
	inStream.clear();
	inStream.str(line); 
	inStream >> tempD>> tempCounts>>tempDecays>>tempNorm>>tempErr;
	if((counter>1)&&(!field.eof()))
	{
	InitDepth.push_back(tempD);
	InitCounts.push_back(tempCounts);
	InitDecays.push_back(tempDecays);
	InitNorm.push_back(tempNorm);
	InitErr.push_back(tempErr);
	InitZeros.push_back(0.0);
	}
    }
    field.close();
   }
  }
  else
  {
     ofstream outputFile;
     outputFile.open(fname);
     outputFile.close();
  }

 //for(int i=0;i<InitDepth.size();i++)
//{
// cout<<InitDepth.at(i)<<" "<<counter<<endl;
//}
double edep=0;
double d=0;
 
     TFile * hfile = new TFile("TiO2D1e-09t0accumulative.root");
TTree *tree = (TTree*)hfile->Get("EnergiesAndDepth");

	tree->SetBranchAddress("EdepAlpha",&edep);
	tree->SetBranchAddress("depth",&d);

	double percent=0.1;

	for (Int_t ev = 0; ev < tree->GetEntries(); ev++) {
	tree->GetEntry(ev);

	if(static_cast<double>(ev)/static_cast<double>(tree->GetEntries())>=percent)
	{
	cout<<"Completed "<<100*percent<<"% of the counts"<<endl;
	percent=percent+0.1;
	}

	    int i= static_cast<int>(d);
	    if(i<InitDepth.size())
	    {
	       if(edep>UPthreshold)
	        {		    		
		  InitCounts[i]++;
		}
		InitDecays[i]++;
	    }
	    else
	    { 
		//cout<<"Depth increase!"<<endl;
		int oldSize=InitDepth.size();
		for (int j=oldSize;j<i;j++)
		{
		   InitDepth.push_back(j);
		   InitCounts.push_back(0);
		   InitDecays.push_back(0);
	           InitNorm.push_back(0);
		   InitErr.push_back(0); 
		}
		InitDecays[i]++;
		if(edep>UPthreshold)
	        {		    		
		  InitCounts[i]++;
		}
	    }	 	
	  }

	 int size=InitDepth.size();
	 double_t *dView=new double_t[size];
	 double_t *ProbView=new double_t[size];
	 double_t *ErrView=new double_t[size];
	 double_t *Zeros=new double_t[size];
//cout<<size<<" "<<InitCounts.size()<<" "<<InitDecays.size()<<" "<<InitNorm.size()<<" "<<InitErr.size()<<endl;
	for (int i=0;i<size;i++)
	{	
	   if(InitDecays.at(i)>0)
	   {
 	     InitNorm[i]=static_cast<double>(InitCounts.at(i))/static_cast<double>(InitDecays.at(i));
	     InitErr[i]=InitNorm[i]*sqrt(1/static_cast<double>(InitDecays.at(i))+1/static_cast<double>(InitCounts.at(i)));
	   }
	   else
	   {
	     InitNorm[i]=0.0;	
	     InitErr[i]=0.0;
	   }	
	   dView[i]=InitDepth.at(i);
	   ProbView[i]=InitNorm.at(i);
	   ErrView[i]=InitErr.at(i);
	   Zeros[i]=0.0;
	//cout<<InitCounts.at(i)<<" "<<InitDecays.at(i)<<" "<<InitNorm[i]<<endl;
        }

TGraphErrors* histProbVsDepth = new TGraphErrors(InitDepth.size(),dView,ProbView,0,ErrView);

//TGraphErrors* histProbVsDepth = new TGraphErrors(InitDepth.size(),InitDepth.data(),InitNorm.data(),InitZeros.data(),InitErr.data());

	histProbVsDepth->SetMarkerStyle(20);
	histProbVsDepth->GetYaxis()->SetTitle("Probability");
	histProbVsDepth->GetXaxis()->SetTitle("depth (nm)");
	histProbVsDepth->SetMarkerColor(1); 

	    TCanvas * can0 = new TCanvas("Depth distribution","Depth distribution") ;
	    can0->SetFillColor(0);
	    can0->SetGridy();
	    can0->SetGridy();
	    histProbVsDepth->Draw("ap");

	delete[]   dView;
	 delete[]     ProbView;
	 delete[]     ErrView;
	 delete[]     Zeros;

       ofstream outputFile;
     outputFile.open(fname);
	outputFile <<"#depth(nm) counts decays prob. error"<<endl;  
     for(int i=0;i<size;i++)
     {
	outputFile << InitDepth[i] << " " << InitCounts.at(i) << " "<< InitDecays[i]<< " "<< InitNorm[i]<< " " << InitErr.at(i) <<endl;
     }	
     outputFile.close();

/*
  string fname = "ProbVsD.txt";
  double tempD=0;
  double tempCounts=0;
  double tempDecays=0;
  double tempNorm=0.0;
  double tempErr=0.0;

std::vector<double> InitDepth;
std::vector<double> InitCounts;
std::vector<double> InitDecays;
std::vector<double> InitNorm;
std::vector<double> InitErr;
std::vector<double> Zeros;

  std::ifstream field;
    int counter=0;
    if (std::ifstream(fname.c_str()))
    {
        // file exists. //
    field.open(fname) ;
    if (field.is_open())
    {
    std::string line;
    std::istringstream inStream; 
    while(!field.eof()) 
    {	
	counter++;
		
	getline(field, line) ;
	inStream.clear();
	inStream.str(line); 
	inStream >> tempD>> tempCounts>>tempDecays>>tempNorm>>tempErr;
	if((counter>1)&&(!field.eof()))
	{
	InitDepth.push_back(tempD);
	InitCounts.push_back(tempCounts);
	InitDecays.push_back(tempDecays);
	InitNorm.push_back(tempNorm);
	InitErr.push_back(tempErr);
	}
    }
    field.close();
   }
  }
  else
  {
     ofstream outputFile;
     outputFile.open(fname);
     outputFile.close();
  }

counter=0;
tempCounts=0; 
tempDecays=0;
 for(int i=5992;i<InitDepth.size();i++)
{
  counter++;
  if(counter<=10)
  {
     tempCounts+=InitCounts[i];
     tempDecays+=InitDecays[i];
  }
  else
  {
    counter=0;
    for(int j=0;j<10;j++)
    {
      InitCounts[i-j]=tempCounts;
      InitDecays[i-j]=tempDecays;
      InitNorm[i-j]=static_cast<double>(tempCounts)/static_cast<double>(tempDecays);
      InitErr[i-j] = InitNorm[i-j]*sqrt(1/static_cast<double>(InitDecays.at(i-j))+1/static_cast<double>(InitCounts.at(i-j)));
    }
    tempCounts=0; 
    tempDecays=0; 
  }
}
       ofstream outputFile;
     outputFile.open(fname);
	outputFile <<"#depth(nm) counts decays prob. error"<<endl;  
     for(int i=0;i<InitDepth.size();i++)
     {
	outputFile << InitDepth[i] << " " << InitCounts.at(i) << " "<< InitDecays[i]<< " "<< InitNorm[i]<< " " << InitErr.at(i) <<endl;
       Zeros.push_back(0.0);
     }	
     outputFile.close();

TGraphErrors* histProbVsDepth = new TGraphErrors(InitDepth.size(),InitDepth.data(),InitNorm.data(),Zeros.data(),InitErr.data());

	histProbVsDepth->SetMarkerStyle(20);
	histProbVsDepth->GetYaxis()->SetTitle("Probability");
	histProbVsDepth->GetXaxis()->SetTitle("depth (nm)");
	histProbVsDepth->SetMarkerColor(1); 

	    TCanvas * can0 = new TCanvas("Depth distribution","Depth distribution") ;
	    can0->SetFillColor(0);
	    can0->SetGridy();
	    can0->SetGridy();
	    histProbVsDepth->Draw("ap");
*/
return 0 ;	
	
}


