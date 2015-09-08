// File Automatically generated by eLiSe
#include "general/all.h"
#include "private/all.h"
#include "cEqAppui_NoDist__PTInc_M2CDRad5.h"


cEqAppui_NoDist__PTInc_M2CDRad5::cEqAppui_NoDist__PTInc_M2CDRad5():
    cElCompiledFonc(2)
{
   AddIntRef (cIncIntervale("Intr",0,10));
   AddIntRef (cIncIntervale("Orient",10,16));
   AddIntRef (cIncIntervale("Tmp_PTer",16,19));
   Close(false);
}



void cEqAppui_NoDist__PTInc_M2CDRad5::ComputeVal()
{
   double tmp0_ = mCompCoord[10];
   double tmp1_ = mCompCoord[11];
   double tmp2_ = cos(tmp1_);
   double tmp3_ = sin(tmp0_);
   double tmp4_ = cos(tmp0_);
   double tmp5_ = sin(tmp1_);
   double tmp6_ = mCompCoord[12];
   double tmp7_ = mCompCoord[16];
   double tmp8_ = mCompCoord[13];
   double tmp9_ = tmp7_-tmp8_;
   double tmp10_ = sin(tmp6_);
   double tmp11_ = -(tmp10_);
   double tmp12_ = -(tmp5_);
   double tmp13_ = cos(tmp6_);
   double tmp14_ = mCompCoord[17];
   double tmp15_ = mCompCoord[14];
   double tmp16_ = tmp14_-tmp15_;
   double tmp17_ = mCompCoord[18];
   double tmp18_ = mCompCoord[15];
   double tmp19_ = tmp17_-tmp18_;
   double tmp20_ = -(tmp3_);
   double tmp21_ = tmp4_*tmp12_;
   double tmp22_ = tmp3_*tmp12_;
   double tmp23_ = tmp20_*tmp11_;
   double tmp24_ = tmp21_*tmp13_;
   double tmp25_ = tmp23_+tmp24_;
   double tmp26_ = (tmp25_)*(tmp9_);
   double tmp27_ = tmp4_*tmp11_;
   double tmp28_ = tmp22_*tmp13_;
   double tmp29_ = tmp27_+tmp28_;
   double tmp30_ = (tmp29_)*(tmp16_);
   double tmp31_ = tmp26_+tmp30_;
   double tmp32_ = tmp2_*tmp13_;
   double tmp33_ = tmp32_*(tmp19_);
   double tmp34_ = tmp31_+tmp33_;
   double tmp35_ = tmp4_*tmp2_;
   double tmp36_ = tmp35_*(tmp9_);
   double tmp37_ = tmp3_*tmp2_;
   double tmp38_ = tmp37_*(tmp16_);
   double tmp39_ = tmp36_+tmp38_;
   double tmp40_ = tmp5_*(tmp19_);
   double tmp41_ = tmp39_+tmp40_;
   double tmp42_ = (tmp41_)/(tmp34_);
   double tmp43_ = tmp20_*tmp13_;
   double tmp44_ = tmp21_*tmp10_;
   double tmp45_ = tmp43_+tmp44_;
   double tmp46_ = (tmp45_)*(tmp9_);
   double tmp47_ = tmp4_*tmp13_;
   double tmp48_ = tmp22_*tmp10_;
   double tmp49_ = tmp47_+tmp48_;
   double tmp50_ = (tmp49_)*(tmp16_);
   double tmp51_ = tmp46_+tmp50_;
   double tmp52_ = tmp2_*tmp10_;
   double tmp53_ = tmp52_*(tmp19_);
   double tmp54_ = tmp51_+tmp53_;
   double tmp55_ = (tmp54_)/(tmp34_);

  mVal[0] = (mLocNDP0_x+mLocNDdx_x*(tmp42_)+mLocNDdy_x*(tmp55_))-mLocXIm;

  mVal[1] = (mLocNDP0_y+mLocNDdx_y*(tmp42_)+mLocNDdy_y*(tmp55_))-mLocYIm;

}


void cEqAppui_NoDist__PTInc_M2CDRad5::ComputeValDeriv()
{
   double tmp0_ = mCompCoord[10];
   double tmp1_ = mCompCoord[11];
   double tmp2_ = cos(tmp1_);
   double tmp3_ = sin(tmp0_);
   double tmp4_ = cos(tmp0_);
   double tmp5_ = sin(tmp1_);
   double tmp6_ = mCompCoord[12];
   double tmp7_ = mCompCoord[16];
   double tmp8_ = mCompCoord[13];
   double tmp9_ = tmp7_-tmp8_;
   double tmp10_ = sin(tmp6_);
   double tmp11_ = -(tmp10_);
   double tmp12_ = -(tmp5_);
   double tmp13_ = cos(tmp6_);
   double tmp14_ = mCompCoord[17];
   double tmp15_ = mCompCoord[14];
   double tmp16_ = tmp14_-tmp15_;
   double tmp17_ = mCompCoord[18];
   double tmp18_ = mCompCoord[15];
   double tmp19_ = tmp17_-tmp18_;
   double tmp20_ = -(tmp3_);
   double tmp21_ = tmp4_*tmp12_;
   double tmp22_ = tmp3_*tmp12_;
   double tmp23_ = tmp20_*tmp11_;
   double tmp24_ = tmp21_*tmp13_;
   double tmp25_ = tmp23_+tmp24_;
   double tmp26_ = (tmp25_)*(tmp9_);
   double tmp27_ = tmp4_*tmp11_;
   double tmp28_ = tmp22_*tmp13_;
   double tmp29_ = tmp27_+tmp28_;
   double tmp30_ = (tmp29_)*(tmp16_);
   double tmp31_ = tmp26_+tmp30_;
   double tmp32_ = tmp2_*tmp13_;
   double tmp33_ = tmp32_*(tmp19_);
   double tmp34_ = tmp31_+tmp33_;
   double tmp35_ = tmp4_*tmp2_;
   double tmp36_ = tmp35_*(tmp9_);
   double tmp37_ = tmp3_*tmp2_;
   double tmp38_ = tmp37_*(tmp16_);
   double tmp39_ = tmp36_+tmp38_;
   double tmp40_ = tmp5_*(tmp19_);
   double tmp41_ = tmp39_+tmp40_;
   double tmp42_ = -(1);
   double tmp43_ = tmp42_*tmp3_;
   double tmp44_ = -(tmp4_);
   double tmp45_ = tmp43_*tmp12_;
   double tmp46_ = tmp21_*tmp10_;
   double tmp47_ = tmp20_*tmp13_;
   double tmp48_ = tmp47_+tmp46_;
   double tmp49_ = (tmp48_)*(tmp9_);
   double tmp50_ = tmp4_*tmp13_;
   double tmp51_ = tmp22_*tmp10_;
   double tmp52_ = tmp50_+tmp51_;
   double tmp53_ = (tmp52_)*(tmp16_);
   double tmp54_ = tmp49_+tmp53_;
   double tmp55_ = tmp2_*tmp10_;
   double tmp56_ = tmp55_*(tmp19_);
   double tmp57_ = tmp54_+tmp56_;
   double tmp58_ = tmp44_*tmp11_;
   double tmp59_ = tmp45_*tmp13_;
   double tmp60_ = tmp58_+tmp59_;
   double tmp61_ = (tmp60_)*(tmp9_);
   double tmp62_ = tmp43_*tmp11_;
   double tmp63_ = tmp62_+tmp24_;
   double tmp64_ = (tmp63_)*(tmp16_);
   double tmp65_ = tmp61_+tmp64_;
   double tmp66_ = ElSquare(tmp34_);
   double tmp67_ = tmp42_*tmp5_;
   double tmp68_ = -(tmp2_);
   double tmp69_ = tmp68_*tmp4_;
   double tmp70_ = tmp68_*tmp3_;
   double tmp71_ = tmp69_*tmp13_;
   double tmp72_ = tmp71_*(tmp9_);
   double tmp73_ = tmp70_*tmp13_;
   double tmp74_ = tmp73_*(tmp16_);
   double tmp75_ = tmp72_+tmp74_;
   double tmp76_ = tmp67_*tmp13_;
   double tmp77_ = tmp76_*(tmp19_);
   double tmp78_ = tmp75_+tmp77_;
   double tmp79_ = -(tmp13_);
   double tmp80_ = tmp42_*tmp10_;
   double tmp81_ = tmp79_*tmp20_;
   double tmp82_ = tmp80_*tmp21_;
   double tmp83_ = tmp81_+tmp82_;
   double tmp84_ = (tmp83_)*(tmp9_);
   double tmp85_ = tmp79_*tmp4_;
   double tmp86_ = tmp80_*tmp22_;
   double tmp87_ = tmp85_+tmp86_;
   double tmp88_ = (tmp87_)*(tmp16_);
   double tmp89_ = tmp84_+tmp88_;
   double tmp90_ = tmp80_*tmp2_;
   double tmp91_ = tmp90_*(tmp19_);
   double tmp92_ = tmp89_+tmp91_;
   double tmp93_ = tmp42_*(tmp25_);
   double tmp94_ = tmp42_*(tmp29_);
   double tmp95_ = tmp42_*tmp32_;
   double tmp96_ = (tmp41_)/(tmp34_);
   double tmp97_ = (tmp57_)/(tmp34_);
   double tmp98_ = tmp43_*tmp2_;
   double tmp99_ = tmp98_*(tmp9_);
   double tmp100_ = tmp35_*(tmp16_);
   double tmp101_ = tmp99_+tmp100_;
   double tmp102_ = (tmp101_)*(tmp34_);
   double tmp103_ = (tmp41_)*(tmp65_);
   double tmp104_ = tmp102_-tmp103_;
   double tmp105_ = (tmp104_)/tmp66_;
   double tmp106_ = tmp44_*tmp13_;
   double tmp107_ = tmp45_*tmp10_;
   double tmp108_ = tmp106_+tmp107_;
   double tmp109_ = (tmp108_)*(tmp9_);
   double tmp110_ = tmp43_*tmp13_;
   double tmp111_ = tmp110_+tmp46_;
   double tmp112_ = (tmp111_)*(tmp16_);
   double tmp113_ = tmp109_+tmp112_;
   double tmp114_ = (tmp113_)*(tmp34_);
   double tmp115_ = (tmp57_)*(tmp65_);
   double tmp116_ = tmp114_-tmp115_;
   double tmp117_ = (tmp116_)/tmp66_;
   double tmp118_ = tmp67_*tmp4_;
   double tmp119_ = tmp118_*(tmp9_);
   double tmp120_ = tmp67_*tmp3_;
   double tmp121_ = tmp120_*(tmp16_);
   double tmp122_ = tmp119_+tmp121_;
   double tmp123_ = tmp2_*(tmp19_);
   double tmp124_ = tmp122_+tmp123_;
   double tmp125_ = (tmp124_)*(tmp34_);
   double tmp126_ = (tmp41_)*(tmp78_);
   double tmp127_ = tmp125_-tmp126_;
   double tmp128_ = (tmp127_)/tmp66_;
   double tmp129_ = tmp69_*tmp10_;
   double tmp130_ = tmp129_*(tmp9_);
   double tmp131_ = tmp70_*tmp10_;
   double tmp132_ = tmp131_*(tmp16_);
   double tmp133_ = tmp130_+tmp132_;
   double tmp134_ = tmp67_*tmp10_;
   double tmp135_ = tmp134_*(tmp19_);
   double tmp136_ = tmp133_+tmp135_;
   double tmp137_ = (tmp136_)*(tmp34_);
   double tmp138_ = (tmp57_)*(tmp78_);
   double tmp139_ = tmp137_-tmp138_;
   double tmp140_ = (tmp139_)/tmp66_;
   double tmp141_ = (tmp41_)*(tmp92_);
   double tmp142_ = -(tmp141_);
   double tmp143_ = tmp142_/tmp66_;
   double tmp144_ = tmp80_*tmp20_;
   double tmp145_ = tmp13_*tmp21_;
   double tmp146_ = tmp144_+tmp145_;
   double tmp147_ = (tmp146_)*(tmp9_);
   double tmp148_ = tmp80_*tmp4_;
   double tmp149_ = tmp13_*tmp22_;
   double tmp150_ = tmp148_+tmp149_;
   double tmp151_ = (tmp150_)*(tmp16_);
   double tmp152_ = tmp147_+tmp151_;
   double tmp153_ = tmp13_*tmp2_;
   double tmp154_ = tmp153_*(tmp19_);
   double tmp155_ = tmp152_+tmp154_;
   double tmp156_ = (tmp155_)*(tmp34_);
   double tmp157_ = (tmp57_)*(tmp92_);
   double tmp158_ = tmp156_-tmp157_;
   double tmp159_ = (tmp158_)/tmp66_;
   double tmp160_ = tmp42_*tmp35_;
   double tmp161_ = tmp160_*(tmp34_);
   double tmp162_ = (tmp41_)*tmp93_;
   double tmp163_ = tmp161_-tmp162_;
   double tmp164_ = (tmp163_)/tmp66_;
   double tmp165_ = tmp42_*(tmp48_);
   double tmp166_ = tmp165_*(tmp34_);
   double tmp167_ = (tmp57_)*tmp93_;
   double tmp168_ = tmp166_-tmp167_;
   double tmp169_ = (tmp168_)/tmp66_;
   double tmp170_ = tmp42_*tmp37_;
   double tmp171_ = tmp170_*(tmp34_);
   double tmp172_ = (tmp41_)*tmp94_;
   double tmp173_ = tmp171_-tmp172_;
   double tmp174_ = (tmp173_)/tmp66_;
   double tmp175_ = tmp42_*(tmp52_);
   double tmp176_ = tmp175_*(tmp34_);
   double tmp177_ = (tmp57_)*tmp94_;
   double tmp178_ = tmp176_-tmp177_;
   double tmp179_ = (tmp178_)/tmp66_;
   double tmp180_ = tmp67_*(tmp34_);
   double tmp181_ = (tmp41_)*tmp95_;
   double tmp182_ = tmp180_-tmp181_;
   double tmp183_ = (tmp182_)/tmp66_;
   double tmp184_ = tmp42_*tmp55_;
   double tmp185_ = tmp184_*(tmp34_);
   double tmp186_ = (tmp57_)*tmp95_;
   double tmp187_ = tmp185_-tmp186_;
   double tmp188_ = (tmp187_)/tmp66_;
   double tmp189_ = tmp35_*(tmp34_);
   double tmp190_ = (tmp41_)*(tmp25_);
   double tmp191_ = tmp189_-tmp190_;
   double tmp192_ = (tmp191_)/tmp66_;
   double tmp193_ = (tmp48_)*(tmp34_);
   double tmp194_ = (tmp57_)*(tmp25_);
   double tmp195_ = tmp193_-tmp194_;
   double tmp196_ = (tmp195_)/tmp66_;
   double tmp197_ = tmp37_*(tmp34_);
   double tmp198_ = (tmp41_)*(tmp29_);
   double tmp199_ = tmp197_-tmp198_;
   double tmp200_ = (tmp199_)/tmp66_;
   double tmp201_ = (tmp52_)*(tmp34_);
   double tmp202_ = (tmp57_)*(tmp29_);
   double tmp203_ = tmp201_-tmp202_;
   double tmp204_ = (tmp203_)/tmp66_;
   double tmp205_ = tmp5_*(tmp34_);
   double tmp206_ = (tmp41_)*tmp32_;
   double tmp207_ = tmp205_-tmp206_;
   double tmp208_ = (tmp207_)/tmp66_;
   double tmp209_ = tmp55_*(tmp34_);
   double tmp210_ = (tmp57_)*tmp32_;
   double tmp211_ = tmp209_-tmp210_;
   double tmp212_ = (tmp211_)/tmp66_;

  mVal[0] = (mLocNDP0_x+mLocNDdx_x*(tmp96_)+mLocNDdy_x*(tmp97_))-mLocXIm;

  mCompDer[0][0] = 0;
  mCompDer[0][1] = 0;
  mCompDer[0][2] = 0;
  mCompDer[0][3] = 0;
  mCompDer[0][4] = 0;
  mCompDer[0][5] = 0;
  mCompDer[0][6] = 0;
  mCompDer[0][7] = 0;
  mCompDer[0][8] = 0;
  mCompDer[0][9] = 0;
  mCompDer[0][10] = (tmp105_)*mLocNDdx_x+(tmp117_)*mLocNDdy_x;
  mCompDer[0][11] = (tmp128_)*mLocNDdx_x+(tmp140_)*mLocNDdy_x;
  mCompDer[0][12] = (tmp143_)*mLocNDdx_x+(tmp159_)*mLocNDdy_x;
  mCompDer[0][13] = (tmp164_)*mLocNDdx_x+(tmp169_)*mLocNDdy_x;
  mCompDer[0][14] = (tmp174_)*mLocNDdx_x+(tmp179_)*mLocNDdy_x;
  mCompDer[0][15] = (tmp183_)*mLocNDdx_x+(tmp188_)*mLocNDdy_x;
  mCompDer[0][16] = (tmp192_)*mLocNDdx_x+(tmp196_)*mLocNDdy_x;
  mCompDer[0][17] = (tmp200_)*mLocNDdx_x+(tmp204_)*mLocNDdy_x;
  mCompDer[0][18] = (tmp208_)*mLocNDdx_x+(tmp212_)*mLocNDdy_x;
  mVal[1] = (mLocNDP0_y+mLocNDdx_y*(tmp96_)+mLocNDdy_y*(tmp97_))-mLocYIm;

  mCompDer[1][0] = 0;
  mCompDer[1][1] = 0;
  mCompDer[1][2] = 0;
  mCompDer[1][3] = 0;
  mCompDer[1][4] = 0;
  mCompDer[1][5] = 0;
  mCompDer[1][6] = 0;
  mCompDer[1][7] = 0;
  mCompDer[1][8] = 0;
  mCompDer[1][9] = 0;
  mCompDer[1][10] = (tmp105_)*mLocNDdx_y+(tmp117_)*mLocNDdy_y;
  mCompDer[1][11] = (tmp128_)*mLocNDdx_y+(tmp140_)*mLocNDdy_y;
  mCompDer[1][12] = (tmp143_)*mLocNDdx_y+(tmp159_)*mLocNDdy_y;
  mCompDer[1][13] = (tmp164_)*mLocNDdx_y+(tmp169_)*mLocNDdy_y;
  mCompDer[1][14] = (tmp174_)*mLocNDdx_y+(tmp179_)*mLocNDdy_y;
  mCompDer[1][15] = (tmp183_)*mLocNDdx_y+(tmp188_)*mLocNDdy_y;
  mCompDer[1][16] = (tmp192_)*mLocNDdx_y+(tmp196_)*mLocNDdy_y;
  mCompDer[1][17] = (tmp200_)*mLocNDdx_y+(tmp204_)*mLocNDdy_y;
  mCompDer[1][18] = (tmp208_)*mLocNDdx_y+(tmp212_)*mLocNDdy_y;
}


void cEqAppui_NoDist__PTInc_M2CDRad5::ComputeValDerivHessian()
{
  ELISE_ASSERT(false,"Foncteur cEqAppui_NoDist__PTInc_M2CDRad5 Has no Der Sec");
}

void cEqAppui_NoDist__PTInc_M2CDRad5::SetNDP0_x(double aVal){ mLocNDP0_x = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetNDP0_y(double aVal){ mLocNDP0_y = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetNDdx_x(double aVal){ mLocNDdx_x = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetNDdx_y(double aVal){ mLocNDdx_y = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetNDdy_x(double aVal){ mLocNDdy_x = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetNDdy_y(double aVal){ mLocNDdy_y = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetXIm(double aVal){ mLocXIm = aVal;}
void cEqAppui_NoDist__PTInc_M2CDRad5::SetYIm(double aVal){ mLocYIm = aVal;}



double * cEqAppui_NoDist__PTInc_M2CDRad5::AdrVarLocFromString(const std::string & aName)
{
   if (aName == "NDP0_x") return & mLocNDP0_x;
   if (aName == "NDP0_y") return & mLocNDP0_y;
   if (aName == "NDdx_x") return & mLocNDdx_x;
   if (aName == "NDdx_y") return & mLocNDdx_y;
   if (aName == "NDdy_x") return & mLocNDdy_x;
   if (aName == "NDdy_y") return & mLocNDdy_y;
   if (aName == "XIm") return & mLocXIm;
   if (aName == "YIm") return & mLocYIm;
   return 0;
}


cElCompiledFonc::cAutoAddEntry cEqAppui_NoDist__PTInc_M2CDRad5::mTheAuto("cEqAppui_NoDist__PTInc_M2CDRad5",cEqAppui_NoDist__PTInc_M2CDRad5::Alloc);


cElCompiledFonc *  cEqAppui_NoDist__PTInc_M2CDRad5::Alloc()
{  return new cEqAppui_NoDist__PTInc_M2CDRad5();
}


