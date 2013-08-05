//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: inverter.cpp,v $
// $Revision: 1.7 $
// $Date: 2012/10/15 15:44:37 $
//
// Permission to use, copy, modify, distribute, and sell this software and its
// documentation for any purpose is hereby granted without fee, provided that
// the above copyright notice appear in all copies and that both the
// copyright notice and this permission notice appear in supporting
// documentation.  No representations are made about the suitability of this
// software for any purpose.  It is provided "as is" without express or 
// implied warranty.
//

#include "fslurp.h"

static Inverter::DeviceTypeMap deviceTypeMap[] =
{
	{ 0xFE, "FRONIUS IG 15 1-phase inverter" },
	{ 0xFD, "FRONIUS IG 20 1-phase inverter" },
	{ 0xFC, "FRONIUS IG 30 1-phase inverter" },
	{ 0xFB, "FRONIUS IG 30 Dummy, Dummy inverter" },
	{ 0xFA, "FRONIUS IG 40 1-phase inverter" },
	{ 0x88, "FRONIUS IG 50 1-phase inverter" },
	{ 0xF9, "FRONIUS IG 60 / IG 60 HV, 1-phase inverter" },
	{ 0xF6, "FRONIUS IG 300 3-phase inverter" },
	{ 0xF5, "FRONIUS IG 400 3-phase inverter" },
	{ 0xF4, "FRONIUS IG 500 3-phase inverter" },
	{ 0xF3, "FRONIUS IG 60 / IG 60 HV, 1-phase inverter" },
	{ 0xEE, "FRONIUS IG 2000 1-phase inverter" },
	{ 0xED, "FRONIUS IG 3000 1-phase inverter" },
	{ 0xEB, "FRONIUS IG 4000 1-phase inverter" },
	{ 0xEA, "FRONIUS IG 5100 1-phase inverter" },
	{ 0xE5, "FRONIUS IG 2500-LV 1-phase inverter" },
	{ 0xE3, "FRONIUS IG 4500-LV 1-phase inverter" },
	{ 0xDF, "Fronius IG Plus 11.4-3 Delta 3-phase inverter" },
	{ 0xDE, "Fronius IG Plus 11.4-1 UNI 1-phase inverter" },
	{ 0xDD, "Fronius IG Plus 10.0-1 UNI 1-phase inverter" },
	{ 0xDC, "Fronius IG Plus 7.5-1 UNI 1-phase inverter" },
	{ 0xDB, "Fronius IG Plus 6.0-1 UNI 1-phase inverter" },
	{ 0xDA, "Fronius IG Plus 5.0-1 UNI 1-phase inverter" },
	{ 0xD9, "Fronius IG Plus 3.8-1 UNI 1-phase inverter" },
	{ 0xD8, "Fronius IG Plus 3.0-1 UNI 1-phase inverter" },
	{ 0xD7, "Fronius IG Plus 120-3 3-phase inverter" },
	{ 0xD6, "Fronius IG Plus 70-2 2-phase inverter" },
	{ 0xD5, "Fronius IG Plus 70-1 1-phase inverter" },
	{ 0xD4, "Fronius IG Plus 35-1 1-phase inverter" },
	{ 0xD3, "Fronius IG Plus 150-3 3-phase inverter" },
	{ 0xD2, "Fronius IG Plus 100-2 2-phase inverter" },
	{ 0xD1, "Fronius IG Plus 100-1 1-phase inverter" },
	{ 0xD0, "Fronius IG Plus 50-1 1-phase inverter" },
	{ 0xCF, "Fronius IG Plus 12.0-3 WYE277 3-phase inverter" },
	{ 0xC1, "Fronius IG TL 3.6 1-phase inverter" },
	{ 0xC0, "Fronius IG TL 5.0 1-phase inverter" },
	{ 0xBF, "Fronius IG TL 4.0 1-phase inverter" },
	{ 0xBE, "Fronius IG TL 3.0 1-phase inverter" },
	{ 0x3C, "Fronius IG TL 4.6 1-phase inverter" },
	{ 0x2C, "Fronius IG TL Dummy, Dummy inverter" },
	{ 0x83, "Fronius IG Plus 25V-1 1-phase inverter" },
	{ 0x89, "Fronius IG Plus 30V-1 1-phase inverter" },
	{ 0xB1, "Fronius IG Plus 35V-1 1-phase inverter" },
	{ 0xB0, "Fronius IG Plus 50V-1 1-phase inverter" },
	{ 0xAF, "Fronius IG Plus 70V-1 1-phase inverter" },
	{ 0xAE, "Fronius IG Plus 70V-2 2-phase inverter" },
	{ 0xAD, "Fronius IG Plus 100V-1 1-phase inverter" },
	{ 0xAC, "Fronius IG Plus 100V-2 2-phase inverter" },
	{ 0x84, "Fronius IG Plus 100V-3 3-phase inverter" },
	{ 0xAB, "Fronius IG Plus 120V-3 3-phase inverter" },
	{ 0xAA, "Fronius IG Plus 150V-3 3-phase inverter" },
	{ 0xA9, "Fronius IG Plus V 3.0-1 UNI 1-phase inverter" },
	{ 0xA8, "Fronius IG Plus V 3.8-1 UNI 1-phase inverter" },
	{ 0xA7, "Fronius IG Plus V 5.0-1 UNI 1-phase inverter" },
	{ 0xA6, "Fronius IG Plus V 6.0-1 UNI 1-phase inverter" },
	{ 0xA5, "Fronius IG Plus V 7.5-1 UNI 1-phase inverter" },
	{ 0xA4, "Fronius IG Plus V 10.0-1 UNI 1-phase inverter" },
	{ 0x87, "Fronius IG Plus V 10.0-3 Delta 3-phase inverter" },
	{ 0xA3, "Fronius IG Plus V 11.4-1 UNI 1-phase inverter" },
	{ 0xA2, "Fronius IG Plus V 11.4-3 DELTA 3-phase inverter" },
	{ 0xA1, "Fronius IG Plus V 12.0-3 WYE 3-phase inverter" },
	{ 0xA0, "Fronius IG Plus 50V-1 Dummy inverter" },
	{ 0x9F, "Fronius IG Plus 100V-2 Dummy inverter" },
	{ 0x9E, "Fronius IG Plus 150V-3 Dummy inverter" },
	{ 0x9D, "Fronius IG Plus V 3.8-1 Dummy inverter" },
	{ 0x9C, "Fronius IG Plus V 7.5-1 Dummy inverter" },
	{ 0x9B, "Fronius IG Plus V 12.0-3 Dummy inverter" },
	{ 0xBC, "Fronius CL 36.0 3-phase inverter" },
	{ 0xBD, "Fronius CL 48.0 3-phase inverter" },
	{ 0xC9, "Fronius CL 60.0 3-phase inverter" },
	{ 0xB9, "Fronius CL 36.0 WYE277 3-phase inverter" },
	{ 0xBA, "Fronius CL 48.0 WYE277 3-phase inverter" },
	{ 0xBB, "Fronius CL 60.0 WYE277 3-phase inverter" },
	{ 0xB6, "Fronius CL 33.3 Delta 3-phase inverter" },
	{ 0xB7, "Fronius CL 44.4 Delta 3-phase inverter" },
	{ 0xB8, "Fronius CL 55.5 Delta 3-phase inverter" },
	{ 0x9A, "Fronius CL 60.0 Dummy inverter" },
	{ 0x99, "Fronius CL 55.5 Delta Dummy inverter" },
	{ 0x98, "Fronius CL 60.0 WYE277 Dummy inverter" },
};

Device::DeviceOption
Inverter::getDeviceOption(void) const
{
	return DeviceOptionInverter;
}

const char*
Inverter::getModel(void) const
{
	unsigned int	i;
	unsigned char	deviceType;

	deviceType = deviceTypeResponse_.getDeviceType();

	for (i = 0; i < NUM_ELEMS(deviceTypeMap); i++)
	{
		if (deviceTypeMap[i].id == deviceType)
		{
			return deviceTypeMap[i].model;
		}
	}

	return "unknown inverter type";
}

unsigned char
Inverter::maximumFieldLabelWidth(void) const
{
	// FIX THIS, DS:
	return 28;
}

void
Inverter::report(Args& args)
{
	NowData		nowData;
	CumulativeData	dayData;
	CumulativeData	yearData;
	CumulativeData	totalData;

	deviceInfoReport(maximumFieldLabelWidth(), args);

	switch(args.getReportType())
	{
		case Args::ReportTypeNow:
			getNowData(nowData);
			displayNowData(args, nowData);
			break;
		case Args::ReportTypeDay:
			getDayData(dayData);
			displayCumulativeData(args, "today", dayData);
			break;
		case Args::ReportTypeYear:
			getYearData(yearData);
			displayCumulativeData(args, "this year", yearData);
			break;
		case Args::ReportTypeTotal:
			getTotalData(totalData);
			displayCumulativeData(args, "total", totalData);
			break;
		case Args::ReportTypeAll:
			getNowData(nowData);
			getDayData(dayData);
			if (supportsYearData())
			{
				getYearData(yearData);
			}
			getTotalData(totalData);
			displayNowData(args, nowData);
			displayCumulativeData(args, "today", dayData);
			if (supportsYearData())
			{
				displayCumulativeData(
					args, "this year", yearData);
			}
			displayCumulativeData(args, "total", totalData);
			break;
		default:
			assert(0);
	}

	if (args.getDelimiter() != NULL)
	{
		printf("\n");
	}
}

void
Inverter::getNowData(NowData& nowData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_POWER_NOW,
		nowData.power);
	getMeasuredValue(
		FRONIUS_CMD_GET_AC_CURRENT_NOW,
		nowData.acCurrent);
	getMeasuredValue(
		FRONIUS_CMD_GET_AC_VOLTAGE_NOW,
		nowData.acVoltage);
	getMeasuredValue(
		FRONIUS_CMD_GET_AC_FREQUENCY_NOW,
		nowData.acFrequency);
	getMeasuredValue(
		FRONIUS_CMD_GET_DC_CURRENT_NOW,
		nowData.dcCurrent);
	getMeasuredValue(
		FRONIUS_CMD_GET_DC_VOLTAGE_NOW,
		nowData.dcVoltage);
}

void
Inverter::getDayData(CumulativeData& dayData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_ENERGY_DAY,
		dayData.energy);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_POWER_DAY,
		dayData.maximumPower);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_DAY,
		dayData.acVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_DAY,
		dayData.acVoltageMinimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_DAY,
		dayData.dcVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_OPERATING_HOURS_DAY,
		dayData.operatingHours);
}

void
Inverter::getYearData(CumulativeData& yearData)
{
	assert(supportsYearData());

	getMeasuredValue(
		FRONIUS_CMD_GET_ENERGY_YEAR,
		yearData.energy);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_POWER_YEAR,
		yearData.maximumPower);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_YEAR,
		yearData.acVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_YEAR,
		yearData.acVoltageMinimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_YEAR,
		yearData.dcVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_OPERATING_HOURS_YEAR,
		yearData.operatingHours);
}

void
Inverter::getTotalData(CumulativeData& totalData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_ENERGY_TOTAL,
		totalData.energy);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL,
		totalData.maximumPower);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL,
		totalData.acVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL,
		totalData.acVoltageMinimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL,
		totalData.dcVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL,
		totalData.operatingHours);
}

void
Inverter::displayNowData(Args& args, NowData& data)
{
	const char*	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %s watts\n",
			maximumFieldLabelWidth(),
			"Power now",
			Utils::getValue(data.power));
		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"AC current now",
			Utils::getValue(data.acCurrent));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"AC voltage now",
			Utils::getValue(data.acVoltage));
		printf("%-*s : %s Hz\n",
			maximumFieldLabelWidth(),
			"AC frequency now",
			Utils::getValue(data.acFrequency));
		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"DC current now",
			Utils::getValue(data.dcCurrent));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"DC voltage now",
			Utils::getValue(data.dcVoltage));
	}
	else
	{
		printf("%s%s", Utils::getValue(data.power), delimiter);
		printf("%s%s", Utils::getValue(data.acCurrent), delimiter);
		printf("%s%s", Utils::getValue(data.acVoltage), delimiter);
		printf("%s%s", Utils::getValue(data.acFrequency), delimiter);
		printf("%s%s", Utils::getValue(data.dcCurrent), delimiter);
		printf("%s%s", Utils::getValue(data.dcVoltage), delimiter);
	}
}

void
Inverter::displayCumulativeData(
	Args& args,
	const char* timePeriod,
	CumulativeData& data)
{
	const char*	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %s kilowatt hours\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Energy ", timePeriod),
			Utils::getThousandsValue(data.energy));
		printf("%-*s : %s watts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Maximum power ", timePeriod),
			Utils::getValue(data.maximumPower));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Maximum AC voltage ", timePeriod),
			Utils::getValue(data.acVoltageMaximum));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Minimum AC voltage ", timePeriod),
			Utils::getValue(data.acVoltageMinimum));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Maximum DC voltage ", timePeriod),
			Utils::getValue(data.dcVoltageMaximum));
		printf("%-*s : %s hours\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Operating time ", timePeriod),
			Utils::getHoursValue(data.operatingHours));
	}
	else
	{
		printf("%s%s",
			Utils::getThousandsValue(data.energy), delimiter);
		printf("%s%s",
			Utils::getValue(data.maximumPower), delimiter);
		printf("%s%s",
			Utils::getValue(data.acVoltageMaximum), delimiter);
		printf("%s%s",
			Utils::getValue(data.acVoltageMinimum), delimiter);
		printf("%s%s",
			Utils::getValue(data.dcVoltageMaximum), delimiter);
		printf("%s%s",
			Utils::getHoursValue(data.operatingHours), delimiter);
	}
}

void
Inverter::displayNowHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Power now%s", delimiter);
	printf("AC current now%s", delimiter);
	printf("AC voltage now%s", delimiter);
	printf("AC frequency now%s", delimiter);
	printf("DC current now%s", delimiter);
	printf("DC voltage now%s", delimiter);
}

void
Inverter::displayDayHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Energy today (KWH)%s", delimiter);
	printf("Maximum power today (W)%s", delimiter);
	printf("Maximum AC voltage today (V)%s", delimiter);
	printf("Minimum AC voltage today (V)%s", delimiter);
	printf("Maximum DC voltage today (V)%s", delimiter);
	printf("Operating time today (H)%s", delimiter);
}

void
Inverter::displayYearHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Energy this year (KWH)%s", delimiter);
	printf("Maximum power this year (W)%s", delimiter);
	printf("Maximum AC voltage this year (V)%s", delimiter);
	printf("Minimum AC voltage this year (V)%s", delimiter);
	printf("Maximum DC voltage this year (V)%s", delimiter);
	printf("Operating time this year (H)%s", delimiter);
}

void
Inverter::displayTotalHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Energy total (KWH)%s", delimiter);
	printf("Maximum power total (W)%s", delimiter);
	printf("Maximum AC voltage total (V)%s", delimiter);
	printf("Minimum AC voltage total (V)%s", delimiter);
	printf("Maximum DC voltage total (V)%s", delimiter);
	printf("Operating time total (H)%s", delimiter);
}
