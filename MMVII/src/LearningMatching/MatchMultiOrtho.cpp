#include "include/MMVII_all.h"

namespace MMVII
{

namespace  cNS_MatchMultipleOrtho
{

class cAppliMatchMultipleOrtho;

class cAppliMatchMultipleOrtho : public cMMVII_Appli
{
     public :
        typedef tU_INT1               tElemMasq;
        typedef tREAL4                tElemOrtho;
        typedef tREAL4                tElemSimil;

        typedef cIm2D<tElemMasq>      tImMasq;
        typedef cIm2D<tElemOrtho>     tImOrtho;
        typedef cIm2D<tElemSimil>     tImSimil;
	
        typedef cDataIm2D<tElemMasq>  tDImMasq;
        typedef cDataIm2D<tElemOrtho> tDImOrtho;
        typedef cDataIm2D<tElemSimil> tDImSimil;


        cAppliMatchMultipleOrtho(const std::vector<std::string> & aVArgs,const cSpecMMVII_Appli & aSpec);

     private :
	/// Name of files imported
	std::string NameIm(int aKIm,const std::string & aPost) const
	{
             return mPrefixZ + "_I" +ToStr(aKIm) + "_" + aPost  + ".tif";
	}
	std::string NameOrtho(int aKIm) const {return NameIm(aKIm,"O");}  ///< Name individual ortho
	std::string NameMasq(int aKIm) const {return NameIm(aKIm,"M");}   ///< Name individual masq

        int Exe() override;
        cCollecSpecArg2007 & ArgObl(cCollecSpecArg2007 & anArgObl) override ;
        cCollecSpecArg2007 & ArgOpt(cCollecSpecArg2007 & anArgOpt) override ;

	//  One option, to replace by whatever you want
	void ComputeSimilByCorrelMaster();
	void CorrelMaster(const cPt2di &,int aKIm,bool & Ok,float & aCorrel);

	// -------------- Mandatory args -------------------
	std::string   mPrefixGlob;   // Prefix to all names
	int           mNbZ;      // Number of independant ortho (=number of Z)
	int           mNbIm;     // Number of images
	cPt2di        mSzW;      // Sizeof of windows
	bool          mIm1Mast;  //  Is first image the master image ?


	// -------------- Internal variables -------------------
	tImSimil                   mImSimil;   // computed image of similarity
	std::string                mPrefixZ;   // Prefix for a gizen Z
	cPt2di                     mSzIms;     // common  size of all ortho
	std::vector<tImOrtho>      mVOrtho;    // vector of loaded ortho at a given Z
	std::vector<tImMasq>       mVMasq;     // vector of loaded masq  at a given Z
};


/* *************************************************** */
/*                                                     */
/*              cAppliMatchMultipleOrtho               */
/*                                                     */
/* *************************************************** */

cAppliMatchMultipleOrtho::cAppliMatchMultipleOrtho(const std::vector<std::string> & aVArgs,const cSpecMMVII_Appli & aSpec) :
   cMMVII_Appli  (aVArgs,aSpec),
   mImSimil      (cPt2di(1,1))
{
}


cCollecSpecArg2007 & cAppliMatchMultipleOrtho::ArgObl(cCollecSpecArg2007 & anArgObl) 
{
 return
      anArgObl
          <<   Arg2007(mPrefixGlob,"Prefix of all names")
          <<   Arg2007(mNbZ,"Number of Z/Layers")
          <<   Arg2007(mNbIm,"Number of images in one layer")
          <<   Arg2007(mSzW,"Size of window")
          <<   Arg2007(mIm1Mast,"Is first image a master image ?")
   ;
}

cCollecSpecArg2007 & cAppliMatchMultipleOrtho::ArgOpt(cCollecSpecArg2007 & anArgOpt)
{
   return anArgOpt
          // << AOpt2007(mStepZ, "StepZ","Step for paralax",{eTA2007::HDV})
   ;
}


void cAppliMatchMultipleOrtho::CorrelMaster
     (
         const cPt2di & aCenter,
	 int aKIm,
	 bool & Ok,
	 float & aCorrel
     )
{
    if (! mVMasq.at(aKIm).DIm().GetV(aCenter) )
    {
        Ok = false;
	return;
    }
    Ok = true;

    const tDImOrtho & aDIO0 =  mVOrtho.at(0   ).DIm();
    const tDImOrtho & aDIOK =  mVOrtho.at(aKIm).DIm();

    cMatIner2Var<tElemOrtho> aMatI;
    for (const auto & aP : cRect2::BoxWindow(aCenter,mSzW))
    {
         aMatI.Add(aDIO0.GetV(aP),aDIOK.GetV(aP));
    }
    aCorrel =  aMatI.Correl(1e-5);
}

void cAppliMatchMultipleOrtho::ComputeSimilByCorrelMaster()
{
   // Just too lazy for making another tutorial ...
   MMVII_INTERNAL_ASSERT_strong(mIm1Mast,"DM4MatchMultipleOrtho, for now, only handle master image mode");

   const tDImMasq & aDIM0  =  mVMasq.at(0   ).DIm();
   tDImSimil & aDImSim = mImSimil.DIm();
   for (const auto & aP : aDImSim)
   {
        float aSumCor  = 0.0; // 
        float aNbOk = 0.0; // 
	if (aDIM0.GetV(aP))
	{
            for (int aKIm=1 ; aKIm<mNbIm ; aKIm++)
	    {
                bool Ok;
	        float aCorrel;
                CorrelMaster(aP,aKIm,Ok,aCorrel);
	        if (Ok)
	        {
                   aSumCor += aCorrel;
	           aNbOk   += 1;
	        }
	    }
	}
	float aAvgCorr =  (aSumCor / std::max(1e-5f,aNbOk)) ;
	aDImSim.SetV(aP,1-aAvgCorr);
   }
}


int  cAppliMatchMultipleOrtho::Exe()
{
   for (int aZ=0 ; aZ<mNbZ ; aZ++)
   {
        mPrefixZ =  mPrefixGlob + "_Z" + ToStr(aZ);

        bool NoFile = ExistFile(mPrefixZ+ "_NoData");
        bool WithFile = ExistFile(NameOrtho(0));
	// A little check
        MMVII_INTERNAL_ASSERT_strong(NoFile!=WithFile,"DM4MatchMultipleOrtho, incoherence file");


	if (WithFile)
        {
	    for (int aKIm=0 ; aKIm<mNbIm ; aKIm++)
	    {
                 mVOrtho.push_back(tImOrtho::FromFile(NameOrtho(aKIm)));
		 mSzIms = mVOrtho[0].DIm().Sz();

                 mVMasq.push_back(tImMasq::FromFile(NameMasq(aKIm)));

                 MMVII_INTERNAL_ASSERT_strong(mVOrtho[aKIm].DIm().Sz()==mSzIms,"DM4O : variable size(ortho)");
                 MMVII_INTERNAL_ASSERT_strong(mVMasq [aKIm].DIm().Sz()==mSzIms,"DM4O : variable size(masq)");
	    }
	    mImSimil = tImSimil(mSzIms);
	    mImSimil.DIm().InitCste(2.0);

            ComputeSimilByCorrelMaster();
	    mImSimil.DIm().ToFile(mPrefixZ+ "_Sim.tif");
	    mVOrtho.clear();
	    mVMasq.clear();
        }
   }
   return EXIT_SUCCESS;
}



};

/* =============================================== */
/*                                                 */
/*                       ::                        */
/*                                                 */
/* =============================================== */
using namespace  cNS_MatchMultipleOrtho;

tMMVII_UnikPApli Alloc_MatchMultipleOrtho(const std::vector<std::string> &  aVArgs,const cSpecMMVII_Appli & aSpec)
{
   return tMMVII_UnikPApli(new cAppliMatchMultipleOrtho(aVArgs,aSpec));
}

cSpecMMVII_Appli  TheSpecMatchMultipleOrtho
(
     "DM4MatchMultipleOrtho",
      Alloc_MatchMultipleOrtho,
      "Compute similarite of overlapping ortho images",
      {eApF::Match},
      {eApDT::Image},
      {eApDT::Image},
      __FILE__
);



};
