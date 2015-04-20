#define _TCSTYPEINTERFACE_
#include "tcstype.h"
#include "htf_props.h"
#include "sam_csp_util.h"
#include "ngcc_powerblock.h"

#include "csp_solver_mspt_receiver_222.h"
#include "csp_solver_util.h"
#include "csp_solver_core.h"

enum{	//Parameters
		P_N_panels,
		P_D_rec,
		P_H_rec,
		P_THT,
		P_D_out,
		P_th_tu,
		P_mat_tube,
		P_field_fl,
		P_field_fl_props,
		P_Flow_type,
		P_epsilon,
		P_hl_ffact,
		P_T_htf_hot_des,
		P_T_htf_cold_des,
		P_f_rec_min,
		P_Q_rec_des,
		P_rec_su_delay,
		P_rec_qf_delay,
		P_m_dot_htf_max,
		P_A_sf,
		P_IS_DIRECT_ISCC,
		P_CYCLE_CONFIG,
//		P_fluxmap_angles,
//		P_fluxmap,       
		P_n_flux_x,
		P_n_flux_y,


		//Inputs
		I_azimuth,
		I_zenith,
		I_T_salt_hot,
		I_T_salt_cold,
		I_v_wind_10,
		I_P_amb,
		I_eta_pump,
		I_T_dp,
		I_I_bn,
		I_field_eff,
		I_T_db,
		I_night_recirc,
		I_hel_stow_deploy,
		I_flux_map,

		//Outputs
		O_m_dot_salt_tot,
		O_eta_therm,
		O_W_dot_pump,
		O_q_conv_sum,
		O_q_rad_sum,
		O_Q_thermal,
		O_T_salt_hot,
		//-not named-,
		O_field_eff_adj,
		O_q_solar_total,
		O_q_startup,
		O_dP_receiver,
		O_dP_total,
		O_vel_htf,
		O_T_salt_in,
		O_M_DOT_SS, 
		O_Q_DOT_SS,
		O_F_TIMESTEP,

		//N_MAX
		N_MAX};

tcsvarinfo sam_mw_pt_type222_variables[] = {
	//PARAMETERS
	{TCS_PARAM, TCS_NUMBER, P_N_panels,			"N_panels",			"Number of individual panels on the receiver",								"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_D_rec,			"D_rec",			"The overall outer diameter of the receiver",								"m",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_H_rec,			"H_rec",			"The height of the receiver",												"m",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_THT,				"THT",				"The height of the tower (hel. pivot to rec equator)",						"m",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_D_out,			"d_tube_out",		"The outer diameter of an individual receiver tube",						"mm",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_th_tu,			"th_tube",			"The wall thickness of a single receiver tube",								"mm",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_mat_tube,			"mat_tube",         "The material name of the receiver tubes",									"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_field_fl,			"rec_htf",			"The name of the HTF used in the receiver",									"",			"", "", ""},
	{TCS_PARAM, TCS_MATRIX, P_field_fl_props,   "field_fl_props",   "User defined field fluid property data",                                   "-",        "7 columns (T,Cp,dens,visc,kvisc,cond,h), at least 3 rows",        "",        ""},
	{TCS_PARAM, TCS_NUMBER, P_Flow_type,		"Flow_type",		"A flag indicating which flow pattern is used",								"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_epsilon,			"epsilon",			"The emissivity of the receiver surface coating",							"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_hl_ffact,			"hl_ffact",			"The heat loss factor (thermal loss fudge factor)",							"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_T_htf_hot_des,	"T_htf_hot_des",	"Hot HTF outlet temperature at design conditions",							"C",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_T_htf_cold_des,	"T_htf_cold_des",	"Cold HTF inlet temperature at design conditions",							"C",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_f_rec_min,		"f_rec_min",		"Minimum receiver mass flow rate turn down fraction",						"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_Q_rec_des,		"Q_rec_des",		"Design-point receiver thermal power output",								"MWt",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_rec_su_delay,		"rec_su_delay",		"Fixed startup delay time for the receiver",								"hr",		"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_rec_qf_delay,		"rec_qf_delay",		"Energy-based receiver startup delay (fraction of rated thermal power)",	"",			"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_m_dot_htf_max,	"m_dot_htf_max",	"Maximum receiver mass flow rate",											"kg/hr",	"", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_A_sf,				"A_sf",				"Solar Field Area",                                                         "m^2",      "", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_IS_DIRECT_ISCC,   "is_direct_iscc",   "Is receiver directly connected to an iscc power block",                    "-",        "", "", "-999"},
	{TCS_PARAM, TCS_NUMBER, P_CYCLE_CONFIG,     "cycle_config",     "Configuration of ISCC power cycle",                                        "-",        "", "", "1"},
	{TCS_PARAM, TCS_NUMBER, P_n_flux_x,         "n_flux_x",         "Receiver flux map resolution - X",                                         "-",        "", "", ""},
	{TCS_PARAM, TCS_NUMBER, P_n_flux_y,         "n_flux_y",         "Receiver flux map resolution - Y",                                         "-",        "", "", ""},
	//{TCS_PARAM, TCS_MATRIX, P_fluxmap_angles,   "fluxmap_angles",   "Matrix containing zenith and azimuth angles for flux maps",                "-",        "2 columns - azimuth angle, zenith angle. number of rows must equal number of flux maps provided", "", "" },
	//{TCS_PARAM, TCS_MATRIX, P_fluxmap,          "fluxmap",          "Matrix containing flux map for various solar positions",                   "-",        "", "", "" },

	//INPUTS
	{TCS_INPUT, TCS_NUMBER, I_azimuth,			"azimuth",			"Solar azimuth angle",														"deg",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_zenith,			"zenith",			"Solar zenith angle",														"deg",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_T_salt_hot,		"T_salt_hot_target", "Desired HTF outlet temperature",											"C",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_T_salt_cold,		"T_salt_cold",		"Desired HTF inlet temperature",											"C",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_v_wind_10,		"V_wind_10",		"Ambient wind velocity, ground level",										"m/s",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_P_amb,			"P_amb",			"Ambient atmospheric pressure",												"mbar",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_eta_pump,			"eta_pump",			"Receiver HTF pump efficiency",												"",			"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_T_dp,				"T_dp",				"Ambient dew point temperature",											"C",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_I_bn,				"I_bn",				"Direct (beam) normal radiation",											"W/m^2-K",  "", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_field_eff,		"field_eff",		"Heliostat field efficiency",												"",			"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_T_db,				"T_db",				"Ambient dry bulb temperature",												"C",		"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_night_recirc,		"night_recirc",		"Flag to indicate night recirculation through the rec.",					"",			"", "", ""},
	{TCS_INPUT, TCS_NUMBER, I_hel_stow_deploy,	"hel_stow_deploy",	"Heliostat field stow/deploy solar angle",									"deg",		"", "", ""},
	{TCS_INPUT, TCS_MATRIX, I_flux_map,			"flux_map",			"Receiver flux map",														"-",		"", "", ""},

	//OUTPUTS
	{TCS_OUTPUT, TCS_NUMBER, O_m_dot_salt_tot,	"m_dot_salt_tot",	"Total HTF flow rate through the receiver",									"kg/hr",	"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_eta_therm,		"eta_therm",		"Receiver thermal efficiency",												"",			"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_W_dot_pump,		"W_dot_pump",		"Receiver pump power",														"MWe",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_q_conv_sum,		"q_conv_sum",		"Receiver convective losses",												"MWt",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_q_rad_sum,		"q_rad_sum",		"Receiver radiative losses",												"MWt",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_Q_thermal,		"Q_thermal",		"Receiver thermal output",								                    "MWt",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_T_salt_hot,		"T_salt_hot",		"HTF outlet temperature",													"C",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_field_eff_adj,	"field_eff_adj",	"Adjusted heliostat field efficiency - includes overdesign adjustment",		"",			"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_q_solar_total,	"Q_solar_total",	"Total incident power on the receiver",										"MWt",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_q_startup,		"q_startup",		"Startup energy consumed during the current time step",						"MWt",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_dP_receiver,		"dP_receiver",		"Receiver HTF pressure drop",												"bar",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_dP_total,		"dP_total",			"Total receiver and tower pressure drop",									"bar",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_vel_htf,			"vel_htf",			"Heat transfer fluid average velocity",										"m/s",		"", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_T_salt_in,       "T_salt_cold",      "Inlet salt temperature",                                                   "C",        "", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_M_DOT_SS,        "m_dot_ss",	        "Mass flow rate at steady state - does not derate for startup",             "kg/hr",    "", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_Q_DOT_SS,        "q_dot_ss",         "Thermal at steady state - does not derate for startup",                    "MW",       "", "", ""},
	{TCS_OUTPUT, TCS_NUMBER, O_F_TIMESTEP,      "f_timestep",       "Fraction of timestep that receiver is operational (not starting-up)",      "-",        "", "", ""},

	//N_MAX
	{TCS_INVALID, TCS_INVALID, N_MAX,			0,					0, 0, 0, 0, 0	} 
} ;
	
	
class sam_mw_pt_type222 : public tcstypeinterface
{
private:
	C_mspt_receiver_222 mspt_receiver;
	C_csp_weatherreader::S_outputs ms_weather;
	C_csp_solver_htf_state ms_htf_state;
	C_mspt_receiver_222::S_inputs ms_inputs;
	C_csp_solver_sim_info ms_sim_info;

public:
	sam_mw_pt_type222( tcscontext *cst, tcstypeinfo *ti)
			: tcstypeinterface( cst, ti)
	{
	}

	virtual ~sam_mw_pt_type222()
	{
	}

	virtual int init()
	{
		mspt_receiver.m_field_fl = (int) value(P_field_fl);

		int n_rows = 0; 
		int n_cols = 0;
		double *field_fl_props = value(P_field_fl_props, &n_rows, &n_cols);
		mspt_receiver.m_field_fl_props.resize(n_rows, n_cols);
		for( int r = 0; r < n_rows; r++ )
			for( int c = 0; c < n_cols; c++ )
				mspt_receiver.m_field_fl_props(r, c) = TCS_MATRIX_INDEX(var(P_field_fl_props), r, c);

		mspt_receiver.m_mat_tube = (int)value(P_mat_tube);
		mspt_receiver.m_n_panels = (int)value(P_N_panels);		//[-] Number of panels in receiver
		mspt_receiver.m_d_rec = value(P_D_rec);					//[m] Diameter of receiver
		mspt_receiver.m_h_rec = value(P_H_rec);					//[m] Height of receiver
		mspt_receiver.m_h_tower = value(P_THT);					//[m] Height of tower
		mspt_receiver.m_od_tube = value(P_D_out);				//[mm] Outer diameter of receiver tubes
		mspt_receiver.m_th_tube = value(P_th_tu);				//[mm] Thickness of receiver tubes

		mspt_receiver.m_flow_type = (int)value(P_Flow_type);	//[-] Numerical code to designate receiver flow type

		mspt_receiver.m_epsilon = value(P_epsilon);				//[-] Emissivity of receiver
		mspt_receiver.m_hl_ffact = value(P_hl_ffact);			//[-] Heat Loss Fudge FACTor
		mspt_receiver.m_T_htf_hot_des = value(P_T_htf_hot_des);	//[C] Design receiver outlet temperature
		mspt_receiver.m_T_htf_cold_des = value(P_T_htf_cold_des);	//[C] Design receiver inlet temperature
		mspt_receiver.m_f_rec_min = value(P_f_rec_min);				//[-] Minimum receiver mass flow rate turn down fraction
		mspt_receiver.m_q_rec_des = value(P_Q_rec_des);				//[MW] Design receiver thermal input
		mspt_receiver.m_rec_su_delay = value(P_rec_su_delay);		//[hr] Receiver startup time duration
		mspt_receiver.m_rec_qf_delay = value(P_rec_qf_delay);		//[-] Energy-based receiver startup delay (fraction of rated thermal power)
		mspt_receiver.m_m_dot_htf_max = value(P_m_dot_htf_max);		//[kg/hr] Maximum mass flow rate through receiver
		mspt_receiver.m_A_sf = value(P_A_sf);						//[m^2] Solar field area

		mspt_receiver.m_n_flux_x = (int)value(P_n_flux_x);
		mspt_receiver.m_n_flux_y = (int)value(P_n_flux_y);

		mspt_receiver.m_T_salt_hot_target = value(I_T_salt_hot);
		mspt_receiver.m_eta_pump = value(I_eta_pump);
		mspt_receiver.m_night_recirc = (int) value(I_night_recirc);
		mspt_receiver.m_hel_stow_deploy = value(I_hel_stow_deploy);

		//allocate the input array for the flux map
		double *p_i_flux_map = allocate(I_flux_map, mspt_receiver.m_n_flux_y, mspt_receiver.m_n_flux_x);

		// Are we modeling a direct ISCC case?
		mspt_receiver.m_is_iscc = value(P_IS_DIRECT_ISCC) == 1;
		// Set cycle configuration in class
		mspt_receiver.m_cycle_config = value(P_CYCLE_CONFIG);
		
		

		int out_type = -1;
		std::string out_msg = ""; 

		try
		{
			mspt_receiver.init();
		}

		catch(C_csp_exception &csp_exception)
		{
			// Report warning before exiting with error
			while( mspt_receiver.csp_messages.get_message(&out_type, &out_msg) )
			{
				if( out_type == C_csp_messages::NOTICE )
					message(TCS_NOTICE, out_msg.c_str());
				else if( out_type == C_csp_messages::WARNING )
					message(TCS_WARNING, out_msg.c_str());
			}

			message(TCS_ERROR, csp_exception.m_error_message.c_str());
			return -1;
		}

		// If no exception, then report messages and move on
		while( mspt_receiver.csp_messages.get_message(&out_type, &out_msg) )
		{
			if( out_type == C_csp_messages::NOTICE )
				message(TCS_NOTICE, out_msg.c_str());
			else if( out_type == C_csp_messages::WARNING )
				message(TCS_WARNING, out_msg.c_str());
		}

		return 0;

	}

	virtual int call( double time, double step, int ncall )
	{		
		// Pass inputs to mspt_receiver class - NO CONVERSIONS!
		ms_weather.m_solazi = value(I_azimuth);							//[deg] Solar azimuth angle 0 - 360, clockwise from due north, northern hemisphere
		ms_weather.m_solzen = value(I_zenith);							//[deg] Solar zenith angle
		double T_salt_cold_in_csp = value(I_T_salt_cold);				//[K] Cold salt inlet temp, convert from C
		ms_weather.m_wspd = value(I_v_wind_10);							//[m/s] Wind velocity
		ms_weather.m_pres = value(I_P_amb);								//[Pa] Ambient pressure, convert from mbar
	
		ms_weather.m_tdew = value(I_T_dp);					//[K] Dewpoint temperature, convert from C
		ms_weather.m_beam = value(I_I_bn);					//[W/m^2-K] Beam normal radiation
		ms_inputs.m_field_eff = value(I_field_eff);			//[-] Field efficiency value
		ms_weather.m_tdry = value(I_T_db);						//[K] Dry bulb temperature, convert from C

		//get the flux map
		int n_flux_y_csp, n_flux_x_csp;	 
		double *p_i_flux_map = value(I_flux_map, &n_flux_y_csp, &n_flux_x_csp);
		util::matrix_t<double> flux_map_in(n_flux_y_csp, n_flux_x_csp);
		for( int i = 0; i < n_flux_y_csp; i++ )
			for( int j = 0; j < n_flux_x_csp; j++ )
				flux_map_in(i, j) = p_i_flux_map[j*n_flux_y_csp + i];

		ms_inputs.m_flux_map_input = &flux_map_in;

		// set sim info
		ms_sim_info.m_time = time;
		ms_sim_info.m_step = step;
		//ms_sim_info.m_ncall = ncall;

		// Set applicable htf state info
		ms_htf_state.m_temp_in = T_salt_cold_in_csp;

		int out_type = -1;
		std::string out_msg = "";

		try
		{
			mspt_receiver.call(&ms_weather, &ms_htf_state, 
				&ms_inputs, 
				&ms_sim_info);
		}
		
		catch( C_csp_exception &csp_exception )
		{
			// Report warning before exiting with error
			while( mspt_receiver.csp_messages.get_message(&out_type, &out_msg) )
			{
				if( out_type == C_csp_messages::NOTICE )
					message(TCS_NOTICE, out_msg.c_str());
				else if( out_type == C_csp_messages::WARNING )
					message(TCS_WARNING, out_msg.c_str());
			}

			message(TCS_ERROR, csp_exception.m_error_message.c_str());
			return -1;
		}

		// If no exception, then report messages and move on
		while( mspt_receiver.csp_messages.get_message(&out_type, &out_msg) )
		{
			if( out_type == C_csp_messages::NOTICE )
				message(TCS_NOTICE, out_msg.c_str());
			else if( out_type == C_csp_messages::WARNING )
				message(TCS_WARNING, out_msg.c_str());
		}
	
		// Get outputs from mspt_receiver class and set to TCS OUTPUT values - do NOT convert
		value(O_m_dot_salt_tot, mspt_receiver.ms_outputs.m_m_dot_salt_tot);	//[kg/hr]
		value(O_eta_therm, mspt_receiver.ms_outputs.m_eta_therm);				//[-]
		value(O_W_dot_pump, mspt_receiver.ms_outputs.m_W_dot_pump);			//[MW]
		value(O_q_conv_sum, mspt_receiver.ms_outputs.m_q_conv_sum);			//[MW]
		value(O_q_rad_sum, mspt_receiver.ms_outputs.m_q_rad_sum);				//[MW]
		value(O_Q_thermal, mspt_receiver.ms_outputs.m_Q_thermal);				//[MW]
		value(O_T_salt_hot, mspt_receiver.ms_outputs.m_T_salt_hot);			//[C] 
		value(O_field_eff_adj, mspt_receiver.ms_outputs.m_field_eff_adj);		//[-]
		value(O_q_solar_total, mspt_receiver.ms_outputs.m_Q_solar_total);		//[MW]
		value(O_q_startup, mspt_receiver.ms_outputs.m_q_startup);				//[MW]
		value(O_dP_receiver, mspt_receiver.ms_outputs.m_dP_receiver);			//[bar]
		value(O_dP_total, mspt_receiver.ms_outputs.m_dP_total);				//[bar]
		value(O_vel_htf, mspt_receiver.ms_outputs.m_vel_htf);					//[m/s]
		value(O_T_salt_in, mspt_receiver.ms_outputs.m_T_salt_cold);			//[C]
		value(O_M_DOT_SS, mspt_receiver.ms_outputs.m_m_dot_ss);				//[kg/hr]
		value(O_Q_DOT_SS, mspt_receiver.ms_outputs.m_q_dot_ss);				//[MW]
		value(O_F_TIMESTEP, mspt_receiver.ms_outputs.m_f_timestep);			//[-]

		return 0;
	}

	virtual int converged( double time )
	{
		int out_type = -1;
		std::string out_msg = "";

		try
		{
			mspt_receiver.converged();
		}

		catch( C_csp_exception &csp_exception )
		{
			// Report warning before exiting with error
			while( mspt_receiver.csp_messages.get_message(&out_type, &out_msg) )
			{
				if( out_type == C_csp_messages::NOTICE )
					message(TCS_NOTICE, out_msg.c_str());
				else if( out_type == C_csp_messages::WARNING )
					message(TCS_WARNING, out_msg.c_str());
			}

			message(TCS_ERROR, csp_exception.m_error_message.c_str());
			return -1;
		}

		// If no exception, then report messages and move on
		while( mspt_receiver.csp_messages.get_message(&out_type, &out_msg) )
		{
			if( out_type == C_csp_messages::NOTICE )
				message(TCS_NOTICE, out_msg.c_str());
			else if( out_type == C_csp_messages::WARNING )
				message(TCS_WARNING, out_msg.c_str());
		}

		return 0;

	}

};

TCS_IMPLEMENT_TYPE( sam_mw_pt_type222, "External Receiver/Tower", "Ty Neises", 1, sam_mw_pt_type222_variables, NULL, 1 )