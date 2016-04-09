/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "heapManager.h"
#include "Filecode.h"
#if CONFIG_USE_OPTION_TABLE
#include <option.h>
#include "option_table.h"
#endif


#include <northbridge/amd/agesa/agesawrapper.h>

#define FILECODE PROC_GNB_PCIE_FAMILY_0X15_F15PCIECOMPLEXCONFIG_FILECODE

static const PCIe_PORT_DESCRIPTOR PortList [] = {
	{
		0,
		PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 3, 3),
		PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 2, 5,
				HotplugDisabled,
				PcieGenMaxSupported,
				PcieGenMaxSupported,
				AspmDisabled, 0x01, 0)
	},
	/* Initialize Port descriptor (PCIe port, Lanes 1, PCI Device Number 2, ...) */
	{
		0,
		PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 2, 2),
		PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 2, 4,
				HotplugDisabled,
				PcieGenMaxSupported,
				PcieGenMaxSupported,
				AspmDisabled, 0x02, 0)
	},
	/* Initialize Port descriptor (PCIe port, Lanes 2, PCI Device Number 2, ...) */
	{
		0,
		PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 1, 1),
		PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 2, 3,
				HotplugDisabled,
				PcieGenMaxSupported,
				PcieGenMaxSupported,
				AspmDisabled, 0x03, 0)
	},
	/* Initialize Port descriptor (PCIe port, Lanes 3, PCI Device Number 2, ...) */
	{
		0,
		PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 0, 0),
		PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 2, 2,
				HotplugDisabled,
				PcieGenMaxSupported,
				PcieGenMaxSupported,
				AspmDisabled, 0x04, 0)
	},
	/* Initialize Port descriptor (PCIe port, Lanes 4-7, PCI Device Number 4, ...) */
	{
		DESCRIPTOR_TERMINATE_LIST,
		PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 4, 7),
		PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 2, 1,
				HotplugDisabled,
				PcieGenMaxSupported,
				PcieGenMaxSupported,
				AspmDisabled, 0x05, 0)
	}
};

static const PCIe_DDI_DESCRIPTOR DdiList [] = {
	/* DP0 to HDMI0/DP */
	{
		0,
		PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 8, 11),
		PCIE_DDI_DATA_INITIALIZER (ConnectorTypeLvds, Aux1, Hdp1)
	},
	/* DP1 to FCH */
	{
		0,
		PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 12, 15),
		PCIE_DDI_DATA_INITIALIZER (ConnectorTypeLvds, Aux2, Hdp2)
	},
	/* DP2 to HDMI1/DP */
	{
		DESCRIPTOR_TERMINATE_LIST,
		PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 16, 19),
		PCIE_DDI_DATA_INITIALIZER (ConnectorTypeCrt, Aux3, Hdp3)
	},
};

static const PCIe_COMPLEX_DESCRIPTOR PcieComplex = {
	.Flags        = DESCRIPTOR_TERMINATE_LIST,
	.SocketId     = 0,
	.PciePortList = PortList,
	.DdiLinkList  = DdiList
};

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitEarly
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      *InitEarly
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/

static AGESA_STATUS OemInitEarly(AMD_EARLY_PARAMS * InitEarly)
{
	AGESA_STATUS            Status;
	PCIe_COMPLEX_DESCRIPTOR *PcieComplexListPtr;
 	#if CONFIG_USE_OPTION_TABLE
	PCIe_DDI_DESCRIPTOR     *PcieDdiDescriptor;
	unsigned                DP_type=0;
	#endif

	ALLOCATE_HEAP_PARAMS AllocHeapParams;

	/* GNB PCIe topology Porting */

	/*  */
	/* Allocate buffer for PCIe_COMPLEX_DESCRIPTOR , PCIe_PORT_DESCRIPTOR and PCIe_DDI_DESCRIPTOR */
	/*  */
	AllocHeapParams.RequestedBufferSize = sizeof(PcieComplex);
	#if CONFIG_USE_OPTION_TABLE
	AllocHeapParams.RequestedBufferSize += sizeof(PCIe_DDI_DESCRIPTOR) * 3;
	#endif

	AllocHeapParams.BufferHandle = AMD_MEM_MISC_HANDLES_START;
	AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
	Status = HeapAllocateBuffer (&AllocHeapParams, &InitEarly->StdHeader);
	ASSERT(Status == AGESA_SUCCESS);

	PcieComplexListPtr  =  (PCIe_COMPLEX_DESCRIPTOR *) AllocHeapParams.BufferPtr;
	LibAmdMemCopy  (PcieComplexListPtr, &PcieComplex, sizeof(PcieComplex), &InitEarly->StdHeader);

	#if CONFIG_USE_OPTION_TABLE
	PcieDdiDescriptor  =  (PCIe_DDI_DESCRIPTOR *) (AllocHeapParams.BufferPtr + sizeof(PcieComplex));
	LibAmdMemCopy  (PcieDdiDescriptor, DdiList, sizeof(PCIe_DDI_DESCRIPTOR) * 3, &InitEarly->StdHeader);

	if (get_option(&DP_type, "DP0_type") == CB_SUCCESS) {
		switch(DP_type) {
		case 0:			/* DP */
			PcieDdiDescriptor[0].Ddi.ConnectorType = ConnectorTypeDP;
			break;
		case 1:			/* S-DVI */
			PcieDdiDescriptor[0].Ddi.ConnectorType = ConnectorTypeSingleLinkDVI;
			break;
		case 2:			/* D-DVI */
			PcieDdiDescriptor[0].Ddi.ConnectorType = ConnectorTypeDualLinkDVI;
			break;
		case 3:			/* HDMI */
			PcieDdiDescriptor[0].Ddi.ConnectorType = ConnectorTypeHDMI;
			break;
		case 4:			/* CRT */
//			PcieDdiDescriptor[0].Ddi.ConnectorType = ConnectorTypeCrt;
			break;
		case 5:			/* LVDS */
			PcieDdiDescriptor[0].Ddi.ConnectorType = ConnectorTypeLvds;
			break;
		}
	}

	if (get_option(&DP_type, "DP1_type") == CB_SUCCESS) {
		switch(DP_type) {
		case 0:			/* DP */
			PcieDdiDescriptor[1].Ddi.ConnectorType = ConnectorTypeDP;
			break;
		case 1:			/* S-DVI */
			PcieDdiDescriptor[1].Ddi.ConnectorType = ConnectorTypeSingleLinkDVI;
			break;
		case 2:			/* D-DVI */
			PcieDdiDescriptor[1].Ddi.ConnectorType = ConnectorTypeDualLinkDVI;
			break;
		case 3:			/* HDMI */
			PcieDdiDescriptor[1].Ddi.ConnectorType = ConnectorTypeHDMI;
			break;
		case 4:			/* CRT */
//			PcieDdiDescriptor[1].Ddi.ConnectorType = ConnectorTypeCrt;
			break;
		case 5:			/* LVDS */
			PcieDdiDescriptor[1].Ddi.ConnectorType = ConnectorTypeLvds;
			break;
		}
	}

	/* Make sure the data are correct. */
	//hexdump(DdiList, sizeof(PCIe_DDI_DESCRIPTOR) * 3);
	//hexdump(PcieDdiDescriptor, sizeof(PCIe_DDI_DESCRIPTOR) * 3);
	PcieComplexListPtr->DdiLinkList = PcieDdiDescriptor;
	#endif

	InitEarly->GnbConfig.PcieComplexList = PcieComplexListPtr;
	return AGESA_SUCCESS;
}

static AGESA_STATUS OemInitMid(AMD_MID_PARAMS * InitMid)
{
	/* 0 iGpuVgaAdapter, 1 iGpuVgaNonAdapter; */
	InitMid->GnbMidConfiguration.iGpuVgaMode = 0;
	return AGESA_SUCCESS;
}

const struct OEM_HOOK OemCustomize = {
	.InitEarly = OemInitEarly,
	.InitMid = OemInitMid,
};
