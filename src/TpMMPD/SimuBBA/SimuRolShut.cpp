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
#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "StdAfx.h"
#include "string.h"
#include "../../uti_phgrm/TiepTri/TiepTri.h"
#include "../../uti_phgrm/TiepTri/MultTieP.h"
#include "../schnaps.h"
#include "SimuBBA.h"

int SimuRolShut_main(int argc, char ** argv)
{
    std::string aPatImgs, aSH, aOri, aSHOut{"SimuRolShut"}, aDir, aImgs,aModifP,aPostfix{".thm.tif"};
    int aLine{3};
    ElInitArgMain
            (
                argc, argv,
                LArgMain() << EAMC(aPatImgs,"Image Pattern",eSAM_IsExistFile)
                           << EAMC(aSH, "PMul File",  eSAM_IsExistFile)
                           << EAMC(aOri, "Ori",  eSAM_IsExistDirOri)
                           << EAMC(aModifP,"File containing pose modification for each image, file size = 1 or # of images"),
                LArgMain() << EAM(aSHOut,"Out",false,"Output name of generated tie points, default=simulated")
                           << EAM(aPostfix,"Postfix",true,"Postfix of images, default=.thm.tif")
                           << EAM(aLine,"Line",true,"Read file containing pose modification from a certain line, def=3 (two lines for file header)")
                );

    // get directory
    SplitDirAndFile(aDir,aImgs,aPatImgs);
    StdCorrecNameOrient(aOri, aDir);
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(aDir);
    const std::vector<std::string> aVImgs = *(aICNM->Get(aImgs));

    std::cout << aVImgs.size() << " image files.\n";

    // Generate poses corresponding to the end of exposure
    std::vector<Orientation> aVOrient;
    ReadModif(aVOrient,aModifP,aLine);

    int aSzPF = aPostfix.size();

    for(uint i=0; i<aVImgs.size();i++)
    {
        CamStenope * aCam = aICNM->StdCamStenOfNames(aVImgs[i],aOri);
        uint j = (aVOrient.size()==1)? 0 : i;
        aCam->AddToCenterOptical(aVOrient.at(j).Translation);
        aCam->MultiToRotation(aVOrient.at(j).Rotation);

        std::string aKeyOut = "NKS-Assoc-Im2Orient@-" + aOri;
        std::string aNameCamOut = aVImgs.at(i).substr(0,aVImgs.at(i).size()-aSzPF)+"_bis"+aPostfix;
        std::string aOriOut = aICNM->Assoc1To1(aKeyOut,aNameCamOut,true);
        cOrientationConique  anOC = aCam->StdExportCalibGlob();
        anOC.Interne().SetNoInit();
        anOC.FileInterne().SetVal(aICNM->StdNameCalib(aOri,aVImgs[i]));

        std::cout << "Generate " << aNameCamOut << endl;
        MakeFileXML(anOC,aOriOut);
    }

    //1. lecture of tie points and orientation
    std::cout << "Loading tie points + orientation...   ";
    cSetTiePMul * pSH = new cSetTiePMul(0);
    pSH->AddFile(aSH);
    std::map<std::string,cCelImTPM *> aVName2Im = pSH->DicoIm().mName2Im;

    // load cam for all Img
    // Iterate through all elements in std::map
    vector<CamStenope*> aVCam (aVName2Im.size());//real poses
    vector<CamStenope*> aVCambis (aVName2Im.size());//generated poses
    for(auto &aName2Im:aVName2Im)
    {
        CamStenope * aCam = aICNM->StdCamStenOfNames(aName2Im.first,aOri);
        aCam->SetNameIm(aName2Im.first);
        aVCam[aName2Im.second->Id()] = aCam;

        std::string aNamebis = aName2Im.first.substr(0,aName2Im.first.size()-aSzPF)+"_bis"+aPostfix;
        CamStenope * aCambis = aICNM->StdCamStenOfNames(aNamebis,aOri);
        aCambis->SetNameIm(aNamebis);
        aVCambis[aName2Im.second->Id()] = aCambis;
    }

    std::cout << "Finish loading " << pSH->VPMul().size() << " CONFIG\n";

    // declare aVStructH to stock generated tie points
    vector<ElPackHomologue> aVPack (aVName2Im.size());
    vector<int> aVIdImS (aVName2Im.size(),-1);
    StructHomol aStructH;
    aStructH.VElPackHomol = aVPack;
    aStructH.VIdImSecond = aVIdImS;
    vector<StructHomol> aVStructH (aVName2Im.size(),aStructH);

    //2. get 2D/3D position of tie points
    std::cout << "Filling ElPackHomologue...   ";


    // parse Configs aVCnf
    std::vector<cSetPMul1ConfigTPM *> aVCnf = pSH->VPMul();
    for (auto &aCnf:aVCnf)
    {
        std::vector<int> aVIdIm =  aCnf->VIdIm();

        // Parse all pts in one Config
        for (uint aKPtCnf=0; aKPtCnf<uint(aCnf->NbPts()); aKPtCnf++)
        {
            vector<Pt2dr> aVPtInter;
            vector<CamStenope*> aVCamInter;
            vector<CamStenope*> aVCamInterbis;
            vector<int> aVIdImInter;

            // Parse all imgs for one pts
            for (uint aKImCnf=0; aKImCnf<aVIdIm.size(); aKImCnf++)
            {

                aVPtInter.push_back(aCnf->Pt(aKPtCnf, aKImCnf));
                aVCamInter.push_back(aVCam[aVIdIm[aKImCnf]]);
                aVCamInterbis.push_back(aVCambis[aVIdIm[aKImCnf]]);
                aVIdImInter.push_back(aVIdIm[aKImCnf]);
            }

            //Intersect aVPtInter:

            ELISE_ASSERT(aVPtInter.size() == aVCamInter.size(), "Size not coherent");
            ELISE_ASSERT(aVPtInter.size() > 1 && aVCamInter.size() > 1, "Nb faiseaux < 2");
            Pt3dr aPInter3D = Intersect_Simple(aVCamInter , aVPtInter);
            //std::cout << aPInter3D << endl;


            // reproject aPInter3D sur tout les images dans aVCamInter
            std::vector<Pt2dr> aVP2d;
            std::vector<CamStenope *> aVCamInterVu;
            std::vector<int> aVIdImInterVu;

            for (uint itVCI=0; itVCI < aVCamInter.size(); itVCI++)
            {
                CamStenope * aCam = aVCamInter[itVCI];
                Pt2dr aPt2d0 = aCam->R3toF2(aPInter3D);//P0

                CamStenope * aCambis = aVCamInterbis[itVCI];
                Pt2dr aPt2d1 = aCambis->R3toF2(aPInter3D);//P1

                // Pl = l*P1 + (1-l)P0
                double aX = aCam->Sz().x * aPt2d0.x / (aCam->Sz().x - aPt2d1.x + aPt2d0.x); // Pl.x
                double aRatio = aX / aCam->Sz().x; // Pl.x / X
                double aY = aRatio * aPt2d1.y + (1-aRatio) * aPt2d0.y;
                Pt2dr aPt2d = Pt2dr(aX,aY);

                //std::cout << "P0 " << aPt2d0 << " Pl " << aPt2d << " P1 " << aPt2d1 << endl;


                //check if the point is in the camera view
                if (aCam->PIsVisibleInImage(aPInter3D) && IsInImage(aCam->Sz(),aPt2d))
                {
                    aVP2d.push_back(aPt2d);
                    //std::cout << aPt2d << endl;
                    aVCamInterVu.push_back(aCam);
                    aVIdImInterVu.push_back(aVIdImInter[itVCI]);
                }
            }

            // parse images to fill ElPackHomologue
            for (uint it1=0; it1 < aVCamInterVu.size(); it1++)
            {
                int aIdIm1=aVIdImInterVu.at(it1);
                aVStructH.at(aIdIm1).IdIm=aIdIm1;

                for (uint it2=0; it2 < aVCamInterVu.size(); it2++)
                {
                    if (it1==it2) continue;

                    int aIdIm2=aVIdImInterVu.at(it2);

                    ElCplePtsHomologues aCPH (aVP2d[it1],aVP2d[it2]);
                    aVStructH.at(aIdIm1).VElPackHomol.at(aIdIm2).Cple_Add(aCPH);
                    aVStructH.at(aIdIm1).VIdImSecond.at(aIdIm2)=aIdIm2;
                }
            }


        }
    }

    std::cout << "ElPackHomologue filled !\n";

    //writing of new tie points
    std::cout << "Writing Homol files...   ";
    //key for tie points
    std::string aKHOut =   std::string("NKS-Assoc-CplIm2Hom@")
            + "_"
            +  std::string(aSHOut)
            +  std::string("@")
            +  std::string("dat");


    for (uint itVSH=0; itVSH < aVStructH.size(); itVSH++)
    {
        int aIdIm1 = aVStructH.at(itVSH).IdIm;
        CamStenope * aCam1 = aVCam.at(aIdIm1);
        std::string aNameIm1 = aCam1->NameIm();
        if (IsInList(aVImgs,aNameIm1))
        {
            for (uint itVElPH=0; itVElPH < aVStructH.at(itVSH).VElPackHomol.size(); itVElPH++)
            {
                int aIdIm2 = aVStructH.at(itVSH).VIdImSecond.at(itVElPH);
                if (aIdIm2 == -1) continue;
                CamStenope * aCam2 = aVCam.at(aIdIm2);
                std::string aNameIm2 = aCam2->NameIm();
                if (IsInList(aVImgs,aNameIm2))
                {
                    std::string aHmOut= aICNM->Assoc1To2(aKHOut, aNameIm1, aNameIm2, true);
                    ElPackHomologue aPck = aVStructH.at(aIdIm1).VElPackHomol.at(aIdIm2);
                    aPck.StdPutInFile(aHmOut);
                }
            }
        }

    }

    std::cout << "Finished writing Homol files ! \n";

    // convert Homol folder into new format
    std::string aComConvFH = MM3dBinFile("TestLib ConvNewFH")
                           + aImgs
                           + " All SH=_"
                           + aSHOut
                           + " ExportBoth=1";
    system_call(aComConvFH.c_str());
    std::cout << aComConvFH << endl;

    return EXIT_SUCCESS;
}

int GenerateOrient_main (int argc, char ** argv)
{
    std::string aPatImgs, aOri, aSHOut{"Modif_orient.txt"}, aDir, aImgs,aOut{"Modif_orient.txt"};
    Pt2dr aTInterv, aGauss;
    int aSeed;
    double aSeuil{0.01};
    ElInitArgMain
            (
                argc, argv,
                LArgMain() << EAMC(aPatImgs,"Image Pattern, make sure images are listed in the right order",eSAM_IsExistFile)
                           << EAMC(aOri, "Ori",  eSAM_IsExistDirOri)
                           << EAMC(aTInterv, "Time Interval, interpolate to generate translation, [cadence (s), exposure time (ms)]")
                           << EAMC(aGauss,"Gaussian distribution parameters for rotation angle generation (radian), [mean,std]"),
                LArgMain() << EAM(aOut,"Out",true,"Output file name for genarated orientation, def=Modif_orient.txt")
                           << EAM(aSeed,"Seed",false,"Random engine, if not give, computer unix time is used.")
                           << EAM(aSeuil,"Threshold",true,"Threshold of the cross product T(i)^T(i-1) to omit the generated translation and use the precedent value,def=0.01")
                );

    // get directory
    SplitDirAndFile(aDir,aImgs,aPatImgs);
    StdCorrecNameOrient(aOri, aDir);
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(aDir);
    const std::vector<std::string> aVImgs = *(aICNM->Get(aImgs));


    int aNbIm = aVImgs.size();
    double aRatio = aTInterv.y/1000/aTInterv.x;
    std::cout << aSeuil << endl;

    std::vector<Pt3dr> aVTrans;
    // generation of translation, interpolation
    for(int i=0; i<aNbIm-1; i++)
    {
        CamStenope * aCam0 = aICNM->StdCamStenOfNames(aVImgs[i],aOri);
        Pt3dr aP0 = aCam0->PseudoOpticalCenter();

        CamStenope * aCam1 = aICNM->StdCamStenOfNames(aVImgs[i+1],aOri);
        Pt3dr aP1 = aCam1->PseudoOpticalCenter();

        Pt3dr aP = (aP1-aP0) * aRatio;
        //Pt3dr aP_Last = i==0 ? aP : aVTrans.back();

        if(i)
        {
            Pt3dr aPb = aVTrans.back();
            Pt3dr aProdVect = Pt3dr(
                        aP.y*aPb.z-aP.z*aPb.y,
                        aP.z*aPb.x-aP.x*aPb.z,
                        aP.x*aPb.y-aP.y*aPb.x
                        );
            double aNormPV = euclid(aProdVect);

            if(aNormPV > aSeuil)
            {
                aP = aPb;
                std::cout << "Reset Translation for " << aVImgs[i] << " : T(i)^T(i-1) = " << aNormPV << " > " << aSeuil << endl;
            }

        }

        aVTrans.push_back(aP);
        if(i==(aNbIm-2)) aVTrans.push_back((aP));
    }
    //std::cout << aVTrans.size() << endl;

    // generation of rotation, axis: uniform distribution, angle: gaussian distribution
    if(!EAMIsInit(&aSeed))  aSeed=time(0);
    std::default_random_engine generator(aSeed);
    std::normal_distribution<double> angle(aGauss.x,aGauss.y);
    std::uniform_real_distribution<double> axis(-1.0,1.0);

    std::vector<ElMatrix<double>> aVRot;
    for(int i=0; i<aNbIm;i++)
    {
        Pt3dr aU = Pt3dr(axis(generator),axis(generator),axis(generator)); // axis of rotation
        double aNorm = euclid(aU);
        aU = aU / aNorm;

        double aTeta = angle(generator);

        Pt3dr aCol1 = Pt3dr(
                            cos(aTeta) + aU.x*aU.x*(1-cos(aTeta)),
                            aU.y*aU.x*(1-cos(aTeta)) + aU.z*sin(aTeta),
                            aU.z*aU.x*(1-cos(aTeta)) - aU.y*sin(aTeta)
                           );
        Pt3dr aCol2 = Pt3dr(
                            aU.x*aU.y*(1-cos(aTeta)) - aU.z*sin(aTeta),
                            cos(aTeta) + aU.y*aU.y*(1-cos(aTeta)),
                            aU.z*aU.y*(1-cos(aTeta)) + aU.x*sin(aTeta)
                           );
        Pt3dr aCol3 = Pt3dr(
                            aU.x*aU.z*(1-cos(aTeta)) + aU.y*sin(aTeta),
                            aU.y*aU.z*(1-cos(aTeta)) + aU.x*sin(aTeta),
                            cos(aTeta) + aU.z*aU.z*(1-cos(aTeta))
                           );

        ElMatrix<double> aRot = MatFromCol(aCol1,aCol2,aCol3);
        aVRot.push_back(aRot);
    }
    //std::cout << aVRot.size() << endl;

    ELISE_ASSERT(aVTrans.size()==aVRot.size(),"Different size for translation and rotation!");
    ofstream aFile;
    aFile.open (aOut);

    aFile << "Random engine :" << aSeed << endl;
    aFile << "#F=Tx_Ty_Tz_R00_R01_R02_R10_R11_R12_R20_R21_R22" << endl;

    for(int i=0; i<aNbIm; i++)
    {
        aFile << aVTrans.at(i).x << " "
              << aVTrans.at(i).y << " "
              << aVTrans.at(i).z << " "
              << aVRot[i](0,0) << " "
              << aVRot[i](0,1) << " "
              << aVRot[i](0,2) << " "
              << aVRot[i](1,0) << " "
              << aVRot[i](1,1) << " "
              << aVRot[i](1,2) << " "
              << aVRot[i](2,0) << " "
              << aVRot[i](2,1) << " "
              << aVRot[i](2,2) << endl;
    }

    aFile.close();

    std::cout << aOut << endl;

    return EXIT_SUCCESS;
}

/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant a  la mise en
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
associés au chargement,  a  l'utilisation,  a  la modification et/ou au
développement et a  la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe a
manipuler et qui le réserve donc a  des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités a  charger  et  tester  l'adéquation  du
logiciel a  leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
a  l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder a cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
