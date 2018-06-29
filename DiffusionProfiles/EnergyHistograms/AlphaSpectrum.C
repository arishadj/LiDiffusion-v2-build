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
//#include<string.h>
//#include<fstream>
//#include<sstream>
//#include<dirent.h>
//#include <sys/types.h>

using namespace std;
//#include <dirent64.h>

//////////Two det of 2 cm length, 1 cm before the sample


///1. det thick and new materials OK
//// 2. check Eneergy conserv OK
/// 3. siplify distribution to three delta funct OK
/// 4. 1.6 MeV alphas OK
//// 5. real Landau
// 6. benzine / polystirene OK

int AlphaSpectrum(){
 gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);




 std::vector<double> I;
 std::vector<double> N;
 std::vector<double> E;
 std::vector<double> counts;
 std::vector<double> LandauN;
 std::vector<double> LandauStep;
 std::vector<double> percentageL;
 std::vector<double> percentageS;
double threshold =1500;

   double_t temp1=0;
   double_t temp2=0;
   int      counter=0;
   double   totalN=0;

const char *cstr = "Wilkinson_raw_data.txt";
  	std::ifstream file;
		file.open(cstr) ;

		if (file.is_open())
		{

		std::string line;
		std::istringstream inStream; 
			while (!file.eof())
			{
				getline(file, line) ;
				inStream.clear();
				inStream.str(line);
				inStream  >> temp1>> temp2;
 				I.push_back(temp1);
				N.push_back(temp2);
				totalN+=N.at(counter);
                                E.push_back(-4.9+49.140*I[counter]+0.00090*std::pow(I[counter],2));
				counter++;
			}
		}
		file.close();

 std::ofstream myfile ;
  myfile.open("AlphaEnergySpectrum.txt") ;

  if (myfile.is_open())
  {
    myfile<<"#Energy(keV)           Counts"<<"\n";
    for(int j=0;j<E.size();j++)
    {
	myfile <<E[j]<<"            "<<N[j]<<"\n";
    }
  }
  myfile.close();	

double constant=839487;
double MPV =1490.52;
double sigma =220.537;
double firstBin=1;

for(int i=0;i<10000;i++)
{
 LandauN.push_back(0);
 LandauStep.push_back(i);
LandauN.at(i)=constant*TMath::Landau(i,MPV,sigma);
}

double totalLandau=0;
for(int i=0;i<LandauN.size();i++)
{
totalLandau+=LandauN.at(i);
}
for(int i=0;i<LandauN.size();i++)
{
  counts.push_back(i*10);
  double newThreshold=threshold-counts.at(i)*counts.at(i)*0.0000001;

  if(newThreshold<0) newThreshold=0;

  percentageS.push_back(0);
  for(int j=0;j<LandauStep.size();j++)
  {
    if(LandauStep.at(j)>newThreshold)
    {
         // cout<<E.at(j)<<" "<<newThreshold<<endl;
	percentageS.at(i)+=LandauN.at(j);//N.at(j);
    }
  }
   //if(i==0) firstBin=percentageS.at(i);

   //percentage.at(i)=counts.at(i)*percentage.at(i)/firstBin-counts.at(i);
//percentageS.at(i)=percentageS.at(i)/firstBin;
percentageS.at(i)=percentageS.at(i)/totalLandau;
}

for(int i=0;i<LandauN.size();i++)
{
  double newThreshold=threshold-counts.at(i)*0.01;

  if(newThreshold<0) newThreshold=0;

  percentageL.push_back(0);
  for(int j=0;j<LandauStep.size();j++)
  {
    if(LandauStep.at(j)>newThreshold)
    {
         // cout<<E.at(j)<<" "<<newThreshold<<endl;
	percentageL.at(i)+=LandauN.at(j);//N.at(j);
    }
  }
   //if(i==0) firstBin=percentageL.at(i);

   //percentage.at(i)=counts.at(i)*percentage.at(i)/firstBin-counts.at(i);
//percentageL.at(i)=percentageL.at(i)/firstBin;
percentageL.at(i)=percentageL.at(i)/totalLandau;
}
/*
 std::vector<double> asymmetryReal;
 std::vector<double> asymmetryCounts;

for(int i=0;i<counts.size();i++)
{
asymmetryReal.push_back(0.1);
double Lowcounter=0;
while((counts.at(i)-counts.at(Lowcounter))/(counts.at(i)+counts.at(Lowcounter))>0.1)
  {
    Lowcounter++;
  }

  asymmetryCounts.push_back((counts.at(i)-counts.at(Lowcounter))/(counts.at(i)+counts.at(Lowcounter)));
  cout<<asymmetryCounts.at(i)<<endl;
}
*/

  auto can0 = new TCanvas("percentage vs counts","percentage vs counts") ;
//auto can0 = new TCanvas("asymmetry vs counts","asymmetry vs counts") ;
  can0->SetFillColor(0);
  can0->SetGridy();
  can0->SetGridy();
	
		//histP = new TGraph(counts.size(),counts.data(),asymmetryCounts.data());	
	        histL = new TGraph(counts.size(),counts.data(),percentageL.data());	
		histL->SetMarkerStyle(20);
		histL->SetMarkerColor(1);
  		histL->GetXaxis()->SetTitle("counts");
  		histL->GetYaxis()->SetTitle("percentage of alphas detected");
		//histP->GetYaxis()->SetTitle("asymmetry");


		//histP = new TGraph(counts.size(),counts.data(),asymmetryCounts.data());	
	        histS = new TGraph(counts.size(),counts.data(),percentageS.data());	
		histS->SetMarkerStyle(20);
		histS->SetMarkerColor(2);
  		histS->GetXaxis()->SetTitle("counts");
  		histS->GetYaxis()->SetTitle("percentage of alphas detected");
		//histP->GetYaxis()->SetTitle("asymmetry");

   auto leg = new TLegend(0.1, 0.7, 0.3, 0.9);
   leg->SetFillColor(0);
   leg->SetHeader("count-dependance of distortion");
   leg->AddEntry(histL, "Linear", "lp");
   leg->AddEntry(histS, "Square", "lp");

                histL->Draw();
                histS->Draw("same");
		leg->Draw("same");

 std::ofstream myfileL ;
  myfileL.open("LinearDistortion.txt") ;

  if (myfileL.is_open())
  {
    myfileL<<"#Counts           Percentage"<<"\n";
    for(int j=0;j<counts.size();j++)
    {
	myfileL <<counts[j]<<"            "<<percentageL[j]<<"\n";
    }
  }
  myfileL.close();

 std::ofstream myfileS ;
  myfileS.open("SquareDistortion.txt") ;

  if (myfileS.is_open())
  {
    myfileS<<"#Counts           Percentage"<<"\n";
    for(int j=0;j<counts.size();j++)
    {
	myfileS <<counts[j]<<"            "<<percentageS[j]<<"\n";
    }
  }
  myfileS.close();

  auto can1 = new TCanvas("N vs E","N vs E") ;
  can1->SetFillColor(0);
  can1->SetGridy();
  can1->SetGridy();

	        histN = new TGraph(N.size(),E.data(),N.data());	
		histN->SetMarkerStyle(20);
		histN->SetMarkerColor(1);
  		histN->GetYaxis()->SetTitle("counts");
  		histN->GetXaxis()->SetTitle("Energy (keV)");
                histN->Draw();

           //gStyle->SetOptStat(kFALSE);

return 0 ;	
	
}


