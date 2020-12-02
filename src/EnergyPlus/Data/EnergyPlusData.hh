// EnergyPlus, Copyright (c) 1996-2020, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef EnergyPlusData_hh_INCLUDED
#define EnergyPlusData_hh_INCLUDED

// C++ Headers
#include <memory>
#include <string>
#include <unordered_map>

// EnergyPlus Headers
#include <EnergyPlus/Data/BaseData.hh>
#include <EnergyPlus/EnergyPlus.hh>
#include <EnergyPlus/IOFiles.hh>

namespace EnergyPlus {

// forward declare all structs
struct AirflowNetworkBalanceManagerData;
struct AirLoopHVACDOASData;
struct AirSystemsData;
struct BaseboardElectricData;
struct BaseboardRadiatorData;
struct BoilersData;
struct BoilerSteamData;
struct BranchInputManagerData;
struct BranchNodeConnectionsData;
struct BSDFWindowData;
struct ChilledCeilingPanelSimpleData;
struct ChillerAbsorberData;
struct ChillerElectricEIRData;
struct ChillerExhaustAbsorptionData;
struct ChillerGasAbsorptionData;
struct ChillerIndirectAbsoprtionData;
struct ChillerReformulatedEIRData;
struct CondenserLoopTowersData;
struct ConstructionData;
struct ContaminantBalanceData;
struct ConvectionCoefficientsData;
struct ConvergParamsData;
struct CoolTowerData;
struct CostEstimateManagerData;
struct CrossVentMgrData;
struct CTElectricGeneratorData;
struct CurveManagerData;
struct DataAirLoopData;
struct DataBranchAirLoopPlantData;
struct DataDaylightingDevicesData;
struct DataGlobal;
struct DataTimingsData;
struct DataWaterData;
struct DaylightingData;
struct DaylightingDevicesData;
struct DaylightingManagerData;
struct DefineEquipData;
struct DemandManagerData;
struct DesiccantDehumidifiersData;
struct DisplacementVentMgrData;
struct DualDuctData;
struct DXCoilsData;
struct DXFEarClippingData;
struct EarthTubeData;
struct EconomicLifeCycleCostData;
struct EconomicTariffData;
struct EcoRoofManagerData;
struct EIRPlantLoopHeatPumpsData;
struct ElectPwrSvcMgrData;
struct ElectricBaseboardRadiatorData;
struct EMSManagerData;
struct EnvironmentData;
struct ErrorTrackingData;
struct EvaporativeCoolersData;
struct EvaporativeFluidCoolersData;
struct ExteriorEnergyUseData;
struct ExternalInterfaceData;
struct FanCoilUnitsData;
struct FansData;
struct FaultsManagerData;
struct FluidCoolersData;
struct FluidPropertiesData;
struct FuelCellElectricGeneratorData;
struct FurnacesData;
struct GeneralData;
struct GeneralRoutinesData;
struct GeneratorFuelSupplyData;
struct GeneratorsData;
struct GlobalNamesData;
struct GroundHeatExchangerData;
struct HeatBalanceAirMgrData;
struct HeatBalanceData;
struct HeatBalanceIntRadExchgData;
struct HeatBalanceKivaMgrData;
struct HeatBalanceMgrData;
struct HeatBalFanSysData;
struct HeatBalFiniteDiffMgr;
struct HeatBalHAMTMgrData;
struct HeatBalInternalHeatGainsData;
struct HeatBalSurfData;
struct HeatBalSurfMgr;
struct HeatingCoilsData;
struct HeatPumpWaterToWaterCOOLINGData;
struct HVACCtrlData;
struct HVACGlobalsData;
struct IPShortCutsData;
struct LoopNodeData;
struct MoistureBalanceData;
struct MoistureBalanceEMPDData;
struct OutputsData;
struct PhotovoltaicsData;
struct PipesData;
struct PlantChillersData;
struct PlantValvesData;
struct ReportFlagData;
struct RoomAirModelData;
struct RuntimeLanguageData;
struct SetPointManagerData;
struct ShadowCombData;
struct SimulationManagerData;
struct SingleDuctData;
struct SizingData;
struct SizingManagerData;
struct SolarCollectorsData;
struct SolarReflectionManagerData;
struct SolarShadingData;
struct SplitterComponentData;
struct SteamBaseboardRadiatorData;
struct SteamCoilsData;
struct SurfaceColorData;
struct SurfaceGeometryData;
struct SurfaceGroundHeatExchangersData;
struct SurfaceListsData;
struct SurfacesData;
struct SwimmingPoolsData;
struct SystemAvailabilityManagerData;
struct SystemVarsData;
struct ThermalChimneysData;
struct ThermalComfortsData;
struct TranspiredCollectorData;
struct UCSDSharedData;
struct UFADManagerData;
struct UnitarySystemsData;
struct UnitHeatersData;
struct UnitVentilatorsData;
struct UserDefinedComponentsData;
struct UtilityRoutinesData;
struct VariableSpeedCoilsData;
struct VentilatedSlabData;
struct ViewFactorInfoData;
struct WaterCoilsData;
struct WaterManagerData;
struct WaterThermalTanksData;
struct WaterToAirHeatPumpData;
struct WaterToAirHeatPumpSimpleData;
struct WaterUseData;
struct WeatherManagerData;
struct WindowACData;
struct WindowComplexManagerData;
struct WindowEquivalentLayerData;
struct WindowEquivLayerData;
struct WindowManagerData;
struct WindTurbineData;
struct ZoneAirLoopEquipmentManagerData;
struct ZoneContaminantPredictorCorrectorData;
struct ZoneDehumidifierData;
struct ZoneEquipmentManagerData;
struct ZonePlenumData;
struct ZoneTempPredictorCorrectorData;

struct EnergyPlusData : BaseGlobalStruct {

    IOFiles files;

    // module globals
    std::unique_ptr<AirflowNetworkBalanceManagerData> dataAirflowNetworkBalanceManager;
    std::unique_ptr<AirLoopHVACDOASData> dataAirLoopHVACDOAS;
    std::unique_ptr<AirSystemsData> dataAirSystemsData;
    std::unique_ptr<BaseboardElectricData> dataBaseboardElectric;
    std::unique_ptr<BaseboardRadiatorData> dataBaseboardRadiator;
    std::unique_ptr<BoilersData> dataBoilers;
    std::unique_ptr<BoilerSteamData> dataBoilerSteam;
    std::unique_ptr<BranchInputManagerData> dataBranchInputManager;
    std::unique_ptr<BranchNodeConnectionsData> dataBranchNodeConnections;
    std::unique_ptr<BSDFWindowData> dataBSDFWindow;
    std::unique_ptr<ChilledCeilingPanelSimpleData> dataChilledCeilingPanelSimple;
    std::unique_ptr<ChillerAbsorberData> dataChillerAbsorber;
    std::unique_ptr<ChillerElectricEIRData> dataChillerElectricEIR;
    std::unique_ptr<ChillerExhaustAbsorptionData> dataChillerExhaustAbsorption;
    std::unique_ptr<ChillerGasAbsorptionData> dataChillerGasAbsorption;
    std::unique_ptr<ChillerIndirectAbsoprtionData> dataChillerIndirectAbsorption;
    std::unique_ptr<ChillerReformulatedEIRData> dataChillerReformulatedEIR;
    std::unique_ptr<CondenserLoopTowersData> dataCondenserLoopTowers;
    std::unique_ptr<ConstructionData> dataConstruction;
    std::unique_ptr<ContaminantBalanceData> dataContaminantBalance;
    std::unique_ptr<ConvectionCoefficientsData> dataConvectionCoefficient;
    std::unique_ptr<ConvergParamsData> dataConvergeParams;
    std::unique_ptr<CoolTowerData> dataCoolTower;
    std::unique_ptr<CostEstimateManagerData> dataCostEstimateManager;
    std::unique_ptr<CrossVentMgrData> dataCrossVentMgr;
    std::unique_ptr<CTElectricGeneratorData> dataCTElectricGenerator;
    std::unique_ptr<CurveManagerData> dataCurveManager;
    std::unique_ptr<DataAirLoopData> dataAirLoop;
    std::unique_ptr<DataBranchAirLoopPlantData> dataBranchAirLoopPlant;
    std::unique_ptr<DataDaylightingDevicesData> dataDaylightingDevicesData;
    std::unique_ptr<DataGlobal> dataGlobal;
    std::unique_ptr<DataTimingsData> dataTimingsData;
    std::unique_ptr<DataWaterData> dataWaterData;
    std::unique_ptr<DaylightingData> dataDaylightingData;
    std::unique_ptr<DaylightingDevicesData> dataDaylightingDevices;
    std::unique_ptr<DaylightingManagerData> dataDaylightingManager;
    std::unique_ptr<DefineEquipData> dataDefineEquipment;
    std::unique_ptr<DemandManagerData> dataDemandManager;
    std::unique_ptr<DesiccantDehumidifiersData> dataDesiccantDehumidifiers;
    std::unique_ptr<DisplacementVentMgrData> dataDispVentMgr;
    std::unique_ptr<DualDuctData> dataDualDuct;
    std::unique_ptr<DXCoilsData> dataDXCoils;
    std::unique_ptr<DXFEarClippingData> dataDXFEarClipping;
    std::unique_ptr<EarthTubeData> dataEarthTube;
    std::unique_ptr<EconomicLifeCycleCostData> dataEconLifeCycleCost;
    std::unique_ptr<EconomicTariffData> dataEconTariff;
    std::unique_ptr<EcoRoofManagerData> dataEcoRoofMgr;
    std::unique_ptr<EIRPlantLoopHeatPumpsData> dataEIRPlantLoopHeatPump;
    std::unique_ptr<ElectPwrSvcMgrData> dataElectPwrSvcMgr;
    std::unique_ptr<ElectricBaseboardRadiatorData> dataElectBaseboardRad;
    std::unique_ptr<EMSManagerData> dataEMSMgr;
    std::unique_ptr<EnvironmentData> dataEnvrn;
    std::unique_ptr<ErrorTrackingData> dataErrTracking;
    std::unique_ptr<EvaporativeCoolersData> dataEvapCoolers;
    std::unique_ptr<EvaporativeFluidCoolersData> dataEvapFluidCoolers;
    std::unique_ptr<ExteriorEnergyUseData> dataExteriorEnergyUse;
    std::unique_ptr<ExternalInterfaceData> dataExternalInterface;
    std::unique_ptr<FanCoilUnitsData> dataFanCoilUnits;
    std::unique_ptr<FansData> dataFans;
    std::unique_ptr<FaultsManagerData> dataFaultsMgr;
    std::unique_ptr<FluidCoolersData> dataFluidCoolers;
    std::unique_ptr<FluidPropertiesData> dataFluidProps;
    std::unique_ptr<FuelCellElectricGeneratorData> dataFuelCellElectGen;
    std::unique_ptr<FurnacesData> dataFurnaces;
    std::unique_ptr<GeneralData> dataGeneral;
    std::unique_ptr<GeneralRoutinesData> dataGeneralRoutines;
    std::unique_ptr<GeneratorFuelSupplyData> dataGeneratorFuelSupply;
    std::unique_ptr<GeneratorsData> dataGenerator;
    std::unique_ptr<GlobalNamesData> dataGlobalNames;
    std::unique_ptr<GroundHeatExchangerData> dataGroundHeatExchanger;
    std::unique_ptr<HeatBalanceAirMgrData> dataHeatBalAirMgr;
    std::unique_ptr<HeatBalanceData> dataHeatBal;
    std::unique_ptr<HeatBalanceIntRadExchgData> dataHeatBalIntRadExchg;
    std::unique_ptr<HeatBalanceKivaMgrData> dataHeatBalKivaMgr;
    std::unique_ptr<HeatBalanceMgrData> dataHeatBalMgr;
    std::unique_ptr<HeatBalFanSysData> dataHeatBalFanSys;
    std::unique_ptr<HeatBalFiniteDiffMgr> dataHeatBalFiniteDiffMgr;
    std::unique_ptr<HeatBalHAMTMgrData> dataHeatBalHAMTMgr;
    std::unique_ptr<HeatBalInternalHeatGainsData> dataHeatBalIntHeatGains;
    std::unique_ptr<HeatBalSurfData> dataHeatBalSurf;
    std::unique_ptr<HeatBalSurfMgr> dataHeatBalSurfMgr;
    std::unique_ptr<HeatingCoilsData> dataHeatingCoils;
    std::unique_ptr<HeatPumpWaterToWaterCOOLINGData> dataHPWaterToWaterClg;
    std::unique_ptr<HVACCtrlData> dataHVACCtrl;
    std::unique_ptr<HVACGlobalsData> dataHVACGlobal;
    std::unique_ptr<IPShortCutsData> dataIPShortCut;
    std::unique_ptr<LoopNodeData> dataLoopNodes;
    std::unique_ptr<MoistureBalanceData> dataMstBal;
    std::unique_ptr<MoistureBalanceEMPDData> dataMstBalEMPD;
    std::unique_ptr<OutputsData> dataOutput;
    std::unique_ptr<PhotovoltaicsData> dataPhotovoltaic;
    std::unique_ptr<PipesData> dataPipes;
    std::unique_ptr<PlantChillersData> dataPlantChillers;
    std::unique_ptr<PlantValvesData> dataPlantValves;
    std::unique_ptr<ReportFlagData> dataReportFlag;
    std::unique_ptr<RoomAirModelData> dataRoomAirMod;
    std::unique_ptr<RuntimeLanguageData> dataRuntimeLang;
    std::unique_ptr<SetPointManagerData> dataSetPointManager;
    std::unique_ptr<ShadowCombData> dataShadowComb;
    std::unique_ptr<SimulationManagerData> dataSimulationManager;
    std::unique_ptr<SingleDuctData> dataSingleDuct;
    std::unique_ptr<SizingData> dataSize;
    std::unique_ptr<SizingManagerData> dataSizingManager;
    std::unique_ptr<SolarCollectorsData> dataSolarCollectors;
    std::unique_ptr<SolarReflectionManagerData> dataSolarReflectionManager;
    std::unique_ptr<SolarShadingData> dataSolarShading;
    std::unique_ptr<SplitterComponentData> dataSplitterComponent;
    std::unique_ptr<SteamBaseboardRadiatorData> dataSteamBaseboardRadiator;
    std::unique_ptr<SteamCoilsData> dataSteamCoils;
    std::unique_ptr<SurfaceColorData> dataSurfColor;
    std::unique_ptr<SurfaceGeometryData> dataSurfaceGeometry;
    std::unique_ptr<SurfaceGroundHeatExchangersData> dataSurfaceGroundHeatExchangers;
    std::unique_ptr<SurfaceListsData> dataSurfLists;
    std::unique_ptr<SurfacesData> dataSurface;
    std::unique_ptr<SwimmingPoolsData> dataSwimmingPools;
    std::unique_ptr<SystemAvailabilityManagerData> dataSystemAvailabilityManager;
    std::unique_ptr<SystemVarsData> dataSysVars;
    std::unique_ptr<ThermalChimneysData> dataThermalChimneys;
    std::unique_ptr<ThermalComfortsData> dataThermalComforts;
    std::unique_ptr<TranspiredCollectorData> dataTranspiredCollector;
    std::unique_ptr<UCSDSharedData> dataUCSDShared;
    std::unique_ptr<UFADManagerData> dataUFADManager;
    std::unique_ptr<UnitarySystemsData> dataUnitarySystems;
    std::unique_ptr<UnitHeatersData> dataUnitHeaters;
    std::unique_ptr<UnitVentilatorsData> dataUnitVentilators;
    std::unique_ptr<UserDefinedComponentsData> dataUserDefinedComponents;
    std::unique_ptr<UtilityRoutinesData> dataUtilityRoutines;
    std::unique_ptr<VariableSpeedCoilsData> dataVariableSpeedCoils;
    std::unique_ptr<VentilatedSlabData> dataVentilatedSlab;
    std::unique_ptr<ViewFactorInfoData> dataViewFactor;
    std::unique_ptr<WaterCoilsData> dataWaterCoils;
    std::unique_ptr<WaterManagerData> dataWaterManager;
    std::unique_ptr<WaterThermalTanksData> dataWaterThermalTanks;
    std::unique_ptr<WaterToAirHeatPumpData> dataWaterToAirHeatPump;
    std::unique_ptr<WaterToAirHeatPumpSimpleData> dataWaterToAirHeatPumpSimple;
    std::unique_ptr<WaterUseData> dataWaterUse;
    std::unique_ptr<WeatherManagerData> dataWeatherManager;
    std::unique_ptr<WindowACData> dataWindowAC;
    std::unique_ptr<WindowComplexManagerData> dataWindowComplexManager;
    std::unique_ptr<WindowEquivalentLayerData> dataWindowEquivalentLayer;
    std::unique_ptr<WindowEquivLayerData> dataWindowEquivLayer;
    std::unique_ptr<WindowManagerData> dataWindowManager;
    std::unique_ptr<WindTurbineData> dataWindTurbine;
    std::unique_ptr<ZoneAirLoopEquipmentManagerData> dataZoneAirLoopEquipmentManager;
    std::unique_ptr<ZoneContaminantPredictorCorrectorData> dataZoneContaminantPredictorCorrector;
    std::unique_ptr<ZoneDehumidifierData> dataZoneDehumidifier;
    std::unique_ptr<ZoneEquipmentManagerData> dataZoneEquipmentManager;
    std::unique_ptr<ZonePlenumData> dataZonePlenum;
    std::unique_ptr<ZoneTempPredictorCorrectorData> dataZoneTempPredictorCorrector;

    EnergyPlusData();

    // Cannot safely copy or delete this until we eradicate all remaining
    // calls to IOFiles::getSingleton and IOFiles::setSingleton
    EnergyPlusData(const EnergyPlusData &) = delete;
    EnergyPlusData(EnergyPlusData &&) = delete;

    void clear_state() override;

};

}
#endif
