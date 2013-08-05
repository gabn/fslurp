//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: device.cpp,v $
// $Revision: 1.6 $
// $Date: 2012/10/20 01:37:07 $
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

Device::Device(void)
{
	deviceID_ = ~0;
	comm_ = NULL;
	supportsYearData_ = false;
}

void
Device::initialize(Comm* comm, unsigned char deviceID, bool supportsYearData)
{
	Request	request;

	CHECK_PTR(comm);
	comm_ = comm;

	request.setDeviceOption(getDeviceOption());
	request.setDeviceNumber(deviceID);
	request.setCommand(FRONIUS_CMD_GET_DEVICE_TYPE);

	request.send(*comm_);
	deviceTypeResponse_.receive(*comm_);

	deviceID_ = deviceID;
	supportsYearData_ = supportsYearData;
}

unsigned char
Device::getDeviceID(void) const
{
	return deviceID_;
}

bool
Device::supportsYearData(void) const
{
	return supportsYearData_;
}

unsigned char
Device::getDeviceType(void) const
{
	return deviceTypeResponse_.getDeviceType();
}

void
Device::getMeasuredValue(
	unsigned char command,
	MeasuredValue& measuredValue)
{
	Request			request;

	// FIX THIS, DS: these are fixed length.
	// provide the buffer from the specialization
	// and compile it in.
	MeasuredValueResponse	response;

	request.setDeviceOption(getDeviceOption());
	request.setDeviceNumber(getDeviceID());
	request.setCommand(command);

	CHECK_PTR(comm_);
	request.send(*comm_);
	response.receive(*comm_);

	measuredValue.lsb = response.getLSB();
	measuredValue.msb = response.getMSB();
	measuredValue.exponent = response.getExponent();
}

void
Device::reportHeader(Args& args)
{
	reportHeader(args, supportsYearData_);
}

// For use by iterators, where we there might not
// be a device instance to ask.
void
Device::reportHeader(Args& args, bool supportsYearData)
{
	const char*		delimiter;
	Args::ReportType	reportType;

	reportType = args.getReportType();
	assert(reportType != Args::ReportTypeSystem);

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	// FIX THIS, DS: this block is repeated in Interface::reportHeader
	printf("Time%s", delimiter);
	printf("Device Number%s", delimiter);
	printf("Device Type%s", delimiter);
	printf("Description%s", delimiter);

	switch (args.getReportType())
	{
		case Args::ReportTypeNow:
			displayNowHeader(args);
			break;

		case Args::ReportTypeDay:
			displayDayHeader(args);
			break;

		case Args::ReportTypeYear:
			displayYearHeader(args);
			break;

		case Args::ReportTypeTotal:
			displayTotalHeader(args);
			break;

		case Args::ReportTypeAll:
			displayNowHeader(args);
			displayDayHeader(args);
			if (supportsYearData)
			{
				displayYearHeader(args);
			}
			displayTotalHeader(args);
			break;

		default:
			assert(0);
	}

	printf("\n");
}

void
Device::deviceInfoReport(
	unsigned char fieldLabelWidth,
	Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %d\n",
			fieldLabelWidth,
			"Device number",
			getDeviceID());
		printf("%-*s : 0x%02X\n",
			fieldLabelWidth,
			"Device type",
			getDeviceType());
		printf("%-*s : %s\n",
			fieldLabelWidth,
			"Device description",
			getModel());
	}
	else
	{
		Utils::displayTimestamp(maximumFieldLabelWidth(), args);
		printf("%d%s", getDeviceID(), delimiter);
		printf("0x%02X%s", getDeviceType(), delimiter);
		printf("%s%s", getModel(), delimiter);

		if (args.getReportType() == Args::ReportTypeSystem)
		{
			printf("\n");
		}
	}
}
