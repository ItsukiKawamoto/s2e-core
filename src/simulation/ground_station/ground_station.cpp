/**
 * @file ground_station.cpp
 * @brief Base class of ground station
 */

#include "ground_station.hpp"

#include <environment/global/physical_constants.hpp>
#include <library/initialize/initialize_file_access.hpp>
#include <library/logger/log_utility.hpp>
#include <library/logger/logger.hpp>
#include <library/math/constants.hpp>
#include <library/utilities/macros.hpp>
#include <string>

GroundStation::GroundStation(SimulationConfig* configuration, int gs_id) : gs_id_(gs_id) {
  Initialize(gs_id_, configuration);
  num_sc_ = configuration->number_of_simulated_spacecraft_;
  for (int i = 0; i < num_sc_; i++) {
    is_visible_[i] = false;
  }
}

GroundStation::~GroundStation() {}

void GroundStation::Initialize(int gs_id, SimulationConfig* configuration) {
  std::string gs_ini_path = configuration->ground_station_file_list_[0];
  auto conf = IniAccess(gs_ini_path);

  const char* section_base = "GROUND_STATION_";
  const std::string section_tmp = section_base + std::to_string(static_cast<long long>(gs_id));
  const char* Section = section_tmp.data();

  double latitude_deg = conf.ReadDouble(Section, "latitude_deg");
  double longitude_deg = conf.ReadDouble(Section, "longitude_deg");
  double height_m = conf.ReadDouble(Section, "height_m");
  gs_position_geo_ = GeodeticPosition(latitude_deg * libra::deg_to_rad, longitude_deg * libra::deg_to_rad, height_m);
  gs_position_ecef_ = gs_position_geo_.CalcEcefPosition();

  elevation_limit_angle_deg_ = conf.ReadDouble(Section, "elevation_limit_angle_deg");

  configuration->main_logger_->CopyFileToLogDirectory(gs_ini_path);
}

void GroundStation::LogSetup(Logger& logger) { logger.AddLogList(this); }

void GroundStation::Update(const CelestialRotation& celes_rotation, const Spacecraft& spacecraft) {
  libra::Matrix<3, 3> dcm_ecef2eci = Transpose(celes_rotation.GetDcmJ2000ToXcxf());
  gs_position_i_ = dcm_ecef2eci * gs_position_ecef_;

  is_visible_[spacecraft.GetSpacecraftId()] = CalcIsVisible(spacecraft.GetDynamics().GetOrbit().GetPosition_ecef_m());
}

bool GroundStation::CalcIsVisible(const libra::Vector<3> sc_pos_ecef_m) {
  libra::Quaternion q_ecef_to_ltc = gs_position_geo_.GetQuaternionXcxfToLtc();

  libra::Vector<3> sc_pos_ltc = q_ecef_to_ltc.FrameConversion(sc_pos_ecef_m - gs_position_ecef_);  // Satellite position in LTC frame [m]
  Normalize(sc_pos_ltc);
  libra::Vector<3> dir_gs_to_zenith = libra::Vector<3>(0);
  dir_gs_to_zenith[2] = 1;

  // Judge the satellite position angle is over the minimum elevation

  if (dot(sc_pos_ltc, dir_gs_to_zenith) > sin(elevation_limit_angle_deg_ * libra::deg_to_rad)) {
    return true;
  } else {
    return false;
  }
}

std::string GroundStation::GetLogHeader() const {
  std::string str_tmp = "";

  std::string head = "ground_station" + std::to_string(gs_id_) + "_";
  for (int i = 0; i < num_sc_; i++) {
    std::string legend = head + "sc" + std::to_string(i) + "_visible_flag";
    str_tmp += WriteScalar(legend);
  }
  return str_tmp;
}

std::string GroundStation::GetLogValue() const {
  std::string str_tmp = "";

  for (int i = 0; i < num_sc_; i++) {
    str_tmp += WriteScalar(is_visible_.at(i));
  }
  return str_tmp;
}
