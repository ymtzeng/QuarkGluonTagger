#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "QuarkGluonTagger/EightTeV/interface/QGLikelihoodCalculator.h"
#include "/afs/cern.ch/work/p/pandolf/CMSSW_5_3_9_patch1_QGSyst/src/QuarkGluonTagger/EightTeV/interface/QGMLPCalculator.h"



float get_xSecWeight( const std::string& dataset );


int main( int argc, char* argv[] ) {


  if( argc<3 ) {
    std::cout << "USAGE: ./makeSunilTreeFlat [DATASET] [SELECTIONTYPE=\"ZJet\" or \"DiJet\"] [DIRECTORY=\"/afs/cern.ch/work/s/sunil/public/forTom/\"]" << std::endl;
    exit(11);
  }


  std::string dataset;
  if( argc>1 ) {
    std::string dataset_str(argv[1]);
    dataset = dataset_str;
  }

  std::string selectionType;
  if( argc>2 ) {
    std::string selectionType_str(argv[2]);
    selectionType = selectionType_str;
  }

  if( selectionType!="ZJet" && selectionType!="DiJet" ) {
    std::cout << "Invalid selectionType: Only 'ZJet' and 'DiJet' currently supported." << std::endl;
    exit(113);
  }


  std::string dir="/afs/cern.ch/work/s/sunil/public/forTom/";
  if( argc>3 ) {
    std::string dir_str(argv[3]);
    dir = dir_str;
  }
    

  std::cout << "dataset: " << dataset << std::endl;
  std::cout << "selectionType: " << selectionType << std::endl;
  std::cout << "dir: " << dir << std::endl;

  std::string sunilTreeName = dir + "/analysis_" + dataset + ".root/Hbb/events";

  TChain* sunilTree = new TChain("events");
  sunilTree->Add(sunilTreeName.c_str());
  
  std::cout << "-> Opening: " <<  sunilTreeName << std::endl;
  int nentries = sunilTree->GetEntries();
  std::cout << "-> Tree has: " << nentries << std::endl;


  std::string flatFileName = "sunilFlat_" + selectionType + "_" + dataset + ".root";
  TFile* newFile = TFile::Open(flatFileName.c_str(), "recreate");
  newFile->cd();

  TTree* flatTree = new TTree("tree_passedEvents", "");

  int run;
  int event;
  float rho;
  float rhoMLP;
  int nvtx;
  std::vector<bool> *triggerResult = 0;
  float jetPt[20];
  float jetEta[20];
  float jetPhi[20];
  float jetEnergy[20];
  float jetBeta[20];
  float jetBtag[20];
  int nCharged[20];
  int nNeutral[20];
  float jetPtD[20];
  float jetPtD_QC[20];
  Float_t jetAxis_QC[2][4];
  Float_t jetAxis[2][4];
  float jetPull_QC[4];
  float jetLead[4];
  int nCharged_QC[20];
  int nCharged_ptCut[20];
  int nNeutral_ptCut[20];
  std::vector<int> *partonId=0;
  std::vector<int> *partonSt=0;
  std::vector<float> *partonPt=0;
  std::vector<float> *partonEta=0;
  std::vector<float> *partonPhi=0;
  std::vector<float> *partonE=0;
  std::vector<float> *MuPt=0;
  std::vector<float> *MuEta=0;
  std::vector<float> *MuPhi=0;
  std::vector<float> *MuE=0;


  sunilTree->SetBranchAddress("runNo", &run);
  sunilTree->SetBranchAddress("evtNo", &event);
  sunilTree->SetBranchAddress("nvtx", &nvtx);
  sunilTree->SetBranchAddress("rhoIso", &rho);
  sunilTree->SetBranchAddress("rho", &rhoMLP);
  sunilTree->SetBranchAddress("triggerResult", &triggerResult);

  sunilTree->SetBranchAddress("jetPt", jetPt);
  sunilTree->SetBranchAddress("jetEta", jetEta);
  sunilTree->SetBranchAddress("jetPhi", jetPhi);
  sunilTree->SetBranchAddress("jetEnergy", jetEnergy);
  sunilTree->SetBranchAddress("jetBeta", jetBeta);
  sunilTree->SetBranchAddress("jetBtag", jetBtag);
  sunilTree->SetBranchAddress("jetChgPart", nCharged);
  sunilTree->SetBranchAddress("jetNeutralPart", nNeutral);
  sunilTree->SetBranchAddress("jetPtD", jetPtD);
  sunilTree->SetBranchAddress("jetPtD_QC", jetPtD_QC);
  sunilTree->SetBranchAddress("jetAxis",&jetAxis);
  sunilTree->SetBranchAddress("jetAxis_QC",&jetAxis_QC);
  sunilTree->SetBranchAddress("jetPull_QC",&jetPull_QC);
  sunilTree->SetBranchAddress("jetLead",&jetLead);
  sunilTree->SetBranchAddress("jetChgPart_QC", nCharged_QC);
  sunilTree->SetBranchAddress("jetChgPart_ptcut", nCharged_ptCut);
  sunilTree->SetBranchAddress("jetNeutralPart_ptcut", nNeutral_ptCut);
  sunilTree->SetBranchAddress("partonId",&partonId);
  sunilTree->SetBranchAddress("partonSt",&partonSt);
  sunilTree->SetBranchAddress("partonPt",&partonPt);
  sunilTree->SetBranchAddress("partonEta",&partonEta);
  sunilTree->SetBranchAddress("partonPhi",&partonPhi);
  sunilTree->SetBranchAddress("partonE",&partonE);
  if( selectionType=="ZJet" ) {
    sunilTree->SetBranchAddress("MuPt",&MuPt);
    sunilTree->SetBranchAddress("MuEta",&MuEta);
    sunilTree->SetBranchAddress("MuPhi",&MuPhi);
    sunilTree->SetBranchAddress("MuE",&MuE);
  }


  int nJet;
  float eventWeight;
  int jetPdgId[20];
  float axis2_QC[20];
  float axis1_QC[20];
  float rmsCand_QC[20];
  float qglMLPJet[20];
  float qglJet[20];

  flatTree->Branch("eventWeight", &eventWeight, "eventWeight/F");
  flatTree->Branch("nJet", &nJet, "nJet/I");
  flatTree->Branch("ptJet", jetPt, "ptJet[nJet]/F");
  flatTree->Branch("etaJet", jetEta, "etaJet[nJet]/F");
  flatTree->Branch("pdgIdJet", jetPdgId, "pdgIdJet[nJet]/I");
  flatTree->Branch("rhoPF", &rho, "rhoPF/F");
  flatTree->Branch("rhoPF_allJet", &rhoMLP, "rhoPF_allJet/F");
  flatTree->Branch("nChargedJet", nCharged, "nChargedJet[nJet]/I");
  flatTree->Branch("nNeutralJet", nNeutral, "nNeutralJet[nJet]/I");
  flatTree->Branch("ptDJet", jetPtD, "ptDJet[nJet]/F");
  flatTree->Branch("ptD_QCJet", jetPtD_QC, "ptDJet_QC[nJet]/F");
  flatTree->Branch("pull_QCJet", jetPull_QC, "pull_QC[nJet]/F");
  flatTree->Branch("RJet", jetLead, "RJet[nJet]/F");
  flatTree->Branch("axis1_QCJet", axis1_QC, "axis1_QCJet[nJet]/F");
  flatTree->Branch("axis2_QCJet", axis2_QC, "axis2_QCJet[nJet]/F");
  flatTree->Branch("rmsCand_QCJet", rmsCand_QC, "rmsCand_QCJet[nJet]/F");
  flatTree->Branch("nChg_QCJet", nCharged_QC, "nChg_QCJet[nJet]/I");
  flatTree->Branch("nNeutral_ptCutJet", nNeutral_ptCut, "nNeutral_ptCutJet[nJet]/I");
  flatTree->Branch("qgMLPJet", qglMLPJet, "qgMLPJet[nJet]/F");
  flatTree->Branch("qglJet", qglJet, "qglJet[nJet]/F");



  std::cout << "-> Booking QGLikelihoodCalculator..." << std::endl;
  QGLikelihoodCalculator* qglc = new QGLikelihoodCalculator("QuarkGluonTagger/EightTeV/data/");
  std::cout << "-> Booking QGMLPCalculator..." << std::endl;
  QGMLPCalculator* qgmlp = new QGMLPCalculator("MLP","QuarkGluonTagger/EightTeV/data/", true);


  std::string puFileName = (selectionType=="ZJet") ? "PU_rewt_ZJets.root" : "PU_rewt_flatP6.root";
  TFile *fPU = TFile::Open(puFileName.c_str());
  std::string puHistName = (selectionType=="ZJet") ? "hist_puWT" : "hist_WT"; //absurd
  TH1F *hPU = (TH1F*)fPU->Get(puHistName.c_str());

  TFile* filePtEtaWeights = TFile::Open("Jetpteta_rewt2D_flatP6.root");
  TH1F* hPtEta_wt = (TH1F*)filePtEtaWeights->Get("hist_WT");



  std::cout << "-> Begin loop." << std::endl;

  for( unsigned int ientry=0; ientry<nentries; ++ientry ) {

    sunilTree->GetEntry(ientry);

    if( ientry % 100000 == 0 ) std::cout << "entry: " << ientry << "/" << nentries << std::endl;

    bool isMC = (run < 10000);

    if( !isMC && !triggerResult->at(1) ) continue;

    float wt_pu = 1.;
    float wt_pteta = 1.;
    float wt_xsec = 1.;

    if( isMC ) {

      // kinematic reweighting only for dijets:
      if( selectionType=="DiJet" ) {
        int ptetabin = hPtEta_wt->FindBin(jetPt[0],fabs(jetEta[0]));
        wt_pteta = hPtEta_wt->GetBinContent(ptetabin);
      }

      wt_xsec =  get_xSecWeight(dataset);

      // pu reweighting:
      int bin = hPU->FindBin(rho);
      wt_pu = hPU->GetBinContent(bin);

    }


    eventWeight = wt_xsec*wt_pu*wt_pteta;


    // event selection:

    if( selectionType=="DiJet" ) {

      if( jetPt[0] < 40.) continue;
      if( jetPt[1] < 20.) continue;

      TLorentzVector jet1, jet2;
      jet1.SetPtEtaPhiE( jetPt[0], jetEta[0], jetPhi[0], jetEnergy[0]);
      jet2.SetPtEtaPhiE( jetPt[1], jetEta[1], jetPhi[1], jetEnergy[1]);
      if( fabs(jet1.DeltaPhi(jet2)) < 2.5 ) continue;

      if( jetPt[2] > 0.3*(jetPt[0]+jetPt[1])/2. )continue;

    } else if( selectionType=="ZJet" ) {

      TLorentzVector mu1, mu2;
      mu1.SetPtEtaPhiE( MuPt->at(0), MuEta->at(0), MuPhi->at(0), MuE->at(0) );
      mu2.SetPtEtaPhiE( MuPt->at(1), MuEta->at(1), MuPhi->at(1), MuE->at(1) );

      TLorentzVector Zmm = mu1 + mu2;

      if( Zmm.M()<70. || Zmm.M()>110. ) continue;
      if( jetPt[0]<20. ) continue; 
      TLorentzVector jet;
      jet.SetPtEtaPhiE( jetPt[0], jetEta[0], jetPhi[0], jetEnergy[0] );
      if( fabs(Zmm.DeltaPhi(jet)) < 2.5 ) continue;
      if( jetPt[1]>0.3*Zmm.Pt() ) continue;

    }

    // common jet ID:
    if( fabs(jetEta[0]) < 2.5 && jetBeta[0] < ( 1.0 -  0.2 * TMath::Log( nvtx - 0.67))) continue; 
    if( jetBtag[0]>0.244 ) continue;
    if( jetPtD_QC[0]>0.9 ) continue;


    axis1_QC[0] = jetAxis_QC[0][0];
    axis2_QC[0] = jetAxis_QC[1][0];
    rmsCand_QC[0] = (axis1_QC[0]>0. && axis2_QC[0]>0.) ? sqrt( axis1_QC[0]*axis1_QC[0] + axis2_QC[0]*axis2_QC[0] ) : -1.;


    nJet=0;
    for( unsigned ijet=0; ijet<4; ++ijet )
      if( jetPt[ijet]>20. ) nJet++;


    TLorentzVector thisJet;
    thisJet.SetPtEtaPhiE( jetPt[0], jetEta[0], jetPhi[0], jetEnergy[0] );

    // match to parton:
    float deltaR_min = 999.;
    int foundPart = -1;

    for(int iPart=0;iPart<int(partonPt->size());iPart++) {

      if( partonSt->at(iPart) != 3 ) continue;
      if( !( fabs(partonId->at(iPart))<6 || fabs(partonId->at(iPart))>0 || partonId->at(iPart)==21) ) continue;
      TLorentzVector thisPart;
      thisPart.SetPtEtaPhiE( partonPt->at(iPart), partonEta->at(iPart), partonPhi->at(iPart), partonE->at(iPart) );
      float deltaR_part = thisPart.DeltaR(thisJet);
      if(deltaR_part< deltaR_min) {
        deltaR_min = deltaR_part;
        foundPart = iPart;
      }

    }

    if(deltaR_min < 0.3 && foundPart>=0) {
      jetPdgId[0] = partonId->at(foundPart);
    } else {
      jetPdgId[0] = 0;
    }


    if( !isMC && fabs(jetEta[0])>2.5 )
      qglJet[0] = qglc->computeQGLikelihood2012( jetPt[0], jetEta[0], rho, nCharged_QC[0]+nNeutral_ptCut[0]-1, jetPtD_QC[0], axis2_QC[0]);
    else
      qglJet[0] = qglc->computeQGLikelihood2012( jetPt[0], jetEta[0], rho, nCharged_QC[0]+nNeutral_ptCut[0], jetPtD_QC[0], axis2_QC[0]);

    std::map<TString,float> variables_MLP;
    variables_MLP["axis1"]=axis1_QC[0];
    variables_MLP["axis2"]=axis2_QC[0];
    variables_MLP["ptD"]=jetPtD_QC[0];
    if( fabs(jetEta[0])<2.5 ) {
      variables_MLP["mult"]=nCharged_QC[0];
    } else {
      if( isMC ) {
        variables_MLP["mult"]=nCharged_ptCut[0]+nNeutral_ptCut[0];
      } else {
        variables_MLP["mult"]=nCharged_ptCut[0]+nNeutral_ptCut[0]-1;
      }
    }
    
    variables_MLP["pt"]=jetPt[0];
    variables_MLP["eta"]=jetEta[0];
    variables_MLP["rho"]=rhoMLP;

    qglMLPJet[0] = qgmlp->QGvalue(variables_MLP);

    flatTree->Fill();

  } // for entries


  newFile->cd();
  flatTree->Write();
  newFile->Close();

  return 0;

}




float get_xSecWeight( const std::string& dataset ) {

  return 1.;

}
