#include "../../uti_phgrm/NewOri/NewOri.h"
#include "Triangle.h"
#include "Pic.h"

PlyProperty face_props[2] =
{
 {"vertex_indices", PLY_INT_32, PLY_INT_32, offsetof(Face,verts),1, PLY_UINT_8, PLY_UINT_8, offsetof(Face,nverts)},
 {"vertex_index", PLY_INT_32, PLY_INT_32, offsetof(Face,verts),1, PLY_UINT_8, PLY_UINT_8, offsetof(Face,nverts)},
};


/*=====get pixel correspondant by Affine Transformation===*/
Pt2dr ApplyAffine (Pt2dr &Pts_Origin, matAffine &matrixAffine)
{
    Pt2dr result;
    result.x = Pts_Origin.x*matrixAffine.el_00 + Pts_Origin.y*matrixAffine.el_01 + matrixAffine.el_02;
    result.y = Pts_Origin.x*matrixAffine.el_10 + Pts_Origin.y*matrixAffine.el_11 + matrixAffine.el_12;
    return result;
}

/*======Constructor class TRIANGLE ======
 * 1. Besoint tout les face et vertex du mesh
 * 2. num_pic: nombre d'image. pour malloc memoire de stocker
 * tout les reprojection un triangle sur des images
 *
*/
triangle::triangle(Face* aFace, vector<Vertex*> VertexList, int num_pic, int index)
{
    mSommet[0].x = VertexList[aFace->verts[0]]->x;
    mSommet[0].y = VertexList[aFace->verts[0]]->y;
    mSommet[0].z = VertexList[aFace->verts[0]]->z;
    mPtrSommetV[0] = VertexList[aFace->verts[0]];

    mSommet[1].x = VertexList[aFace->verts[1]]->x;
    mSommet[1].y = VertexList[aFace->verts[1]]->y;
    mSommet[1].z = VertexList[aFace->verts[1]]->z;
    mPtrSommetV[1] = VertexList[aFace->verts[1]];

    mSommet[2].x = VertexList[aFace->verts[2]]->x;
    mSommet[2].y = VertexList[aFace->verts[2]]->y;
    mSommet[2].z = VertexList[aFace->verts[2]]->z;
    mPtrSommetV[2] = VertexList[aFace->verts[2]];
    Pt2dr a[3];
    mResultTriReprSurAllImg = (Tri2d**) malloc (sizeof(a) * num_pic);
    mIndex = index;
}

/*=====Reproject triangle 3D vers une image (pic)========
 * Tri2D: array 3 position Pt2dr stocker 3 sommet du triangle
 * 1. Reproject 3 sommet 3D sur image, en utilisant Orientation
 * 2. Control si tout les 3 sommet reprojecte se situent dans Image
 * 3. Sauvgarder le résultat de reporojection dans l'odre avec ind = index d'image entrain de reproject

 * reprOK = result de control triangle reprojecte est dans image
 * result = Tri2D contient triangle reprojected
 * ind = index du image pour indiquer la position de memoire pour sauvgarder, -1 pour ne pas sauvgarde
*/
void triangle::reproject(pic *aPic, bool &reprOK, Tri2d &result, int ind=-1)
{
    Pt2dr result1[3];
    Pt2dr Som0Repro = aPic->mOriPic->ElCamera::R3toF2(mSommet[0]);
    bool inside = aPic->checkInSide(Som0Repro);
    Pt2dr Som1Repro = aPic->mOriPic->ElCamera::R3toF2(mSommet[1]);
    bool inside2 = aPic->checkInSide(Som1Repro);
    Pt2dr Som2Repro = aPic->mOriPic->ElCamera::R3toF2(mSommet[2]);
    bool inside3 = aPic->checkInSide(Som2Repro);
    if (inside && inside2 && inside3)
        {reprOK = true; }
    else
        {reprOK = false;}
    result1[0] = Som0Repro;
    result1[1] = Som1Repro;
    result1[2] = Som2Repro;
    if (ind != -1)
    {
        mResultTriReprSurAllImg[ind] = (Tri2d*) malloc(sizeof(Tri2d));
        mResultTriReprSurAllImg[ind]->sommet1[0].x = result1[0].x;
        mResultTriReprSurAllImg[ind]->sommet1[0].y = result1[0].y;
        mResultTriReprSurAllImg[ind]->sommet1[1].x = result1[1].x;
        mResultTriReprSurAllImg[ind]->sommet1[1].y = result1[1].y;
        mResultTriReprSurAllImg[ind]->sommet1[2].x = result1[2].x;
        mResultTriReprSurAllImg[ind]->sommet1[2].y = result1[2].y;
        mResultTriReprSurAllImg[ind]->insidePic = reprOK;
    }
    result.sommet1[0] = result1[0];
    result.sommet1[1] = result1[1];
    result.sommet1[2] = result1[2];
    result.insidePic = reprOK;
}

/*=====Calcule affine transformation b/w 2 triangle 2D on 2 image========
*/
matAffine triangle::CalAffine(pic* pic1 , pic* pic2)
{
    matAffine result;
    //cout<<"  Affine :"<<triangle1[0]<<triangle1[1]<<triangle1[2]<<triangle2[0]<<triangle2[1]<<triangle2[2]<<endl;
    //check if there is pts (0,0,0)
    Tri2d triangle1 = *this->getReprSurImg()[pic1->mIndex];
    Tri2d triangle2 = *this->getReprSurImg()[pic2->mIndex];
    bool triInImg = true;
    if(triangle1.insidePic && triangle2.insidePic)
        triInImg = false;
    double* aData1 = NULL;
    if (triInImg)
    {
        L2SysSurResol aSys1(6);
        double aEq1[6] = {triangle1.sommet1[0].x, triangle1.sommet1[0].y, 1, 0, 0, 0};
        double aEq2[6] = {0, 0, 0, triangle1.sommet1[0].x, triangle1.sommet1[0].y, 1};
        double aEq3[6] = {triangle1.sommet1[1].x, triangle1.sommet1[1].y, 1, 0, 0, 0};
        double aEq4[6] = {0, 0, 0, triangle1.sommet1[1].x, triangle1.sommet1[1].y, 1};
        double aEq5[6] = {triangle1.sommet1[2].x, triangle1.sommet1[2].y, 1, 0, 0, 0};
        double aEq6[6] = {0, 0, 0, triangle1.sommet1[2].x, triangle1.sommet1[2].y, 1};

        double aC1 = triangle2.sommet1[0].x; double aC2 = triangle2.sommet1[0].y;
        double aC3 = triangle2.sommet1[1].x; double aC4 = triangle2.sommet1[1].y;
        double aC5 = triangle2.sommet1[2].x; double aC6 = triangle2.sommet1[2].y;

        aSys1.AddEquation(1, aEq1, aC1);
        aSys1.AddEquation(1, aEq2, aC2);
        aSys1.AddEquation(1, aEq3, aC3);
        aSys1.AddEquation(1, aEq4, aC4);
        aSys1.AddEquation(1, aEq5, aC5);
        aSys1.AddEquation(1, aEq6, aC6);

        bool ok;
        Im1D_REAL8 aResol1 = aSys1.GSSR_Solve(&ok);
        aData1 = aResol1.data();
        result.el_00 = aData1[0];
        result.el_01 = aData1[1];
        result.el_02 = aData1[2];
        result.el_10 = aData1[3];
        result.el_11 = aData1[4];
        result.el_12 = aData1[5];
    }
    delete aData1;
    return result;
}

/*=======Creer un imagette autour la triangle 2D - taille flexible======
 *  1. chercher la cote plus long du triangle => longeur de la fenetre imagette
 *  2. calcul centre imagette = centre-geo du triangle
 *  3. Former une imagette (cCorrelImage) avec centre-geo et longeur de la fenetre
 *  4. Control si imagette est dans image
 *  5. return imagette (cCorrelImage) ou NULL si imagette hors image
*/
cCorrelImage* triangle::create_Imagette_autour_triangle_i(pic * PicMaitre,
                                                        Tri2d &triangle,
                                                        Pt2dr &centre_geo,
                                                        bool & ok_in,
                                                        Pt2dr &PtOrigin)
{
    ok_in = true;
    /*creat a rectangle autour triangle on master picture*/
    double coteLong;
    double cote1 = sqrt(pow((triangle.sommet1[0].x-triangle.sommet1[1].x),2) + pow((triangle.sommet1[0].y-triangle.sommet1[1].y),2));
    double cote2 = sqrt(pow((triangle.sommet1[0].x-triangle.sommet1[2].x),2) + pow((triangle.sommet1[0].y-triangle.sommet1[2].y),2));
    double cote3 = sqrt(pow((triangle.sommet1[1].x-triangle.sommet1[2].x),2) + pow((triangle.sommet1[1].y-triangle.sommet1[2].y),2));
    coteLong=cote1;
    if (cote2 > coteLong)
        {coteLong = cote2;}
    if (cote3 > coteLong)
        {coteLong = cote3;}

    //former un rectangle autour
    int szWindows;
    centre_geo.x = (triangle.sommet1[0].x+triangle.sommet1[1].x+triangle.sommet1[2].x)/3;
    centre_geo.y = (triangle.sommet1[0].y+triangle.sommet1[1].y+triangle.sommet1[2].y)/3;
    szWindows = coteLong/2;
    cCorrelImage::setSzW(szWindows);
    cCorrelImage *ImgetteMaitre = new cCorrelImage();
    //check if rectangle is inside img
    ok_in = ok_in && PicMaitre->checkInSide(Pt2dr(centre_geo.x-szWindows, centre_geo.y-szWindows))
    && PicMaitre->checkInSide(Pt2dr(centre_geo.x-szWindows, centre_geo.y+szWindows))
    && PicMaitre->checkInSide(Pt2dr(centre_geo.x+szWindows, centre_geo.y-szWindows))
    && PicMaitre->checkInSide(Pt2dr(centre_geo.x+szWindows, centre_geo.y+szWindows));
    //creat imagette autour triangle (imagette avec centre_geo et szWindows)
    if (ok_in)
    {
        /*
        Tiff_Im PicMaitre_Tiff( Tiff_Im::StdConvGen(PicMaitre->aICNM_NameChantier->Dir()+*PicMaitre->nameImg,1,false));  //read Tiff header

        TIm2D<U_INT1,INT4> PicMaitre_TIm2D(PicMaitre_Tiff.sz());                              //convert to TIm2D
        ELISE_COPY(PicMaitre_TIm2D.all_pts(),PicMaitre_Tiff.in(),PicMaitre_TIm2D.out());

        Im2D<U_INT1,INT4> PicMaitre_Im2D(PicMaitre_Tiff.sz().x,PicMaitre_Tiff.sz().y);        //convert to Im2D
        ELISE_COPY( PicMaitre_Tiff.all_pts(),PicMaitre_Tiff.in(), PicMaitre_Im2D.out() );
        */
        ImgetteMaitre->getFromIm(PicMaitre->mPic_Im2D, int(centre_geo.x), int(centre_geo.y));
    }
    else
        ImgetteMaitre = NULL;
    PtOrigin = Pt2dr (centre_geo.x-szWindows, centre_geo.y-szWindows);
    return ImgetteMaitre;
}

ImgetOfTri triangle::create_Imagette_autour_triangle (pic* aPic)
{
    bool ok_in = true;
    ImgetOfTri aImagette;
    /*creat a rectangle autour triangle on master picture*/
    Tri2d triangle = *this->getReprSurImg()[aPic->mIndex];
    aImagette.aTri = triangle;
    double coteLong;
    double cote1 = sqrt(pow((triangle.sommet1[0].x-triangle.sommet1[1].x),2) + pow((triangle.sommet1[0].y-triangle.sommet1[1].y),2));
    double cote2 = sqrt(pow((triangle.sommet1[0].x-triangle.sommet1[2].x),2) + pow((triangle.sommet1[0].y-triangle.sommet1[2].y),2));
    double cote3 = sqrt(pow((triangle.sommet1[1].x-triangle.sommet1[2].x),2) + pow((triangle.sommet1[1].y-triangle.sommet1[2].y),2));
    coteLong=cote1;
    if (cote2 > coteLong)
        {coteLong = cote2;}
    if (cote3 > coteLong)
        {coteLong = cote3;}

    //former un rectangle autour
    int szWindows;
    Pt2dr centre_geo;
    centre_geo.x = (triangle.sommet1[0].x+triangle.sommet1[1].x+triangle.sommet1[2].x)/3;
    centre_geo.y = (triangle.sommet1[0].y+triangle.sommet1[1].y+triangle.sommet1[2].y)/3;
    aImagette.centre_geo = centre_geo;
    szWindows = coteLong/2;
    aImagette.szW = szWindows;
    cCorrelImage::setSzW(szWindows);
    cCorrelImage *ImgetteMaitre = new cCorrelImage();
    //check if rectangle is inside img
    ok_in = ok_in && aPic->checkInSide(Pt2dr(centre_geo.x-szWindows, centre_geo.y-szWindows))
    && aPic->checkInSide(Pt2dr(centre_geo.x-szWindows, centre_geo.y+szWindows))
    && aPic->checkInSide(Pt2dr(centre_geo.x+szWindows, centre_geo.y-szWindows))
    && aPic->checkInSide(Pt2dr(centre_geo.x+szWindows, centre_geo.y+szWindows));
    //creat imagette autour triangle (imagette avec centre_geo et szWindows)
    if (ok_in)
    {
        /*
        Tiff_Im PicMaitre_Tiff( Tiff_Im::StdConvGen(PicMaitre->aICNM_NameChantier->Dir()+*PicMaitre->nameImg,1,false));  //read Tiff header

        TIm2D<U_INT1,INT4> PicMaitre_TIm2D(PicMaitre_Tiff.sz());                              //convert to TIm2D
        ELISE_COPY(PicMaitre_TIm2D.all_pts(),PicMaitre_Tiff.in(),PicMaitre_TIm2D.out());

        Im2D<U_INT1,INT4> PicMaitre_Im2D(PicMaitre_Tiff.sz().x,PicMaitre_Tiff.sz().y);        //convert to Im2D
        ELISE_COPY( PicMaitre_Tiff.all_pts(),PicMaitre_Tiff.in(), PicMaitre_Im2D.out() );
        */
        ImgetteMaitre->getFromIm(aPic->mPic_Im2D, int(centre_geo.x), int(centre_geo.y));
    }
    else
        ImgetteMaitre = NULL;
    Pt2dr PtOrigin = Pt2dr (centre_geo.x-szWindows, centre_geo.y-szWindows);
    aImagette.ptOriginImaget = PtOrigin;
    aImagette.imaget = ImgetteMaitre;
    return aImagette;
}


/*=======Creer un imagette de la triangle - taille fix======
 * 1. Taille fenetre fixer à 11*11 (szWindows = 5)
 * 2. Calcul centre-geo d'image
 * taille sub-pixel correlation = 3*3
*/
cCorrelImage* triangle::create_Imagette_adapt_triangle(pic * PicMaitre,
                                                       Tri2d &triangle,
                                                       Pt2dr &centre_geo,
                                                       bool & ok_in,
                                                       Pt2dr &PtOrigin)
{
    ok_in = true;
    //former un rectangle 11*11 autour centre-geo triangle
    int szWindows;
    centre_geo.x = (triangle.sommet1[0].x+triangle.sommet1[1].x+triangle.sommet1[2].x)/3;
    centre_geo.y = (triangle.sommet1[0].y+triangle.sommet1[1].y+triangle.sommet1[2].y)/3;
    szWindows = 5;
    cCorrelImage::setSzW(szWindows);
    cCorrelImage *ImgetteMaitre = new cCorrelImage();
    //check if rectangle is inside img
    ok_in = ok_in && PicMaitre->checkInSide(Pt2dr(centre_geo.x-szWindows, centre_geo.y-szWindows))
    && PicMaitre->checkInSide(Pt2dr(centre_geo.x-szWindows, centre_geo.y+szWindows))
    && PicMaitre->checkInSide(Pt2dr(centre_geo.x+szWindows, centre_geo.y-szWindows))
    && PicMaitre->checkInSide(Pt2dr(centre_geo.x+szWindows, centre_geo.y+szWindows));
    //creat imagette autour triangle (imagette avec centre_geo et szWindows)
    if (ok_in)
    {
        ImgetteMaitre->getFromIm(PicMaitre->mPic_Im2D, int(centre_geo.x), int(centre_geo.y));
    }
    PtOrigin = Pt2dr (centre_geo.x-szWindows, centre_geo.y-szWindows);
    return ImgetteMaitre;
}

/*=====Calcul Vector Normal de la triangle 3D sur mesh======
 * 1. Vector nomal tracer de la centre-geo 3D
 * 2. Vector Normal = crossProduct( Vector(sommet1,centre-geo) , Vector(sommet2,centre-geo) )
 * 3. Direction du vector normal ?
 * 4. Peut tracer et verifier sur mesh :-) tres bon idee
*/
Pt3dr triangle::CalVecNormal(Pt3dr & returnPtOrg)
{
    Pt3dr vecNormal;
    Pt3dr centre_geo = Pt3dr (
                                (mSommet[0].x + mSommet[1].x + mSommet[2].x)/3,
                                (mSommet[0].y + mSommet[1].y + mSommet[2].y)/3,
                                (mSommet[0].z + mSommet[1].z + mSommet[2].z)/3
                             );
    //creat 2 vector on plane, origin is centre_geo
    Pt3dr Vec1 = mSommet[1] - centre_geo;
    Pt3dr Vec2 = mSommet[2] - centre_geo;
    //cross product Vec1xVec2 give VecNormal (direction of vecNormal ?)
    vecNormal = Pt3dr   ( Vec1.y*Vec2.z - Vec1.z*Vec2.y,
                          Vec1.z*Vec2.x - Vec1.x*Vec2.z,
                          Vec1.x*Vec2.y - Vec1.y*Vec2.x  );
    returnPtOrg=centre_geo;
    return vecNormal;
}

/*=====Chercher image maitraisse et image viewable======
 * 1. PtrListPic = list d'image pour chercher
 * 2. VecNormal = result de calcul vector normal du triangle
 * 3. PtsOrg = centre-geo du triangle 3D
 * 4. ptrListPicViewable = list d'image peut voir la triangle
 * 5. assum1er = assume que 1er image dans PtrListPic est pic plus procher et les autre sont viewable
 *
 * 1. calcul vecNormal de chaque triangle
 * 2. Cal angle entre 2 vector (VecNormal, Vec(centre-geo, centre-optique de la camera))
 * 3. Si angle < PI/2 (a fixer) => pic est viewable
 * 4. ? direction de la vec-normal
*/
pic* triangle::getPicPlusProche(vector<pic*>PtrListPic,
                                Pt3dr & VecNormal,
                                Pt3dr & PtsOrg,
                                vector<pic*>&ptrListPicViewable,
                                bool assum1er)
{
    if (!assum1er)
    {
        VecNormal=this->CalVecNormal(PtsOrg);
        double min_angle = 0;
        pic * picMaitraiss = NULL;
        for (uint i=0; i<PtrListPic.size(); i++)
        {
            double angle = abs(this->calAngle
                                (PtrListPic[i]->mOriPic->VraiOpticalCenter(),
                                VecNormal,
                                PtsOrg));   //ERROR ? abs of angle ?
            if(i == 0)
            {
                min_angle = angle;
                picMaitraiss = PtrListPic[i];
                if (angle<=PI/2)
                    {ptrListPicViewable.push_back(PtrListPic[i]);}
            }
            else
            {
                if (angle<=PI/2)
                {
                    ptrListPicViewable.push_back(PtrListPic[i]);
                    if (angle<min_angle)
                    {
                        min_angle = angle;
                        picMaitraiss = PtrListPic[i];
                    }
                }
            }
        }
        if (min_angle >= PI/2)
            {picMaitraiss = NULL;cout<<"== No Pic Mai =="<<min_angle/PI*180<<"° ";}
        else
            { cout<<"== Pic Mai = "<<*picMaitraiss->mNameImg<<"=== View = "<<min_angle/PI*180<<"°"; }
        return picMaitraiss;
    }
    else
    {
        ptrListPicViewable = PtrListPic;
        return PtrListPic[0];
    }
}

/*=====Calcul Angle entre 2 vector======
*/
double triangle::calAngle(Pt3dr Vec1, Pt3dr Vec2, Pt3dr PtsOrg)
{
    double angle;
    Pt3dr Vec1Org = Vec1-PtsOrg;
    Pt3dr Vec2Org = Vec2-PtsOrg;
    angle = acos(
                (Vec1Org.x*Vec2Org.x + Vec1Org.y*Vec2Org.y)/
                (sqrt(pow(Vec1Org.x,2) + pow(Vec1Org.y,2) + pow(Vec1Org.z,2))*sqrt(pow(Vec2Org.x,2) + pow(Vec2Org.y,2) + pow(Vec2Org.z,2)))
                );
    return angle;
}

/*=====Prendre imagette sur 2eme image grace a imagette maitre et Affine transform======
 * TriReprMaitre :
 * matrixAffine : Affine transformation entre 2 imagette
 * centre_geo_master : centre-geo de la triangle sur image master
 * getImgetteFalse : output - false si il y a un pixel hors image 2eme
 *
 * Pour chaque pixel d'imagett maitre => affine => pixel correspondant sur image 2eme
 * Si pixel hors image 2eme => return NULL
*/
cCorrelImage* triangle::get_Imagette_by_affine(Tri2d &TriReprMaitre,
                                               cCorrelImage * Imgette_Maitre,
                                               pic* Img2nd,
                                               matAffine &matrixAffine,
                                               Pt2dr &centre_geo_master,
                                               bool & getImgetteFalse)
{
    double SzW = ((Imgette_Maitre->getmSz().x-1)/2);
    //Pour imagette de triangle dans image master, get imagette correspondance dans Img2nd par affine Transforme
    TIm2D<U_INT1,INT4> Imgette_2nd(Pt2di(SzW*2+1, SzW*2+1));
    for (int i=-SzW; i<=SzW; i++)
    {
        getImgetteFalse=false;
        for (int j=-SzW; j<=SzW; j++)
        {
            Pt2di aVois(i,j);
            Pt2dr pix_ImgMaitre(centre_geo_master.x+i, centre_geo_master.y+j);
            Pt2dr pix_Img2nd = ApplyAffine(pix_ImgMaitre, matrixAffine);
            if (Img2nd->checkInSide(pix_Img2nd))
            {
                //INT4 val = Pic2nd_TIm2D.getr(pix_Img2nd, -1);
                INT4 val = Img2nd->mPic_TIm2D->getr(pix_Img2nd, -1);
                Imgette_2nd.oset_svp(aVois+Pt2di(SzW,SzW),val);
            }
            else
                {getImgetteFalse=true; break; }
        }
        if (getImgetteFalse)
            {break;}
    }
    cCorrelImage::setSzW(SzW);
    cCorrelImage * result = new cCorrelImage();
    result->getWholeIm(&Imgette_2nd._the_im);  //it will do ELISECOPY Imgette_2ndT to object result
    return result;
}

ImgetOfTri triangle::get_Imagette_by_affine_n( ImgetOfTri &ImgetMaitre,
                                               pic* Img2nd,
                                               matAffine &matrixAffine,
                                               bool & getImgetteFalse
                                             )
{
    ImgetOfTri imget2nd;
    Pt2dr centre_geo_master = ImgetMaitre.centre_geo;
    double SzW = ((ImgetMaitre.imaget->getmSz().x-1)/2);
    imget2nd.szW = SzW;
    //Pour imagette de triangle dans image master, get imagette correspondance dans Img2nd par affine Transforme
    TIm2D<U_INT1,INT4> Imgette_2nd(Pt2di(SzW*2+1, SzW*2+1));
    for (int i=-SzW; i<=SzW; i++)
    {
        getImgetteFalse=false;
        for (int j=-SzW; j<=SzW; j++)
        {
            Pt2di aVois(i,j);
            Pt2dr pix_ImgMaitre(centre_geo_master.x+i, centre_geo_master.y+j);
            Pt2dr pix_Img2nd = ApplyAffine(pix_ImgMaitre, matrixAffine);
            if (Img2nd->checkInSide(pix_Img2nd))
            {
                //INT4 val = Pic2nd_TIm2D.getr(pix_Img2nd, -1);
                INT4 val = Img2nd->mPic_TIm2D->getr(pix_Img2nd, -1);
                Imgette_2nd.oset_svp(aVois+Pt2di(SzW,SzW),val);
            }
            else
                {getImgetteFalse=true; break; }
        }
        if (getImgetteFalse)
            {break;}
    }
    imget2nd.aPic = Img2nd;
    imget2nd.aTri = *this->getReprSurImg()[Img2nd->mIndex];
    cCorrelImage::setSzW(SzW);
    cCorrelImage * result = new cCorrelImage();
    result->getWholeIm(&Imgette_2nd._the_im);  //it will do ELISECOPY Imgette_2ndT to object result
    imget2nd.imaget = result;
    imget2nd.centre_geo = ApplyAffine(ImgetMaitre.centre_geo, matrixAffine);
    imget2nd.ptOriginImaget = ApplyAffine(ImgetMaitre.ptOriginImaget, matrixAffine);
    return imget2nd;
}

/*=====Calcul coor 3D d'un point dans la triangle 2D======
 * pts2DinTri : pts 2D dans la triangle
 * img : image qu'on va utiliser pour calcul coor 3D
 *
 * point2D(x,y) = point dans la triangle 2D
 * point3D(X,Y,Z) = point dans la triangle 3D
 * En 2D:
 * vec(point2D, som0) = alpha*vec(som1, som0) + beta*vec(som2,som0)
 *      => 2 equation, 2 inconnu (alpha, beta)
 * En 3D:
 * vec(point3D, som0) = alpha*vec(som1, som0) + beta*vec(som2,som0)
 *      => on a (alpha, beta) => trouver point 3D
*/
vector<Pt3dr> triangle::ptsInTri2Dto3D(vector<Pt2dr> pts2DinTri, pic *img)
{
    vector<Pt3dr> result;
    if ((pts2DinTri.size() > 0) && (img!=NULL))
    {
        Pt3dr vec_I=this->getSommet(1) - this->getSommet(0);
        Pt3dr vec_J=this->getSommet(2) - this->getSommet(0);
        //get triange 2D i sur img j
        Tri2d * trionImg = this->getReprSurImg()[img->mIndex];
        Pt2dr vec_i = trionImg->sommet1[1] - trionImg->sommet1[0];
        Pt2dr vec_j = trionImg->sommet1[2] - trionImg->sommet1[0];
        for (uint i=0; i<pts2DinTri.size(); i++)
        {
            Pt2dr aP = pts2DinTri[i] - trionImg->sommet1[0];
            double alpha = (aP.x*vec_j.y - aP.y*vec_j.x)/(vec_i.x*vec_j.y-vec_j.x*vec_i.y);
            double beta = (aP.y-alpha*vec_i.y)/vec_j.y;
            Pt3dr pts3DInTri;
            pts3DInTri.x = alpha*vec_I.x + beta*vec_J.x;
            pts3DInTri.y = alpha*vec_I.y + beta*vec_J.y;
            pts3DInTri.z = alpha*vec_I.z + beta*vec_J.z;
            result.push_back(pts3DInTri + this->getSommet(0));
        }
    }
    return result;
}
/*=====Sauvgarder un pts dans la triangle======
 * aPoint : pts2D interet dans la triangle
 * img : image ou on se trouve la point et la triangle
 *
 * => on peut savoir: coor des pts d'interet dans ce triangle sur certain image
*/
void triangle::savePtInteret2D(Pt2dr aPoint, pic*img)
{
    PtInteretInTriangle aPts;
    aPts.aPoint = aPoint; aPts.imageContainPtAndTriangle = img;
    mPtsInteret2DInImagetteDuTri.push_back(aPts);
}

vector<Vertex> triangle::toTriangle(Face * aFace, vector<Vertex*> VertexList)
{
    vector<Vertex> result;
    Vertex aVertex;
    aVertex.x = VertexList[aFace->verts[0]]->x;
    aVertex.y = VertexList[aFace->verts[0]]->y;
    aVertex.z = VertexList[aFace->verts[0]]->z;
    result.push_back(aVertex);
    aVertex.x = VertexList[aFace->verts[1]]->x;
    aVertex.y = VertexList[aFace->verts[1]]->y;
    aVertex.z = VertexList[aFace->verts[1]]->z;
    result.push_back(aVertex);
    aVertex.x = VertexList[aFace->verts[2]]->x;
    aVertex.y = VertexList[aFace->verts[2]]->y;
    aVertex.z = VertexList[aFace->verts[2]]->z;
    result.push_back(aVertex);
    return result;
}


//==========determine if point is inside triangle 2D ========//
double triangle::det(Pt2dr u, Pt2dr v)
    {return (u.x*v.y-u.y*v.x);}
bool triangle::check_inside_triangle (Pt2dr v, Tri2d aTri2D)
{
    Pt2dr a = aTri2D.sommet1[0];
    Pt2dr b = aTri2D.sommet1[1];
    Pt2dr c = aTri2D.sommet1[2];
    bool inside = false;
    Pt2dr v2 = b-a;
    Pt2dr v1 = c-a;
    Pt2dr v0 = a;
    double alpha = (det(v,v2) - det(v0,v2))/det(v1,v2);
    double beta = (det(v,v1) - det(v0,v1))/det(v1,v2);
    if ( (alpha > 0) && (beta > 0) && ((alpha+beta)<1) )
        {inside = true;}
    return inside;
}
//=============================================================//
