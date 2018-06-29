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

int DiffusionProfilesView(){
 gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);


 int Nfiles=0;
std::list<string>  mylist;

//for (int i=1; i<=2; ++i) mylist.push_back("dsf"); // 1 2 3 4 5

std::list<string>::iterator it;
  it = mylist.begin();
         
  auto can0 = new TCanvas("N vs d","N vs d") ;
  can0->SetFillColor(0);
  can0->SetGridy();
  can0->SetGridy();

  auto mg = new TMultiGraph();

auto leg = new TLegend(0.1, 0.7, 0.3, 0.9);
leg->SetFillColor(0);
leg->SetHeader("test legend");


  TString path = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
   path.ReplaceAll("/./", "/");
   path.ReplaceAll("DiffusionProfilesView.C", "");
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
	 std::string key=".txt";
    if (FileName.find(key) != std::string::npos) 
       {
           // cout << FileName << endl;
	    	  Nfiles++;
		  mylist.insert(it,FileName);
		  it++;	  
         }
      }
   }
   cout <<"Number of Files: "<<Nfiles << endl;	 

   double *timeT=new double[Nfiles];
   TGraph *hist[Nfiles];// = new TGraph(FileLength,posz,C0);

   for(int i=0;i<Nfiles;i++)
     {

     int FileLength=0;

const char *cstr = mylist.front().c_str();
//cout << mylist.front().c_str() <<" "<<cstr<< endl;

	std::ifstream field;
 //cout << cstr << endl;
	field.open(cstr) ;

        if (field.is_open())
        {

       	std::string line;
        std::istringstream inStream; 
        while (!field.eof())
        {
        	getline(field, line) ;
	        inStream.clear();
	       	inStream.str(line); 
		FileLength++;
       	}
	}
	else
	  {
	    cout<<"FILE NOT FOUND!!! "<<i<<endl;
	  }
	FileLength--;
	field.close();

       double_t *posz=new double_t[FileLength];
  double_t *C0=new double_t[FileLength];
  double_t total=0;

  	std::ifstream file;
		file.open(cstr) ;

		if (file.is_open())
		{

		std::string line2;
		std::istringstream inStream2; 
		getline(file, line2) ;
		inStream2.clear();
		inStream2.str(line2); 
		inStream2  >>timeT[i]  ;
			for (int j=2 ; j< FileLength ; j++) 
			{
				getline(file, line2) ;
				inStream2.clear();
				inStream2.str(line2); 
				inStream2  >> posz[j-1]>>C0[j-1] ;
				//posz[j]=j-1;
				total+=C0[j-1];
				//	cout<<posz[j]<<" "<<C0[j]<<endl;
			}
		}
		file.close();
		     mylist.pop_front();
		for (int j=0 ; j< FileLength ; j++) 
			{
			  C0[j]=C0[j]/total;
			}
	cout<<timeT[i]<<" "<<C0[0]<<endl;
		hist[i] = new TGraph(FileLength,posz,C0);	

		//	hist1->SetMarkerStyle(i);
		//	hist1->GetYaxis()->SetTitle("N");
		//	hist1->GetXaxis()->SetTitle("depth (nm)");
		//	hist1->SetMarkerColor(i); 
		hist[i]->SetMarkerStyle(20);
		hist[i]->SetMarkerColor(i+1);

	mg->Add(hist[i]);
	std::ostringstream s;
	s << timeT[i];
	std::string output = s.str();
	const char* label =output.c_str();
	leg->AddEntry(hist[i], label, "lp");
    
	delete[] posz;
	delete[] C0;
     }


   delete[] timeT;
   //delete[] hist;

	    mg->Draw("APL");
	    mg->GetXaxis()->SetTitle("z (nm)");
	    mg->GetYaxis()->SetTitle("N/nm");
   gPad->Modified();
   mg->GetXaxis()->SetLimits(0,1000);
   mg->SetMinimum(0.);
   mg->SetMaximum(0.01);
	    // can0->Update();
    leg->Draw("same");


	 

return 0 ;	
	
}


