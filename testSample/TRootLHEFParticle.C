#define TRootLHEFParticle_cxx
#include "TRootLHEFParticle.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void TRootLHEFParticle::Loop()
{
//   In a ROOT session, you can do:
//      root> .L TRootLHEFParticle.C
//      root> TRootLHEFParticle t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

   if (fChain == 0) return;

   TH1F *h_higgsPt[3];
    TH1F *h_higgsPz[3];
    TH1F *h_higgsRap[3];
    TH1F *h_nH = new TH1F("h_nH","h_nH",8,-0.5,7.5);
    TH1F *h_hhDeltaR = new TH1F("h_hhDeltaR","h_hhDeltaR",60,2,5);
    TH1F *h_hhM = new TH1F("h_hhNewResonanceM","h_hhNewResonanceM",60,1500,4500);
    string text[3] = {"higgs1","higgs2","NewResonance"};
    for (int i=0;i<3;i++) {
        h_higgsPt[i] = new TH1F(Form("h_%sPt",text[i].data()),Form("h_%sPt",text[i].data()),40,0,2000);
        h_higgsPz[i] = new TH1F(Form("h_%sPz",text[i].data()),Form("h_%sPz",text[i].data()),40,-2000,2000);
        h_higgsRap[i] = new TH1F(Form("h_%sRapidity",text[i].data()),Form("h_%sRapidity",text[i].data()),60,-3,3);
    }
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   const Int_t nPar = kMaxParticle;
   for (Int_t jentry=0; jentry<nentries;jentry++) {
       Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      Int_t nHiggs = 0, nhh = 0;
      Int_t hIndex[2] = {-1,-1};
      TLorentzVector higgsVect[2];
      //for (int i=0;i<nPar;i++) {
      for (int i=0;i<Particle_size;i++) {
            if (Particle_PID[i]==25) nhh++;
            if (Particle_PID[i]==25&&nHiggs<2) {
                h_higgsPt[nHiggs]->Fill(Particle_PT[i]);
                h_higgsPz[nHiggs]->Fill(Particle_Pz[i]);
                h_higgsRap[nHiggs]->Fill(Particle_Rapidity[i]);
                higgsVect[nHiggs].SetPxPyPzE(Particle_Px[i],Particle_Py[i],Particle_Pz[i],Particle_E[i]);
                hIndex[nHiggs] = i;
                nHiggs++;
            }
      }
      if (nhh>2) cout << jentry << endl;
      h_nH->Fill(nhh);
      if (hIndex[1]>=0&&hIndex[0]>=0) {
        TLorentzVector ResonanceVect = higgsVect[0] + higgsVect[1];
        h_higgsPt[2]->Fill(ResonanceVect.Pt());
        h_higgsPz[2]->Fill(ResonanceVect.Pz());
        h_higgsRap[2]->Fill(Particle_Rapidity[hIndex[0]]+Particle_Rapidity[hIndex[1]]);
        h_hhDeltaR->Fill(higgsVect[0].DeltaR(higgsVect[1]));
        h_hhM->Fill((higgsVect[0]+higgsVect[1]).M());
      }
      //if (jentry%100) cout << "ggg  " << ientry << "\t" << jentry << endl;
      // if (Cut(ientry) < 0) continue;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
   }
   string pdfName = "BulkGraviton_hh_5perWidth_M2200.pdf";
   TCanvas *c1 = new TCanvas("c1","c1",3);
   c1->Print((pdfName+"[").data());
   h_nH->Draw("hist text 0");
   c1->Print(pdfName.data());
   h_hhM->Draw("hist");
   c1->Print(pdfName.data());
   for (int i=0;i<3;i++) {
      h_higgsPt[i]->Draw("hist");
      c1->Print(pdfName.data());
      h_higgsPz[i]->Draw("hist");
      c1->Print(pdfName.data());
      h_higgsRap[i]->Draw("hist");
      c1->Print(pdfName.data());
   }
   h_hhDeltaR->Draw("hist");
   c1->Print(pdfName.data());
   // RooFit
   using namespace RooFit;
   RooRealVar x("x","new Resonance (GeV)",1500,4500);
   RooDataHist data("data","new Resonance",x,h_hhM);
   RooRealVar mean("mean","mean",2000,6000);
   RooRealVar width("width","width",0,500);
   RooBreitWigner fitFun("fit","fit",x,mean,width);
   fitFun.fitTo(data);
   RooPlot* xframe = x.frame();
   data.plotOn(xframe) ;
   fitFun.plotOn(xframe);
   //fitFun.paramOn(xframe,RooArgSet(mean,width));
   fitFun.paramOn(xframe,Layout(0.5,0.9,0.9));
   //fitFun.paramOn(xframe,mean,width);
   xframe->Draw();
   c1->SetLeftMargin(0.15);
   c1->Print(pdfName.data());
   
   c1->Print((pdfName+"]").data());
   cout << fitFun.getVal() << endl;
   cout << fitFun.getVal(x) << endl;
   cout << fitFun.getVal(mean) << endl;
   cout << fitFun.getVal(width) << endl;
}
