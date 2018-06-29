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
#include "TTree.h"
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

int NormalizedSignal(){
 gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

 int Nfiles=0;
std::list<string>  mylist;
std::list<string>::iterator it;

  it = mylist.begin();
         

  auto mgAlphaV = new TMultiGraph();
  auto mgAlpha = new TMultiGraph();
  auto mgDepth = new TMultiGraph();

auto legDepth = new TLegend(0.1, 0.7, 0.3, 0.9);
legDepth->SetFillColor(0);
legDepth->SetHeader("depth");

auto legAlpha = new TLegend(0.1, 0.7, 0.3, 0.9);
legAlpha->SetFillColor(0);
legAlpha->SetHeader("alphaEdep");

auto legAlphaV = new TLegend(0.1, 0.7, 0.3, 0.9);
legAlphaV->SetFillColor(0);
legAlphaV->SetHeader("alphaVertex");

auto legBeta = new TLegend(0.1, 0.7, 0.3, 0.9);
legBeta->SetFillColor(0);
legBeta->SetHeader("Beta");

	double_t UPthreshold=1000.;
	double_t MIDthreshold=750.;
	double_t LOWthreshold=250.;

	double_t BETAthreshold=1000.;


       	double_t edepAlpha =0.0;
       	double_t edepBeta =0.0;
	double_t t =0.0;



  TString path = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
   path.ReplaceAll("/./", "/");
   path.ReplaceAll("NormalizedSignal.C", "");
   //          cout <<path << endl;
   TSystemDirectory dir(path, path);
     TList *files = dir.GetListOfFiles();
   if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         string FileName  = file->GetName();
//   cout << FileName << endl;
	 std::string key=".root";
    if (FileName.find(key) != std::string::npos) 
       {
         //   cout << FileName << endl;
	    	  Nfiles++;
		  mylist.insert(it,FileName);
		  it++;

         }
      }
   }
   cout <<"Number of Files: "<<Nfiles << endl;	 

 int totalFiles=Nfiles;

	double_t *yLTotal=new double_t[totalFiles];
	double_t *yHTotal= new double_t[totalFiles];
	double_t *y0Total= new double_t[totalFiles];

	double_t *time=new double_t[totalFiles];


double_t *erLTotalExp=new double_t[totalFiles];
double_t *erHTotalExp=new double_t[totalFiles];
double_t *er0TotalExp=new double_t[totalFiles];


double_t *expLTotal=new double_t[totalFiles] ;
double_t *expHTotal=new double_t[totalFiles] ;
double_t *exp0Total=new double_t[totalFiles] ;

double_t *expLNormTotal=new double_t[totalFiles];
double_t *expHNormTotal=new double_t[totalFiles];

double_t *erLNormTotal=new double_t[totalFiles];
double_t *erHNormTotal=new double_t[totalFiles];

double_t *zero=new double_t[totalFiles] ;

double_t *counts=new double_t[totalFiles] ;
 double_t *Nsample=new double_t[totalFiles];

   TH1 *histAlpha[Nfiles];
   TH1 *histAlphaV[Nfiles];
   TH1 *histBeta[Nfiles];
   TH1 *histDepth[Nfiles];

 for(int i=0;i<totalFiles;i++)
   {

      yLTotal[i]=0;
      yHTotal[i]=0;

      y0Total[i]=0;

      time[i] = -1.;


 erLTotalExp[i]=0;
 erHTotalExp[i]=0;
 er0TotalExp[i]=0;


 expLTotal[i]=0;
 expHTotal[i]=0;
 exp0Total[i]=0 ;

expLNormTotal[i]=0;
expHNormTotal[i]=0;

erLNormTotal[i]=0;
erHNormTotal[i]=0;


zero[i]=0 ;
counts[i]=0 ;
Nsample[i]=0;


	double_t countsLAlphaTotal=0;
	double_t countsHAlphaTotal=0;
	double_t countsBetaTotal=0;



		//	hist1->SetMarkerStyle(i);
		//	hist1->GetYaxis()->SetTitle("N");
		//	hist1->GetXaxis()->SetTitle("depth (nm)");
		//	hist1->SetMarkerColor(i); 
	
	



const char *cstr = mylist.front().c_str();
     TFile * hfile = new TFile(cstr);
     mylist.pop_front();
	TTree *tree = (TTree*)hfile->Get("EnergiesAndDepth");


//TH1* Ebeta = 0;

/*
 TTreeReader myReader("EnergiesAndDepth", hfile);
myReader.Draw("EdepAlpha");

 TTreeReaderValue<Int_t> edepAlpha(myReader, "EdepAlpha");*/
// while (myReader.Next()) {
      // Get the data from the current TTree entry by getting
      // the value from the connected reader (eventSize):
      //totalSize += *eventSize;
	//cout<<*edepAlpha<<endl;
   //}



	tree->SetBranchAddress("EdepAlpha",&edepAlpha);
	tree->SetBranchAddress("EdepBeta",&edepBeta);
	tree->SetBranchAddress("time",&t);

	for (Int_t ev = 0; ev < tree->GetEntries(); ev++) {
	tree->GetEntry(ev);


	if(edepAlpha>UPthreshold)
	{
	countsHAlphaTotal+=1;		
	}

	if((edepAlpha>LOWthreshold)&&(edepAlpha<MIDthreshold))
	{
	countsLAlphaTotal+=1;	
	}
	//	cout <<edepBetaTotal<<endl;


	if(edepBeta>BETAthreshold)
	{
	countsBetaTotal+=1;		
	}

	}
	yLTotal[i]=countsLAlphaTotal;
	yHTotal[i]=countsHAlphaTotal;

	y0Total[i]=countsBetaTotal;
	time[i]=t;
	
	countsLAlphaTotal=0;
	countsHAlphaTotal=0;
	countsBetaTotal=0;
	
hfile->GetObject("depth", histDepth[i]);
 hfile->GetObject("Ealpha",histAlphaV[i]);
 hfile->GetObject("Ebeta",histBeta[i]);
 hfile->GetObject("EnergyDeposition",histAlpha[i]);


	histDepth[i]->SetMarkerStyle(20);
	histDepth[i]->SetMarkerColor(i+2);

		histAlpha[i]->SetMarkerStyle(20);
		histAlpha[i]->SetMarkerColor(i+2);

		histAlphaV[i]->SetMarkerStyle(20);
		histAlphaV[i]->SetMarkerColor(i+2);

		histBeta[i]->SetMarkerStyle(20);
		histBeta[i]->SetMarkerColor(i+2);


        std::ostringstream s;
	s << time[i]<<" s";
	std::string output = s.str();
	const char* label =output.c_str();

	legAlpha->AddEntry(histAlpha[i], label, "lp");
	legAlphaV->AddEntry(histAlphaV[i], label, "lp");
	legDepth->AddEntry(histDepth[i], label, "lp");
	legBeta->AddEntry(histBeta[i], label, "lp");
	




   }

 	double_t swap1=0;
	double_t swap2=0;
 	double_t swap3=0;
 	double_t swap4=0;



 for (int c = 0 ; c <totalFiles-1 ; c++)
  {
    for (int d=0;d<totalFiles-c-1;d++)
    {
      if (time [d] > time[d+1]) 
      {
        swap1       = time[d];
        swap2       =y0Total[d];
        swap3       =yLTotal[d];
        swap4       =yHTotal[d];


        time[d]   = time[d+1];
        yLTotal[d]   = yLTotal[d+1];
        yHTotal[d]   = yHTotal[d+1];
        y0Total[d]   = y0Total[d+1];


        time[d+1] = swap1;
        y0Total[d+1] = swap2;
        yLTotal[d+1] = swap3;
        yHTotal[d+1] = swap4;

      }
    }
  }


 
 for(int j=0;j<totalFiles;j++)
   {

     cout<<"Low "<<yLTotal[j]<<" High "<<yHTotal[j]<<" Beta "<<y0Total[j]<<endl;
     if(time[j]>0)
       {
	 double_t Dt=0.01;
 int size=time[j]*100;
double_t N[size];
 double_t R[size];
 double_t t[size];

 for(int i=0;i<size;i++)
   {
   N[i]=0;
 R[i]=0;
   }
 N[0]=1000000*Dt;
 // R[0]=N[0]*0.8249*(1-exp(-0.1*0.8249));
for(int i=1;i<size;i++)
  {
    R[i]=N[i-1]-N[i-1]*exp(-0.01*0.8249);
      if(i<100)
      {
     	N[i]=N[i-1]+1000000*Dt-R[i];
       }
      else
       {
	 N[i]=N[i-1]-R[i];
        }
 }    
       
 counts[j]=R[size-1];
 Nsample[j]=N[size-1];
 cout <<"Time: " <<time[j]<<" N: "<<Nsample[j]<< " R: "<<R[size-1]<< endl;

   }
 else
   {
     counts[j]=0;
   }
 double_t factor=counts[j]/1000000;



 exp0Total[j]=100*factor*y0Total[j] ;
expLTotal[j]=100*factor*yLTotal[j];
expHTotal[j]=100*factor*yHTotal[j];



erLTotalExp[j] =expLTotal[j]*sqrt(1/counts[j]);
erHTotalExp[j] =expHTotal[j]*sqrt(1/counts[j]);

er0TotalExp[j] =exp0Total[j]*sqrt(1/counts[j]);

 if(exp0Total[j]>0){
   double_t fL =(expLTotal[0]/exp0Total[0]);
   expLNormTotal[j]=(expLTotal[j]/exp0Total[j])/fL;
   
   double_t fH =(expHTotal[0]/exp0Total[0]);
   expHNormTotal[j]=(expHTotal[j]/exp0Total[j])/fH;

   erLNormTotal[j]=expLNormTotal[j]*sqrt((erLTotalExp[j]*erLTotalExp[j])/(expLTotal[j]*expLTotal[j])+(er0TotalExp[j]*er0TotalExp[j])/(exp0Total[j]*exp0Total[j]));

   erHNormTotal[j]=expHNormTotal[j]*sqrt((erHTotalExp[j]*erHTotalExp[j])/(expHTotal[j]*expHTotal[j])+(er0TotalExp[j]*er0TotalExp[j])/(exp0Total[j]*exp0Total[j]));

   
 }
 else
   {
     cout<<"ZERO NORM!!"<<endl;
     expLNormTotal[j]=0;
     expHNormTotal[j]=0;

   }
}
	


TGraph* histN = new TGraph(totalFiles,time,Nsample);


TGraphErrors* histExpLTotal = new TGraphErrors(totalFiles,time,expLTotal,zero,erLTotalExp);
TGraphErrors* histExpHTotal = new TGraphErrors(totalFiles,time,expHTotal,zero,erHTotalExp);

TGraphErrors* histExp0Total = new TGraphErrors(totalFiles,time,exp0Total,zero,er0TotalExp);

 TGraphErrors* histExpLNormTotal = new TGraphErrors(totalFiles,time,expLNormTotal,zero,erLNormTotal);
 TGraphErrors* histExpHNormTotal = new TGraphErrors(totalFiles,time,expHNormTotal,zero,erHNormTotal);

	histN->SetMarkerStyle(20);
	histN->GetYaxis()->SetTitle("N");
	histN->GetXaxis()->SetTitle("time (sec)");
	histN->SetMarkerColor(4); 



	histExpLTotal->SetMarkerStyle(20);
	histExpLTotal->GetYaxis()->SetTitle("N");
	histExpLTotal->GetXaxis()->SetTitle("time (sec)");
	histExpLTotal->SetMarkerColor(4); 

	histExpHTotal->SetMarkerStyle(20);
	histExpHTotal->GetYaxis()->SetTitle("N");
	histExpHTotal->GetXaxis()->SetTitle("time (sec)");
	histExpHTotal->SetMarkerColor(4); 

	histExp0Total->SetMarkerStyle(20);
	histExp0Total->GetYaxis()->SetTitle("N");
	histExp0Total->GetXaxis()->SetTitle("time (sec)");
	histExp0Total->SetMarkerColor(4); 




	histExpLNormTotal->SetMarkerStyle(20);
	histExpLNormTotal->GetYaxis()->SetTitle("N_TB(D,t)/N_TB(0,t)");
	histExpLNormTotal->GetXaxis()->SetTitle("time (sec)");
	histExpLNormTotal->SetMarkerColor(4); 	


	histExpHNormTotal->SetMarkerStyle(20);
	histExpHNormTotal->GetYaxis()->SetTitle("N_TB(D,t)/N_TB(0,t)");
	histExpHNormTotal->GetXaxis()->SetTitle("time (sec)");
	histExpHNormTotal->SetMarkerColor(4); 

//	TLegend * leg = new TLegend(0.78,0.78,0.98,0.98);
  //	leg->AddEntry(histExp0, "Counts with no diffusion", "lp");
//	leg->AddEntry(histExpEdep, "Counts with D=10^-12 cm^2/sec", "lp");
	
	    TCanvas * can0 = new TCanvas("N vs t","N vs t") ;
	    can0->SetFillColor(0);
	    can0->SetGridy();
	    can0->SetGridy();
	    histN->Draw("ap");

	    TCanvas * can = new TCanvas("Low energy Total Counts at Alpha Detector vs time","Low energy Total Counts at Alpha Detector vs time") ;
	    can->SetFillColor(0);
	    can->SetGridy();
	    can->SetGridy();
	    histExpLTotal->Draw("ap");

	    TCanvas * can2 = new TCanvas("High energy Total Counts at Alpha Detector vs time","High energy Total Counts at Alpha Detector vs time") ;
	    can2->SetFillColor(0);
	    can2->SetGridy();
	    can2->SetGridy();
	    histExpHTotal->Draw("ap");

	    TCanvas * canT = new TCanvas("Low energy Counts at Alpha Detector vs time Normalized to no diffusion ","Low Energy Counts at Alpha Detector vs time Normalized to no diffusion") ;
	    canT->SetFillColor(0);
	    canT->SetGridy();
	    canT->SetGridy();	
	    histExpLNormTotal->Draw("ap");


	     TCanvas * canT2 = new TCanvas("High Energy Counts at Alpha Detector vs time Normalized to no diffusion ","High Energy Counts at Alpha Detector vs time Normalized to no diffusion") ;
	    canT2->SetFillColor(0);
	    canT2->SetGridy();
	    canT2->SetGridy();	
	    histExpHNormTotal->Draw("ap");

	     TCanvas * canAlpha = new TCanvas("Energy deposition at alpha detector ","Energy deposition at alpha detector") ;
	    canAlpha->SetFillColor(0);
	    canAlpha->SetGridy();
	    canAlpha->SetGridy();	
	    histAlpha[0]->Draw();
 	    for(int i=1;i<totalFiles;i++)
   	    {
 		 histAlpha[i]->Draw("same");
	    }
		legAlpha->Draw("same");

	     TCanvas * canAlphaV = new TCanvas("Energy of alpha at vertex ","Energy of alpha at vertex") ;
	    canAlphaV->SetFillColor(0);
	    canAlphaV->SetGridy();
	    canAlphaV->SetGridy();	
	    histAlphaV[0]->Draw();
 	    for(int i=1;i<totalFiles;i++)
   	    {
 		 histAlphaV[i]->Draw("same");
	    }
		legAlphaV->Draw("same");

     TCanvas * canBeta = new TCanvas("Energy of beta at vertex ","Energy of beta at vertex") ;
	    canBeta->SetFillColor(0);
	    canBeta->SetGridy();
	    canBeta->SetGridy();	
	    histBeta[0]->Draw();
 	    for(int i=1;i<totalFiles;i++)
   	    {
 		 histBeta[i]->Draw("same");
	    }
		legBeta->Draw("same");

     TCanvas * canDepth = new TCanvas("Depth distribution ","Depth distribution") ;
	    canDepth->SetFillColor(0);
	    canDepth->SetGridy();
	    canDepth->SetGridy();	
	    histDepth[0]->Draw();
 	    for(int i=1;i<totalFiles;i++)
   	    {
 		 histDepth[i]->Draw("same");
	    }
		legDepth->Draw("same");


 ofstream myfile1;
 ofstream myfile2;
  myfile1.open ("Low40keV11R.txt");
  myfile2.open ("High40keV11R.txt");
  //myfile1 << "TS ns nserr\n";

  for (int i=0;i<Nfiles;i++){
    // loop over the tree
    myfile1 << time[i]<< " "<< expLNormTotal[i]<< " "<< erLNormTotal[i] << endl; //print to the file
    myfile2 << time[i]<< " "<< expLNormTotal[i]<< " "<< erHNormTotal[i] << endl; //print to the file
  }
  myfile1.close();
  myfile2.close();

	   delete[]  yLTotal;
	   delete[]  yHTotal;
	   delete[]  y0Total;	 
	   delete[]	time;


	   delete[] erLTotalExp;
	   delete[] erHTotalExp;
	   delete[] er0TotalExp;


	   delete[] expLTotal ;
	   delete[] expHTotal;
	   delete[] exp0Total ;

	   delete[] expLNormTotal;
	   delete[] expHNormTotal;

	   delete[] erLNormTotal;
	   delete[] erHNormTotal;

	   delete[] zero ;

	   delete[] counts ;
	   delete[] Nsample;

return 0 ;	
	
}


