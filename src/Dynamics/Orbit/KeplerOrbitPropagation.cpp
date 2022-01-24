#include "KeplerOrbitPropagation.h"
#include "../../Library/math/s2e_math.hpp"

KeplerOrbitPropagation::KeplerOrbitPropagation(
  const double current_jd,
  KeplerOrbit kepler_orbit
):KeplerOrbit(kepler_orbit)
{
  UpdateState(current_jd);
}

KeplerOrbitPropagation::~KeplerOrbitPropagation()
{
}

void KeplerOrbitPropagation::Propagate(double endtime, double current_jd)
{
  if (!IsCalcEnabled) return;

  UpdateState(current_jd);
}

std::string KeplerOrbitPropagation::GetLogHeader() const
{
  std::string str_tmp = "";

  str_tmp += WriteVector("sat_position", "i", "m", 3);
  str_tmp += WriteVector("sat_velocity", "i", "m/s", 3);
  str_tmp += WriteVector("sat_velocity", "b", "m/s", 3);
  str_tmp += WriteVector("sat_acc_i", "i", "m/s^2", 3);
  str_tmp += WriteScalar("lat", "rad");
  str_tmp += WriteScalar("lon", "rad");
  str_tmp += WriteScalar("alt", "m");

  return str_tmp;
}

std::string KeplerOrbitPropagation::GetLogValue() const
{
  std::string str_tmp = "";

  str_tmp += WriteVector(sat_position_i_, 16);
  str_tmp += WriteVector(sat_velocity_i_, 10);
  str_tmp += WriteVector(sat_velocity_b_);
  str_tmp += WriteVector(acc_i_, 10);
  str_tmp += WriteScalar(lat_rad_);
  str_tmp += WriteScalar(lon_rad_);
  str_tmp += WriteScalar(alt_m_);

  return str_tmp;
}

// Private Function
void KeplerOrbitPropagation::UpdateState(const double current_jd)
{
  CalcPosVel(current_jd);
  sat_position_i_ = position_i_m_;
  sat_velocity_i_ = velocity_i_m_s_;
  TransECIToGeo(current_jd);
}
