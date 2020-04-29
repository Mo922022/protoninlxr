#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TApplication.h>
#include <TTree.h>
#include <TH2D.h>
#include <THStack.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TRootCanvas.h>
#include <TPaveStats.h>
#include <TObjArray.h>
#include <TVirtualHistPainter.h>
#include "modes.hh"

std::vector<int> colors = {632, 416, 600, 400, 616, 432, 800, 820, 840, 860, 880, 900};

int main(int argc, char **argv)
{
    auto app = new TApplication("app", &argc, argv);
    auto _file0 = new TFile("RootOut.root");
    TTree *Tree = nullptr;
    auto leg = new TLegend(.05, .82, .25, .95);
    auto hs = new THStack("hs", (std::string("Stack by ") + (argc == 1 ? "process" : argv[1])).c_str());
    _file0->GetObject("process", Tree);
    std::vector<TH2D *> hists;
    // std::vector<TPaveStats *> tphs;
    auto c1 = new TCanvas();
    gStyle->SetOptStat(0);
    for (long unsigned int i = 0; i < list.size(); i++)
    {
        if (Tree->Draw(("dedx:Depth>>hist" + std::to_string(i)).c_str(),
                       ((argc == 1 ? "process" : argv[1]) + std::string("==") + std::to_string(i)).c_str()) != 0)
        {
            hists.push_back((TH2D *)gDirectory->Get(("hist" + std::to_string(i)).c_str()));
            hists[hists.size() - 1]->SetLineColor(colors[hists.size() - 1]);
            hists[hists.size() - 1]->SetFillStyle(1001);
            hists[hists.size() - 1]->SetFillColorAlpha(colors[hists.size() - 1], 0.5);
            hists[hists.size() - 1]->SetTitle(list[i].c_str());
            leg->AddEntry(hists[hists.size() - 1], list[i].c_str());
            // tphs.push_back((TPaveStats *)(hists[hists.size() - 1]->GetListOfFunctions()->FindObject("stats")));
        }
    }
    {
        int max = 0;
        for (auto &i : hists)
        {
            // std::cout<< i->GetMaximum() <<std::endl;
            max = max > i->GetMaximum() ? max : i->GetMaximum();
            hs->SetMaximum(max);
            hs->Add(i, "SCAT");
        }
    }
    hs->SetTitle((std::string("Stack by ") + (argc == 1 ? "process" : argv[1]) + ";depth(cm);Count").c_str());
    hs->Draw("SCAT");

    leg->Draw();
    c1->Draw();
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    app->Run();
    delete app;
    delete c1;
    delete Tree;
    delete _file0;
}
