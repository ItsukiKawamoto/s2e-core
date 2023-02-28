﻿/*
 * @file initialize_solar_array_panel.cpp
 * @brief Initialize function of SAP (Solar Array Panel)
 */
#define _CRT_SECURE_NO_WARNINGS
#include "initialize_solar_array_panel.hpp"

#include <string.h>

#include "library/initialize/initialize_file_access.hpp"

SAP InitSAP(ClockGenerator* clock_generator, int sap_id, const std::string file_name, const SolarRadiationPressureEnvironment* srp,
            const LocalCelestialInformation* local_celestial_information, double component_step_time_s) {
  IniAccess sap_conf(file_name);

  const std::string st_sap_id = std::to_string(sap_id);
  const char* cs = st_sap_id.data();

  char Section[30] = "SOLAR_ARRAY_PANEL_";
  strcat(Section, cs);

  int prescaler = sap_conf.ReadInt(Section, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  int number_of_series;
  number_of_series = sap_conf.ReadInt(Section, "number_of_series");

  int number_of_parallel;
  number_of_parallel = sap_conf.ReadInt(Section, "number_of_parallel");

  double cell_area;
  cell_area = sap_conf.ReadDouble(Section, "cell_area_m2");

  libra::Vector<3> normal_vector;
  sap_conf.ReadVector(Section, "normal_vector_b", normal_vector);

  double cell_efficiency;
  cell_efficiency = sap_conf.ReadDouble(Section, "cell_efficiency");

  double transmission_efficiency;
  transmission_efficiency = sap_conf.ReadDouble(Section, "transmission_efficiency");

  SAP sap(prescaler, clock_generator, sap_id, number_of_series, number_of_parallel, cell_area, normal_vector, cell_efficiency,
          transmission_efficiency, srp, local_celestial_information, component_step_time_s);

  return sap;
}

SAP InitSAP(ClockGenerator* clock_generator, int sap_id, const std::string file_name, const SolarRadiationPressureEnvironment* srp,
            double component_step_time_s) {
  IniAccess sap_conf(file_name);

  const std::string st_sap_id = std::to_string(sap_id);
  const char* cs = st_sap_id.data();

  char Section[30] = "SOLAR_ARRAY_PANEL_";
  strcat(Section, cs);

  int prescaler = sap_conf.ReadInt(Section, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  int number_of_series;
  number_of_series = sap_conf.ReadInt(Section, "number_of_series");

  int number_of_parallel;
  number_of_parallel = sap_conf.ReadInt(Section, "number_of_parallel");

  double cell_area;
  cell_area = sap_conf.ReadDouble(Section, "cell_area_m2");

  libra::Vector<3> normal_vector;
  sap_conf.ReadVector(Section, "normal_vector_b", normal_vector);

  double cell_efficiency;
  cell_efficiency = sap_conf.ReadDouble(Section, "cell_efficiency");

  double transmission_efficiency;
  transmission_efficiency = sap_conf.ReadDouble(Section, "transmission_efficiency");

  SAP sap(prescaler, clock_generator, sap_id, number_of_series, number_of_parallel, cell_area, normal_vector, cell_efficiency,
          transmission_efficiency, srp, component_step_time_s);

  return sap;
}
