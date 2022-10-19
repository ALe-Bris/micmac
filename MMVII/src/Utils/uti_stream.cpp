#include "include/MMVII_all.h"

//#define DEBUGKILL_MMVIISTREAM()\
//std::cout << "MMVIISTREAM HERE " << __LINE__ << "\n";
#define DEBUGKILL_MMVIISTREAM()

namespace MMVII
{
/*=============================================*/
/*                                             */
/*      cMultipleOfs::                      */
/*                                             */
/*=============================================*/
cMultipleOfs::cMultipleOfs(std::ostream & aOfs) : mOfsCreated(nullptr)
{
    DEBUGKILL_MMVIISTREAM();
    Add(aOfs);
}
cMultipleOfs::cMultipleOfs(const std::string & aS, bool ModeAppend)
{
    mOfsCreated = nullptr;
    mOfsCreated = new cMMVII_Ofs(aS,ModeAppend);
    Add(mOfsCreated->Ofs());
}
cMultipleOfs::~cMultipleOfs()
{
    DEBUGKILL_MMVIISTREAM();
    if (mOfsCreated != nullptr) delete mOfsCreated;
    DEBUGKILL_MMVIISTREAM();
}

void cMultipleOfs::Add(std::ostream & aOfs)
{
    mVOfs.push_back(&aOfs);
}
void cMultipleOfs::Clear()
{
    mVOfs.clear();
}

};

