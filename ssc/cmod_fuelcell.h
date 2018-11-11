/*******************************************************************************************************
*  Copyright 2017 Alliance for Sustainable Energy, LLC
*
*  NOTICE: This software was developed at least in part by Alliance for Sustainable Energy, LLC
*  (Alliance) under Contract No. DE-AC36-08GO28308 with the U.S. Department of Energy and the U.S.
*  The Government retains for itself and others acting on its behalf a nonexclusive, paid-up,
*  irrevocable worldwide license in the software to reproduce, prepare derivative works, distribute
*  copies to the public, perform publicly and display publicly, and to permit others to do so.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted
*  provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer in the documentation and/or
*  other materials provided with the distribution.
*
*  3. The entire corresponding source code of any redistribution, with or without modification, by a
*  research entity, including but not limited to any contracting manager/operator of a United States
*  National Laboratory, any institution of higher learning, and any non-profit organization, must be
*  made publicly available under this license for as long as the redistribution is made available by
*  the research entity.
*
*  4. Redistribution of this software, without modification, must refer to the software by the same
*  designation. Redistribution of a modified version of this software (i) may not refer to the modified
*  version by the same designation, or by any confusingly similar designation, and (ii) must refer to
*  the underlying software originally provided by Alliance as System Advisor Model or SAM. Except
*  to comply with the foregoing, the terms System Advisor Model, SAM, or any confusingly similar
*  designation may not be used to refer to any modified version of this software or any modified
*  version of the underlying software originally provided by Alliance without the prior written consent
*  of Alliance.
*
*  5. The name of the copyright holder, contributors, the United States Government, the United States
*  Department of Energy, or any of their employees may not be used to endorse or promote products
*  derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER,
*  CONTRIBUTORS, UNITED STATES GOVERNMENT OR UNITED STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR
*  EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
*  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************************************/
#ifndef _CMOD_FUEL_CELL_H_
#define _CMOD_FUEL_CELL_H_

#include <map>
#include <memory>

#include "core.h"
#include "lib_fuel_cell.h"
#include "lib_fuel_cell_dispatch.h"
#include "lib_util.h"

struct fuelCellVariables
{
public:

	fuelCellVariables() {/* nothing to do */ };
	fuelCellVariables(compute_module & cm) :
		systemUseLifetimeOutput(cm.as_boolean("system_use_lifetime_output")),
		systemGeneration_kW(cm.as_vector_double("gen")),
		unitPowerMax_kW(cm.as_double("fuelcell_unit_max_power")),
		unitPowerMin_kW(cm.as_double("fuelcell_unit_min_power")),
		startup_hours(cm.as_double("fuelcell_startup_time")),
		dynamicResponse_kWperHour(cm.as_double("fuelcell_dynamic_response")),
		degradation_kWperHour(cm.as_double("fuelcell_degradation")),
		degradationRestart_kW(cm.as_double("fuelcell_degradation_restart")),
		replacement_percent(cm.as_double("fuelcell_replacement_percent")),
		efficiencyTable(cm.as_matrix("fuelcell_efficiency")),
		lowerHeatingValue_BtuPerFt3(cm.as_double("fuelcell_lhv")),
		higherHeatingValue_BtuPerFt3(cm.as_double("fuelcell_hhv")),
		availableFuel_MCf(cm.as_double("fuelcell_fuel_available")),
		shutdownOption(cm.as_integer("shutdownOption")),
		numberOfUnits(cm.as_integer("fuelcell_number_of_units")),
		dispatchOption(cm.as_integer("dispatchOption")),
		fixed_percent(cm.as_double("fixed_percent")),
		canCharge(cm.as_vector_bool("dispatch_manual_fuelcellcharge")),
		canDischarge(cm.as_vector_bool("dispatch_manual_fuelcelldischarge")),
		discharge_percent(cm.as_vector_double("dispatch_manual_percent_fc_discharge")),
		scheduleWeekday(cm.as_matrix_unsigned_long("dispatch_manual_sched")),
		scheduleWeekend(cm.as_matrix_unsigned_long("dispatch_manual_sched_weekend"))
	{
		numberOfYears = 1;
		if (systemUseLifetimeOutput) {
			numberOfYears = cm.as_unsigned_long("analysis_period");
		}
		dt_hour = (double)(systemGeneration_kW.size() / (size_t)(numberOfYears * 8760));

		for (size_t p = 0; p < discharge_percent.size(); p++) {
			discharge_percentByPeriod[p] = discharge_percent[p];
		}
	}

	// simulation inputs
	bool systemUseLifetimeOutput;
	size_t numberOfYears;

	// generation input
	std::vector<double> systemGeneration_kW;

	// fuel cell
	double dt_hour;
	double unitPowerMax_kW;
	double unitPowerMin_kW;
	double startup_hours;
	double dynamicResponse_kWperHour;
	double degradation_kWperHour;
	double degradationRestart_kW;
	double replacement_percent;
	util::matrix_t<double> efficiencyTable;
	double lowerHeatingValue_BtuPerFt3;
	double higherHeatingValue_BtuPerFt3;
	double availableFuel_MCf;
	int shutdownOption;

	// dispatch
	size_t numberOfUnits;
	int dispatchOption;
	double fixed_percent;
	std::vector<bool> canCharge;
	std::vector<bool> canDischarge;
	std::vector<double> discharge_percent;
	std::map<size_t, double> discharge_percentByPeriod;
	util::matrix_t<size_t> scheduleWeekday;
	util::matrix_t<size_t> scheduleWeekend;
};

class cm_fuelcell : public compute_module 
{
public: 

	/// Default constructor
	cm_fuelcell();

	/// Default destructor
	~cm_fuelcell();

	/// Main execution
	void exec() throw(general_error);

protected:

	std::unique_ptr<fuelCellVariables> fcVars;
	std::unique_ptr<FuelCell> fuelCell;
	std::unique_ptr<FuelCellDispatch> fuelCellDispatch;
};

#endif
