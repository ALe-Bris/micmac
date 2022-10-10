#include "cMMVII_Appli.h"
#include "MMVII_Sys.h"


namespace MMVII
{


/* ==================================================== */
/*                                                      */
/*          cAppli_GenGui                               */
/*                                                      */
/* ==================================================== */


class cAppli_GenGui : public cMMVII_Appli
{
     public :
        cAppli_GenGui(const std::vector<std::string> & aVArgs,const cSpecMMVII_Appli &);  ///< constructor
        int Exe() override;                                             ///< execute action
        cCollecSpecArg2007 & ArgObl(cCollecSpecArg2007 & anArgObl) override; ///< return spec of  mandatory args
        cCollecSpecArg2007 & ArgOpt(cCollecSpecArg2007 & anArgOpt) override; ///< return spec of optional args
     protected :
     private :
};



cCollecSpecArg2007 & cAppli_GenGui::ArgObl(cCollecSpecArg2007 & anArgObl)
{
   return anArgObl;
}

cCollecSpecArg2007 & cAppli_GenGui::ArgOpt(cCollecSpecArg2007 & anArgOpt)
{
   return anArgOpt;
}


cAppli_GenGui::cAppli_GenGui(const std::vector<std::string> & aVArgs,const cSpecMMVII_Appli & aSpec) :
  cMMVII_Appli (aVArgs,aSpec)
{
}

int cAppli_GenGui::Exe()
{
    StdOut() << "[\n";
    StdOut().flush();
    for (const auto & aSpec : cSpecMMVII_Appli::VecAll())
    {
        GlobSysCall(mFullBin + " v" + aSpec->Name(),true);
        if (aSpec != cSpecMMVII_Appli::VecAll().back()) {
            StdOut()<<"  ,\n";
            StdOut().flush();
        }
    }
    StdOut() << "]\n";
    StdOut().flush();
    return EXIT_SUCCESS;
}


tMMVII_UnikPApli Alloc_GenGui(const std::vector<std::string> & aVArgs,const cSpecMMVII_Appli & aSpec)
{
   return tMMVII_UnikPApli(new cAppli_GenGui(aVArgs,aSpec));
}

cSpecMMVII_Appli  TheSpecGenGui
(
    "GenGui",
    Alloc_GenGui,
    "This command is used to generate GUI specifications",
    {eApF::ManMMVII},
    {eApDT::ToDef},
    {eApDT::ToDef},
    __FILE__
);

}
