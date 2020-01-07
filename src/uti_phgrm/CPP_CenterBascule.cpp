/*Header-MicMac-eLiSe-25/06/2007

    MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
    eLiSe  : ELements of an Image Software Environnement

    www.micmac.ign.fr


    Copyright : Institut Geographique National
    Author : Marc Pierrot Deseilligny
    Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
    "A multiresolution and optimization-based image matching approach:
    An application to surface reconstruction from SPOT5-HRS stereo imagery."
    In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
    (With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
    d'images, adapte au contexte geograhique" to appears in
    Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

   MicMac est un logiciel de mise en correspondance d'image adapte
   au contexte de recherche en information geographique. Il s'appuie sur
   la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
   licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/
#include "StdAfx.h"
#include "math.h"

#define DEF_OFSET -12349876

int CentreBascule_main(int argc,char ** argv)
{
    NoInit = "NoP1P2";
    aNoPt = Pt2dr(123456,-8765432);

    // MemoArg(argc,argv);
    MMD_InitArgcArgv(argc,argv);
    std::string  aDir,aPat,aFullDir;


    std::string AeroOut;
    std::string AeroIn;
    //std::string DicoPts;
    std::string BDC;
    bool ModeL1 = false;
    bool CalcV   = false;


    ElInitArgMain
    (
    argc,argv,
    LArgMain()  << EAMC(aFullDir,"Full name (Dir+Pat)", eSAM_IsPatFile )
                    << EAMC(AeroIn,"Orientation in", eSAM_IsExistDirOri)
                    << EAMC(BDC,"Localization of Information on Centers", eSAM_IsExistDirOri)
                    << EAMC(AeroOut,"Orientation out", eSAM_IsOutputDirOri),
    LArgMain()
                    <<  EAM(ModeL1,"L1",true,"L1 minimization vs L2; (Def=false)", eSAM_IsBool)
                    <<  EAM(CalcV,"CalcV",true,"Use speed to estimate time delay (Def=false)", eSAM_IsBool)
    );

    if (!MMVisualMode)
    {
#if (ELISE_windows)
        replace( aFullDir.begin(), aFullDir.end(), '\\', '/' );
#endif
        SplitDirAndFile(aDir,aPat,aFullDir);
        StdCorrecNameOrient(AeroIn,aDir);
        StdCorrecNameOrient(BDC,aDir);



        std::string aCom =   MMDir() + std::string("bin/mm3d Apero ")
                + XML_MM_File("Apero-Center-Bascule.xml")
                + std::string(" DirectoryChantier=") +aDir +  std::string(" ")
                + std::string(" +PatternAllIm=") + QUOTE(aPat) + std::string(" ")
                + std::string(" +AeroIn=") + AeroIn
                + std::string(" +AeroOut=") +  AeroOut
                + std::string(" +BDDC=") +  BDC
                ;

        if (ModeL1)
        {
            aCom = aCom+ std::string(" +L2Basc=") + ToString(!ModeL1);
        }

        if (CalcV)
        {
            aCom = aCom+ std::string(" +CalcV=") + ToString(CalcV);
        }


        std::cout << "Com = " << aCom << "\n";
        int aRes = system_call(aCom.c_str());

        return aRes;
    }
    else return EXIT_SUCCESS;
}


/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/

class cCmpOriOneSom
{
    public :
       cCmpOriOneSom(const std::string& aName,CamStenope* C1,CamStenope* C2) ;
       void SetPrec(const cCmpOriOneSom & aS1);

       void Show(ofstream & ofs,bool WithRel,bool DoCirc) const;
       void PlyShowDiffRot(cPlyCloud & aPlyFile,double aMult,const Pt3di & aCol) const;
       void PlyShowDiffCenter(cPlyCloud & aPlyFile,double aMult,const Pt3di & aCol) const;
  
    public :
       const std::string mName;
       const CamStenope * mCam1;
       //   mCam1.Orient() =   Orientation Monde to Cam
       ElRotation3D       mRC1ToM;
       Pt3dr              mC1;
       Pt2dr              mP1;
       const CamStenope * mCam2;
       ElRotation3D       mRC2ToM;
       Pt3dr              mC2;
       Pt2dr              mP2;
       int                mNum;
       double             mAbsCurv;
       double             mDC;
       double             mDMat;
       double             mDMatRel;
       double             mDCRel;
       ElRotation3D       mRC1ToC2;
       // For analyzing circular traj
       double             mDistBcl;  // Ratio of "bouclage"
       bool               mIsPivot;
       int                mNumTour;
       int                mNumInTour;
       double             mAbscInTour;
};


void cCmpOriOneSom::SetPrec(const cCmpOriOneSom & aSPrec)
{
    mNum = aSPrec.mNum + 1;
    mAbsCurv = aSPrec.mAbsCurv+ euclid(mC1-aSPrec.mC1);
    // Calcul des orientations relatives / � image prec pour 1 et 2
    ElRotation3D aCurToPrec1 =  aSPrec.mRC1ToM.inv()  * mRC1ToM ;  
    ElRotation3D aCurToPrec2 =  aSPrec.mRC2ToM.inv()  * mRC2ToM ;  

    // Calcul des difference en rotation et centres pour ces orientations relatives
    mDMatRel = sqrt(aCurToPrec1.Mat().L2(aCurToPrec2.Mat()))  ;  // plus ou moin homogene a des radians
    mDCRel   = euclid(aCurToPrec1.tr()-aCurToPrec2.tr()) ;

    // They are similar but different 
    // double aDD = sqrt(mRC1ToC2.Mat().L2(aSPrec.mRC1ToC2.Mat()));
    // std::cout << "FFFFff  " << aDD / mDMatRel << "\n";  
}

void cCmpOriOneSom::Show(ofstream & ofs,bool WithRel,bool DoCirc) const
{
    double MultDMat = 1e5;

    ofs << mName
        << "," << ToString(mC1.x) 
        << "," << ToString(mC1.y) 
        << "," << ToString(mC1.z) 
        << "," << ToString(abs(mC1.x - mC2.x)) 
        << "," << ToString(abs(mC1.y - mC2.y)) 
        << "," << ToString(mC1.z - mC2.z) 
        << "," << ToString(euclid(mP1-mP2))
        << "," << ToString(euclid(mC1-mC2))
        << "," << ToString(mDMat*MultDMat)
    ;

    if (WithRel)
    {
        ofs 
            << "," << ToString(mDCRel) 
            << "," << ToString(mDMatRel*MultDMat)
        ;
    }
    if (DoCirc)
    {
        ofs 
            << "," << ToString(mNumTour) 
            << "," << ToString(mNumInTour) 
            << "," << ToString(mAbscInTour)
        ;
    }
    ofs << "\n";

}

void cCmpOriOneSom::PlyShowDiffRot(cPlyCloud & aPlyFile,double aMult,const Pt3di & aCol) const
{
    // We represent the Axiator of differential orientation 
    Pt3dr anAxe = AxeRot(mRC1ToC2.Mat());
    double anAngle = TetaOfAxeRot(mRC1ToC2.Mat(),anAxe);
    aPlyFile.AddSeg(aCol,mC1,mC1+anAxe*(anAngle*aMult),3000);
}

void cCmpOriOneSom::PlyShowDiffCenter(cPlyCloud & aPlyFile,double aMult,const Pt3di & aCol) const
{
    aPlyFile.AddSeg(aCol,mC1,mC1+(mC2-mC1)*aMult,3000);
}

cCmpOriOneSom::cCmpOriOneSom(const std::string& aName,CamStenope* aCam1,CamStenope* aCam2)  :
   mName       (aName),
   mCam1       (aCam1),
   mRC1ToM     (aCam1->Orient().inv()),
   mC1         (mRC1ToM.tr()),
   mP1         (mC1.x,mC1.y),
   mCam2       (aCam2),
   mRC2ToM     (aCam2->Orient().inv()),
   mC2         (mRC2ToM.tr()),
   mP2         (mC2.x,mC2.y),
   mNum        (0.0),
   mAbsCurv    (0.0),
   mDC         (euclid(mC1-mC2)),
   mDMat       (sqrt(mRC1ToM.Mat().L2(mRC2ToM.Mat()))),
   mDMatRel    (0.0),
   mDCRel      (0.0),
   mRC1ToC2    (mRC2ToM.inv() * mRC1ToM),
   mDistBcl    (0.0),
   mIsPivot    (false),
   mNumTour    (0),
   mNumInTour  (0),
   mAbscInTour (0)
{
   ELISE_ASSERT(euclid( mCam1->VraiOpticalCenter() - mC1)<1e-5,"Verif conventions");
  // Pt3dr AxeRot(const ElMatrix<REAL> & aMat);
}

class cAppli_CmpOriCam : public cAppliWithSetImage
{
    public :

        cAppli_CmpOriCam(int argc, char** argv);
        void ComputeCircular();

        std::string mPat,mOri1,mOri2;
        std::string mDirOri2;
        std::string mXmlG;
        std::string mCSV = "CSVEachPose.csv";
        std::string mPly;
        std::vector<cCmpOriOneSom> mVCmp;
        cInterfChantierNameManipulateur * mICNM2;
        std::vector<double>  mSeuilsCircs;

};

void cAppli_CmpOriCam::ComputeCircular()
{
   ELISE_ASSERT(mSeuilsCircs.size()==2,"Size Seuil Circs");
    int aI0 = round_ni(mSeuilsCircs.at(0));
    double aSeuilDist  = mSeuilsCircs.at(1);
    std::vector<double> aVRatio;
    cCmpOriOneSom & aS0=mVCmp[aI0];
    for (auto & aSom : mVCmp)
    {
        aSom.mDistBcl = euclid(aSom.mC1-aS0.mC1) ;
    }
    for (int aK=1 ; aK<int(mVCmp.size()-1); aK++)
    {
       cCmpOriOneSom & aSom=mVCmp[aK];
       if (
                 (aSom.mDistBcl<aSeuilDist)  
              && (aSom.mDistBcl<mVCmp[aK-1].mDistBcl) 
              && (aSom.mDistBcl<mVCmp[aK+1].mDistBcl)
          )
       {
          aSom.mIsPivot= true;
          std::cout << aSom.mName << " " << aSom.mDistBcl << "\n";
       }
    }
    for (int aK=1 ; aK<int(mVCmp.size()); aK++)
    {
       cCmpOriOneSom & aPrec = mVCmp[aK-1];
       cCmpOriOneSom & aSom  = mVCmp[aK];
       if (aSom.mIsPivot)
       {
           aSom.mNumTour = aPrec.mNumTour+1;
           aSom.mNumInTour = 0;
           aSom.mAbscInTour = 0;
       }
       else
       {
           aSom.mNumTour = aPrec.mNumTour;
           aSom.mNumInTour = aPrec.mNumInTour+1;
           aSom.mAbscInTour = aPrec.mAbscInTour + euclid(aSom.mC1-aPrec.mC1);
       }
    }
    std::cout << "END PIVOT \n";
    getchar();
}

cAppli_CmpOriCam::cAppli_CmpOriCam(int argc, char** argv) :
    cAppliWithSetImage(argc-1,argv+1,0)
{
   Pt3di aColXY(255,0,0);
   Pt3di aColZ(0,0,255);
   Pt3di aColOri(255,255,0);
   double aScaleC;
   double aScaleO;
   double aF;
   Pt2dr SeuilMatRel;

   ElInitArgMain
   (
        argc,argv,
        LArgMain()  << EAMC(mPat,"Full Name (Dir+Pattern)",eSAM_IsPatFile)
                    << EAMC(mOri1,"Orientation 1", eSAM_IsExistDirOri)
                    << EAMC(mOri2,"Orientation 2"),
        LArgMain()  << EAM(mDirOri2,"DirOri2", true,"Orientation 2")
					<< EAM(mXmlG,"XmlG",true,"Generate Xml")
                    << EAM(mCSV,"CSV",true,"Generate detail CSV (excel compatible) for each image")
                    << EAM(mPly,"Ply",true,"Generate .ply File")
                    << EAM(aColXY,"ColXY", true, "color for XY component of .ply")
                    << EAM(aColZ,"ColZ", true, "color for Z component of .ply")
                    << EAM(aColOri,"ColOri",true,"color for orientation component of .ply")
                    << EAM(aScaleC,"ScaleC",true,"Scale for camera center difference, the center diff is displayed when this option is activated")
                    << EAM(aScaleO,"ScaleO",true,"Scale for camera orientation difference, the ori diff is displayed when this option is activated")
                    << EAM(aF,"F",true,"approximate value of focal length in (m), Def=0.03875m for Camlight")
                    << EAM(SeuilMatRel,"SMR",true,"Seuil Mat Rel [Ratio,Prop] ")
                    << EAM(mSeuilsCircs,"SeuilCirc",true,"Thresholds to compute circ [I0,RatBcl]")
   );
   bool DoRel = true; // Do relative informatio,
   bool DoCirc = EAMIsInit(&mSeuilsCircs);

   mICNM2 = mEASF.mICNM;
   if (EAMIsInit(&mDirOri2))
   {
       mICNM2 = cInterfChantierNameManipulateur::BasicAlloc(mDirOri2);
   }


   mICNM2->CorrecNameOrient(mOri2);


   double aSomDC = 0;
   double aSomDM = 0;

   bool isCSV = false;
   ofstream mCSVContent;
   if (EAMIsInit(&mCSV))
   {
     mCSVContent.open(mCSV);
     isCSV = true;
     mCSVContent<< "Img,X1,Y1,Z1,dX,dY,dZ,dXY,dXYZ,dMat";
     if (DoRel)
        mCSVContent<< ",dTrRel,dMatRel";
     if (DoCirc)
        mCSVContent<< ",NumTour,NumInTour,AbscInTour";
     mCSVContent<< "\n";
   }
   cPlyCloud aPlyC, aPlyO;

   // Calcul de la structure de sommets fusionnant les 2 orientations
   for (int aK=0 ; aK<int(mVSoms.size()) ; aK++)
   {
       cImaMM * anIm = mVSoms[aK]->attr().mIma;
       CamStenope * aCam1 =  anIm->CamSNN();
       CamStenope * aCam2 = mICNM2->StdCamStenOfNames(anIm->mNameIm,mOri2);

       mVCmp.push_back(cCmpOriOneSom(anIm->mNameIm,aCam1,aCam2));
   }

   // Calcul des orientation relatives, des abscisses etc ...
   std::vector<double>      aVDMatRel;
   for (int aK=1 ; aK<int(mVCmp.size()) ; aK++)
   {
      mVCmp[aK].SetPrec(mVCmp[aK-1]);
      aVDMatRel.push_back(mVCmp[aK].mDMatRel);
   }

   if (DoCirc)
   {
        ComputeCircular();
   }


   for (const auto & aSom : mVCmp)
   {
       aSomDC += aSom.mDC;
       aSomDM += aSom.mDMat;
       if (EAMIsInit(&aScaleO))
       {
           aSom.PlyShowDiffRot(aPlyO,aScaleO,aColOri);
       }

       if (isCSV)
       {
           aSom.Show(mCSVContent,DoRel,DoCirc);
       }

       if(EAMIsInit(&aScaleC))
       {
            aSom.PlyShowDiffCenter(aPlyC,aScaleC,aColXY);
       }
   }
	
   std::cout << "Aver;  DistCenter= " << aSomDC/mVSoms.size()
             << " DistMatrix= " << aSomDM/mVSoms.size()
             << "\n";
   if(mXmlG!="")
   {
	   cXmlTNR_TestOriReport aCmpOri;
	   aCmpOri.OriName() = mOri2;
	   aCmpOri.DistCenter() = aSomDC/mVSoms.size();
	   aCmpOri.DistMatrix() = aSomDM/mVSoms.size();
	   if(aSomDC/mVSoms.size()==0&&aSomDM/mVSoms.size()==0)
	   {
		   aCmpOri.TestOriDiff() = true;
	   }
	   else{aCmpOri.TestOriDiff() = false;}
	   MakeFileXML(aCmpOri, mXmlG);
   }

   if (isCSV)
   {
       mCSVContent.close();
   }

   if(EAMIsInit(&mPly))
   {
       aPlyC.PutFile(mPly.substr(0,mPly.size()-4)+"_Center.ply");
       aPlyO.PutFile(mPly.substr(0,mPly.size()-4)+"_Orientation.ply");
   }


   if (EAMIsInit(&SeuilMatRel)) 
   {
       std::cout << "======= Threshold Matrix Relative ======\n";
       double aValStd = KthValProp(aVDMatRel,SeuilMatRel.y);

       for (const auto & aSom : mVCmp)
       {
           double aRatio = aSom.mDMatRel / aValStd;
           if (aRatio>SeuilMatRel.x)
              std::cout << aSom.mName << " " << aRatio << "\n";
       }
   }
}

int CPP_CmpOriCam_main(int argc, char** argv)
{
    cAppli_CmpOriCam anApplu(argc,argv);

    return EXIT_SUCCESS;
}




/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant �  la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  �  l'utilisation,  �  la modification et/ou au
développement et �  la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe �
manipuler et qui le réserve donc �  des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités �  charger  et  tester  l'adéquation  du
logiciel �  leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
�  l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder �  cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
