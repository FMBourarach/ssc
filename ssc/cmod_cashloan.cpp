#include "core.h"
#include "lib_financial.h"
using namespace libfin;

static var_info vtab_cashloan[] = {
/*   VARTYPE           DATATYPE          NAME                        LABEL                                  UNITS         META                      GROUP            REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/

	{ SSC_INPUT,        SSC_NUMBER,		 "market",                   "Residential or Commercial Market",   "0/1",          "0=residential,1=comm.", "Cashloan",      "?=1",                     "INTEGER,MIN=0,MAX=1",            "" },
	{ SSC_INPUT,        SSC_NUMBER,		 "mortgage",		         "Use mortgage style loan (res. only)","0/1",          "0=standard loan,1=mortgage",                      "Cashloan",      "?=0",                     "BOOLEAN",                        "" },

	{ SSC_INPUT,        SSC_NUMBER,      "total_installed_cost",                          "Total installed cost",                               "$",      "",                      "Cashloan",            "*",                      "MIN=0",                                         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "salvage_percentage",                       "Salvage value percentage",                        "%",      "",                      "Cashloan",      "?=0.0",                  "MIN=0,MAX=100",                 "" },
	
	{ SSC_INPUT,        SSC_ARRAY,       "energy_value",             "Energy value",                       "$",            "",                      "Cashloan",      "*",                       "",                                         "" },
	{ SSC_INPUT,        SSC_ARRAY,       "energy_net",               "Net energy",                         "kWh",          "",                      "Cashloan",      "*",                       "",                                         "" },

	/* financial outputs */
	{ SSC_OUTPUT,        SSC_NUMBER,     "cf_length",                "Number of periods in cashflow",      "",             "",                      "Cashloan",      "*",                       "INTEGER",                                  "" },

	{ SSC_OUTPUT,        SSC_NUMBER,     "lcoe_real",                "Real LCOE",                          "cents/kWh",    "",                      "Cashloan",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "lcoe_nom",                 "Nominal LCOE",                       "cents/kWh",    "",                      "Cashloan",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "payback",                  "Payback",                            "years",        "",                      "Cashloan",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "npv",                      "Net present value",				   "$",            "",                      "Cashloan",      "*",                       "",                                         "" },

	{ SSC_OUTPUT,        SSC_NUMBER,     "present_value_oandm",                      "Present value of O and M",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "present_value_oandm_nonfuel",              "Present value of non-fuel O and M",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "present_value_fuel",                      "Present value of fuel O and M",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "present_value_insandproptax",                      "Present value of Insurance and Prop Tax",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },


	{ SSC_OUTPUT,        SSC_NUMBER,      "sv_first_year_energy_net",    "Net Annual Energy",  "", "",                      "DHF",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,      "sv_capacity_factor",    "Capacity factor",  "", "",                      "DHF",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,      "sv_kwh_per_kw",    "First year kWh/kW",  "", "",                      "DHF",      "*",                     "",                "" },


	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net",      "Energy",                  "kWh",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_value",      "Energy Value",                  "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	// real estate value added 6/24/13
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_value_added",      "Real Estate Value Added",                  "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_fixed_expense",      "O&M Fixed expense",                  "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_production_expense", "O&M Production-based expense",       "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_capacity_expense",   "O&M Capacity-based expense",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_fuel_expense",       "O&M Fuel expense",                   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_opt_fuel_1_expense",       "O&M Optional Fuel 1 expense",                   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_opt_fuel_2_expense",       "O&M Optional Fuel 2 expense",                   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },


	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_property_tax_assessed_value","Property tax net assessed value", "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_property_tax_expense",  "Property tax expense",               "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_insurance_expense",     "Insurance expense",                  "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_net_salvage_value",     "Net salvage value",                  "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_operating_expenses",    "Total operating expense",            "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_deductible_expenses",   "Deductible expenses",                "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
		
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_balance",          "Debt balance",                       "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_payment_interest", "Interest payment",                   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_payment_principal","Principal payment",                  "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_payment_total",    "Total P&I debt payment",             "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	
	{ SSC_OUTPUT,        SSC_NUMBER,     "ibi_total_fed",             "Federal IBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "ibi_total_sta",             "State IBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "ibi_total_uti",             "Utility IBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "ibi_total_oth",             "Other IBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "ibi_total",             "Total IBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	
	{ SSC_OUTPUT,        SSC_NUMBER,     "cbi_total_fed",             "Federal CBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "cbi_total_sta",             "State CBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "cbi_total_uti",             "Utility CBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "cbi_total_oth",             "Other CBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "cbi_total",             "Total CBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_pbi_total_fed",             "Federal PBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_pbi_total_sta",             "State PBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_pbi_total_uti",             "Utility PBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_pbi_total_oth",             "Other PBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_pbi_total",             "Total PBI incentive income",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_ptc_fed",               "Federal PTC income",                 "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_ptc_sta",               "State PTC income",                   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_ptc_total",               "Total PTC income",                   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_NUMBER,      "itc_fed_total",         "Federal ITC income",                 "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,      "itc_sta_total",         "State ITC income",                   "$",            "",                      "Cashloan",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,      "itc_total",         "Total ITC income",                   "$",            "",                      "Cashloan",      "*",                     "",                "" },
	
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_depr_sched",                        "State depreciation schedule",              "%",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_depreciation",                      "State depreciation",                       "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_incentive_income_less_deductions",  "State incentive income less deductions",   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_taxable_income_less_deductions",    "State taxable income less deductions",     "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_tax_savings",                       "State tax savings",                        "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_depr_sched",                        "Federal depreciation schedule",            "%",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_depreciation",                      "Federal depreciation",                     "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_incentive_income_less_deductions",  "Federal incentive income less deductions", "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_taxable_income_less_deductions",    "Federal taxable income less deductions",   "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_tax_savings",                       "Federal tax savings",                      "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_and_fed_tax_savings",               "Total tax savings (Federal & State)",      "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_after_tax_net_equity_cost_flow",        "After tax net equity cost flow",           "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_after_tax_cash_flow",                   "After tax cash flow",                      "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_payback_with_expenses",                 "Payback with expenses",                    "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_cumulative_payback_with_expenses",      "Cumulative payback with expenses",         "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_payback_without_expenses",              "Payback without expenses",                 "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_cumulative_payback_without_expenses",   "Cumulative payback without expenses",      "$",            "",                      "Cashloan",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	


	{ SSC_OUTPUT,        SSC_NUMBER,     "sv_lcoptc_fed_real",                "Levelized Federal PTC (real)",                          "",    "",                      "DHF",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "sv_lcoptc_fed_nom",                 "Levelized Federal PTC (nominal)",                       "",    "",                      "DHF",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "sv_lcoptc_sta_real",                "Levelized State PTC (real)",                          "",    "",                      "DHF",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "sv_lcoptc_sta_nom",                 "Levelized State PTC (nominal)",                       "",    "",                      "DHF",      "*",                       "",                                         "" },

	{ SSC_OUTPUT,        SSC_NUMBER,     "sv_wacc",                "Weighted Average Cost of Capital (WACC)",                          "",    "",                      "DHF",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "sv_effective_tax_rate",                 "Effective Tax Rate",                       "",    "",                      "DHF",      "*",                       "",                                         "" },


var_info_invalid };

extern var_info
	vtab_standard_financial[],
	vtab_standard_loan[],
	vtab_oandm[],
	vtab_depreciation[],
//	vtab_utility_rate[],
	vtab_tax_credits[],
	vtab_payment_incentives[];

enum {
	CF_energy_net,
	CF_energy_value,
	CF_value_added,

	CF_om_fixed_expense,
	CF_om_production_expense,
	CF_om_capacity_expense,
	CF_om_fuel_expense,

	CF_om_opt_fuel_2_expense,
	CF_om_opt_fuel_1_expense,


	CF_property_tax_assessed_value,
	CF_property_tax_expense,
	CF_insurance_expense,
	CF_net_salvage_value,
	CF_operating_expenses,

	CF_deductible_expenses,

	CF_debt_balance,
	CF_debt_payment_interest,
	CF_debt_payment_principal,
	CF_debt_payment_total,
	
	CF_pbi_fed,
	CF_pbi_sta,
	CF_pbi_uti,
	CF_pbi_oth,
	CF_pbi_total,
	
	CF_ptc_fed,
	CF_ptc_sta,
	CF_ptc_total,
	
	CF_sta_depr_sched,
	CF_sta_depreciation,
	CF_sta_incentive_income_less_deductions,
	CF_sta_taxable_income_less_deductions,
	CF_sta_tax_savings,
	
	CF_fed_depr_sched,
	CF_fed_depreciation,
	CF_fed_incentive_income_less_deductions,
	CF_fed_taxable_income_less_deductions,
	CF_fed_tax_savings,

	CF_sta_and_fed_tax_savings,
	CF_after_tax_net_equity_cost_flow,
	CF_after_tax_cash_flow,

	CF_payback_with_expenses,
	CF_cumulative_payback_with_expenses,
	
	CF_payback_without_expenses,
	CF_cumulative_payback_without_expenses,

	CF_max };




class cm_cashloan : public compute_module
{
private:
	util::matrix_t<double> cf;
	double ibi_fed_amount;
	double ibi_sta_amount;
	double ibi_uti_amount;
	double ibi_oth_amount;
	double ibi_fed_per;
	double ibi_sta_per;
	double ibi_uti_per;
	double ibi_oth_per;
	double cbi_fed_amount;
	double cbi_sta_amount;
	double cbi_uti_amount;
	double cbi_oth_amount;
public:
	cm_cashloan()
	{
		add_var_info( vtab_standard_financial );
		add_var_info( vtab_standard_loan );
		add_var_info( vtab_oandm );
		add_var_info( vtab_depreciation );
		add_var_info( vtab_tax_credits );
		add_var_info( vtab_payment_incentives );
				
		add_var_info( vtab_cashloan );
	}

	void exec( ) throw( general_error )
	{
		int i;

		bool is_commercial = (as_integer("market")==1);
		bool is_mortgage = (as_integer("mortgage")==1);

		/*
		if (is_commercial) log("commercial market"); else log("residential market");
		if (is_mortgage) log("mortgage loan"); else log("standard loan");
		*/

		int nyears = as_integer("analysis_years");

		// initialize cashflow matrix
		cf.resize_fill( CF_max, nyears+1, 0.0 );

		// initialize energy and revenue
		size_t count = 0;
		ssc_number_t *arrp = 0;
		
		arrp = as_array("energy_net", &count);
		i=0;
		while ( i < nyears && i < (int)count )
		{
			cf.at(CF_energy_net, i+1) = (double) arrp[i];
			i++;
		}

		arrp = as_array("energy_value", &count);
		i=0;
		while ( i < nyears && i < (int)count )
		{
			cf.at(CF_energy_value, i+1) = (double) arrp[i];
			i++;
		}
		
		double year1_fuel_use = as_double("annual_fuel_usage"); // kWht
    	double nameplate = as_double("system_capacity"); // kW
		
		double inflation_rate = as_double("inflation_rate")*0.01;
		double property_tax = as_double("property_tax_rate")*0.01;
		double property_tax_decline_percentage = as_double("prop_tax_assessed_decline");
		double insurance_rate = as_double("insurance_rate")*0.01;
		double salvage_frac = as_double("salvage_percentage")*0.01;
		double federal_tax_rate = as_double("federal_tax_rate")*0.01;
		double state_tax_rate = as_double("state_tax_rate")*0.01;
		double effective_tax_rate = state_tax_rate + (1.0-state_tax_rate)*federal_tax_rate;
		
		double real_discount_rate = as_double("real_discount_rate")*0.01;
		double nom_discount_rate = (1.0 + real_discount_rate) * (1.0 + inflation_rate) - 1.0;


//		double hard_cost = as_double("total_hard_cost");
//		double total_sales_tax = as_double("percent_of_cost_subject_sales_tax")*0.01*hard_cost*as_double("sales_tax_rate")*0.01;
//		double soft_cost = as_double("total_soft_cost") + total_sales_tax;
//		double total_cost = hard_cost + soft_cost;
		double total_cost = as_double("total_installed_cost");
		double property_tax_assessed_value = total_cost * as_double("prop_tax_cost_assessed_percent") * 0.01;

		int loan_term = as_integer("loan_term");
		double loan_rate = as_double("loan_rate")*0.01;
		double debt_frac = as_double("loan_debt")*0.01;
				
		// precompute expenses from annual schedules or value+escalation
		escal_or_annual( CF_om_fixed_expense, nyears, "om_fixed", inflation_rate, 1.0, false, as_double("om_fixed_escal")*0.01 );
		escal_or_annual( CF_om_production_expense, nyears, "om_production", inflation_rate, 0.001, false, as_double("om_production_escal")*0.01 );  
		escal_or_annual( CF_om_capacity_expense, nyears, "om_capacity", inflation_rate, 1.0, false, as_double("om_capacity_escal")*0.01 );  
		escal_or_annual( CF_om_fuel_expense, nyears, "om_fuel_cost", inflation_rate, as_double("system_heat_rate")*0.001, false, as_double("om_fuel_cost_escal")*0.01 );

		escal_or_annual( CF_om_opt_fuel_1_expense, nyears, "om_opt_fuel_1_cost", inflation_rate, 1.0, false, as_double("om_opt_fuel_1_cost_escal")*0.01 );  
		escal_or_annual( CF_om_opt_fuel_2_expense, nyears, "om_opt_fuel_2_cost", inflation_rate, 1.0, false, as_double("om_opt_fuel_2_cost_escal")*0.01 );  

		double om_opt_fuel_1_usage = as_double("om_opt_fuel_1_usage");
		double om_opt_fuel_2_usage = as_double("om_opt_fuel_2_usage");
		
		// ibi fixed
		ibi_fed_amount = as_double("ibi_fed_amount");
		ibi_sta_amount = as_double("ibi_sta_amount");
		ibi_uti_amount = as_double("ibi_uti_amount");
		ibi_oth_amount = as_double("ibi_oth_amount");

		// ibi percent
		ibi_fed_per = as_double("ibi_fed_percent")*0.01*total_cost;
		if (ibi_fed_per > as_double("ibi_fed_percent_maxvalue")) ibi_fed_per = as_double("ibi_fed_percent_maxvalue"); 
		ibi_sta_per = as_double("ibi_sta_percent")*0.01*total_cost;
		if (ibi_sta_per > as_double("ibi_sta_percent_maxvalue")) ibi_sta_per = as_double("ibi_sta_percent_maxvalue"); 
		ibi_uti_per = as_double("ibi_uti_percent")*0.01*total_cost;
		if (ibi_uti_per > as_double("ibi_uti_percent_maxvalue")) ibi_uti_per = as_double("ibi_uti_percent_maxvalue"); 
		ibi_oth_per = as_double("ibi_oth_percent")*0.01*total_cost;
		if (ibi_oth_per > as_double("ibi_oth_percent_maxvalue")) ibi_oth_per = as_double("ibi_oth_percent_maxvalue"); 

		// cbi
		cbi_fed_amount = 1000.0*nameplate*as_double("cbi_fed_amount");
		if (cbi_fed_amount > as_double("cbi_fed_maxvalue")) cbi_fed_amount = as_double("cbi_fed_maxvalue"); 
		cbi_sta_amount = 1000.0*nameplate*as_double("cbi_sta_amount");
		if (cbi_sta_amount > as_double("cbi_sta_maxvalue")) cbi_sta_amount = as_double("cbi_sta_maxvalue"); 
		cbi_uti_amount = 1000.0*nameplate*as_double("cbi_uti_amount");
		if (cbi_uti_amount > as_double("cbi_uti_maxvalue")) cbi_uti_amount = as_double("cbi_uti_maxvalue"); 
		cbi_oth_amount = 1000.0*nameplate*as_double("cbi_oth_amount");
		if (cbi_oth_amount > as_double("cbi_oth_maxvalue")) cbi_oth_amount = as_double("cbi_oth_maxvalue"); 
		
		// precompute pbi
		compute_production_incentive( CF_pbi_fed, nyears, "pbi_fed_amount", "pbi_fed_term", "pbi_fed_escal" );
		compute_production_incentive( CF_pbi_sta, nyears, "pbi_sta_amount", "pbi_sta_term", "pbi_sta_escal" );
		compute_production_incentive( CF_pbi_uti, nyears, "pbi_uti_amount", "pbi_uti_term", "pbi_uti_escal" );
		compute_production_incentive( CF_pbi_oth, nyears, "pbi_oth_amount", "pbi_oth_term", "pbi_oth_escal" );

		// precompute ptc
		compute_production_incentive_IRS_2010_37( CF_ptc_sta, nyears, "ptc_sta_amount", "ptc_sta_term", "ptc_sta_escal" );
		compute_production_incentive_IRS_2010_37( CF_ptc_fed, nyears, "ptc_fed_amount", "ptc_fed_term", "ptc_fed_escal" );
	
		// reduce itc bases
		double federal_itc_basis = total_cost
			- ( as_boolean("ibi_fed_amount_deprbas_fed")  ? ibi_fed_amount : 0 )
			- ( as_boolean("ibi_sta_amount_deprbas_fed")  ? ibi_sta_amount : 0 )
			- ( as_boolean("ibi_uti_amount_deprbas_fed")  ? ibi_uti_amount : 0 )
			- ( as_boolean("ibi_oth_amount_deprbas_fed")  ? ibi_oth_amount : 0 )
			- ( as_boolean("ibi_fed_percent_deprbas_fed") ? ibi_fed_per : 0 )
			- ( as_boolean("ibi_sta_percent_deprbas_fed") ? ibi_sta_per : 0 )
			- ( as_boolean("ibi_uti_percent_deprbas_fed") ? ibi_uti_per : 0 )
			- ( as_boolean("ibi_oth_percent_deprbas_fed") ? ibi_oth_per : 0 )
			- ( as_boolean("cbi_fed_deprbas_fed")  ? cbi_fed_amount : 0 )
			- ( as_boolean("cbi_sta_deprbas_fed")  ? cbi_sta_amount : 0 )
			- ( as_boolean("cbi_uti_deprbas_fed")  ? cbi_uti_amount : 0 )
			- ( as_boolean("cbi_oth_deprbas_fed")  ? cbi_oth_amount : 0 );


		// itc fixed
		double itc_fed_amount = as_double("itc_fed_amount");

		// itc percent - max value used for comparison to qualifying costs
		double itc_fed_frac = as_double("itc_fed_percent")*0.01;
		double itc_fed_per = itc_fed_frac * federal_itc_basis;
		if (itc_fed_per > as_double("itc_fed_percent_maxvalue")) itc_fed_per = as_double("itc_fed_percent_maxvalue");



		double state_itc_basis = total_cost
			- ( as_boolean("ibi_fed_amount_deprbas_sta")  ? ibi_fed_amount : 0 )
			- ( as_boolean("ibi_sta_amount_deprbas_sta")  ? ibi_sta_amount : 0 )
			- ( as_boolean("ibi_uti_amount_deprbas_sta")  ? ibi_uti_amount : 0 )
			- ( as_boolean("ibi_oth_amount_deprbas_sta")  ? ibi_oth_amount : 0 )
			- ( as_boolean("ibi_fed_percent_deprbas_sta") ? ibi_fed_per : 0 )
			- ( as_boolean("ibi_sta_percent_deprbas_sta") ? ibi_sta_per : 0 )
			- ( as_boolean("ibi_uti_percent_deprbas_sta") ? ibi_uti_per : 0 )
			- ( as_boolean("ibi_oth_percent_deprbas_sta") ? ibi_oth_per : 0 )
			- ( as_boolean("cbi_fed_deprbas_sta")  ? cbi_fed_amount : 0 )
			- ( as_boolean("cbi_sta_deprbas_sta")  ? cbi_sta_amount : 0 )
			- ( as_boolean("cbi_uti_deprbas_sta")  ? cbi_uti_amount : 0 )
			- ( as_boolean("cbi_oth_deprbas_sta")  ? cbi_oth_amount : 0 );


		// itc fixed
		double itc_sta_amount = as_double("itc_sta_amount");

		// itc percent - max value used for comparison to qualifying costs
		double itc_sta_frac = as_double("itc_sta_percent")*0.01;
		double itc_sta_per = itc_sta_frac * state_itc_basis;
		if (itc_sta_per > as_double("itc_sta_percent_maxvalue")) itc_sta_per = as_double("itc_sta_percent_maxvalue");


		double federal_depr_basis = federal_itc_basis
			- ( as_boolean("itc_fed_amount_deprbas_fed")   ? 0.5*itc_fed_amount : 0 )
			- ( as_boolean("itc_fed_percent_deprbas_fed")  ? 0.5*itc_fed_per : 0 )
			- ( as_boolean("itc_sta_amount_deprbas_fed")   ? 0.5*itc_sta_amount : 0 )
			- ( as_boolean("itc_sta_percent_deprbas_fed")  ? 0.5*itc_sta_per : 0 );

		double state_depr_basis = state_itc_basis 
			- ( as_boolean("itc_fed_amount_deprbas_sta")   ? 0.5*itc_fed_amount : 0 )
			- ( as_boolean("itc_fed_percent_deprbas_sta")  ? 0.5*itc_fed_per : 0 )
			- ( as_boolean("itc_sta_amount_deprbas_sta")   ? 0.5*itc_sta_amount : 0 )
			- ( as_boolean("itc_sta_percent_deprbas_sta")  ? 0.5*itc_sta_per : 0 );

		if (is_commercial)
		{
			// only compute depreciation for commercial market

			switch( as_integer("depr_sta_type") )
			{
			case 1: depreciation_sched_macrs_half_year( CF_sta_depr_sched, nyears ); break;
			case 2: depreciation_sched_straight_line( CF_sta_depr_sched, nyears, as_integer("depr_sta_sl_years") ); break;
			case 3: 
				{
					size_t arr_len;
					ssc_number_t *arr_cust = as_array( "depr_sta_custom", &arr_len );
					depreciation_sched_custom( CF_sta_depr_sched, nyears, arr_cust, (int)arr_len );
					break;
				}
			}

			switch( as_integer("depr_fed_type") )
			{
			case 1: depreciation_sched_macrs_half_year( CF_fed_depr_sched, nyears ); break;
			case 2: depreciation_sched_straight_line( CF_fed_depr_sched, nyears, as_integer("depr_fed_sl_years") ); break;
			case 3: 
				{
					size_t arr_len;
					ssc_number_t *arr_cust = as_array( "depr_fed_custom", &arr_len );
					depreciation_sched_custom( CF_fed_depr_sched, nyears, arr_cust, (int)arr_len );
					break;
				}
			}
		}
		
		double state_tax_savings = 0.0;
		double federal_tax_savings = 0.0;

		double adjusted_installed_cost = total_cost
			- ibi_fed_amount
			- ibi_sta_amount
			- ibi_uti_amount
			- ibi_oth_amount
			- ibi_fed_per
			- ibi_sta_per
			- ibi_uti_per
			- ibi_oth_per
			- cbi_fed_amount
			- cbi_sta_amount
			- cbi_uti_amount
			- cbi_oth_amount;

		double loan_amount = debt_frac * adjusted_installed_cost;
		double first_cost = adjusted_installed_cost - loan_amount;
		double capital_investment = loan_amount + first_cost;
		
		cf.at(CF_after_tax_net_equity_cost_flow,0) = -first_cost + state_tax_savings + federal_tax_savings;
		cf.at(CF_after_tax_cash_flow,0) = cf.at(CF_after_tax_net_equity_cost_flow,0);

		cf.at(CF_payback_with_expenses,0) = -capital_investment;
		cf.at(CF_cumulative_payback_with_expenses,0) = -capital_investment;
		
		cf.at(CF_payback_without_expenses,0) = -capital_investment;
		cf.at(CF_cumulative_payback_without_expenses,0) = -capital_investment;

		double ibi_total = ibi_fed_amount + ibi_fed_per + ibi_sta_amount + ibi_sta_per + ibi_uti_amount + ibi_uti_per + ibi_oth_amount + ibi_oth_per;
		double cbi_total = cbi_fed_amount + cbi_sta_amount + cbi_uti_amount + cbi_oth_amount;
		double itc_fed_total = itc_fed_amount + itc_fed_per;
		double itc_sta_total = itc_sta_amount + itc_sta_per;

		for (i=1; i<=nyears; i++)
		{			
			// compute expenses
			cf.at(CF_om_production_expense,i) *= cf.at(CF_energy_net,i);
			cf.at(CF_om_capacity_expense,i) *= nameplate;
			cf.at(CF_om_fuel_expense,i) *= year1_fuel_use;
			cf.at(CF_om_opt_fuel_1_expense,i) *= om_opt_fuel_1_usage;
			cf.at(CF_om_opt_fuel_2_expense,i) *= om_opt_fuel_2_usage;
			double decline_percent = 100 - (i-1)*property_tax_decline_percentage;
			cf.at(CF_property_tax_assessed_value,i) = (decline_percent > 0) ? property_tax_assessed_value * decline_percent * 0.01:0.0;
			cf.at(CF_property_tax_expense,i) = cf.at(CF_property_tax_assessed_value,i) * property_tax;
			
			cf.at(CF_insurance_expense,i) = total_cost * insurance_rate * pow( 1 + inflation_rate, i-1 );

			if (i == nyears) /* salvage value handled as negative operating expense in last year */
				cf.at(CF_net_salvage_value,i) = total_cost * salvage_frac;
			else
				cf.at(CF_net_salvage_value,i) = 0.0;

			cf.at(CF_operating_expenses,i) = 
				+ cf.at(CF_om_fixed_expense,i)
				+ cf.at(CF_om_production_expense,i)
				+ cf.at(CF_om_capacity_expense,i)
				+ cf.at(CF_om_fuel_expense,i)
				+ cf.at(CF_om_opt_fuel_1_expense,i)
				+ cf.at(CF_om_opt_fuel_2_expense,i)
				+ cf.at(CF_property_tax_expense,i)
				+ cf.at(CF_insurance_expense,i)
				- cf.at(CF_net_salvage_value,i);

			if (i == nyears) /* salvage value handled as negative operating expense in last year */
				cf.at(CF_operating_expenses,i) -= total_cost * salvage_frac; // updated to be consistent with DHF models - not inflated
//				cf.at(CF_operating_expenses,i) -= total_cost * salvage_frac * pow( 1+inflation_rate, i-1 );
			
			if (is_commercial)
				cf.at(CF_deductible_expenses,i) = -cf.at(CF_operating_expenses,i);  // commercial
			else
				cf.at(CF_deductible_expenses,i) = -cf.at(CF_property_tax_expense,i); // residential

			if (i == 1)
			{
				cf.at(CF_debt_balance,i) = -loan_amount;
				cf.at(CF_debt_payment_interest,i) = loan_amount * loan_rate;
				cf.at(CF_debt_payment_principal,i) = -ppmt( loan_rate,       // Rate
																i,           // Period
																loan_term,   // Number periods
																loan_amount, // Present Value
																0,           // future Value
																0 );         // cash flow at end of period
			}
			else
			{
				if (i <= loan_term) 
				{
					cf.at(CF_debt_balance,i) = cf.at(CF_debt_balance,i-1) + cf.at(CF_debt_payment_principal,i-1);
					cf.at(CF_debt_payment_interest,i) = -loan_rate * cf.at(CF_debt_balance,i);

					if (loan_rate != 0.0)
					{
						cf.at(CF_debt_payment_principal,i) = loan_rate * loan_amount/(1 - pow((1 + loan_rate),-loan_term))
							- cf.at(CF_debt_payment_interest,i);
					}
					else
					{
						cf.at(CF_debt_payment_principal,i) = loan_amount / loan_term - cf.at(CF_debt_payment_interest,i);
					}
				}
			}

			cf.at(CF_debt_payment_total,i) = cf.at(CF_debt_payment_principal,i) + cf.at(CF_debt_payment_interest,i);
			
			// compute pbi total		
			cf.at(CF_pbi_total, i) = cf.at(CF_pbi_fed, i) + cf.at(CF_pbi_sta, i) + cf.at(CF_pbi_uti, i) + cf.at(CF_pbi_oth, i);
			// compute ptc total		
			cf.at(CF_ptc_total, i) = cf.at(CF_ptc_fed, i) + cf.at(CF_ptc_sta, i);
			
			// compute depreciation from basis and precalculated schedule
			cf.at(CF_sta_depreciation,i) = cf.at(CF_sta_depr_sched,i)*state_depr_basis;
			cf.at(CF_fed_depreciation,i) = cf.at(CF_fed_depr_sched,i)*federal_depr_basis;

			
			// ************************************************
			// tax effect on equity (state)

			cf.at(CF_sta_incentive_income_less_deductions, i) =
				+ cf.at(CF_deductible_expenses, i) 
				+ cf.at(CF_pbi_total,i)
				- cf.at(CF_sta_depreciation,i);

			if (i==1) cf.at(CF_sta_incentive_income_less_deductions, i) += ibi_total + cbi_total;

// sales tax is in depreciable bases and is already written off according to depreciation schedule.
//			if (is_commercial && i == 1) cf.at(CF_sta_incentive_income_less_deductions, i) -= total_sales_tax;


			if (is_commercial || is_mortgage) // interest only deductible if residential mortgage or commercial
				cf.at(CF_sta_incentive_income_less_deductions, i) -= cf.at(CF_debt_payment_interest,i);

			cf.at(CF_sta_taxable_income_less_deductions, i) = taxable_incentive_income( i, "sta" )
				+ cf.at(CF_deductible_expenses,i)
				- cf.at(CF_sta_depreciation,i);
			
// sales tax is in depreciable bases and is already written off according to depreciation schedule.
//			if (is_commercial && i == 1) cf.at(CF_sta_taxable_income_less_deductions,i) -= total_sales_tax;

			if (is_commercial || is_mortgage) // interest only deductible if residential mortgage or commercial
				cf.at(CF_sta_taxable_income_less_deductions, i) -= cf.at(CF_debt_payment_interest,i);

			cf.at(CF_sta_tax_savings, i) = cf.at(CF_ptc_sta,i) - state_tax_rate*cf.at(CF_sta_taxable_income_less_deductions,i);
			if (i==1) cf.at(CF_sta_tax_savings, i) += itc_sta_amount + itc_sta_per;
			
			// ************************************************
			//	tax effect on equity (federal)

			cf.at(CF_fed_incentive_income_less_deductions, i) =
				+ cf.at(CF_deductible_expenses, i)
				+ cf.at(CF_pbi_total,i)
				- cf.at(CF_fed_depreciation,i)
				+ cf.at(CF_sta_tax_savings, i);
			
			if (i==1) cf.at(CF_fed_incentive_income_less_deductions, i) += ibi_total + cbi_total;

// sales tax is in depreciable bases and is already written off according to depreciation schedule.
//			if (is_commercial && i == 1) cf.at(CF_fed_incentive_income_less_deductions, i) -= total_sales_tax;
			
			if (is_commercial || is_mortgage) // interest only deductible if residential mortgage or commercial
				cf.at(CF_fed_incentive_income_less_deductions, i) -= cf.at(CF_debt_payment_interest,i);

			cf.at(CF_fed_taxable_income_less_deductions, i) = taxable_incentive_income( i, "fed" )
				+ cf.at(CF_deductible_expenses,i)
				- cf.at(CF_fed_depreciation,i)
				+ cf.at(CF_sta_tax_savings, i);

// sales tax is in depreciable bases and is already written off according to depreciation schedule.
//			if (is_commercial && i == 1) cf.at(CF_fed_taxable_income_less_deductions, i) -= total_sales_tax;

			if (is_commercial || is_mortgage) // interest only deductible if residential mortgage or commercial
				cf.at(CF_fed_taxable_income_less_deductions, i) -= cf.at(CF_debt_payment_interest,i);
			
			cf.at(CF_fed_tax_savings, i) = cf.at(CF_ptc_fed,i) - federal_tax_rate*cf.at(CF_fed_taxable_income_less_deductions,i);
			if (i==1) cf.at(CF_fed_tax_savings, i) += itc_fed_amount + itc_fed_per;
			
			// ************************************************
			// combined tax savings and cost/cash flows
				
			cf.at(CF_sta_and_fed_tax_savings,i) = cf.at(CF_sta_tax_savings, i)+cf.at(CF_fed_tax_savings, i);

			cf.at(CF_after_tax_net_equity_cost_flow, i) =
				+ (is_commercial ? cf.at(CF_deductible_expenses, i) : -cf.at(CF_operating_expenses,i) )
				- cf.at(CF_debt_payment_total, i)
				+ cf.at(CF_pbi_total, i)
				+ cf.at(CF_sta_and_fed_tax_savings,i);

			cf.at(CF_after_tax_cash_flow,i) = 
				cf.at(CF_after_tax_net_equity_cost_flow, i)
				+ ((is_commercial?(1.0 - effective_tax_rate):1.0)*cf.at(CF_energy_value, i));

			if ( is_commercial || is_mortgage )
				cf.at(CF_payback_with_expenses,i) =
					cf.at(CF_after_tax_cash_flow,i)
					+ cf.at(CF_debt_payment_interest,i) * (1-effective_tax_rate)
					+ cf.at(CF_debt_payment_principal,i);
			else
				cf.at(CF_payback_with_expenses,i) =
					cf.at(CF_after_tax_cash_flow,i)
					+ cf.at(CF_debt_payment_interest,i)
					+ cf.at(CF_debt_payment_principal,i);


			cf.at(CF_cumulative_payback_with_expenses,i) = 
				cf.at(CF_cumulative_payback_with_expenses,i-1)
				+cf.at(CF_payback_with_expenses,i);
	
			if ( is_commercial || is_mortgage )
				cf.at(CF_payback_without_expenses,i) =
					+ cf.at(CF_after_tax_cash_flow,i)
					+ cf.at(CF_debt_payment_interest,i) * (1.0 - effective_tax_rate)
					+ cf.at(CF_debt_payment_principal,i)
					- cf.at(CF_deductible_expenses,i)
					+ cf.at(CF_deductible_expenses,i) * effective_tax_rate;
			else
				cf.at(CF_payback_without_expenses,i) =
					+ cf.at(CF_after_tax_cash_flow,i)
					+ cf.at(CF_debt_payment_interest,i)
					+ cf.at(CF_debt_payment_principal,i)
					- cf.at(CF_deductible_expenses,i)
					+ cf.at(CF_deductible_expenses,i) * effective_tax_rate;


			cf.at(CF_cumulative_payback_without_expenses,i) =
				+ cf.at(CF_cumulative_payback_without_expenses,i-1)
				+ cf.at(CF_payback_without_expenses,i);	
		}
		
		double npv_energy_real = npv( CF_energy_net, nyears, real_discount_rate );
//		if (npv_energy_real == 0.0) throw general_error("lcoe real failed because energy npv is zero");
//		double lcoe_real = -( cf.at(CF_after_tax_net_equity_cost_flow,0) + npv(CF_after_tax_net_equity_cost_flow, nyears, nom_discount_rate) ) * 100 / npv_energy_real;
		double lcoe_real = -( cf.at(CF_after_tax_net_equity_cost_flow,0) + npv(CF_after_tax_net_equity_cost_flow, nyears, nom_discount_rate) ) * 100;
		if (npv_energy_real == 0.0) 
		{
			lcoe_real = std::numeric_limits<double>::quiet_NaN();
		}
		else
		{
			lcoe_real /= npv_energy_real;
		}

		double npv_energy_nom = npv( CF_energy_net, nyears, nom_discount_rate );
//		if (npv_energy_nom == 0.0) throw general_error("lcoe nom failed because energy npv is zero");
//		double lcoe_nom = -( cf.at(CF_after_tax_net_equity_cost_flow,0) + npv(CF_after_tax_net_equity_cost_flow, nyears, nom_discount_rate) ) * 100 / npv_energy_nom;
		double lcoe_nom = -( cf.at(CF_after_tax_net_equity_cost_flow,0) + npv(CF_after_tax_net_equity_cost_flow, nyears, nom_discount_rate) ) * 100;
		if (npv_energy_nom == 0.0) 
		{
			lcoe_nom = std::numeric_limits<double>::quiet_NaN();
		}
		else
		{
			lcoe_nom /= npv_energy_nom;
		}

		double net_present_value = cf.at(CF_after_tax_cash_flow, 0) + npv(CF_after_tax_cash_flow, nyears, nom_discount_rate );

		double payback = compute_payback( CF_cumulative_payback_with_expenses, CF_payback_with_expenses, nyears );

		// save outputs


	double npv_fed_ptc = npv(CF_ptc_fed,nyears,nom_discount_rate);
	double npv_sta_ptc = npv(CF_ptc_sta,nyears,nom_discount_rate);

	npv_fed_ptc /= (1.0 - effective_tax_rate);
	npv_sta_ptc /= (1.0 - effective_tax_rate);

	double lcoptc_fed_nom=0.0;
	if (npv_energy_nom != 0) lcoptc_fed_nom = npv_fed_ptc / npv_energy_nom * 100.0;
	double lcoptc_fed_real=0.0;
	if (npv_energy_real != 0) lcoptc_fed_real = npv_fed_ptc / npv_energy_real * 100.0;

	double lcoptc_sta_nom=0.0;
	if (npv_energy_nom != 0) lcoptc_sta_nom = npv_sta_ptc / npv_energy_nom * 100.0;
	double lcoptc_sta_real=0.0;
	if (npv_energy_real != 0) lcoptc_sta_real = npv_sta_ptc / npv_energy_real * 100.0;

	assign("sv_lcoptc_fed_nom", var_data((ssc_number_t) lcoptc_fed_nom));
	assign("sv_lcoptc_fed_real", var_data((ssc_number_t) lcoptc_fed_real));
	assign("sv_lcoptc_sta_nom", var_data((ssc_number_t) lcoptc_sta_nom));
	assign("sv_lcoptc_sta_real", var_data((ssc_number_t) lcoptc_sta_real));



	double wacc = 0.0;
	wacc = (1.0-debt_frac)*nom_discount_rate + debt_frac*loan_rate*(1.0-effective_tax_rate);

	wacc *= 100.0;
	effective_tax_rate *= 100.0;


	assign("sv_wacc", var_data( (ssc_number_t) wacc));
	assign("sv_effective_tax_rate", var_data( (ssc_number_t) effective_tax_rate));



		assign( "cf_length", var_data( (ssc_number_t) nyears+1 ));

		assign( "payback", var_data((ssc_number_t)payback) );
		assign( "lcoe_real", var_data((ssc_number_t)lcoe_real) );
		assign( "lcoe_nom", var_data((ssc_number_t)lcoe_nom) );
		assign( "npv",  var_data((ssc_number_t)net_present_value) );

		assign("sv_first_year_energy_net", var_data((ssc_number_t) cf.at(CF_energy_net,1)));
		double kWhperkW = 0.0;
		if (nameplate > 0) kWhperkW = cf.at(CF_energy_net,1) / nameplate;
		assign( "sv_capacity_factor", var_data((ssc_number_t) (kWhperkW / 87.6)) );
		assign( "sv_kwh_per_kw", var_data((ssc_number_t) kWhperkW) );


		assign( "depr_basis_fed", var_data((ssc_number_t)federal_depr_basis ));
		assign( "depr_basis_sta", var_data((ssc_number_t)state_depr_basis ));
		assign( "discount_nominal", var_data((ssc_number_t)(nom_discount_rate*100.0) ));		
//		assign( "sales_tax_deduction", var_data((ssc_number_t)total_sales_tax ));		
		assign( "adj_installed_cost", var_data((ssc_number_t)adjusted_installed_cost ));		
		
		save_cf( CF_energy_net, nyears, "cf_energy_net" );
		save_cf( CF_energy_value, nyears, "cf_energy_value" );


// real estate value added 6/24/13
		for (int i=1;i<nyears+1;i++)
		{
			double rr = 1.0;
			if (nom_discount_rate != -1.0) rr = 1.0/(1.0+nom_discount_rate);
			double result = 0;
			for (int j=nyears;j>=i;j--) 
			result = rr * result + cf.at(CF_energy_value,j);
			cf.at(CF_value_added,i) = result*rr + cf.at(CF_net_salvage_value,i);
		}
		save_cf( CF_value_added, nyears, "cf_value_added" );



		save_cf( CF_om_fixed_expense, nyears, "cf_om_fixed_expense" );
		save_cf( CF_om_production_expense, nyears, "cf_om_production_expense" );
		save_cf( CF_om_capacity_expense, nyears, "cf_om_capacity_expense" );
		save_cf( CF_om_fuel_expense, nyears, "cf_om_fuel_expense" );
		save_cf( CF_om_opt_fuel_1_expense, nyears, "cf_om_opt_fuel_1_expense" );
		save_cf( CF_om_opt_fuel_2_expense, nyears, "cf_om_opt_fuel_2_expense" );
		save_cf( CF_property_tax_assessed_value, nyears, "cf_property_tax_assessed_value" );
		save_cf( CF_property_tax_expense, nyears, "cf_property_tax_expense" );
		save_cf( CF_insurance_expense, nyears, "cf_insurance_expense" );
		save_cf( CF_net_salvage_value, nyears, "cf_net_salvage_value" );
		save_cf( CF_operating_expenses, nyears, "cf_operating_expenses" );

		save_cf( CF_deductible_expenses, nyears, "cf_deductible_expenses");
		
		save_cf( CF_debt_balance, nyears, "cf_debt_balance" );
		save_cf( CF_debt_payment_interest, nyears, "cf_debt_payment_interest" );
		save_cf( CF_debt_payment_principal, nyears, "cf_debt_payment_principal" );
		save_cf( CF_debt_payment_total, nyears, "cf_debt_payment_total" );

		assign( "ibi_total_fed", var_data((ssc_number_t) (ibi_fed_amount + ibi_fed_per)));
		assign( "ibi_total_sta", var_data((ssc_number_t) (ibi_sta_amount + ibi_sta_per)));
		assign( "ibi_total_uti", var_data((ssc_number_t) (ibi_uti_amount + ibi_uti_per)));
		assign( "ibi_total_oth", var_data((ssc_number_t) (ibi_oth_amount + ibi_oth_per)));
		assign( "ibi_total", var_data((ssc_number_t) ibi_total));

		assign( "cbi_total_fed", var_data((ssc_number_t) (cbi_fed_amount)));
		assign( "cbi_total_sta", var_data((ssc_number_t) (cbi_sta_amount)));
		assign( "cbi_total_uti", var_data((ssc_number_t) (cbi_uti_amount)));
		assign( "cbi_total_oth", var_data((ssc_number_t) (cbi_oth_amount)));
		assign( "cbi_total", var_data((ssc_number_t) cbi_total));
		
		
		save_cf( CF_pbi_fed, nyears, "cf_pbi_total_fed" );
		save_cf( CF_pbi_sta, nyears, "cf_pbi_total_sta" );
		save_cf( CF_pbi_uti, nyears, "cf_pbi_total_uti" );
		save_cf( CF_pbi_oth, nyears, "cf_pbi_total_oth" );
		save_cf( CF_pbi_total, nyears, "cf_pbi_total" );
	
		save_cf( CF_ptc_fed, nyears, "cf_ptc_fed" );
		save_cf( CF_ptc_sta, nyears, "cf_ptc_sta" );
		save_cf( CF_ptc_total, nyears, "cf_ptc_total" );

		assign( "itc_fed_total", var_data((ssc_number_t) itc_fed_total));
		assign( "itc_sta_total", var_data((ssc_number_t) itc_sta_total));
		assign( "itc_total", var_data((ssc_number_t) (itc_fed_total+itc_sta_total)));
	
		save_cf( CF_sta_depr_sched, nyears, "cf_sta_depr_sched" );
		save_cf( CF_sta_depreciation, nyears, "cf_sta_depreciation" );
		save_cf( CF_sta_incentive_income_less_deductions, nyears, "cf_sta_incentive_income_less_deductions" );
		save_cf( CF_sta_taxable_income_less_deductions, nyears, "cf_sta_taxable_income_less_deductions" );
		save_cf( CF_sta_tax_savings, nyears, "cf_sta_tax_savings" );
	
		save_cf( CF_fed_depr_sched, nyears, "cf_fed_depr_sched" );
		save_cf( CF_fed_depreciation, nyears, "cf_fed_depreciation" );
		save_cf( CF_fed_incentive_income_less_deductions, nyears, "cf_fed_incentive_income_less_deductions" );
		save_cf( CF_fed_taxable_income_less_deductions, nyears, "cf_fed_taxable_income_less_deductions" );
		save_cf( CF_fed_tax_savings, nyears, "cf_fed_tax_savings" );

		save_cf( CF_sta_and_fed_tax_savings, nyears, "cf_sta_and_fed_tax_savings" );
		save_cf( CF_after_tax_net_equity_cost_flow, nyears, "cf_after_tax_net_equity_cost_flow" );
		save_cf( CF_after_tax_cash_flow, nyears, "cf_after_tax_cash_flow" );

		save_cf( CF_payback_with_expenses, nyears, "cf_payback_with_expenses" );
		save_cf( CF_cumulative_payback_with_expenses, nyears, "cf_cumulative_payback_with_expenses" );
	
		save_cf( CF_payback_without_expenses, nyears, "cf_payback_without_expenses" );
		save_cf( CF_cumulative_payback_without_expenses, nyears, "cf_cumulative_payback_without_expenses" );

	// for cost stacked bars
		//npv(CF_energy_value, nyears, nom_discount_rate)
		// present value of o and m value - note - present value is distributive - sum of pv = pv of sum
		double pvAnnualOandM = npv(CF_om_fixed_expense, nyears, nom_discount_rate);
		double pvFixedOandM = npv(CF_om_capacity_expense, nyears, nom_discount_rate);
		double pvVariableOandM = npv(CF_om_production_expense, nyears, nom_discount_rate);
		double pvFuelOandM = npv(CF_om_fuel_expense, nyears, nom_discount_rate);
		double pvOptFuel1OandM = npv(CF_om_opt_fuel_1_expense, nyears, nom_discount_rate);
		double pvOptFuel2OandM = npv(CF_om_opt_fuel_2_expense, nyears, nom_discount_rate);
	//	double pvWaterOandM = NetPresentValue(sv[svNominalDiscountRate], cf[cfAnnualWaterCost], analysis_period);

		assign( "present_value_oandm",  var_data((ssc_number_t)(pvAnnualOandM + pvFixedOandM + pvVariableOandM + pvFuelOandM))); // + pvWaterOandM);

		assign( "present_value_oandm_nonfuel", var_data((ssc_number_t)(pvAnnualOandM + pvFixedOandM + pvVariableOandM)));
		assign( "present_value_fuel", var_data((ssc_number_t)(pvFuelOandM + pvOptFuel1OandM + pvOptFuel2OandM)));

		// present value of insurance and property tax
		double pvInsurance = npv(CF_insurance_expense, nyears, nom_discount_rate);
		double pvPropertyTax = npv(CF_property_tax_expense, nyears, nom_discount_rate);

		assign( "present_value_insandproptax", var_data((ssc_number_t)(pvInsurance + pvPropertyTax)));
	}

/* These functions can be placed in common financial library with matrix and constants passed? */

	void save_cf(int cf_line, int nyears, const std::string &name)
	{
		ssc_number_t *arrp = allocate( name, nyears+1 );
		for (int i=0;i<=nyears;i++)
			arrp[i] = (ssc_number_t)cf.at(cf_line, i);
	}

	double compute_payback( int cf_cpb, int cf_pb, int nyears )
	{	
		double dPayback = 1e99; // report as > analysis period
		bool bolPayback = false;
		int iPayback = 0;
		int i = 1; 
		while ((i<=nyears) && (!bolPayback))
		{
			if (cf.at(cf_cpb,i) > 0)
			{
				bolPayback = true;
				iPayback = i;
			}
			i++;
		}

		if (bolPayback)
		{
			dPayback = iPayback;
			if (cf.at(cf_pb, iPayback) != 0.0)
				dPayback -= cf.at(cf_cpb,iPayback) / cf.at(cf_pb,iPayback);
		}

		return dPayback;
	}

	double npv( int cf_line, int nyears, double rate ) throw ( general_error )
	{		
		if (rate <= -1.0) throw general_error("cannot calculate NPV with discount rate less or equal to -1.0");

		double rr = 1/(1+rate);
		double result = 0;
		for (int i=nyears;i>0;i--)
			result = rr * result + cf.at(cf_line,i);

		return result*rr;
	}

	void compute_production_incentive( int cf_line, int nyears, const std::string &s_val, const std::string &s_term, const std::string &s_escal )
	{
		size_t len = 0;
		ssc_number_t *parr = as_array(s_val, &len);
		int term = as_integer(s_term);
		double escal = as_double(s_escal)/100.0;

		if (len == 1)
		{
			for (int i=1;i<=nyears;i++)
				cf.at(cf_line, i) = (i <= term) ? parr[0] * cf.at(CF_energy_net,i) * pow(1 + escal, i-1) : 0.0;
		}
		else
		{
			for (int i=1;i<=nyears && i <= (int)len;i++)
				cf.at(cf_line, i) = parr[i-1]*cf.at(CF_energy_net,i);
		}
	}

		void compute_production_incentive_IRS_2010_37( int cf_line, int nyears, const std::string &s_val, const std::string &s_term, const std::string &s_escal )
	{
		// rounding based on IRS document and emails from John and Matt from DHF Financials 2/24/2011 and DHF model v4.4
		size_t len = 0;
		ssc_number_t *parr = as_array(s_val, &len);
		int term = as_integer(s_term);
		double escal = as_double(s_escal)/100.0;

		if (len == 1)
		{
			for (int i=1;i<=nyears;i++)
				cf.at(cf_line, i) = (i <= term) ? cf.at(CF_energy_net,i) / 1000.0 * round_dhf(1000.0 * parr[0] * pow(1 + escal, i-1)) : 0.0;
		}
		else
		{
			for (int i=1;i<=nyears && i <= (int)len;i++)
				cf.at(cf_line, i) = parr[i-1]*cf.at(CF_energy_net,i);
		}
	}


	void single_or_schedule( int cf_line, int nyears, double scale, const std::string &name )
	{
		size_t len = 0;
		ssc_number_t *p = as_array(name, &len);
		for (int i=1;i<=(int)len && i <= nyears;i++)
			cf.at(cf_line, i) = scale*p[i-1];
	}
	
	void single_or_schedule_check_max( int cf_line, int nyears, double scale, const std::string &name, const std::string &maxvar )
	{
		double max = as_double(maxvar);
		size_t len = 0;
		ssc_number_t *p = as_array(name, &len);
		for (int i=1;i<=(int)len && i <= nyears;i++)
			cf.at(cf_line, i) = min( scale*p[i-1], max );
	}

	double taxable_incentive_income(int year, const std::string &fed_or_sta)
	{
		double ti = 0.0;
		if (year==1) 
		{
			if ( as_boolean("ibi_fed_amount_tax_"+fed_or_sta) ) ti += ibi_fed_amount;
			if ( as_boolean("ibi_sta_amount_tax_"+fed_or_sta) ) ti += ibi_sta_amount;
			if ( as_boolean("ibi_uti_amount_tax_"+fed_or_sta) ) ti += ibi_uti_amount;
			if ( as_boolean("ibi_oth_amount_tax_"+fed_or_sta) ) ti += ibi_oth_amount;
		
			if ( as_boolean("ibi_fed_percent_tax_"+fed_or_sta) ) ti += ibi_fed_per;
			if ( as_boolean("ibi_sta_percent_tax_"+fed_or_sta) ) ti += ibi_sta_per;
			if ( as_boolean("ibi_uti_percent_tax_"+fed_or_sta) ) ti += ibi_uti_per;
			if ( as_boolean("ibi_oth_percent_tax_"+fed_or_sta) ) ti += ibi_oth_per;

			if ( as_boolean("cbi_fed_tax_"+fed_or_sta) ) ti += cbi_fed_amount;
			if ( as_boolean("cbi_sta_tax_"+fed_or_sta) ) ti += cbi_sta_amount;
			if ( as_boolean("cbi_uti_tax_"+fed_or_sta) ) ti += cbi_uti_amount;
			if ( as_boolean("cbi_oth_tax_"+fed_or_sta) ) ti += cbi_oth_amount;
		}

		if ( as_boolean("pbi_fed_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_fed, year );
		if ( as_boolean("pbi_sta_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_sta, year );
		if ( as_boolean("pbi_uti_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_uti, year );
		if ( as_boolean("pbi_oth_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_oth, year );

		return ti;
	}
	
	void depreciation_sched_macrs_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.2000; break;
			case 2: factor = 0.3200; break;
			case 3: factor = 0.1920; break;
			case 4: factor = 0.1152; break;
			case 5: factor = 0.1152; break;
			case 6: factor = 0.0576; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}

	void depreciation_sched_straight_line( int cf_line, int nyears, int slyears )
	{
		double depr_per_year = (slyears!=0) ? 1.0 / ((double)slyears) : 0;
		for (int i=1; i<=nyears; i++)
			cf.at(cf_line, i) = (i<=slyears) ? depr_per_year : 0.0;
	}
	
	void depreciation_sched_custom( int cf_line, int nyears, ssc_number_t *custp, int custp_len )
	{
		// note - allows for greater than or less than 100% depreciation - warning to user in samsim
		if (custp_len < 2)
		{
			cf.at(cf_line, 1) = custp[0]/100.0;
		}
		else
		{
			for (int i=1;i<=nyears && i-1 < custp_len;i++)
				cf.at(cf_line, i) = custp[i-1]/100.0;
		}
	}

	void escal_or_annual( int cf_line, int nyears, const std::string &variable, 
			double inflation_rate, double scale, bool as_rate=true, double escal = 0.0)
	{
		size_t count;
		ssc_number_t *arrp = as_array(variable, &count);

		if (as_rate)
		{
			if (count == 1)
			{
				escal = inflation_rate + scale*arrp[0];
				for (int i=0; i < nyears; i++)
					cf.at(cf_line, i+1) = pow( 1+escal, i );
			}
			else
			{
				for (int i=0; i < nyears && i < (int)count; i++)
					cf.at(cf_line, i+1) = 1 + arrp[i]*scale;
			}
		}
		else
		{
			if (count == 1)
			{
				for (int i=0;i<nyears;i++)
					cf.at(cf_line, i+1) = arrp[0]*scale*pow( 1+escal+inflation_rate, i );
			}
			else
			{
				for (int i=0;i<nyears && i<(int)count;i++)
					cf.at(cf_line, i+1) = arrp[i]*scale;
			}
		}
	}

	double min( double a, double b )
	{
		return (a < b) ? a : b;
	}

};

DEFINE_MODULE_ENTRY( cashloan, "Residential/Commerical Finance model.", 1 );
