#ifndef  _MMVII_Ptxd_H_
#define  _MMVII_Ptxd_H_
namespace MMVII
{


/** \file MMVII_Ptxd.h
    \brief Basic N-dimensionnal point facilities

   Don't know exactly where we go ...  Probably will contain :

      - point in N dim template
      - specialization to 1,2,3,4  dims
      - dynamic dyn
      - boxes on others "small" connected classes

*/


template <class Type,const int Dim> class cPtxd;
template <class Type> class cPt1d;
template <class Type> class cPt2d;


/*   Relation/Bijection entre cPtxd et cPt1d,cPt2d etc ....
   La conversion de fait dans les deux sens

      cPtxd<Type,1> => cPt1d via le constructeur
      cPt1d =>  cPtxd<Type,1>  implicite via l'heritage

      Les deux sont donc plus ou moins synomymes 
*/


///  template class for Points
/**
   This class allow to manipulate points indepently of their dimension,
   with no or few, waste of performance
*/

template <class Type,const int Dim> class cPtxd
{
    public :
       /// Maybe some function will require generic access to data
       Type * Data() {return mCoords;}

       // cPtxd(const cPt1d<Type>);
       // cPtxd(const cPt2d<Type>);
         // MMVII_INTERNAL_ASSERT(Dim==1);

       /// Some function requires default constructor (serialization ?)
       cPtxd() {}

       /// Contructor for 1 dim point, statically checked
       cPtxd(const Type & x) :  mCoords{x} {static_assert(Dim==1,"bad dim in cPtxd initializer");}
       /// Contructor for 2 dim point, statically checked
       cPtxd(const Type & x,const Type &y) :  mCoords{x,y} {static_assert(Dim==2,"bad dim in cPtxd initializer");}
       /// Contructor for 3 dim point, statically checked
       cPtxd(const Type & x,const Type &y,const Type &z) :  mCoords{x,y,z} {static_assert(Dim==3,"bad dim in cPtxd initializer");}


    protected :
       Type mCoords[Dim];
};

    ///  1 dimension specializatio,

template <class Type> class cPt1d : public cPtxd<Type,1>
{
     public :
        // typedef typename cPtxd<Type,1> tBase;
        typedef cPtxd<Type,1> tBase;

     // Constructeur
        // inline cPt1d(const Type& anX) {tBase::mCoords[0] = anX;}
        inline cPt1d(const Type& anX) : tBase(anX) {}
        inline cPt1d(const tBase & aP) {static_cast<tBase&>(*this) = aP;}

     // Accesseurs
        inline Type & x()             {return tBase::mCoords[0];}
        inline const Type & x() const {return tBase::mCoords[0];}
};

template <class Type> inline cPt1d<Type> operator + (const cPt1d<Type> & aP1,const cPt1d<Type> & aP2) 
{return  cPt1d<Type>(aP1.x()+aP2.x());}
template <class Type> inline cPt1d<Type> operator - (const cPt1d<Type> & aP1,const cPt1d<Type> & aP2) 
{return  cPt1d<Type>(aP1.x()-aP2.x());}
template <class Type> inline cPt1d<Type> operator * (const Type & aVal ,const cPt1d<Type> & aP) 
{return  cPt1d<Type>(aP.x()*aVal);}
template <class Type> inline cPt1d<Type> operator * (const cPt1d<Type> & aP,const Type & aVal) 
{return  cPt1d<Type>(aP.x()*aVal);}
template <class Type> inline cPt1d<Type> operator / (const cPt1d<Type> & aP,const Type & aVal) 
{return  cPt1d<Type>(aP.x()/aVal);}
template <class Type> inline bool operator == (const cPt1d<Type> & aP1,const cPt1d<Type> & aP2) {return  (aP1.x()==aP2.x());}
template <class Type> inline bool operator != (const cPt1d<Type> & aP1,const cPt1d<Type> & aP2) {return !(aP1==aP2);}


typedef cPt1d<double>  cPt1dr ;
typedef cPt1d<int>     cPt1di ;
typedef cPt1d<float>   cPt1df ;


    ///  2 dimension specializatio,

template <class Type> class cPt2d : public cPtxd<Type,2>
{
     public :
        typedef cPtxd<Type,2> tBase;

   // Constructeur
//         cPt2d(const Type& anX,const Type& anY) {tBase::mCoords[0] = anX;tBase::mCoords[1] = anY;}

        inline cPt2d(const Type& anX,const Type& anY) : tBase(anX,anY) {}
        inline cPt2d() : cPt2d(0,0) {}
        inline cPt2d(const tBase & aP) {static_cast<tBase&>(*this) = aP;}

   // Accesseurs
        Type & x()             {return tBase::mCoords[0];}
        const Type & x() const {return tBase::mCoords[0];}
        Type & y()             {return tBase::mCoords[1];}
        const Type & y() const {return tBase::mCoords[1];}
};


template <class Type> inline cPt2d<Type> operator + (const cPt2d<Type> & aP1,const cPt2d<Type> & aP2) 
{return  cPt2d<Type>(aP1.x()+aP2.x(),aP1.y()+aP2.y());}
template <class Type> inline cPt2d<Type> operator - (const cPt2d<Type> & aP1,const cPt2d<Type> & aP2) 
{return  cPt2d<Type>(aP1.x()-aP2.x(),aP1.y()-aP2.y());}
template <class Type> inline cPt2d<Type> operator * (const Type & aVal ,const cPt2d<Type> & aP) 
{return  cPt2d<Type>(aP.x()*aVal,aP.y()*aVal);}
template <class Type> inline cPt2d<Type> operator * (const cPt2d<Type> & aP,const Type & aVal ) 
{return  cPt2d<Type>(aP.x()*aVal,aP.y()*aVal);}
template <class Type> inline cPt2d<Type> operator / (const cPt2d<Type> & aP,const Type & aVal ) 
{return  cPt2d<Type>(aP.x()/aVal,aP.y()/aVal);}

template <class Type> inline bool operator == (const cPt2d<Type> & aP1,const cPt2d<Type> & aP2) 
{return  (aP1.x()==aP2.x())&&(aP1.y()==aP2.y());}
template <class Type> inline bool operator != (const cPt2d<Type> & aP1,const cPt2d<Type> & aP2)  {return !(aP1==aP2);}

typedef cPt2d<double>  cPt2dr ;
typedef cPt2d<int>     cPt2di ;
typedef cPt2d<float>   cPt2df ;

template <class Type> std::ostream & operator << (std::ostream & OS,const cPt2d<Type> &aP)
{
   return  OS << "[" << aP.x() << "," << aP.y() << "]";
}


template <class Type> class cPt3d : public cPtxd<Type,3>
{
     public :
        typedef cPtxd<Type,3> tBase;

   // Constructeur

        inline cPt3d(const Type& anX,const Type& anY,const Type& aZ) : tBase(anX,anY,aZ) {}
        inline cPt3d() : cPt3d(0,0,0) {}
        inline cPt3d(const tBase & aP) {static_cast<tBase&>(*this) = aP;}

   // Accesseurs
        Type & x()             {return tBase::mCoords[0];}
        const Type & x() const {return tBase::mCoords[0];}
        Type & y()             {return tBase::mCoords[1];}
        const Type & y() const {return tBase::mCoords[1];}
        Type & z()             {return tBase::mCoords[2];}
        const Type & z() const {return tBase::mCoords[2];}

};

template <class Type> inline cPt3d<Type> operator + (const cPt3d<Type> & aP1,const cPt3d<Type> & aP2) 
{return  cPt3d<Type>(aP1.x()+aP2.x(),aP1.y()+aP2.y(),aP1.z()+aP2.z());}
template <class Type> inline cPt3d<Type> operator - (const cPt3d<Type> & aP1,const cPt3d<Type> & aP2) 
{return  cPt3d<Type>(aP1.x()-aP2.x(),aP1.y()-aP2.y(),aP1.z()-aP2.z());}
template <class Type> inline cPt3d<Type> operator * (const Type & aVal ,const cPt3d<Type> & aP) 
{return  cPt3d<Type>(aP.x()*aVal,aP.y()*aVal,aP.z()*aVal);}
template <class Type> inline cPt3d<Type> operator * (const cPt3d<Type> & aP,const Type & aVal ) 
{return  cPt3d<Type>(aP.x()*aVal,aP.y()*aVal,aP.z()*aVal);}
template <class Type> inline cPt3d<Type> operator / (const cPt3d<Type> & aP,const Type & aVal ) 
{return  cPt3d<Type>(aP.x()/aVal,aP.y()/aVal,aP.z()/aVal);}

template <class Type> inline bool operator == (const cPt3d<Type> & aP1,const cPt3d<Type> & aP2) 
{return  (aP1.x()==aP2.x())&&(aP1.y()==aP2.y())&&(aP1.z()==aP2.z());}
template <class Type> inline bool operator != (const cPt3d<Type> & aP1,const cPt3d<Type> & aP2)  {return !(aP1==aP2);}

typedef cPt3d<double>  cPt3dr ;
typedef cPt3d<int>     cPt3di ;
typedef cPt3d<float>   cPt3df ;

template <class Type> std::ostream & operator << (std::ostream & OS,const cPt3d<Type> &aP)
{
   return  OS << "[" << aP.x() << "," << aP.y() << "," << aP.z()<< "]";
}


};

#endif  //  _MMVII_Ptxd_H_
