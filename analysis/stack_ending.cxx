#include <TCanvas.h>
#include <iostream>
#include <TApplication.h>
#include <TTree.h>
#include <TH1F.h>
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
    auto _file0 = new TFile(argc==1?"RootOut.root":argv[1]);
    TTree *Tree = nullptr;
    auto leg = new TLegend(.05, .82, .25, .95);
    auto hs = new THStack("hs", (std::string("Stack by ") + "stop_mode").c_str());
    _file0->GetObject("Ntuple1", Tree);
    std::vector<TH1F *> hists;
    // std::vector<TPaveStats *> tphs;
    auto c1 = new TCanvas();
    for (long unsigned int i = 0; i < list.size(); i++)
    {
        if (Tree->Draw(("Depth>>hist" + std::to_string(i)).c_str(),
                       ("stop_mode" + std::string("==") + std::to_string(i)).c_str()) != 0)
        {
            hists.push_back((TH1F *)gDirectory->Get(("hist" + std::to_string(i)).c_str()));
            hists[hists.size() - 1]->SetLineColor(i+1!=10?i+1:11);
            hists[hists.size() - 1]->SetFillStyle(3001);
            hists[hists.size() - 1]->SetFillColorAlpha(i+1!=10?i+1:11, 0.5);
            hists[hists.size() - 1]->SetTitle(list[i].c_str());
            leg->AddEntry(hists[hists.size() - 1], list[i].c_str());
            // tphs.push_back((TPaveStats *)(hists[hists.size() - 1]->GetListOfFunctions()->FindObject("stats")));
            std::cout << "mode\t" << list[i] << "\tfound" << std::endl;
        }
        std::cout << "mode\t" << list[i] << "\tnot found" << std::endl;
    }
    {
        int max = 0;
        for (auto &i : hists)
        {
            // i->GetXaxis()->SetRangeUser(-80, 80);
            i->Scale(1, "width");
            // std::cout<< i->GetMaximum() <<std::endl;
            max = max > i->GetMaximum() ? max : i->GetMaximum();
            hs->SetMaximum(max);
            hs->Add(i);
        }
    }
    hs->SetTitle("Range for different stopping mode;Range (cm);Count");
    hs->Draw("nostack hist");

    {
        double i = 0;
        for (auto &hist : hists)
        {
            double s[4] = {0};
            hist->GetStats(s);
            i += s[0];
        }
        std::cout << "Total Found\t" << i << std::endl;
    }
    leg->Draw();
    c1->Draw();
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    app->Run();
    delete app;
    delete c1;
    delete Tree;
    delete _file0;
}
