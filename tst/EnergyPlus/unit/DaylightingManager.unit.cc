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

// Google Test Headers
#include <gtest/gtest.h>

// C++ Headers
#include <array>

// ObjexxFCL Headers
#include <ObjexxFCL/Array1D.hh>

// EnergyPlus Headers
#include "Fixtures/EnergyPlusFixture.hh"
#include <EnergyPlus/Construction.hh>
#include <EnergyPlus/Data/EnergyPlusData.hh>
#include <EnergyPlus/DataDaylighting.hh>
#include <EnergyPlus/DataEnvironment.hh>
#include <EnergyPlus/DataGlobals.hh>
#include <EnergyPlus/DataHeatBalance.hh>
#include <EnergyPlus/DataSurfaces.hh>
#include <EnergyPlus/DaylightingDevices.hh>
#include <EnergyPlus/DaylightingManager.hh>
#include <EnergyPlus/General.hh>
#include <EnergyPlus/HeatBalanceIntRadExchange.hh>
#include <EnergyPlus/HeatBalanceManager.hh>
#include <EnergyPlus/IOFiles.hh>
#include <EnergyPlus/InputProcessing/InputProcessor.hh>
#include <EnergyPlus/InternalHeatGains.hh>
#include <EnergyPlus/ScheduleManager.hh>
#include <EnergyPlus/SurfaceGeometry.hh>

using namespace EnergyPlus;
using namespace EnergyPlus::Construction;
using namespace EnergyPlus::DaylightingDevices;
using namespace EnergyPlus::DaylightingManager;
using namespace EnergyPlus::DataDaylighting;
using namespace EnergyPlus::DataSurfaces;
using namespace EnergyPlus::HeatBalanceManager;

TEST_F(EnergyPlusFixture, DaylightingManager_GetInputDaylightingControls_Test)
{
    using HeatBalanceManager::GetZoneData;

    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    West Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:Controls,                                                                                           ",
        "    West Zone_DaylCtrl,      !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    West Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    West Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    1.0,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.;                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt1,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;
    GetZoneData(foundErrors);
    ASSERT_FALSE(foundErrors);

    int numObjs = inputProcessor->getNumObjectsFound("Daylighting:Controls");
    GetInputDayliteRefPt(foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(1, TotRefPoints);

    GetDaylightingControls(numObjs, foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(1, numObjs);

    EXPECT_EQ("WEST ZONE_DAYLCTRL", ZoneDaylight(1).Name);
    EXPECT_EQ("WEST ZONE", ZoneDaylight(1).ZoneName);
    EXPECT_EQ(SplitFluxDaylighting, ZoneDaylight(1).DaylightMethod);
    EXPECT_EQ(Continuous, ZoneDaylight(1).LightControlType);

    EXPECT_EQ(0.3, ZoneDaylight(1).MinPowerFraction);
    EXPECT_EQ(0.2, ZoneDaylight(1).MinLightFraction);
    EXPECT_EQ(1, ZoneDaylight(1).LightControlSteps);
    EXPECT_EQ(1.0, ZoneDaylight(1).LightControlProbability);

    EXPECT_EQ(1, ZoneDaylight(1).glareRefPtNumber);
    EXPECT_EQ(180., ZoneDaylight(1).ViewAzimuthForGlare);
    EXPECT_EQ(20., ZoneDaylight(1).MaxGlareallowed);
    EXPECT_EQ(0, ZoneDaylight(1).DElightGriddingResolution);

    EXPECT_EQ(1, ZoneDaylight(1).TotalDaylRefPoints);

    EXPECT_EQ(1, ZoneDaylight(1).DaylRefPtNum(1));
    EXPECT_EQ(1., ZoneDaylight(1).FracZoneDaylit(1));
    EXPECT_EQ(500., ZoneDaylight(1).IllumSetPoint(1));
}

TEST_F(EnergyPlusFixture, DaylightingManager_GetInputDaylightingControls_3RefPt_Test)
{
    using HeatBalanceManager::GetZoneData;

    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    West Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:Controls,                                                                                           ",
        "    West Zone_DaylCtrl,      !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    West Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    West Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.35,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    400.,                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "    West Zone_DaylRefPt2,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.4,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.,                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "    West Zone_DaylRefPt3,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.25,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    450.;                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt1,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    2.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.7;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt2,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.8;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt3,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    4.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;
    GetZoneData(foundErrors);
    ASSERT_FALSE(foundErrors);

    int numObjs = inputProcessor->getNumObjectsFound("Daylighting:Controls");
    GetInputDayliteRefPt(foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(3, TotRefPoints);

    GetDaylightingControls(numObjs, foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(1, numObjs);

    EXPECT_EQ("WEST ZONE_DAYLCTRL", ZoneDaylight(1).Name);
    EXPECT_EQ("WEST ZONE", ZoneDaylight(1).ZoneName);
    EXPECT_EQ(SplitFluxDaylighting, ZoneDaylight(1).DaylightMethod);
    EXPECT_EQ(Continuous, ZoneDaylight(1).LightControlType);

    EXPECT_EQ(0.3, ZoneDaylight(1).MinPowerFraction);
    EXPECT_EQ(0.2, ZoneDaylight(1).MinLightFraction);
    EXPECT_EQ(1, ZoneDaylight(1).LightControlSteps);
    EXPECT_EQ(1.0, ZoneDaylight(1).LightControlProbability);

    EXPECT_EQ(1, ZoneDaylight(1).glareRefPtNumber);
    EXPECT_EQ(180., ZoneDaylight(1).ViewAzimuthForGlare);
    EXPECT_EQ(20., ZoneDaylight(1).MaxGlareallowed);
    EXPECT_EQ(0, ZoneDaylight(1).DElightGriddingResolution);

    EXPECT_EQ(3, ZoneDaylight(1).TotalDaylRefPoints);

    EXPECT_EQ(1, ZoneDaylight(1).DaylRefPtNum(1));
    EXPECT_EQ(0.35, ZoneDaylight(1).FracZoneDaylit(1));
    EXPECT_EQ(400., ZoneDaylight(1).IllumSetPoint(1));

    EXPECT_EQ(2, ZoneDaylight(1).DaylRefPtNum(2));
    EXPECT_EQ(0.4, ZoneDaylight(1).FracZoneDaylit(2));
    EXPECT_EQ(500., ZoneDaylight(1).IllumSetPoint(2));

    EXPECT_EQ(3, ZoneDaylight(1).DaylRefPtNum(3));
    EXPECT_EQ(0.25, ZoneDaylight(1).FracZoneDaylit(3));
    EXPECT_EQ(450., ZoneDaylight(1).IllumSetPoint(3));
}

TEST_F(EnergyPlusFixture, DaylightingManager_GetInputDayliteRefPt_Test)
{
    using HeatBalanceManager::GetZoneData;

    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    West Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt1,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    2.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.7;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt2,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.8;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt3,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    3.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    4.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;
    GetZoneData(foundErrors);
    ASSERT_FALSE(foundErrors);

    GetInputDayliteRefPt(foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(3, TotRefPoints);

    EXPECT_EQ("WEST ZONE_DAYLREFPT1", DaylRefPt(1).Name);
    EXPECT_EQ(1, DaylRefPt(1).ZoneNum);
    EXPECT_EQ(3.048, DaylRefPt(1).x);
    EXPECT_EQ(2.048, DaylRefPt(1).y);
    EXPECT_EQ(0.7, DaylRefPt(1).z);

    EXPECT_EQ("WEST ZONE_DAYLREFPT2", DaylRefPt(2).Name);
    EXPECT_EQ(1, DaylRefPt(2).ZoneNum);
    EXPECT_EQ(3.048, DaylRefPt(2).x);
    EXPECT_EQ(3.048, DaylRefPt(2).y);
    EXPECT_EQ(0.8, DaylRefPt(2).z);

    EXPECT_EQ("WEST ZONE_DAYLREFPT3", DaylRefPt(3).Name);
    EXPECT_EQ(1, DaylRefPt(3).ZoneNum);
    EXPECT_EQ(3.048, DaylRefPt(3).x);
    EXPECT_EQ(4.048, DaylRefPt(3).y);
    EXPECT_EQ(0.9, DaylRefPt(3).z);
}

TEST_F(EnergyPlusFixture, DaylightingManager_GetInputOutputIlluminanceMap_Test)
{
    using HeatBalanceManager::GetZoneData;

    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    West Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Output:IlluminanceMap,                                              ",
        "    Map1,                    !- Name                                  ",
        "    West Zone,               !- Zone Name                             ",
        "    0,                       !- Z height {m}                          ",
        "    0.1,                     !- X Minimum Coordinate {m}              ",
        "    6.0,                     !- X Maximum Coordinate {m}              ",
        "    10,                      !- Number of X Grid Points               ",
        "    0.2,                     !- Y Minimum Coordinate {m}              ",
        "    5.0,                     !- Y Maximum Coordinate {m}              ",
        "    11;                      !- Number of Y Grid Points               ",
        "                                                                      ",
        "  OutputControl:IlluminanceMap:Style,                                 ",
        "    Comma;                   !- Column Separator                      ",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;
    GetZoneData(foundErrors);
    ASSERT_FALSE(foundErrors);

    GetInputIlluminanceMap(state.files, foundErrors);
    // compare_err_stream(""); // expecting errors because zone is not really defined

    EXPECT_EQ(1, TotIllumMaps);

    EXPECT_EQ("MAP1", IllumMap(1).Name);
    EXPECT_EQ(1, IllumMap(1).Zone);
    EXPECT_EQ(0, IllumMap(1).Z);
    EXPECT_EQ(0.1, IllumMap(1).Xmin);
    EXPECT_EQ(6.0, IllumMap(1).Xmax);
    EXPECT_EQ(10, IllumMap(1).Xnum);
    EXPECT_EQ(0.2, IllumMap(1).Ymin);
    EXPECT_EQ(5.0, IllumMap(1).Ymax);
    EXPECT_EQ(11, IllumMap(1).Ynum);

    // OutputControl:IlluminanceMap:Style
    EXPECT_EQ(',', MapColSep);
}

TEST_F(EnergyPlusFixture, DaylightingManager_doesDayLightingUseDElight_Test)
{
    EXPECT_FALSE(doesDayLightingUseDElight());

    ZoneDaylight.allocate(3);
    ZoneDaylight(1).DaylightMethod = SplitFluxDaylighting;
    ZoneDaylight(2).DaylightMethod = SplitFluxDaylighting;
    ZoneDaylight(3).DaylightMethod = SplitFluxDaylighting;

    EXPECT_FALSE(doesDayLightingUseDElight());

    ZoneDaylight(2).DaylightMethod = DElightDaylighting;

    EXPECT_TRUE(doesDayLightingUseDElight());
}

TEST_F(EnergyPlusFixture, DaylightingManager_GetDaylParamInGeoTrans_Test)
{
    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    West Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:Controls,                                                                                           ",
        "    West Zone_DaylCtrl,      !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    West Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    West Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    1.0,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.;                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt1,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Lights,                                                                                                         ",
        "    West Zone Lights 1,      !- Name                                                                              ",
        "    West Zone,               !- Zone or ZoneList Name                                                             ",
        "    Office Lighting,         !- Schedule Name                                                                     ",
        "    LightingLevel,           !- Design Level Calculation Method                                                   ",
        "    1464.375,                !- Lighting Level {W}                                                                ",
        "    ,                        !- Watts per Zone Floor Area {W/m2}                                                  ",
        "    ,                        !- Watts per Person {W/person}                                                       ",
        "    0.0000000E+00,           !- Return Air Fraction                                                               ",
        "    0.2000000,               !- Fraction Radiant                                                                  ",
        "    0.2000000,               !- Fraction Visible                                                                  ",
        "    1.0,                     !- Fraction Replaceable                                                              ",
        "    GeneralLights;           !- End-Use Subcategory                                                               ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall001,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                         ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall002,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                         ",
        "    0.0000000E+00,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall003,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                        ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    0.0000000E+00,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall004,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Flr001,            !- Name                                                                              ",
        "    Floor,                   !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    NoSun,                   !- Sun Exposure                                                                      ",
        "    NoWind,                  !- Wind Exposure                                                                     ",
        "    1.000000,                !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 1 {m}                                         ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Roof001,           !- Name                                                                              ",
        "    Roof,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.0000000E+00,           !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  FenestrationSurface:Detailed,                                                                                   ",
        "    Zn001:Wall001:Win001,    !- Name                                                                              ",
        "    Window,                  !- Surface Type                                                                      ",
        "    WIN-CON-SINGLEPANE,      !- Construction Name                                                                 ",
        "    Zn001:Wall001,           !- Building Surface Name                                                             ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    ,                        !- Frame and Divider Name                                                            ",
        "    1.0,                     !- Multiplier                                                                        ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.548000,0.0000000E+00,2.5000,  !- X,Y,Z ==> Vertex 1 {m}                                                     ",
        "    0.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 2 {m}                                                     ",
        "    5.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 3 {m}                                                     ",
        "    5.548000,0.0000000E+00,2.5000;  !- X,Y,Z ==> Vertex 4 {m}                                                     ",
        "                                                                                                                  ",
        "  Zone,                                                                                                           ",
        "    East Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:Controls,                                                                                           ",
        "    East Zone_DaylCtrl,      !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    East Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    East Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.5,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.,                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "    East Zone_DaylRefPt2,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.5,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.;                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt1,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt2,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Lights,                                                                                                         ",
        "    East Zone Lights 1,      !- Name                                                                              ",
        "    East Zone,               !- Zone or ZoneList Name                                                             ",
        "    Office Lighting,         !- Schedule Name                                                                     ",
        "    LightingLevel,           !- Design Level Calculation Method                                                   ",
        "    1464.375,                !- Lighting Level {W}                                                                ",
        "    ,                        !- Watts per Zone Floor Area {W/m2}                                                  ",
        "    ,                        !- Watts per Person {W/person}                                                       ",
        "    0.0000000E+00,           !- Return Air Fraction                                                               ",
        "    0.2000000,               !- Fraction Radiant                                                                  ",
        "    0.2000000,               !- Fraction Visible                                                                  ",
        "    1.0,                     !- Fraction Replaceable                                                              ",
        "    GeneralLights;           !- End-Use Subcategory                                                               ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall001,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                         ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall002,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                         ",
        "    0.0000000E+00,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall003,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                        ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    0.0000000E+00,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall004,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Flr001,            !- Name                                                                              ",
        "    Floor,                   !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    NoSun,                   !- Sun Exposure                                                                      ",
        "    NoWind,                  !- Wind Exposure                                                                     ",
        "    1.000000,                !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 1 {m}                                         ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Roof001,           !- Name                                                                              ",
        "    Roof,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.0000000E+00,           !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  FenestrationSurface:Detailed,                                                                                   ",
        "    Zn002:Wall001:Win001,    !- Name                                                                              ",
        "    Window,                  !- Surface Type                                                                      ",
        "    WIN-CON-SINGLEPANE,      !- Construction Name                                                                 ",
        "    Zn002:Wall001,           !- Building Surface Name                                                             ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    ,                        !- Frame and Divider Name                                                            ",
        "    1.0,                     !- Multiplier                                                                        ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.548000,0.0000000E+00,2.5000,  !- X,Y,Z ==> Vertex 1 {m}                                                     ",
        "    0.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 2 {m}                                                     ",
        "    5.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 3 {m}                                                     ",
        "    5.548000,0.0000000E+00,2.5000;  !- X,Y,Z ==> Vertex 4 {m}                                                     ",
        "                                                                                                                  ",
        "  Construction,                                                                                                   ",
        "    WALL80,               !- Name                                                                                 ",
        "    C4 - 4 IN COMMON BRICK;  !- Layer 1                                                                           ",
        "                                                                                                                  ",
        "  Material,                                                                                                       ",
        "    C4 - 4 IN COMMON BRICK,  !- Name                                                                              ",
        "    Rough,                   !- Roughness                                                                         ",
        "    0.1014984,               !- Thickness {m}                                                                     ",
        "    0.7264224,               !- Conductivity {W/m-K}                                                              ",
        "    1922.216,                !- Density {kg/m3}                                                                   ",
        "    836.8000,                !- Specific Heat {J/kg-K}                                                            ",
        "    0.9000000,               !- Thermal Absorptance                                                               ",
        "    0.7600000,               !- Solar Absorptance                                                                 ",
        "    0.7600000;               !- Visible Absorptance                                                               ",
        "                                                                                                                  ",
        "  Schedule:Compact,                                                                                               ",
        "    Office Lighting,         !- Name                                                                              ",
        "    ANY NUMBER,              !- Schedule Type Limits Name                                                         ",
        "    Through: 12/31,          !- Field 1                                                                           ",
        "    For: Alldays,            !- Field 2                                                                           ",
        "    Until: 24:00,1.00;       !- Field 7                                                                           ",
        "                                                                                                                  ",
        "  Construction,                                                                                                   ",
        "    WIN-CON-SINGLEPANE,      !- Name                                                                              ",
        "    SINGLEPANE;              !- Outside Layer                                                                     ",
        "                                                                                                                  ",
        "  WindowMaterial:Glazing,                                                                                         ",
        "    SINGLEPANE,              !- Name                                                                              ",
        "    SpectralAverage,         !- Optical Data Type                                                                 ",
        "    ,                        !- Window Glass Spectral Data Set Name                                               ",
        "    0.003,                   !- Thickness {m}                                                                     ",
        "    0.90,                    !- Solar Transmittance at Normal Incidence                                           ",
        "    0.031,                   !- Front Side Solar Reflectance at Normal Incidence                                  ",
        "    0.031,                   !- Back Side Solar Reflectance at Normal Incidence                                   ",
        "    0.90,                    !- Visible Transmittance at Normal Incidence                                         ",
        "    0.05,                    !- Front Side Visible Reflectance at Normal Incidence                                ",
        "    0.05,                    !- Back Side Visible Reflectance at Normal Incidence                                 ",
        "    0.0,                     !- Infrared Transmittance at Normal Incidence                                        ",
        "    0.84,                    !- Front Side Infrared Hemispherical Emissivity                                      ",
        "    0.84,                    !- Back Side Infrared Hemispherical Emissivity                                       ",
        "    0.9;                     !- Conductivity {W/m-K}                                                              ",
        "SurfaceConvectionAlgorithm:Inside,TARP;",
        "SurfaceConvectionAlgorithm:Outside,DOE-2;",
        "HeatBalanceAlgorithm,ConductionTransferFunction;",
        "ZoneAirHeatBalanceAlgorithm,",
        "    AnalyticalSolution;      !- Algorithm",
        "ScheduleTypeLimits,",
        "    Any Number;              !- Name",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;

    HeatBalanceManager::GetProjectControlData(state, foundErrors); // read project control data
    EXPECT_FALSE(foundErrors);                              // expect no errors

    HeatBalanceManager::GetMaterialData(state, *state.dataWindowEquivalentLayer, state.files, foundErrors); // read material data
    EXPECT_FALSE(foundErrors);                        // expect no errors

    HeatBalanceManager::GetConstructData(state.files, foundErrors); // read construction data
    compare_err_stream("");
    EXPECT_FALSE(foundErrors); // expect no errors

    HeatBalanceManager::GetZoneData(foundErrors); // read zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::CosZoneRelNorth.allocate(2);
    SurfaceGeometry::SinZoneRelNorth.allocate(2);

    SurfaceGeometry::CosZoneRelNorth(1) = std::cos(-DataHeatBalance::Zone(1).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::SinZoneRelNorth(1) = std::sin(-DataHeatBalance::Zone(1).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::CosZoneRelNorth(2) = std::cos(-DataHeatBalance::Zone(2).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::SinZoneRelNorth(2) = std::sin(-DataHeatBalance::Zone(2).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::CosBldgRelNorth = 1.0;
    SurfaceGeometry::SinBldgRelNorth = 0.0;

    SurfaceGeometry::GetSurfaceData(state, state.files, foundErrors); // setup zone geometry and get zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::SetupZoneGeometry(state, foundErrors); // this calls GetSurfaceData()
    EXPECT_FALSE(foundErrors);                       // expect no errors
    HeatBalanceIntRadExchange::InitSolarViewFactors(state.files);

    DataGlobals::NumOfTimeStepInHour = 1; // must initialize this to get schedules initialized
    DataGlobals::MinutesPerTimeStep = 60; // must initialize this to get schedules initialized
    ScheduleManager::ProcessScheduleInput(state.files);
    ScheduleManager::ScheduleInputProcessed = true;
    DataGlobals::TimeStep = 1;
    DataGlobals::HourOfDay = 1;
    DataGlobals::PreviousHour = 1;
    DataEnvironment::Month = 1;
    DataEnvironment::DayOfMonth = 21;
    DataGlobals::HourOfDay = 1;
    DataEnvironment::DSTIndicator = 0;
    DataEnvironment::DayOfWeek = 2;
    DataEnvironment::HolidayIndex = 0;
    DataEnvironment::DayOfYear_Schedule = General::OrdinalDay(DataEnvironment::Month, DataEnvironment::DayOfMonth, 1);
    ScheduleManager::UpdateScheduleValues();
    InternalHeatGains::GetInternalHeatGainsInput(state);
    InternalHeatGains::GetInternalHeatGainsInputFlag = false;

    GetDaylightingParametersInput(state.files);
    compare_err_stream("");
    EXPECT_EQ(3, TotRefPoints);

    EXPECT_NEAR(2.048, ZoneDaylight(1).DaylRefPtAbsCoord(1, 1), 0.001);
    EXPECT_NEAR(3.048, ZoneDaylight(1).DaylRefPtAbsCoord(2, 1), 0.001);
    EXPECT_NEAR(0.9, ZoneDaylight(1).DaylRefPtAbsCoord(3, 1), 0.001);

    DataHeatBalance::Zone(1).RelNorth = 45.;

    GeometryTransformForDaylighting();

    EXPECT_NEAR(3.603, ZoneDaylight(1).DaylRefPtAbsCoord(1, 1), 0.001);
    EXPECT_NEAR(0.707, ZoneDaylight(1).DaylRefPtAbsCoord(2, 1), 0.001);
    EXPECT_NEAR(0.9, ZoneDaylight(1).DaylRefPtAbsCoord(3, 1), 0.001);

    DataHeatBalance::Zone(1).RelNorth = 90.;

    GeometryTransformForDaylighting();

    EXPECT_NEAR(3.048, ZoneDaylight(1).DaylRefPtAbsCoord(1, 1), 0.001);
    EXPECT_NEAR(-2.048, ZoneDaylight(1).DaylRefPtAbsCoord(2, 1), 0.001);
    EXPECT_NEAR(0.9, ZoneDaylight(1).DaylRefPtAbsCoord(3, 1), 0.001);

    DataGlobals::BeginSimFlag = true;
    DataGlobals::WeightNow = 1.0;
    DataGlobals::WeightPreviousHour = 0.0;
    CalcDayltgCoefficients(state.files);
    int zoneNum = 1;
    // test that tmp arrays are allocated to correct dimension
    // zone 1 has only 1 daylighting reference point
    DayltgInteriorIllum(zoneNum);
    zoneNum += 1;
    // zone 2 has 2 daylighting reference points and will crash if not dimensioned appropriately.
    DayltgInteriorIllum(zoneNum);
}

TEST_F(EnergyPlusFixture, DaylightingManager_ProfileAngle_Test)
{

    Surface.allocate(1);
    Surface(1).Tilt = 90.0;
    Surface(1).Azimuth = 180.0;
    int horiz = 1;
    int vert = 2;
    Real64 ProfAng;
    Vector3<Real64> CosDirSun; // Solar direction cosines

    CosDirSun(1) = 0.882397;
    CosDirSun(2) = 0.470492;
    CosDirSun(3) = 0.003513;

    ProfileAngle(1, CosDirSun, horiz, ProfAng);
    EXPECT_NEAR(0.00747, ProfAng, 0.00001);

    ProfileAngle(1, CosDirSun, vert, ProfAng);
    EXPECT_NEAR(2.06065, ProfAng, 0.00001);

    CosDirSun(1) = 0.92318;
    CosDirSun(2) = 0.36483;
    CosDirSun(3) = 0.12094;

    ProfileAngle(1, CosDirSun, horiz, ProfAng);
    EXPECT_NEAR(0.32010, ProfAng, 0.00001);

    ProfileAngle(1, CosDirSun, vert, ProfAng);
    EXPECT_NEAR(1.94715, ProfAng, 0.00001);
}

TEST_F(EnergyPlusFixture, AssociateWindowShadingControlWithDaylighting_Test)
{
    DataGlobals::NumOfZones = 4;
    ZoneDaylight.allocate(DataGlobals::NumOfZones);
    ZoneDaylight(1).Name = "ZD1";
    ZoneDaylight(2).Name = "ZD2";
    ZoneDaylight(3).Name = "ZD3";
    ZoneDaylight(4).Name = "ZD4";

    TotWinShadingControl = 3;
    WindowShadingControl.allocate(TotWinShadingControl);

    WindowShadingControl(1).Name = "WSC1";
    WindowShadingControl(1).DaylightingControlName = "ZD3";

    WindowShadingControl(2).Name = "WSC2";
    WindowShadingControl(2).DaylightingControlName = "ZD1";

    WindowShadingControl(3).Name = "WSC3";
    WindowShadingControl(3).DaylightingControlName = "ZD-NONE";

    AssociateWindowShadingControlWithDaylighting();

    EXPECT_EQ(WindowShadingControl(1).DaylightControlIndex, 3);
    EXPECT_EQ(WindowShadingControl(2).DaylightControlIndex, 1);
    EXPECT_EQ(WindowShadingControl(3).DaylightControlIndex, 0);
}

TEST_F(EnergyPlusFixture, CreateShadeDeploymentOrder_test)
{
    TotWinShadingControl = 3;
    WindowShadingControl.allocate(TotWinShadingControl);
    int zn = 1;

    WindowShadingControl(1).Name = "WSC1";
    WindowShadingControl(1).ZoneIndex = zn;
    WindowShadingControl(1).SequenceNumber = 2;
    WindowShadingControl(1).MultiSurfaceCtrlIsGroup = true;
    WindowShadingControl(1).FenestrationCount = 3;
    WindowShadingControl(1).FenestrationIndex.allocate(WindowShadingControl(1).FenestrationCount);
    WindowShadingControl(1).FenestrationIndex(1) = 1;
    WindowShadingControl(1).FenestrationIndex(2) = 2;
    WindowShadingControl(1).FenestrationIndex(3) = 3;

    WindowShadingControl(2).Name = "WSC2";
    WindowShadingControl(2).ZoneIndex = zn;
    WindowShadingControl(2).SequenceNumber = 3;
    WindowShadingControl(2).MultiSurfaceCtrlIsGroup = false;
    WindowShadingControl(2).FenestrationCount = 4;
    WindowShadingControl(2).FenestrationIndex.allocate(WindowShadingControl(2).FenestrationCount);
    WindowShadingControl(2).FenestrationIndex(1) = 4;
    WindowShadingControl(2).FenestrationIndex(2) = 5;
    WindowShadingControl(2).FenestrationIndex(3) = 6;
    WindowShadingControl(2).FenestrationIndex(4) = 7;

    WindowShadingControl(3).Name = "WSC3";
    WindowShadingControl(3).ZoneIndex = zn;
    WindowShadingControl(3).SequenceNumber = 1;
    WindowShadingControl(3).MultiSurfaceCtrlIsGroup = true;
    WindowShadingControl(3).FenestrationCount = 2;
    WindowShadingControl(3).FenestrationIndex.allocate(WindowShadingControl(3).FenestrationCount);
    WindowShadingControl(3).FenestrationIndex(1) = 8;
    WindowShadingControl(3).FenestrationIndex(2) = 9;

    DataGlobals::NumOfZones = zn;
    ZoneDaylight.allocate(DataGlobals::NumOfZones);

    CreateShadeDeploymentOrder(zn);

    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins.size(), 6ul);

    std::vector<int> compare1;
    compare1.push_back(8);
    compare1.push_back(9);
    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins[0], compare1);

    std::vector<int> compare2;
    compare2.push_back(1);
    compare2.push_back(2);
    compare2.push_back(3);
    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins[1], compare2);

    std::vector<int> compare3;
    compare3.push_back(4);
    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins[2], compare3);

    std::vector<int> compare4;
    compare4.push_back(5);
    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins[3], compare4);

    std::vector<int> compare5;
    compare5.push_back(6);
    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins[4], compare5);

    std::vector<int> compare6;
    compare6.push_back(7);
    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins[5], compare6);
}

TEST_F(EnergyPlusFixture, MapShadeDeploymentOrderToLoopNumber_Test)
{
    TotWinShadingControl = 3;
    WindowShadingControl.allocate(TotWinShadingControl);
    int zn = 1;

    WindowShadingControl(1).Name = "WSC1";
    WindowShadingControl(1).ZoneIndex = zn;
    WindowShadingControl(1).SequenceNumber = 2;
    WindowShadingControl(1).MultiSurfaceCtrlIsGroup = true;
    WindowShadingControl(1).FenestrationCount = 3;
    WindowShadingControl(1).FenestrationIndex.allocate(WindowShadingControl(1).FenestrationCount);
    WindowShadingControl(1).FenestrationIndex(1) = 1;
    WindowShadingControl(1).FenestrationIndex(2) = 2;
    WindowShadingControl(1).FenestrationIndex(3) = 3;

    WindowShadingControl(2).Name = "WSC2";
    WindowShadingControl(2).ZoneIndex = zn;
    WindowShadingControl(2).SequenceNumber = 3;
    WindowShadingControl(2).MultiSurfaceCtrlIsGroup = false;
    WindowShadingControl(2).FenestrationCount = 4;
    WindowShadingControl(2).FenestrationIndex.allocate(WindowShadingControl(2).FenestrationCount);
    WindowShadingControl(2).FenestrationIndex(1) = 4;
    WindowShadingControl(2).FenestrationIndex(2) = 5;
    WindowShadingControl(2).FenestrationIndex(3) = 6;
    WindowShadingControl(2).FenestrationIndex(4) = 7;

    WindowShadingControl(3).Name = "WSC3";
    WindowShadingControl(3).ZoneIndex = zn;
    WindowShadingControl(3).SequenceNumber = 1;
    WindowShadingControl(3).MultiSurfaceCtrlIsGroup = true;
    WindowShadingControl(3).FenestrationCount = 2;
    WindowShadingControl(3).FenestrationIndex.allocate(WindowShadingControl(3).FenestrationCount);
    WindowShadingControl(3).FenestrationIndex(1) = 8;
    WindowShadingControl(3).FenestrationIndex(2) = 9;

    DataGlobals::NumOfZones = zn;
    ZoneDaylight.allocate(DataGlobals::NumOfZones);

    CreateShadeDeploymentOrder(zn);

    EXPECT_EQ(ZoneDaylight(zn).ShadeDeployOrderExtWins.size(), 6ul);

    ZoneDaylight(zn).TotalDaylRefPoints = 1;
    ZoneDaylight(zn).NumOfDayltgExtWins = 9;
    ZoneDaylight(zn).MapShdOrdToLoopNum.allocate(ZoneDaylight(zn).NumOfDayltgExtWins);
    ZoneDaylight(zn).DayltgExtWinSurfNums.allocate(ZoneDaylight(zn).NumOfDayltgExtWins);
    ZoneDaylight(zn).DayltgExtWinSurfNums(1) = 1;
    ZoneDaylight(zn).DayltgExtWinSurfNums(2) = 2;
    ZoneDaylight(zn).DayltgExtWinSurfNums(3) = 3;
    ZoneDaylight(zn).DayltgExtWinSurfNums(4) = 4;
    ZoneDaylight(zn).DayltgExtWinSurfNums(5) = 5;
    ZoneDaylight(zn).DayltgExtWinSurfNums(6) = 6;
    ZoneDaylight(zn).DayltgExtWinSurfNums(7) = 7;
    ZoneDaylight(zn).DayltgExtWinSurfNums(8) = 8;
    ZoneDaylight(zn).DayltgExtWinSurfNums(9) = 9;

    MapShadeDeploymentOrderToLoopNumber(zn);

    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(1), 8);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(2), 9);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(3), 1);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(4), 2);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(5), 3);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(6), 4);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(7), 5);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(8), 6);
    EXPECT_EQ(ZoneDaylight(zn).MapShdOrdToLoopNum(9), 7);
}
TEST_F(EnergyPlusFixture, DaylightingManager_DayltgInteriorIllum_Test)
{
    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    East Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall001,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                         ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall002,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                         ",
        "    0.0000000E+00,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall003,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                        ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    0.0000000E+00,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall004,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Flr001,            !- Name                                                                              ",
        "    Floor,                   !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    NoSun,                   !- Sun Exposure                                                                      ",
        "    NoWind,                  !- Wind Exposure                                                                     ",
        "    1.000000,                !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 1 {m}                                         ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Roof001,           !- Name                                                                              ",
        "    Roof,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.0000000E+00,           !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  FenestrationSurface:Detailed,                                                                                   ",
        "    Zn001:Wall001:Win001,    !- Name                                                                              ",
        "    Window,                  !- Surface Type                                                                      ",
        "    WIN-CON-SINGLEPANE,      !- Construction Name                                                                 ",
        "    Zn001:Wall001,           !- Building Surface Name                                                             ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    ,                        !- Frame and Divider Name                                                            ",
        "    1.0,                     !- Multiplier                                                                        ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.548000,0.0000000E+00,2.5000,  !- X,Y,Z ==> Vertex 1 {m}                                                     ",
        "    0.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 2 {m}                                                     ",
        "    5.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 3 {m}                                                     ",
        "    5.548000,0.0000000E+00,2.5000;  !- X,Y,Z ==> Vertex 4 {m}                                                     ",
        "                                                                                                                  ",
        "  Construction,                                                                                                   ",
        "    WALL80,               !- Name                                                                                 ",
        "    C4 - 4 IN COMMON BRICK;  !- Layer 1                                                                           ",
        "                                                                                                                  ",
        "  Material,                                                                                                       ",
        "    C4 - 4 IN COMMON BRICK,  !- Name                                                                              ",
        "    Rough,                   !- Roughness                                                                         ",
        "    0.1014984,               !- Thickness {m}                                                                     ",
        "    0.7264224,               !- Conductivity {W/m-K}                                                              ",
        "    1922.216,                !- Density {kg/m3}                                                                   ",
        "    836.8000,                !- Specific Heat {J/kg-K}                                                            ",
        "    0.9000000,               !- Thermal Absorptance                                                               ",
        "    0.7600000,               !- Solar Absorptance                                                                 ",
        "    0.7600000;               !- Visible Absorptance                                                               ",
        "                                                                                                                  ",
        "  Construction,                                                                                                   ",
        "    WIN-CON-SINGLEPANE,      !- Name                                                                              ",
        "    SINGLEPANE;              !- Outside Layer                                                                     ",
        "                                                                                                                  ",
        "  WindowMaterial:Glazing,                                                                                         ",
        "    SINGLEPANE,              !- Name                                                                              ",
        "    SpectralAverage,         !- Optical Data Type                                                                 ",
        "    ,                        !- Window Glass Spectral Data Set Name                                               ",
        "    0.003,                   !- Thickness {m}                                                                     ",
        "    0.90,                    !- Solar Transmittance at Normal Incidence                                           ",
        "    0.031,                   !- Front Side Solar Reflectance at Normal Incidence                                  ",
        "    0.031,                   !- Back Side Solar Reflectance at Normal Incidence                                   ",
        "    0.90,                    !- Visible Transmittance at Normal Incidence                                         ",
        "    0.05,                    !- Front Side Visible Reflectance at Normal Incidence                                ",
        "    0.05,                    !- Back Side Visible Reflectance at Normal Incidence                                 ",
        "    0.0,                     !- Infrared Transmittance at Normal Incidence                                        ",
        "    0.84,                    !- Front Side Infrared Hemispherical Emissivity                                      ",
        "    0.84,                    !- Back Side Infrared Hemispherical Emissivity                                       ",
        "    0.9;                     !- Conductivity {W/m-K}                                                              ",
        "  Daylighting:Controls,                                                                                           ",
        "    East Zone_DaylCtrl,      !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    East Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    East Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.5,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.,                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "    East Zone_DaylRefPt2,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.5,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.;                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt1,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt2,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",

        "  Lights,                                                                                                         ",
        "    East Zone Lights 1,      !- Name                                                                              ",
        "    East Zone,               !- Zone or ZoneList Name                                                             ",
        "    Office Lighting,         !- Schedule Name                                                                     ",
        "    LightingLevel,           !- Design Level Calculation Method                                                   ",
        "    1464.375,                !- Lighting Level {W}                                                                ",
        "    ,                        !- Watts per Zone Floor Area {W/m2}                                                  ",
        "    ,                        !- Watts per Person {W/person}                                                       ",
        "    0.0000000E+00,           !- Return Air Fraction                                                               ",
        "    0.2000000,               !- Fraction Radiant                                                                  ",
        "    0.2000000,               !- Fraction Visible                                                                  ",
        "    1.0,                     !- Fraction Replaceable                                                              ",
        "    GeneralLights;           !- End-Use Subcategory                                                               ",
        "  Schedule:Constant, Office Lighting, AnyNumber, 1.0;",
        "ScheduleTypeLimits,",
        "    AnyNumber;              !- Name",

    });

    ASSERT_TRUE(process_idf(idf_objects));
    DataGlobals::NumOfTimeStepInHour = 1;
    ScheduleManager::ProcessScheduleInput(state.files);
    ScheduleManager::ScheduleInputProcessed = true;
    DataGlobals::TimeStep = 1;
    DataGlobals::HourOfDay = 10;
    DataGlobals::PreviousHour = 10;
    DataEnvironment::Month = 1;
    DataEnvironment::DayOfMonth = 21;
    DataEnvironment::DSTIndicator = 0;
    DataEnvironment::DayOfWeek = 2;
    DataEnvironment::HolidayIndex = 0;

    bool foundErrors = false;
    HeatBalanceManager::GetProjectControlData(state, foundErrors); // read project control data
    EXPECT_FALSE(foundErrors);                              // expect no errors

    HeatBalanceManager::GetMaterialData(state, *state.dataWindowEquivalentLayer, state.files, foundErrors); // read material data
    EXPECT_FALSE(foundErrors);                        // expect no errors

    HeatBalanceManager::GetConstructData(state.files, foundErrors); // read construction data
    compare_err_stream("");
    EXPECT_FALSE(foundErrors); // expect no errors

    HeatBalanceManager::GetZoneData(foundErrors); // read zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::SetupZoneGeometry(state, foundErrors); // this calls GetSurfaceData()
    EXPECT_FALSE(foundErrors);                       // expect no errors
    HeatBalanceIntRadExchange::InitSolarViewFactors(state.files);

    int ZoneNum = UtilityRoutines::FindItemInList("EAST ZONE", DataHeatBalance::Zone);
    InternalHeatGains::GetInternalHeatGainsInput(state);
    InternalHeatGains::GetInternalHeatGainsInputFlag = false;
    DaylightingManager::GetInputDayliteRefPt(foundErrors);
    DaylightingManager::GetDaylightingParametersInput(state.files);
    DaylightingManager::GILSK = 100.0;
    DataGlobals::WeightNow = 1.0;
    DataEnvironment::HISUNF = 100.0;
    DataEnvironment::HISKF = 100.0;
    DataEnvironment::SkyClearness = 6.0;

    // Set all daylighting factors to zero
    ZoneDaylight(ZoneNum).DaylIllFacSky = 0.0;
    ZoneDaylight(ZoneNum).DaylIllFacSun = 0.0;
    ZoneDaylight(ZoneNum).DaylIllFacSunDisk = 0.0;
    ZoneDaylight(ZoneNum).DaylBackFacSky = 0.0;
    ZoneDaylight(ZoneNum).DaylBackFacSun = 0.0;
    ZoneDaylight(ZoneNum).DaylBackFacSunDisk = 0.0;
    ZoneDaylight(ZoneNum).DaylSourceFacSky = 0.0;
    ZoneDaylight(ZoneNum).DaylSourceFacSun = 0.0;
    ZoneDaylight(ZoneNum).DaylSourceFacSunDisk = 0.0;
    DaylightingManager::DayltgInteriorIllum(ZoneNum);
    EXPECT_NEAR(DaylightingManager::DaylIllum(1), 0.0, 0.001);

    int ISky = 1;
    int DayltgExtWin = 1;
    int Shaded = 2;
    int Unshaded = 1;
    int IWin = UtilityRoutines::FindItemInList("ZN001:WALL001:WIN001", DataSurfaces::Surface);
    EXPECT_GT(IWin, 0);

    // Set un-shaded surface illuminance factor to 1.0 for RefPt1, 0.1 for RefPt2
    // Set shaded surface illuminance factor to 0.5 for RefPt1, 0.05 for RefPt2
    int RefPt = 1;
    ZoneDaylight(ZoneNum).DaylIllFacSky(DataGlobals::HourOfDay, Unshaded, ISky, RefPt, DayltgExtWin) = 1.0;
    ZoneDaylight(ZoneNum).DaylIllFacSky(DataGlobals::HourOfDay, Shaded, ISky, RefPt, DayltgExtWin) = 0.5;
    RefPt = 2;
    ZoneDaylight(ZoneNum).DaylIllFacSky(DataGlobals::HourOfDay, Unshaded, ISky, RefPt, DayltgExtWin) = 0.1;
    ZoneDaylight(ZoneNum).DaylIllFacSky(DataGlobals::HourOfDay, Shaded, ISky, RefPt, DayltgExtWin) = 0.05;

    // Window5 model - expect 100 for unshaded and 50 for shaded (10 and 5 for RefPt2)
    SurfWinWindowModelType(IWin) = Window5DetailedModel;
    SurfWinShadingFlag(IWin) = DataSurfaces::NoShade;
    DaylightingManager::DayltgInteriorIllum(ZoneNum);
    EXPECT_NEAR(DaylightingManager::DaylIllum(1), 100.0, 0.001);
    EXPECT_NEAR(DaylightingManager::DaylIllum(2), 10.0, 0.001);

    SurfWinShadingFlag(IWin) = DataSurfaces::ExtBlindOn;
    DaylightingManager::DayltgInteriorIllum(ZoneNum);
    EXPECT_NEAR(DaylightingManager::DaylIllum(1), 50.0, 0.001);
    EXPECT_NEAR(DaylightingManager::DaylIllum(2), 5.0, 0.001);

    // BSDF model - expect 100 for unshaded and 100 for shaded (10 for RefPt2
    // BSDF does shading differently, it's integrated in the base state
    SurfWinWindowModelType(IWin) = WindowBSDFModel;
    SurfWinShadingFlag(IWin) = DataSurfaces::NoShade;
    DaylightingManager::DayltgInteriorIllum(ZoneNum);
    EXPECT_NEAR(DaylightingManager::DaylIllum(1), 100.0, 0.001);
    EXPECT_NEAR(DaylightingManager::DaylIllum(2), 10.0, 0.001);

    SurfWinShadingFlag(IWin) = DataSurfaces::ExtBlindOn;
    DaylightingManager::DayltgInteriorIllum(ZoneNum);
    EXPECT_NEAR(DaylightingManager::DaylIllum(1), 100.0, 0.001);
    EXPECT_NEAR(DaylightingManager::DaylIllum(2), 10.0, 0.001);
}


// Test for #7809: Daylighting:Controls has 10 ref points with fraction that do sum exactly to 1,
// yet with double rounding errors it throws a severe about sum of fraction > 1.0
TEST_F(EnergyPlusFixture, DaylightingManager_GetInputDaylightingControls_RoundingError)
{

    std::string const idf_objects = delimited_string({
        "Zone,",
        "  West Zone,               !- Name",
        "  0.0000000E+00,           !- Direction of Relative North {deg}",
        "  0.0000000E+00,           !- X Origin {m}",
        "  0.0000000E+00,           !- Y Origin {m}",
        "  0.0000000E+00,           !- Z Origin {m}",
        "  1,                       !- Type",
        "  1,                       !- Multiplier",
        "  autocalculate,           !- Ceiling Height {m}",
        "  autocalculate;           !- Volume {m3}",

        "Daylighting:Controls,",
        "  West Zone_DaylCtrl,      !- Name",
        "  West Zone,               !- Zone Name",
        "  SplitFlux,               !- Daylighting Method",
        "  ,                        !- Availability Schedule Name",
        "  Continuous,              !- Lighting Control Type",
        "  0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control",
        "  0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control",
        "  ,                        !- Number of Stepped Control Steps",
        "  1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control",
        "  West Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name",
        "  180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "  20.0,                    !- Maximum Allowable Discomfort Glare Index",
        "  ,                        !- DElight Gridding Resolution {m2}",
        "  West Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name",
        "  0.1053,                  !- Fraction of Zone Controlled by Reference Point 1",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 1",
        "  West Zone_DaylRefPt2,    !- Daylighting Reference Point 2 Name",
        "  0.0936,                  !- Fraction of Zone Controlled by Reference Point 2",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 2",
        "  West Zone_DaylRefPt3,    !- Daylighting Reference Point 3 Name",
        "  0.1213,                  !- Fraction of Zone Controlled by Reference Point 3",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 3",
        "  West Zone_DaylRefPt4,    !- Daylighting Reference Point 4 Name",
        "  0.1018,                  !- Fraction of Zone Controlled by Reference Point 4",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 4",
        "  West Zone_DaylRefPt5,    !- Daylighting Reference Point 5 Name",
        "  0.0893,                  !- Fraction of Zone Controlled by Reference Point 5",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 5",
        "  West Zone_DaylRefPt6,    !- Daylighting Reference Point 6 Name",
        "  0.0842,                  !- Fraction of Zone Controlled by Reference Point 6",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 6",
        "  West Zone_DaylRefPt7,    !- Daylighting Reference Point 7 Name",
        "  0.0882,                  !- Fraction of Zone Controlled by Reference Point 7",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 7",
        "  West Zone_DaylRefPt8,    !- Daylighting Reference Point 8 Name",
        "  0.1026,                  !- Fraction of Zone Controlled by Reference Point 8",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 8",
        "  West Zone_DaylRefPt9,    !- Daylighting Reference Point 9 Name",
        "  0.1134,                  !- Fraction of Zone Controlled by Reference Point 9",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 9",
        "  West Zone_DaylRefPt10,    !- Daylighting Reference Point 10 Name",
        "  0.1003,                  !- Fraction of Zone Controlled by Reference Point 10",
        "  200.0;                   !- Illuminance Setpoint at Reference Point 10",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt1,    !- Name",
        "  West Zone,               !- Zone Name",
        "  0.5,                     !- X-Coordinate of Reference Point {m}",
        "  0.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt2,    !- Name",
        "  West Zone,               !- Zone Name",
        "  0.5,                     !- X-Coordinate of Reference Point {m}",
        "  1.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt3,    !- Name",
        "  West Zone,               !- Zone Name",
        "  0.5,                     !- X-Coordinate of Reference Point {m}",
        "  2.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt4,    !- Name",
        "  West Zone,               !- Zone Name",
        "  1.5,                     !- X-Coordinate of Reference Point {m}",
        "  0.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt5,    !- Name",
        "  West Zone,               !- Zone Name",
        "  1.5,                     !- X-Coordinate of Reference Point {m}",
        "  1.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt6,    !- Name",
        "  West Zone,               !- Zone Name",
        "  1.5,                     !- X-Coordinate of Reference Point {m}",
        "  2.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt7,    !- Name",
        "  West Zone,               !- Zone Name",
        "  2.5,                     !- X-Coordinate of Reference Point {m}",
        "  0.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt8,    !- Name",
        "  West Zone,               !- Zone Name",
        "  2.5,                     !- X-Coordinate of Reference Point {m}",
        "  1.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt9,    !- Name",
        "  West Zone,               !- Zone Name",
        "  2.5,                     !- X-Coordinate of Reference Point {m}",
        "  2.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt10,   !- Name",
        "  West Zone,               !- Zone Name",
        "  3.0,                     !- X-Coordinate of Reference Point {m}",
        "  2.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;
    HeatBalanceManager::GetZoneData(foundErrors);
    ASSERT_FALSE(foundErrors);

    int numObjs = inputProcessor->getNumObjectsFound("Daylighting:Controls");
    EXPECT_EQ(1, numObjs);

    DaylightingManager::GetInputDayliteRefPt(foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(10, DataDaylighting::TotRefPoints);

    DaylightingManager::GetDaylightingControls(numObjs, foundErrors);
    // Used to throw
    //    ** Severe  ** GetDaylightingControls: Fraction of Zone controlled by the Daylighting reference points is > 1.0.
    //    **   ~~~   ** ..discovered in \"Daylighting:Controls\" for Zone=\"WEST ZONE\", trying to control 1.00 of the zone.\n
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);

    EXPECT_EQ("WEST ZONE_DAYLCTRL", DataDaylighting::ZoneDaylight(1).Name);
    EXPECT_EQ("WEST ZONE", DataDaylighting::ZoneDaylight(1).ZoneName);
    EXPECT_EQ(DataDaylighting::SplitFluxDaylighting, DataDaylighting::ZoneDaylight(1).DaylightMethod);
    EXPECT_EQ(DataDaylighting::Continuous, DataDaylighting::ZoneDaylight(1).LightControlType);

    EXPECT_EQ(0.3, DataDaylighting::ZoneDaylight(1).MinPowerFraction);
    EXPECT_EQ(0.2, DataDaylighting::ZoneDaylight(1).MinLightFraction);
    EXPECT_EQ(1, DataDaylighting::ZoneDaylight(1).LightControlSteps);
    EXPECT_EQ(1.0, DataDaylighting::ZoneDaylight(1).LightControlProbability);

    EXPECT_EQ(1, DataDaylighting::ZoneDaylight(1).glareRefPtNumber);
    EXPECT_EQ(180., DataDaylighting::ZoneDaylight(1).ViewAzimuthForGlare);
    EXPECT_EQ(20., DataDaylighting::ZoneDaylight(1).MaxGlareallowed);
    EXPECT_EQ(0, DataDaylighting::ZoneDaylight(1).DElightGriddingResolution);

    EXPECT_EQ(10, DataDaylighting::ZoneDaylight(1).TotalDaylRefPoints);

    std::vector<Real64> fractions({0.1053, 0.0936, 0.1213, 0.1018, 0.0893, 0.0842, 0.0882, 0.1026, 0.1134, 0.1003});
    Real64 sum(0.0);
    int i = 1;
    for (auto frac: fractions) {
        sum += frac;
        EXPECT_EQ(i, DataDaylighting::ZoneDaylight(1).DaylRefPtNum(i));
        EXPECT_EQ("WEST ZONE_DAYLREFPT" + std::to_string(i),
                  DataDaylighting::DaylRefPt(DataDaylighting::ZoneDaylight(1).DaylRefPtNum(i)).Name);
        EXPECT_EQ(frac, DataDaylighting::ZoneDaylight(1).FracZoneDaylit(i));
        EXPECT_EQ(200., DataDaylighting::ZoneDaylight(1).IllumSetPoint(i));
        ++i;
    }

    // It does sum up to 1.0
    EXPECT_DOUBLE_EQ(1.0, sum);
    EXPECT_FALSE(std::abs(sum - 1.0) > std::numeric_limits<double>::epsilon());
    // Yet, if you are being very litteral, then it's slightly more
    EXPECT_TRUE(sum > 1.0);
    EXPECT_FALSE(sum < 1.0);

}

TEST_F(EnergyPlusFixture, DaylightingManager_GetInputDaylightingControls_NotAroundOne)
{

    std::string const idf_objects = delimited_string({
        "Zone,",
        "  West Zone,               !- Name",
        "  0.0000000E+00,           !- Direction of Relative North {deg}",
        "  0.0000000E+00,           !- X Origin {m}",
        "  0.0000000E+00,           !- Y Origin {m}",
        "  0.0000000E+00,           !- Z Origin {m}",
        "  1,                       !- Type",
        "  1,                       !- Multiplier",
        "  autocalculate,           !- Ceiling Height {m}",
        "  autocalculate;           !- Volume {m3}",

        "Daylighting:Controls,",
        "  West Zone_DaylCtrl,      !- Name",
        "  West Zone,               !- Zone Name",
        "  SplitFlux,               !- Daylighting Method",
        "  ,                        !- Availability Schedule Name",
        "  Continuous,              !- Lighting Control Type",
        "  0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control",
        "  0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control",
        "  ,                        !- Number of Stepped Control Steps",
        "  1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control",
        "  West Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name",
        "  180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "  20.0,                    !- Maximum Allowable Discomfort Glare Index",
        "  ,                        !- DElight Gridding Resolution {m2}",
        "  West Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name",
        "  0.5011,                  !- Fraction of Zone Controlled by Reference Point 1",
        "  200.0,                   !- Illuminance Setpoint at Reference Point 1",
        "  West Zone_DaylRefPt1,    !- Daylighting Reference Point 2 Name",
        "  0.5,                     !- Fraction of Zone Controlled by Reference Point 2",
        "  200.0;                   !- Illuminance Setpoint at Reference Point 2",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt1,    !- Name",
        "  West Zone,               !- Zone Name",
        "  0.5,                     !- X-Coordinate of Reference Point {m}",
        "  0.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

        "Daylighting:ReferencePoint,",
        "  West Zone_DaylRefPt2,    !- Name",
        "  West Zone,               !- Zone Name",
        "  1.5,                     !- X-Coordinate of Reference Point {m}",
        "  0.5,                     !- Y-Coordinate of Reference Point {m}",
        "  0.8;                     !- Z-Coordinate of Reference Point {m}",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;
    HeatBalanceManager::GetZoneData(foundErrors);
    ASSERT_FALSE(foundErrors);

    int numObjs = inputProcessor->getNumObjectsFound("Daylighting:Controls");
    EXPECT_EQ(1, numObjs);

    DaylightingManager::GetInputDayliteRefPt(foundErrors);
    compare_err_stream("");
    EXPECT_FALSE(foundErrors);
    EXPECT_EQ(2, DataDaylighting::TotRefPoints);

    DaylightingManager::GetDaylightingControls(numObjs, foundErrors);

    std::string const error_string = delimited_string({
      "   ** Severe  ** GetDaylightingControls: Fraction of Zone controlled by the Daylighting reference points is > 1.0.",
      "   **   ~~~   ** ..discovered in \"Daylighting:Controls\" for Zone=\"WEST ZONE\", trying to control 1.001 of the zone.",
    });
    EXPECT_TRUE(compare_err_stream(error_string, true));
    EXPECT_TRUE(foundErrors);
}

TEST_F(EnergyPlusFixture, DaylightingManager_OutputFormats)
{
    // Test for #6976 - Support more than 2 reference points for EIO / DFS output

    std::string const idf_objects = delimited_string({
        "  Zone,                                                                                                           ",
        "    West Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:Controls,                                                                                           ",
        "    West Zone_DaylCtrl,      !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    West Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    West Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    1.0,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.;                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    West Zone_DaylRefPt1,    !- Name                                                                              ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Lights,                                                                                                         ",
        "    West Zone Lights 1,      !- Name                                                                              ",
        "    West Zone,               !- Zone or ZoneList Name                                                             ",
        "    Office Lighting,         !- Schedule Name                                                                     ",
        "    LightingLevel,           !- Design Level Calculation Method                                                   ",
        "    1464.375,                !- Lighting Level {W}                                                                ",
        "    ,                        !- Watts per Zone Floor Area {W/m2}                                                  ",
        "    ,                        !- Watts per Person {W/person}                                                       ",
        "    0.0000000E+00,           !- Return Air Fraction                                                               ",
        "    0.2000000,               !- Fraction Radiant                                                                  ",
        "    0.2000000,               !- Fraction Visible                                                                  ",
        "    1.0,                     !- Fraction Replaceable                                                              ",
        "    GeneralLights;           !- End-Use Subcategory                                                               ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall001,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                         ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall002,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                         ",
        "    0.0000000E+00,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall003,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                        ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    0.0000000E+00,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Wall004,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Flr001,            !- Name                                                                              ",
        "    Floor,                   !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    NoSun,                   !- Sun Exposure                                                                      ",
        "    NoWind,                  !- Wind Exposure                                                                     ",
        "    1.000000,                !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 1 {m}                                         ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn001:Roof001,           !- Name                                                                              ",
        "    Roof,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    West Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.0000000E+00,           !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  FenestrationSurface:Detailed,                                                                                   ",
        "    Zn001:Wall001:Win001,    !- Name                                                                              ",
        "    Window,                  !- Surface Type                                                                      ",
        "    WIN-CON-SINGLEPANE,      !- Construction Name                                                                 ",
        "    Zn001:Wall001,           !- Building Surface Name                                                             ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    ,                        !- Frame and Divider Name                                                            ",
        "    1.0,                     !- Multiplier                                                                        ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.548000,0.0000000E+00,2.5000,  !- X,Y,Z ==> Vertex 1 {m}                                                     ",
        "    0.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 2 {m}                                                     ",
        "    5.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 3 {m}                                                     ",
        "    5.548000,0.0000000E+00,2.5000;  !- X,Y,Z ==> Vertex 4 {m}                                                     ",
        "                                                                                                                  ",
        "  Zone,                                                                                                           ",
        "    East Zone,               !- Name                                                                              ",
        "    0.0000000E+00,           !- Direction of Relative North {deg}                                                 ",
        "    0.0000000E+00,           !- X Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Y Origin {m}                                                                      ",
        "    0.0000000E+00,           !- Z Origin {m}                                                                      ",
        "    1,                       !- Type                                                                              ",
        "    1,                       !- Multiplier                                                                        ",
        "    autocalculate,           !- Ceiling Height {m}                                                                ",
        "    autocalculate;           !- Volume {m3}                                                                       ",
        "                                                                                                                  ",
        "  Daylighting:Controls,                                                                                           ",
        "    East Zone_DaylCtrl,      !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    SplitFlux,               !- Daylighting Method                                                                ",
        "    ,                        !- Availability Schedule Name                                                        ",
        "    Continuous,              !- Lighting Control Type                                                             ",
        "    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control      ",
        "    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control     ",
        "    ,                        !- Number of Stepped Control Steps                                                   ",
        "    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control          ",
        "    East Zone_DaylRefPt1,    !- Glare Calculation Daylighting Reference Point Name                                ",
        "    180.0,                   !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg}",
        "    20.0,                    !- Maximum Allowable Discomfort Glare Index                                          ",
        "    ,                        !- DElight Gridding Resolution {m2}                                                  ",
        "    East Zone_DaylRefPt1,    !- Daylighting Reference Point 1 Name                                                ",
        "    0.6,                     !- Fraction of Zone Controlled by Reference Point 1                                  ",
        "    500.,                    !- Illuminance Setpoint at Reference Point 1 {lux}                                   ",
        "    East Zone_DaylRefPt2,    !- Daylighting Reference Point 2 Name                                                ",
        "    0.3,                     !- Fraction of Zone Controlled by Reference Point 2                                  ",
        "    400.,                    !- Illuminance Setpoint at Reference Point 2 {lux}                                   ",
        "    East Zone_DaylRefPt3,    !- Daylighting Reference Point 3 Name                                                ",
        "    0.1,                     !- Fraction of Zone Controlled by Reference Point 3                                  ",
        "    300.;                    !- Illuminance Setpoint at Reference Point 3 {lux}                                   ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt1,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    3.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt2,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    2.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    1.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Daylighting:ReferencePoint,                                                                                     ",
        "    East Zone_DaylRefPt3,    !- Name                                                                              ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    1.048,                   !- X-Coordinate of Reference Point {m}                                               ",
        "    2.048,                   !- Y-Coordinate of Reference Point {m}                                               ",
        "    0.9;                     !- Z-Coordinate of Reference Point {m}                                               ",
        "                                                                                                                  ",
        "  Lights,                                                                                                         ",
        "    East Zone Lights 1,      !- Name                                                                              ",
        "    East Zone,               !- Zone or ZoneList Name                                                             ",
        "    Office Lighting,         !- Schedule Name                                                                     ",
        "    LightingLevel,           !- Design Level Calculation Method                                                   ",
        "    1464.375,                !- Lighting Level {W}                                                                ",
        "    ,                        !- Watts per Zone Floor Area {W/m2}                                                  ",
        "    ,                        !- Watts per Person {W/person}                                                       ",
        "    0.0000000E+00,           !- Return Air Fraction                                                               ",
        "    0.2000000,               !- Fraction Radiant                                                                  ",
        "    0.2000000,               !- Fraction Visible                                                                  ",
        "    1.0,                     !- Fraction Replaceable                                                              ",
        "    GeneralLights;           !- End-Use Subcategory                                                               ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall001,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                         ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall002,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                         ",
        "    0.0000000E+00,0.0000000E+00,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall003,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                        ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                                   ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                              ",
        "    0.0000000E+00,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                   ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Wall004,           !- Name                                                                              ",
        "    Wall,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Flr001,            !- Name                                                                              ",
        "    Floor,                   !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    NoSun,                   !- Sun Exposure                                                                      ",
        "    NoWind,                  !- Wind Exposure                                                                     ",
        "    1.000000,                !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,0.0000000E+00,0.0000000E+00,  !- X,Y,Z ==> Vertex 1 {m}                                         ",
        "    0.0000000E+00,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,6.096000,0.0000000E+00,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,0.0000000E+00,0.0000000E+00;  !- X,Y,Z ==> Vertex 4 {m}                                              ",
        "                                                                                                                  ",
        "  BuildingSurface:Detailed,                                                                                       ",
        "    Zn002:Roof001,           !- Name                                                                              ",
        "    Roof,                    !- Surface Type                                                                      ",
        "    WALL80,                  !- Construction Name                                                                 ",
        "    East Zone,               !- Zone Name                                                                         ",
        "    Outdoors,                !- Outside Boundary Condition                                                        ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    SunExposed,              !- Sun Exposure                                                                      ",
        "    WindExposed,             !- Wind Exposure                                                                     ",
        "    0.0000000E+00,           !- View Factor to Ground                                                             ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.0000000E+00,6.096000,3.048000,  !- X,Y,Z ==> Vertex 1 {m}                                                   ",
        "    0.0000000E+00,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 2 {m}                                              ",
        "    6.096000,0.0000000E+00,3.048000,  !- X,Y,Z ==> Vertex 3 {m}                                                   ",
        "    6.096000,6.096000,3.048000;  !- X,Y,Z ==> Vertex 4 {m}                                                        ",
        "                                                                                                                  ",
        "  FenestrationSurface:Detailed,                                                                                   ",
        "    Zn002:Wall001:Win001,    !- Name                                                                              ",
        "    Window,                  !- Surface Type                                                                      ",
        "    WIN-CON-SINGLEPANE,      !- Construction Name                                                                 ",
        "    Zn002:Wall001,           !- Building Surface Name                                                             ",
        "    ,                        !- Outside Boundary Condition Object                                                 ",
        "    0.5000000,               !- View Factor to Ground                                                             ",
        "    ,                        !- Frame and Divider Name                                                            ",
        "    1.0,                     !- Multiplier                                                                        ",
        "    4,                       !- Number of Vertices                                                                ",
        "    0.548000,0.0000000E+00,2.5000,  !- X,Y,Z ==> Vertex 1 {m}                                                     ",
        "    0.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 2 {m}                                                     ",
        "    5.548000,0.0000000E+00,0.5000,  !- X,Y,Z ==> Vertex 3 {m}                                                     ",
        "    5.548000,0.0000000E+00,2.5000;  !- X,Y,Z ==> Vertex 4 {m}                                                     ",
        "                                                                                                                  ",
        "  Construction,                                                                                                   ",
        "    WALL80,               !- Name                                                                                 ",
        "    C4 - 4 IN COMMON BRICK;  !- Layer 1                                                                           ",
        "                                                                                                                  ",
        "  Material,                                                                                                       ",
        "    C4 - 4 IN COMMON BRICK,  !- Name                                                                              ",
        "    Rough,                   !- Roughness                                                                         ",
        "    0.1014984,               !- Thickness {m}                                                                     ",
        "    0.7264224,               !- Conductivity {W/m-K}                                                              ",
        "    1922.216,                !- Density {kg/m3}                                                                   ",
        "    836.8000,                !- Specific Heat {J/kg-K}                                                            ",
        "    0.9000000,               !- Thermal Absorptance                                                               ",
        "    0.7600000,               !- Solar Absorptance                                                                 ",
        "    0.7600000;               !- Visible Absorptance                                                               ",
        "                                                                                                                  ",
        "  Schedule:Compact,                                                                                               ",
        "    Office Lighting,         !- Name                                                                              ",
        "    ANY NUMBER,              !- Schedule Type Limits Name                                                         ",
        "    Through: 12/31,          !- Field 1                                                                           ",
        "    For: Alldays,            !- Field 2                                                                           ",
        "    Until: 24:00,1.00;       !- Field 7                                                                           ",
        "                                                                                                                  ",
        "  Construction,                                                                                                   ",
        "    WIN-CON-SINGLEPANE,      !- Name                                                                              ",
        "    SINGLEPANE;              !- Outside Layer                                                                     ",
        "                                                                                                                  ",
        "  WindowMaterial:Glazing,                                                                                         ",
        "    SINGLEPANE,              !- Name                                                                              ",
        "    SpectralAverage,         !- Optical Data Type                                                                 ",
        "    ,                        !- Window Glass Spectral Data Set Name                                               ",
        "    0.003,                   !- Thickness {m}                                                                     ",
        "    0.90,                    !- Solar Transmittance at Normal Incidence                                           ",
        "    0.031,                   !- Front Side Solar Reflectance at Normal Incidence                                  ",
        "    0.031,                   !- Back Side Solar Reflectance at Normal Incidence                                   ",
        "    0.90,                    !- Visible Transmittance at Normal Incidence                                         ",
        "    0.05,                    !- Front Side Visible Reflectance at Normal Incidence                                ",
        "    0.05,                    !- Back Side Visible Reflectance at Normal Incidence                                 ",
        "    0.0,                     !- Infrared Transmittance at Normal Incidence                                        ",
        "    0.84,                    !- Front Side Infrared Hemispherical Emissivity                                      ",
        "    0.84,                    !- Back Side Infrared Hemispherical Emissivity                                       ",
        "    0.9;                     !- Conductivity {W/m-K}                                                              ",
        "SurfaceConvectionAlgorithm:Inside,TARP;",
        "SurfaceConvectionAlgorithm:Outside,DOE-2;",
        "HeatBalanceAlgorithm,ConductionTransferFunction;",
        "ZoneAirHeatBalanceAlgorithm,",
        "    AnalyticalSolution;      !- Algorithm",
        "ScheduleTypeLimits,",
        "    Any Number;              !- Name",

        "  Output:DaylightFactors,",
        "    SizingDays;              !- Reporting Days",

    });

    ASSERT_TRUE(process_idf(idf_objects));

    bool foundErrors = false;

    HeatBalanceManager::GetProjectControlData(state, foundErrors); // read project control data
    EXPECT_FALSE(foundErrors);                              // expect no errors

    HeatBalanceManager::GetMaterialData(state, *state.dataWindowEquivalentLayer, state.files, foundErrors); // read material data
    EXPECT_FALSE(foundErrors);                        // expect no errors

    HeatBalanceManager::GetConstructData(state.files, foundErrors); // read construction data
    compare_err_stream("");
    EXPECT_FALSE(foundErrors); // expect no errors

    HeatBalanceManager::GetZoneData(foundErrors); // read zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::CosZoneRelNorth.allocate(2);
    SurfaceGeometry::SinZoneRelNorth.allocate(2);

    SurfaceGeometry::CosZoneRelNorth(1) = std::cos(-DataHeatBalance::Zone(1).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::SinZoneRelNorth(1) = std::sin(-DataHeatBalance::Zone(1).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::CosZoneRelNorth(2) = std::cos(-DataHeatBalance::Zone(2).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::SinZoneRelNorth(2) = std::sin(-DataHeatBalance::Zone(2).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::CosBldgRelNorth = 1.0;
    SurfaceGeometry::SinBldgRelNorth = 0.0;

    SurfaceGeometry::GetSurfaceData(state, state.files, foundErrors); // setup zone geometry and get zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::SetupZoneGeometry(state, foundErrors); // this calls GetSurfaceData()
    EXPECT_FALSE(foundErrors);                       // expect no errors
    HeatBalanceIntRadExchange::InitSolarViewFactors(state.files);

    DataGlobals::NumOfTimeStepInHour = 1; // must initialize this to get schedules initialized
    DataGlobals::MinutesPerTimeStep = 60; // must initialize this to get schedules initialized
    ScheduleManager::ProcessScheduleInput(state.files);
    ScheduleManager::ScheduleInputProcessed = true;
    DataGlobals::TimeStep = 1;
    DataGlobals::HourOfDay = 1;
    DataGlobals::PreviousHour = 1;
    DataEnvironment::Month = 1;
    DataEnvironment::DayOfMonth = 21;
    DataGlobals::HourOfDay = 1;
    DataEnvironment::DSTIndicator = 0;
    DataEnvironment::DayOfWeek = 2;
    DataEnvironment::HolidayIndex = 0;
    DataEnvironment::CurMnDy = "01/21";
    DataEnvironment::DayOfYear_Schedule = General::OrdinalDay(DataEnvironment::Month, DataEnvironment::DayOfMonth, 1);
    ScheduleManager::UpdateScheduleValues();
    InternalHeatGains::GetInternalHeatGainsInput(state);
    InternalHeatGains::GetInternalHeatGainsInputFlag = false;

    GetDaylightingParametersInput(state.files);
    compare_err_stream("");
    EXPECT_EQ(4, TotRefPoints);

    EXPECT_NEAR(2.048, ZoneDaylight(1).DaylRefPtAbsCoord(1, 1), 0.001);
    EXPECT_NEAR(3.048, ZoneDaylight(1).DaylRefPtAbsCoord(2, 1), 0.001);
    EXPECT_NEAR(0.9, ZoneDaylight(1).DaylRefPtAbsCoord(3, 1), 0.001);

    DataHeatBalance::Zone(1).RelNorth = 45.;

    GeometryTransformForDaylighting();

    EXPECT_NEAR(3.603, ZoneDaylight(1).DaylRefPtAbsCoord(1, 1), 0.001);
    EXPECT_NEAR(0.707, ZoneDaylight(1).DaylRefPtAbsCoord(2, 1), 0.001);
    EXPECT_NEAR(0.9, ZoneDaylight(1).DaylRefPtAbsCoord(3, 1), 0.001);

    DataHeatBalance::Zone(1).RelNorth = 90.;

    GeometryTransformForDaylighting();

    EXPECT_NEAR(3.048, ZoneDaylight(1).DaylRefPtAbsCoord(1, 1), 0.001);
    EXPECT_NEAR(-2.048, ZoneDaylight(1).DaylRefPtAbsCoord(2, 1), 0.001);
    EXPECT_NEAR(0.9, ZoneDaylight(1).DaylRefPtAbsCoord(3, 1), 0.001);

    // reset eio stream
    EXPECT_TRUE(has_eio_output(true));
    EXPECT_FALSE(has_dfs_output(true));

    DataGlobals::BeginSimFlag = true;
    DataGlobals::WeightNow = 1.0;
    DataGlobals::WeightPreviousHour = 0.0;
    CalcDayltgCoefficients(state.files);
    int zoneNum = 1;
    // test that tmp arrays are allocated to correct dimension
    // zone 1 has only 1 daylighting reference point
    DayltgInteriorIllum(zoneNum);
    zoneNum += 1;
    // zone 2 has 2 daylighting reference points and will crash if not dimensioned appropriately.
    DayltgInteriorIllum(zoneNum);

    // EIO/DFS output uses specifically newline `\n`, so pass that in or on Windows it'll use '\r\n`
    std::string const delim = "\n";

    std::string const eiooutput = delimited_string({
        "! <Zone/Window Adjacency Daylighting Counts>, Zone Name, Number of Exterior Windows, Number of Exterior Windows in Adjacent Zones",
        "Zone/Window Adjacency Daylighting Counts, WEST ZONE,2,-1",
        "Zone/Window Adjacency Daylighting Counts, EAST ZONE,2,-1",
        "! <Zone/Window Adjacency Daylighting Matrix>, Zone Name, Number of Adjacent Zones with Windows,Adjacent Zone Names - 1st 100 (max)",
        "Zone/Window Adjacency Daylighting Matrix, WEST ZONE,0",
        "Zone/Window Adjacency Daylighting Matrix, EAST ZONE,0",
        "! <Sky Daylight Factors>, MonthAndDay, Zone Name, Window Name, Reference Point, Daylight Factor",
        " Sky Daylight Factors,Clear Sky,01/21,WEST ZONE,ZN001:WALL001:WIN001,WEST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Clear Turbid Sky,01/21,WEST ZONE,ZN001:WALL001:WIN001,WEST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Intermediate Sky,01/21,WEST ZONE,ZN001:WALL001:WIN001,WEST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Overcast Sky,01/21,WEST ZONE,ZN001:WALL001:WIN001,WEST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Clear Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Clear Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT2,0.0000",
        " Sky Daylight Factors,Clear Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT3,0.0000",
        " Sky Daylight Factors,Clear Turbid Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Clear Turbid Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT2,0.0000",
        " Sky Daylight Factors,Clear Turbid Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT3,0.0000",
        " Sky Daylight Factors,Intermediate Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Intermediate Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT2,0.0000",
        " Sky Daylight Factors,Intermediate Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT3,0.0000",
        " Sky Daylight Factors,Overcast Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT1,0.0000",
        " Sky Daylight Factors,Overcast Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT2,0.0000",
        " Sky Daylight Factors,Overcast Sky,01/21,EAST ZONE,ZN002:WALL001:WIN001,EAST ZONE_DAYLREFPT3,0.0000",
    }, delim);

    EXPECT_TRUE(compare_eio_stream(eiooutput, true));

    std::string const dfsoutput = delimited_string({

        "This file contains daylight factors for all exterior windows of daylight zones.",
        "MonthAndDay,Zone Name,Window Name,Window State",
        "Hour,Reference Point,Daylight Factor for Clear Sky,Daylight Factor for Clear Turbid Sky,Daylight Factor for Intermediate Sky,Daylight Factor for Overcast Sky",
        "01/21,WEST ZONE,ZN001:WALL001:WIN001,Base Window",
        "1,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "2,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "3,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "4,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "5,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "6,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "7,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "8,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "9,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "10,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "11,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "12,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "13,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "14,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "15,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "16,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "17,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "18,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "19,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "20,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "21,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "22,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "23,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "24,WEST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "01/21,EAST ZONE,ZN002:WALL001:WIN001,Base Window",
        "1,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "1,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "1,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "2,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "2,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "2,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "3,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "3,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "3,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "4,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "4,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "4,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "5,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "5,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "5,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "6,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "6,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "6,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "7,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "7,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "7,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "8,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "8,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "8,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "9,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "9,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "9,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "10,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "10,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "10,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "11,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "11,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "11,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "12,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "12,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "12,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "13,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "13,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "13,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "14,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "14,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "14,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "15,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "15,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "15,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "16,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "16,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "16,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "17,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "17,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "17,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "18,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "18,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "18,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "19,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "19,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "19,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "20,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "20,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "20,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "21,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "21,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "21,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "22,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "22,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "22,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "23,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "23,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "23,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
        "24,EAST ZONE_DAYLREFPT1,0.00000,0.00000,0.00000,0.00000",
        "24,EAST ZONE_DAYLREFPT2,0.00000,0.00000,0.00000,0.00000",
        "24,EAST ZONE_DAYLREFPT3,0.00000,0.00000,0.00000,0.00000",
    }, delim);

    EXPECT_TRUE(compare_dfs_stream(dfsoutput, true));
}

TEST_F(EnergyPlusFixture, DaylightingManager_TDD_NoDaylightingControls)
{
    std::string const idf_objects = delimited_string({
        "  Zone,",
        "    Daylit Zone,             !- Name",
        "    0.0,                     !- Direction of Relative North {deg}",
        "    0.0,                     !- X Origin {m}",
        "    0.0,                     !- Y Origin {m}",
        "    0.0,                     !- Z Origin {m}",
        "    1,                       !- Type",
        "    1,                       !- Multiplier",
        "    autocalculate,           !- Ceiling Height {m}",
        "    autocalculate;           !- Volume {m3}",

        "  BuildingSurface:Detailed,",
        "    Daylit South Wall,       !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Zone,             !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,0.0,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,0.0,0.0,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,0.0,0.0,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,0.0,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit West Wall,        !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Zone,             !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,10.0,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,10.0,0.0,  !- X,Y,Z ==> Vertex 2 {m}",
        "    0.0,0.0,0.0,  !- X,Y,Z ==> Vertex 3 {m}",
        "    0.0,0.0,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit North Wall,       !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Zone,             !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    5.0,10.0,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    5.0,10.0,0.0,  !- X,Y,Z ==> Vertex 2 {m}",
        "    0.0,10.0,0.0,  !- X,Y,Z ==> Vertex 3 {m}",
        "    0.0,10.0,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit East Wall,        !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Zone,             !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    5.0,0.0,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    5.0,0.0,0.0,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,10.0,0.0,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,10.0,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Floor,            !- Name",
        "    Floor,                   !- Surface Type",
        "    FLOOR SLAB 8 IN,         !- Construction Name",
        "    Daylit Zone,             !- Zone Name",
        "    Surface,                 !- Outside Boundary Condition",
        "    Daylit Floor,            !- Outside Boundary Condition Object",
        "    NoSun,                   !- Sun Exposure",
        "    NoWind,                  !- Wind Exposure",
        "    1.0,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,0.0,0.0,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,10.0,0.0,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,10.0,0.0,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,0.0,0.0;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Ceiling,          !- Name",
        "    Roof,                    !- Surface Type",
        "    CEILING IN ZONE,         !- Construction Name",
        "    Daylit Zone,             !- Zone Name",
        "    Surface,                 !- Outside Boundary Condition",
        "    Daylit Attic Floor,      !- Outside Boundary Condition Object",
        "    NoSun,                   !- Sun Exposure",
        "    NoWind,                  !- Wind Exposure",
        "    0.0,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,10.0,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,0.0,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,0.0,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,10.0,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  Zone,",
        "    Daylit Attic Zone,       !- Name",
        "    0.0,                     !- Direction of Relative North {deg}",
        "    0.0,                     !- X Origin {m}",
        "    0.0,                     !- Y Origin {m}",
        "    0.0,                     !- Z Origin {m}",
        "    1,                       !- Type",
        "    1,                       !- Multiplier",
        "    autocalculate,           !- Ceiling Height {m}",
        "    autocalculate;           !- Volume {m3}",

        "  BuildingSurface:Detailed,",
        "    Daylit Attic South Wall, !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Attic Zone,       !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,0.0,3.0,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,0.0,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,0.0,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,0.0,3.0;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Attic West Wall,  !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Attic Zone,       !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,10.0,5.0,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,10.0,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    0.0,0.0,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    0.0,0.0,3.0;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Attic North Wall, !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Attic Zone,       !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    5.0,10.0,5.0,  !- X,Y,Z ==> Vertex 1 {m}",
        "    5.0,10.0,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    0.0,10.0,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    0.0,10.0,5.0;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Attic East Wall,  !- Name",
        "    Wall,                    !- Surface Type",
        "    EXTWALL80,               !- Construction Name",
        "    Daylit Attic Zone,       !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.5,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    5.0,0.0,3.0,  !- X,Y,Z ==> Vertex 1 {m}",
        "    5.0,0.0,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,10.0,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,10.0,5.0;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Attic Floor,      !- Name",
        "    Floor,                   !- Surface Type",
        "    CEILING IN ATTIC,        !- Construction Name",
        "    Daylit Attic Zone,       !- Zone Name",
        "    Surface,                 !- Outside Boundary Condition",
        "    Daylit Ceiling,          !- Outside Boundary Condition Object",
        "    NoSun,                   !- Sun Exposure",
        "    NoWind,                  !- Wind Exposure",
        "    0.0,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,0.0,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,10.0,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,10.0,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,0.0,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  BuildingSurface:Detailed,",
        "    Daylit Attic Roof,       !- Name",
        "    Roof,                    !- Surface Type",
        "    ROOF,                    !- Construction Name",
        "    Daylit Attic Zone,       !- Zone Name",
        "    Outdoors,                !- Outside Boundary Condition",
        "    ,                        !- Outside Boundary Condition Object",
        "    SunExposed,              !- Sun Exposure",
        "    WindExposed,             !- Wind Exposure",
        "    0.0,                     !- View Factor to Ground",
        "    4,                       !- Number of Vertices",
        "    0.0,10.0,5.0,  !- X,Y,Z ==> Vertex 1 {m}",
        "    0.0,0.0,3.0,  !- X,Y,Z ==> Vertex 2 {m}",
        "    5.0,0.0,3.0,  !- X,Y,Z ==> Vertex 3 {m}",
        "    5.0,10.0,5.0;  !- X,Y,Z ==> Vertex 4 {m}",

        "  DaylightingDevice:Tubular,",
        "    Pipe1,                   !- Name",
        "    Dome1,                   !- Dome Name",
        "    Diffuser1,               !- Diffuser Name",
        "    TDD Pipe,                !- Construction Name",
        "    0.3556,                  !- Diameter {m}",
        "    1.4,                     !- Total Length {m}",
        "    0.28,                    !- Effective Thermal Resistance {m2-K/W}",
        "    Daylit Attic Zone,       !- Transition Zone 1 Name",
        "    1.1;                     !- Transition Zone 1 Length {m}",

        "  FenestrationSurface:Detailed,",
        "    Dome1,                   !- Name",
        "    TubularDaylightDome,     !- Surface Type",
        "    TDD Dome,                !- Construction Name",
        "    Daylit Attic Roof,       !- Building Surface Name",
        "    ,                        !- Outside Boundary Condition Object",
        "    0.0,                     !- View Factor to Ground",
        "    ,                        !- Frame and Divider Name",
        "    1.0,                     !- Multiplier",
        "    4,                       !- Number of Vertices",
        "    2.3425,3.209,3.64,  !- X,Y,Z ==> Vertex 1 {m}",
        "    2.3425,2.906,3.58,  !- X,Y,Z ==> Vertex 2 {m}",
        "    2.6575,2.906,3.58,  !- X,Y,Z ==> Vertex 3 {m}",
        "    2.6575,3.209,3.64;  !- X,Y,Z ==> Vertex 4 {m}",

        "  FenestrationSurface:Detailed,",
        "    Diffuser1,               !- Name",
        "    TubularDaylightDiffuser, !- Surface Type",
        "    TDD Diffuser,            !- Construction Name",
        "    Daylit Ceiling,          !- Building Surface Name",
        "    ,                        !- Outside Boundary Condition Object",
        "    0.0,                     !- View Factor to Ground",
        "    ,                        !- Frame and Divider Name",
        "    1.0,                     !- Multiplier",
        "    4,                       !- Number of Vertices",
        "    2.3425,3.1575,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    2.3425,2.8425,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    2.6575,2.8425,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    2.6575,3.1575,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  DaylightingDevice:Tubular,",
        "    Pipe2,                   !- Name",
        "    Dome2,                   !- Dome Name",
        "    Diffuser2,               !- Diffuser Name",
        "    TDD Pipe,                !- Construction Name",
        "    0.3556,                  !- Diameter {m}",
        "    2.2,                     !- Total Length {m}",
        "    0.28,                    !- Effective Thermal Resistance {m2-K/W}",
        "    Daylit Attic Zone,       !- Transition Zone 1 Name",
        "    1.9;                     !- Transition Zone 1 Length {m}",

        "  FenestrationSurface:Detailed,",
        "    Dome2,                   !- Name",
        "    TubularDaylightDome,     !- Surface Type",
        "    TDD Dome,                !- Construction Name",
        "    Daylit Attic Roof,       !- Building Surface Name",
        "    ,                        !- Outside Boundary Condition Object",
        "    0.0,                     !- View Factor to Ground",
        "    ,                        !- Frame and Divider Name",
        "    1.0,                     !- Multiplier",
        "    4,                       !- Number of Vertices",
        "    2.3425,7.209134615385,4.441826923077,  !- X,Y,Z ==> Vertex 1 {m}",
        "    2.3425,6.90625,4.38125,  !- X,Y,Z ==> Vertex 2 {m}",
        "    2.6575,6.90625,4.38125,  !- X,Y,Z ==> Vertex 3 {m}",
        "    2.6575,7.209134615385,4.441826923077;  !- X,Y,Z ==> Vertex 4 {m}",

        "  FenestrationSurface:Detailed,",
        "    Diffuser2,               !- Name",
        "    TubularDaylightDiffuser, !- Surface Type",
        "    TDD Diffuser,            !- Construction Name",
        "    Daylit Ceiling,          !- Building Surface Name",
        "    ,                        !- Outside Boundary Condition Object",
        "    0.0,                     !- View Factor to Ground",
        "    ,                        !- Frame and Divider Name",
        "    1.0,                     !- Multiplier",
        "    4,                       !- Number of Vertices",
        "    2.3425,7.1575,2.5,  !- X,Y,Z ==> Vertex 1 {m}",
        "    2.3425,6.8425,2.5,  !- X,Y,Z ==> Vertex 2 {m}",
        "    2.6575,6.8425,2.5,  !- X,Y,Z ==> Vertex 3 {m}",
        "    2.6575,7.1575,2.5;  !- X,Y,Z ==> Vertex 4 {m}",

        "  Material,",
        "    A1 - 1 IN STUCCO,        !- Name",
        "    Smooth,                  !- Roughness",
        "    2.5389841E-02,           !- Thickness {m}",
        "    0.6918309,               !- Conductivity {W/m-K}",
        "    1858.142,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.92,                    !- Solar Absorptance",
        "    0.92;                    !- Visible Absorptance",

        "  Material,",
        "    C4 - 4 IN COMMON BRICK,  !- Name",
        "    Rough,                   !- Roughness",
        "    0.1014984,               !- Thickness {m}",
        "    0.7264224,               !- Conductivity {W/m-K}",
        "    1922.216,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.76,                    !- Solar Absorptance",
        "    0.76;                    !- Visible Absorptance",

        "  Material,",
        "    E1 - 3 / 4 IN PLASTER OR GYP BOARD,  !- Name",
        "    Smooth,                  !- Roughness",
        "    1.9050000E-02,           !- Thickness {m}",
        "    0.7264224,               !- Conductivity {W/m-K}",
        "    1601.846,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.92,                    !- Solar Absorptance",
        "    0.92;                    !- Visible Absorptance",

        "  Material,",
        "    C6 - 8 IN CLAY TILE,     !- Name",
        "    Smooth,                  !- Roughness",
        "    0.2033016,               !- Thickness {m}",
        "    0.5707605,               !- Conductivity {W/m-K}",
        "    1121.292,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.82,                    !- Solar Absorptance",
        "    0.82;                    !- Visible Absorptance",

        "  Material,",
        "    C10 - 8 IN HW CONCRETE,  !- Name",
        "    MediumRough,             !- Roughness",
        "    0.2033016,               !- Thickness {m}",
        "    1.729577,                !- Conductivity {W/m-K}",
        "    2242.585,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.65,                    !- Solar Absorptance",
        "    0.65;                    !- Visible Absorptance",

        "  Material,",
        "    E2 - 1 / 2 IN SLAG OR STONE,  !- Name",
        "    Rough,                   !- Roughness",
        "    1.2710161E-02,           !- Thickness {m}",
        "    1.435549,                !- Conductivity {W/m-K}",
        "    881.0155,                !- Density {kg/m3}",
        "    1673.6,                  !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.55,                    !- Solar Absorptance",
        "    0.55;                    !- Visible Absorptance",

        "  Material,",
        "    E3 - 3 / 8 IN FELT AND MEMBRANE,  !- Name",
        "    Rough,                   !- Roughness",
        "    9.5402403E-03,           !- Thickness {m}",
        "    0.1902535,               !- Conductivity {W/m-K}",
        "    1121.292,                !- Density {kg/m3}",
        "    1673.6,                  !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.75,                    !- Solar Absorptance",
        "    0.75;                    !- Visible Absorptance",

        "  Material,",
        "    B5 - 1 IN DENSE INSULATION,  !- Name",
        "    VeryRough,               !- Roughness",
        "    2.5389841E-02,           !- Thickness {m}",
        "    4.3239430E-02,           !- Conductivity {W/m-K}",
        "    91.30524,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.50,                    !- Solar Absorptance",
        "    0.50;                    !- Visible Absorptance",

        "  Material,",
        "    C12 - 2 IN HW CONCRETE,  !- Name",
        "    MediumRough,             !- Roughness",
        "    5.0901599E-02,           !- Thickness {m}",
        "    1.729577,                !- Conductivity {W/m-K}",
        "    2242.585,                !- Density {kg/m3}",
        "    836.8,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.65,                    !- Solar Absorptance",
        "    0.65;                    !- Visible Absorptance",

        "  Material,",
        "    ROOFING - ASPHALT SHINGLES,  !- Name",
        "    VeryRough,               !- Roughness",
        "    3.1999999E-03,           !- Thickness {m}",
        "    2.9999999E-02,           !- Conductivity {W/m-K}",
        "    1121.29,                 !- Density {kg/m3}",
        "    830.0,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.70,                    !- Solar Absorptance",
        "    0.70;                    !- Visible Absorptance",

        "  Material,",
        "    BB46 - 5 / 8 IN PLYWOOD, !- Name",
        "    Smooth,                  !- Roughness",
        "    9.9999998E-03,           !- Thickness {m}",
        "    0.110,                   !- Conductivity {W/m-K}",
        "    544.62,                  !- Density {kg/m3}",
        "    1210.0,                  !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.70,                    !- Solar Absorptance",
        "    0.70;                    !- Visible Absorptance",

        "  Material,",
        "    INS - GLASS FIBER BONDED 3 IN,  !- Name",
        "    VeryRough,               !- Roughness",
        "    7.000E-02,               !- Thickness {m}",
        "    2.9999999E-02,           !- Conductivity {W/m-K}",
        "    96.11,                   !- Density {kg/m3}",
        "    790.0,                   !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.50,                    !- Solar Absorptance",
        "    0.50;                    !- Visible Absorptance",

        "  WindowMaterial:Glazing,",
        "    Clear Acrylic Plastic,   !- Name",
        "    SpectralAverage,         !- Optical Data Type",
        "    ,                        !- Window Glass Spectral Data Set Name",
        "    0.003,                   !- Thickness {m}",
        "    0.92,                    !- Solar Transmittance at Normal Incidence",
        "    0.05,                    !- Front Side Solar Reflectance at Normal Incidence",
        "    0.05,                    !- Back Side Solar Reflectance at Normal Incidence",
        "    0.92,                    !- Visible Transmittance at Normal Incidence",
        "    0.05,                    !- Front Side Visible Reflectance at Normal Incidence",
        "    0.05,                    !- Back Side Visible Reflectance at Normal Incidence",
        "    0.00,                    !- Infrared Transmittance at Normal Incidence",
        "    0.90,                    !- Front Side Infrared Hemispherical Emissivity",
        "    0.90,                    !- Back Side Infrared Hemispherical Emissivity",
        "    0.90;                    !- Conductivity {W/m-K}",

        "  WindowMaterial:Glazing,",
        "    Diffusing Acrylic Plastic,  !- Name",
        "    SpectralAverage,         !- Optical Data Type",
        "    ,                        !- Window Glass Spectral Data Set Name",
        "    0.0022,                  !- Thickness {m}",
        "    0.90,                    !- Solar Transmittance at Normal Incidence",
        "    0.08,                    !- Front Side Solar Reflectance at Normal Incidence",
        "    0.08,                    !- Back Side Solar Reflectance at Normal Incidence",
        "    0.90,                    !- Visible Transmittance at Normal Incidence",
        "    0.08,                    !- Front Side Visible Reflectance at Normal Incidence",
        "    0.08,                    !- Back Side Visible Reflectance at Normal Incidence",
        "    0.00,                    !- Infrared Transmittance at Normal Incidence",
        "    0.90,                    !- Front Side Infrared Hemispherical Emissivity",
        "    0.90,                    !- Back Side Infrared Hemispherical Emissivity",
        "    0.90;                    !- Conductivity {W/m-K}",

        "  Material,",
        "    Very High Reflectivity Surface,  !- Name",
        "    Smooth,                  !- Roughness",
        "    0.0005,                  !- Thickness {m}",
        "    237,                     !- Conductivity {W/m-K}",
        "    2702,                    !- Density {kg/m3}",
        "    903,                     !- Specific Heat {J/kg-K}",
        "    0.90,                    !- Thermal Absorptance",
        "    0.05,                    !- Solar Absorptance",
        "    0.05;                    !- Visible Absorptance",

        "  Construction,",
        "    EXTWALL80,               !- Name",
        "    A1 - 1 IN STUCCO,        !- Outside Layer",
        "    C4 - 4 IN COMMON BRICK,  !- Layer 2",
        "    E1 - 3 / 4 IN PLASTER OR GYP BOARD;  !- Layer 3",

        "  Construction,",
        "    FLOOR SLAB 8 IN,         !- Name",
        "    C10 - 8 IN HW CONCRETE;  !- Outside Layer",

        "  Construction,",
        "    ROOF,                    !- Name",
        "    ROOFING - ASPHALT SHINGLES,  !- Outside Layer",
        "    E3 - 3 / 8 IN FELT AND MEMBRANE,  !- Layer 2",
        "    BB46 - 5 / 8 IN PLYWOOD; !- Layer 3",

        "  Construction,",
        "    CEILING IN ZONE,         !- Name",
        "    INS - GLASS FIBER BONDED 3 IN,  !- Outside Layer",
        "    E1 - 3 / 4 IN PLASTER OR GYP BOARD;  !- Layer 2",

        "  Construction,",
        "    CEILING IN ATTIC,        !- Name",
        "    E1 - 3 / 4 IN PLASTER OR GYP BOARD,  !- Outside Layer",
        "    INS - GLASS FIBER BONDED 3 IN;  !- Layer 2",

        "  Construction,",
        "    TDD Pipe,                !- Name",
        "    Very High Reflectivity Surface;  !- Outside Layer",

        "  Construction,",
        "    TDD Dome,                !- Name",
        "    Clear Acrylic Plastic;   !- Outside Layer",

        "  Construction,",
        "    TDD Diffuser,            !- Name",
        "    Diffusing Acrylic Plastic;  !- Outside Layer",

    });

    ASSERT_TRUE(process_idf(idf_objects));
    bool foundErrors = false;

    HeatBalanceManager::GetProjectControlData(state, foundErrors); // read project control data
    EXPECT_FALSE(foundErrors);                              // expect no errors

    HeatBalanceManager::GetMaterialData(state, *state.dataWindowEquivalentLayer, state.files, foundErrors); // read material data
    EXPECT_FALSE(foundErrors);                        // expect no errors

    HeatBalanceManager::GetConstructData(state.files, foundErrors); // read construction data
    compare_err_stream("");
    EXPECT_FALSE(foundErrors); // expect no errors

    HeatBalanceManager::GetZoneData(foundErrors); // read zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::CosZoneRelNorth.allocate(2);
    SurfaceGeometry::SinZoneRelNorth.allocate(2);

    SurfaceGeometry::CosZoneRelNorth(1) = std::cos(-DataHeatBalance::Zone(1).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::SinZoneRelNorth(1) = std::sin(-DataHeatBalance::Zone(1).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::CosZoneRelNorth(2) = std::cos(-DataHeatBalance::Zone(2).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::SinZoneRelNorth(2) = std::sin(-DataHeatBalance::Zone(2).RelNorth * DataGlobals::DegToRadians);
    SurfaceGeometry::CosBldgRelNorth = 1.0;
    SurfaceGeometry::SinBldgRelNorth = 0.0;

    SurfaceGeometry::GetSurfaceData(state, state.files, foundErrors); // setup zone geometry and get zone data
    EXPECT_FALSE(foundErrors);                    // expect no errors

    SurfaceGeometry::SetupZoneGeometry(state, foundErrors); // this calls GetSurfaceData()
    EXPECT_FALSE(foundErrors);                       // expect no errors
    HeatBalanceIntRadExchange::InitSolarViewFactors(state.files);

    dataConstruction.Construct(Surface(7).Construction).TransDiff = 0.001;  // required for GetTDDInput function to work.
    DaylightingDevices::GetTDDInput();
    CalcDayltgCoefficients(state.files);

    std::string const error_string = delimited_string({
      "   ** Warning ** DaylightingDevice:Tubular = PIPE1:  is not connected to a Zone that has Daylighting, no visible transmittance will be modeled through the daylighting device.",
      "   ** Warning ** DaylightingDevice:Tubular = PIPE2:  is not connected to a Zone that has Daylighting, no visible transmittance will be modeled through the daylighting device.",
    });
    EXPECT_TRUE(compare_err_stream(error_string, true));
}
