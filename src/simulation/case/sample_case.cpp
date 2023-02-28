/**
 * @file sample_case.cpp
 * @brief Example of user defined simulation case
 */

#include "sample_case.hpp"

#include "../spacecraft/sample_spacecraft/sample_spacecraft.hpp"

using std::cout;
using std::string;

SampleCase::SampleCase(string ini_base) : SimulationCase(ini_base) {}

SampleCase::~SampleCase() { delete sample_sat_; }

void SampleCase::Initialize() {
  // Instantiate the target of the simulation
  // `spacecraft_id` corresponds to the index of `sat_file` in Simbase.ini
  const int spacecraft_id = 0;
  sample_sat_ = new SampleSat(&sim_config_, global_environment_, spacecraft_id);
  const int ground_station_id = 0;
  sample_gs_ = new SampleGroundStation(&sim_config_, ground_station_id);

  // Register the log output
  global_environment_->LogSetup(*(sim_config_.main_logger_));
  sample_sat_->LogSetup(*(sim_config_.main_logger_));
  sample_gs_->LogSetup(*(sim_config_.main_logger_));

  // Write headers to the log
  sim_config_.main_logger_->WriteHeaders();

  // Start the simulation
  cout << "\nSimulationDateTime \n";
  global_environment_->GetSimulationTime().PrintStartDateTime();
}

void SampleCase::Main() {
  global_environment_->Reset();  // for MonteCarlo Sim
  while (!global_environment_->GetSimulationTime().GetState().finish) {
    // Logging
    if (global_environment_->GetSimulationTime().GetState().log_output) {
      sim_config_.main_logger_->WriteValues();
    }

    // Global Environment Update
    global_environment_->Update();
    // Spacecraft Update
    sample_sat_->Update(&(global_environment_->GetSimulationTime()));
    // Ground Station Update
    sample_gs_->Update(global_environment_->GetCelestialInformation().GetEarthRotation(), *sample_sat_);

    // Debug output
    if (global_environment_->GetSimulationTime().GetState().disp_output) {
      cout << "Progresss: " << global_environment_->GetSimulationTime().GetProgressionRate() << "%\r";
    }
  }
}

string SampleCase::GetLogHeader() const {
  string str_tmp = "";

  str_tmp += WriteScalar("time", "s");
  // str_tmp += WriteVector("position", "i", "m", 3);
  // str_tmp += WriteVector("velocity", "i", "m/s", 3);
  // str_tmp += WriteVector("quaternion", "i2b", "-", 4);
  // str_tmp += WriteVector("omega", "b", "-", 3);

  return str_tmp;
}

string SampleCase::GetLogValue() const {
  string str_tmp = "";

  // auto pos_i = sample_sat->dynamics_->GetOrbit().GetPosition_i_m();
  // auto vel_i = sample_sat->dynamics_->GetOrbit().GetVelocity_i_m_s();
  // auto quat_i2b = sample_sat->dynamics_->GetAttitude().GetQuaternion_i2b();
  // auto omega_b = sample_sat->dynamics_->GetAttitude().GetOmega_b();

  // Need to match the contents of log with header setting above
  str_tmp += WriteScalar(global_environment_->GetSimulationTime().GetElapsedTime_s());
  // str_tmp += WriteVector(pos_i, 16);
  // str_tmp += WriteVector(vel_i, 10);
  // str_tmp += WriteQuaternion(quat_i2b);
  // str_tmp += WriteVector(omega_b, 10);

  return str_tmp;
}
