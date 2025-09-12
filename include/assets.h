#ifndef ASSETS_H
#define ASSETS_H

#include <globals.h>

//
extern const unsigned char KBStatusKBStatus0 [] PROGMEM;
extern const unsigned char KBStatusKBStatus1 [] PROGMEM;
extern const unsigned char KBStatusKBStatus2 [] PROGMEM;
extern const unsigned char KBStatusKBStatus3 [] PROGMEM;
extern const unsigned char KBStatusKBStatus4 [] PROGMEM;
extern const unsigned char KBStatusKBStatus5 [] PROGMEM;
extern const unsigned char KBStatusKBStatus6 [] PROGMEM;
extern const unsigned char KBStatusKBStatus7 [] PROGMEM;
extern const int KBStatusallArray_LEN;
extern const unsigned char* KBStatusallArray[8];

//
extern const unsigned char backgroundaero [] PROGMEM;
extern const unsigned char backgroundbliss [] PROGMEM;
extern const int backgroundallArray_LEN;
extern const unsigned char* backgroundallArray[2];

//
extern const unsigned char textApp [] PROGMEM;

//
extern const unsigned char _homeIcons2 [] PROGMEM;
extern const unsigned char _homeIcons3 [] PROGMEM;
extern const unsigned char _homeIcons4 [] PROGMEM;
extern const unsigned char _homeIcons5 [] PROGMEM;
extern const unsigned char _homeIcons6 [] PROGMEM;
extern const unsigned char _homeIcons7 [] PROGMEM;
extern const unsigned char _homeIcons8 [] PROGMEM;
extern const unsigned char _homeIcons9 [] PROGMEM;
extern const unsigned char _homeIcons10 [] PROGMEM;
extern const unsigned char _homeIcons11 [] PROGMEM;
extern const unsigned char* homeIconsAllArray[10];

extern const unsigned char _noIconFound [] PROGMEM;

//
extern const unsigned char fileWizardfileWiz0 [] PROGMEM;
extern const unsigned char fileWizardfileWiz1 [] PROGMEM;
extern const unsigned char fileWizardfileWiz2 [] PROGMEM;
extern const unsigned char fileWizardfileWiz3 [] PROGMEM;
extern const int fileWizardallArray_LEN;
extern const unsigned char* fileWizardallArray[4];

//
extern const unsigned char fileWizLitefileWizLite0 [] PROGMEM;
extern const unsigned char fileWizLitefileWizLite1 [] PROGMEM;
extern const unsigned char fileWizLitefileWizLite2 [] PROGMEM;
extern const unsigned char fileWizLitefileWizLite3 [] PROGMEM;
extern const int fileWizLiteallArray_LEN;
extern const unsigned char* fileWizLiteallArray[4];

//
extern const unsigned char sleep0 [] PROGMEM;
extern const unsigned char sleep1 [] PROGMEM;

//
extern const unsigned char nowLaternowAndLater0 [] PROGMEM;
extern const unsigned char nowLaternowAndLater1 [] PROGMEM;
extern const unsigned char nowLaternowAndLater2 [] PROGMEM;
extern const unsigned char nowLaternowAndLater3 [] PROGMEM;
extern const int nowLaterallArray_LEN;
extern const unsigned char* nowLaterallArray[4];

//
extern const unsigned char tasksApp0 [] PROGMEM;
extern const unsigned char tasksApp1 [] PROGMEM;

//
extern const unsigned char taskIconTasks0 [] PROGMEM;

//
extern const unsigned char fontfont0 [] PROGMEM;

//
extern const unsigned char scrolloled0 [] PROGMEM;

//
extern const unsigned char _ScreenSaver0 [] PROGMEM;
extern const unsigned char _ScreenSaver1 [] PROGMEM;
extern const unsigned char _ScreenSaver2 [] PROGMEM;
extern const unsigned char _ScreenSaver3 [] PROGMEM;
extern const unsigned char _ScreenSaver4 [] PROGMEM;
extern const unsigned char _ScreenSaver5 [] PROGMEM;
extern const unsigned char _ScreenSaver6 [] PROGMEM;
extern const unsigned char _ScreenSaver7 [] PROGMEM;
extern const unsigned char _ScreenSaver8 [] PROGMEM;
extern const unsigned char _ScreenSaver9 [] PROGMEM;
extern const unsigned char _ScreenSaver10 [] PROGMEM;
extern const unsigned char _ScreenSaver11 [] PROGMEM;
extern const unsigned char _ScreenSaver12 [] PROGMEM;
extern const unsigned char _ScreenSaver13 [] PROGMEM;
extern const unsigned char _ScreenSaver14 [] PROGMEM;
extern const unsigned char _ScreenSaver15 [] PROGMEM;
extern const unsigned char _ScreenSaver16 [] PROGMEM;
extern const unsigned char _ScreenSaver17 [] PROGMEM;
extern const unsigned char* ScreenSaver_allArray[18];

//
extern const unsigned char _batt0 [] PROGMEM;
extern const unsigned char _batt1 [] PROGMEM;
extern const unsigned char _batt2 [] PROGMEM;
extern const unsigned char _batt3 [] PROGMEM;
extern const unsigned char _batt4 [] PROGMEM;
extern const unsigned char _batt5 [] PROGMEM;
extern const unsigned char* batt_allArray[6];

//
extern const unsigned char _settings [] PROGMEM;

//
extern const unsigned char _toggle [] PROGMEM;
extern const unsigned char _toggleON [] PROGMEM;
extern const unsigned char _toggleOFF [] PROGMEM;

//
extern const unsigned char _calendar00 [] PROGMEM;
extern const unsigned char _calendar01 [] PROGMEM;
extern const unsigned char _calendar02 [] PROGMEM;
extern const unsigned char _calendar03 [] PROGMEM;
extern const unsigned char _calendar04 [] PROGMEM;
extern const unsigned char _calendar05 [] PROGMEM;
extern const unsigned char _calendar06 [] PROGMEM;
extern const unsigned char _calendar07 [] PROGMEM;
extern const unsigned char _calendar08 [] PROGMEM;
extern const unsigned char _calendar09 [] PROGMEM;
extern const unsigned char _calendar10 [] PROGMEM;
extern const unsigned char* calendar_allArray[11];

//
extern const unsigned char _lex0 [] PROGMEM;
extern const unsigned char _lex1 [] PROGMEM;
extern const unsigned char* lex_allArray[2];

//
extern const unsigned char _usb [] PROGMEM;

//
extern const unsigned char _eventMarker0 [] PROGMEM;
extern const unsigned char _eventMarker1 [] PROGMEM;

//
extern const unsigned char _journal [] PROGMEM;

//
extern const unsigned char _appLoader [] PROGMEM;

// File icons
extern const unsigned char _LFileIcon0 [] PROGMEM;
extern const unsigned char _LFileIcon1 [] PROGMEM;
extern const unsigned char _LFileIcon2 [] PROGMEM;
extern const unsigned char _LFileIcon3 [] PROGMEM;
extern const unsigned char* _LFileIcons[4];

extern const unsigned char _SFileIcon0 [] PROGMEM;
extern const unsigned char _SFileIcon1 [] PROGMEM;
extern const unsigned char _SFileIcon2 [] PROGMEM;
extern const unsigned char _SFileIcon3 [] PROGMEM;
extern const unsigned char* _SFileIcons[4];


//

extern const unsigned char calcApp0 [] PROGMEM;
extern const int calcAllArray_LEN;
extern const unsigned char* calcAllArray[1];





#pragma region setupData
#pragma region helpText

const char hl0[]  PROGMEM = "\n";
const char hl1[]  PROGMEM = " vvv scroll down vvv\n";
const char hl2[]  PROGMEM = " This is the help screen\n";
const char hl3[]  PROGMEM = "\n";
const char hl4[]  PROGMEM = " press enter to exit\n";
const char hl5[]  PROGMEM = "\n";
const char hl6[]  PROGMEM = " NOTES:\n";
const char hl7[]  PROGMEM = "~C~/6 -> EXIT APP\n";
const char hl8[]  PROGMEM = "\n";
const char hl9[]  PROGMEM = "~C~scroll <- ->\n";
const char hl10[] PROGMEM = "\n";
const char hl11[] PROGMEM = "~C~',,' equals '.'\n";
const char hl12[] PROGMEM = "\n";
const char hl13[] PROGMEM = " vvv scroll down vvv\n";
const char hl14[] PROGMEM = "\n";
const char hl15[] PROGMEM = "\n";
const char hl16[] PROGMEM = " commands:\n";
const char hl17[] PROGMEM = "\n";
const char hl18[] PROGMEM = "~C~'/' + <command> \n";
const char hl19[] PROGMEM = " 0 : standard\n";
const char hl20[] PROGMEM = "  \n";
const char hl21[] PROGMEM = " 1 : programming\n";
const char hl22[] PROGMEM = "~C~(not implemented) \n";
const char hl23[] PROGMEM = " 2 : scientific \n";
const char hl24[] PROGMEM = "\n";
const char hl25[] PROGMEM = " 3 : conversions \n";
const char hl26[] PROGMEM = " <- = convA select \n";
const char hl27[] PROGMEM = " -> = convB select \n";
const char hl28[] PROGMEM = " select = unit select \n";
const char hl29[] PROGMEM = " tab = dir select \n";
const char hl30[] PROGMEM = " ' ' = main screen \n";
const char hl31[] PROGMEM = " \n";
const char hl32[] PROGMEM = " 4 : help\n";
const char hl33[] PROGMEM = "  \n";
const char hl34[] PROGMEM = " 5 : export to txt\n";
const char hl35[] PROGMEM = "  \n";
const char hl36[] PROGMEM = " 6 : EXIT\n";
const char hl37[] PROGMEM = "  \n";
const char hl38[] PROGMEM = " rad : radian mode\n";
const char hl39[] PROGMEM = "  \n";
const char hl40[] PROGMEM = " deg : degree mode\n";
const char hl41[] PROGMEM = "  \n";
const char hl42[] PROGMEM = " gon : gradian mode\n";
const char hl43[] PROGMEM = "  \n";
const char hl44[] PROGMEM = " keyboard changes:\n";
const char hl45[] PROGMEM = "~C~default kb state:FUNC\n";
const char hl46[] PROGMEM = "~C~tab && fn(tab) == font\n";
const char hl47[] PROGMEM = "~C~bksp == fn(bskp)\n";
const char hl48[] PROGMEM = "~C~left arrow scroll  ^ \n";
const char hl49[] PROGMEM = "~C~right arrow scroll v \n";
const char hl50[] PROGMEM = "operators:\n";
const char hl51[] PROGMEM = "  \n";
const char hl52[] PROGMEM = " - (unary included)\n";
const char hl53[] PROGMEM = " +\n";
const char hl54[] PROGMEM = " * (type: ' or a(b))\n";
const char hl55[] PROGMEM = " /\n";
const char hl56[] PROGMEM = " %\n";
const char hl57[] PROGMEM = " !\n";
const char hl58[] PROGMEM = " !! repeat prev line\n";
const char hl59[] PROGMEM = " ^ (type: \")\n";
const char hl60[] PROGMEM = " = (type: :)\n";
const char hl61[] PROGMEM = " E (aEb)\n";
const char hl62[] PROGMEM = "\n";
const char hl63[] PROGMEM = "functions: \n";
const char hl64[] PROGMEM = "\n";
const char hl65[] PROGMEM = " sin(a) asin(a) sinh(a)\n";
const char hl66[] PROGMEM = " csc(a) acsc(a) csch(a)\n";
const char hl67[] PROGMEM = " cos(a) acos(a) cosh(a)\n";
const char hl68[] PROGMEM = " sec(a) asec(a) sech(a)\n";
const char hl69[] PROGMEM = " tan(a) atan(a) tanh(a)\n";
const char hl70[] PROGMEM = " cot(a) acot(a) coth(a)\n";
const char hl71[] PROGMEM = " sqrt(a)\n";
const char hl72[] PROGMEM = " exp(a)     log(a)\n";
const char hl73[] PROGMEM = " pow(a,b)   log10(a\n";
const char hl74[] PROGMEM = " floor(a)   ceil(a)\n";
const char hl75[] PROGMEM = " min(a)     max(a)\n";
const char hl76[] PROGMEM = " round(a)\n";
const char hl77[] PROGMEM = " abs(a)\n";
const char hl78[] PROGMEM = " rand(a,b) from min a->b\n";
const char hl79[] PROGMEM = " dice(a,b) a: num b:sides\n";
const char hl80[] PROGMEM = " pick(a,n) a: choices\n";
const char hl81[] PROGMEM = "~C~n: no. choices\n";
const char hl82[] PROGMEM = "\n";
const char hl83[] PROGMEM = "variables: \n";
const char hl84[] PROGMEM = "\n";
const char hl85[] PROGMEM = "must start with alpha\n";
const char hl86[] PROGMEM = "excluding constants \n";
const char hl87[] PROGMEM = "~C~^^^ scroll up ^^^";

// Table of pointers (in PROGMEM) and count
const char* const HELP_LINES[] PROGMEM = {
  hl0, hl1, hl2, hl3, hl4, hl5, hl6, hl7, hl8, hl9,
  hl10, hl11, hl12, hl13, hl14, hl15, hl16, hl17, hl18, hl19,
  hl20, hl21, hl22, hl23, hl24, hl25, hl26, hl27, hl28, hl29,
  hl30, hl31, hl32, hl33, hl34, hl35, hl36, hl37, hl38, hl39,
  hl40, hl41, hl42, hl43, hl44, hl45, hl46, hl47, hl48, hl49,
  hl50, hl51, hl52, hl53, hl54, hl55, hl56, hl57, hl58, hl59,
  hl60, hl61, hl62, hl63, hl64, hl65, hl66, hl67, hl68, hl69,
  hl70, hl71, hl72, hl73, hl74, hl75, hl76, hl77, hl78, hl79,
  hl80, hl81, hl82, hl83, hl84, hl85, hl86, hl87
};

const size_t HELP_COUNT = sizeof(HELP_LINES) / sizeof(HELP_LINES[0]);

#pragma endregion // End of helpText
#pragma region unitTypes

const char ut0[]  PROGMEM = "~C~length\n";
const char ut1[]  PROGMEM = "~C~area\n";
const char ut2[]  PROGMEM = "~C~volume\n";
const char ut3[]  PROGMEM = "~C~weight&mass\n";
const char ut4[]  PROGMEM = "~C~temperature\n";
const char ut5[]  PROGMEM = "~C~energy\n";
const char ut6[]  PROGMEM = "~C~speed\n";
const char ut7[]  PROGMEM = "~C~pressure\n";
const char ut8[]  PROGMEM = "~C~data\n";
const char ut9[]  PROGMEM = "~C~angle";
const char ut10[]  PROGMEM = "~C~time";
const char ut11[]  PROGMEM = "~C~power";
const char ut12[]  PROGMEM = "~C~force";
const char ut13[]  PROGMEM = "~C~frequency";

const char* const UNIT_TYPES_LINES[] PROGMEM = {
  ut0, ut1, ut2, ut3, ut4, ut5, ut6, ut7, ut8, ut9, ut10, ut11, ut12, ut13
};

const size_t UNIT_TYPES_COUNT = sizeof(UNIT_TYPES_LINES) / sizeof(UNIT_TYPES_LINES[0]);

#pragma endregion // End of unitTypes
#pragma region convDir


const char cdir0[]  PROGMEM = "~C~->\n";
const char cdir1[]  PROGMEM = "~C~<-";

const char* const CONV_DIR_LINES[] PROGMEM = {
  cdir0, cdir1
};

const size_t CONV_DIR_COUNT = sizeof(CONV_DIR_LINES) / sizeof(CONV_DIR_LINES[0]);

#pragma endregion // End of convDir
#pragma region convLength

const char cl0[]  PROGMEM = "~C~m\n";
const char cl1[]  PROGMEM = "~C~in\n";
const char cl2[]  PROGMEM = "~C~yd\n";
const char cl3[]  PROGMEM = "~C~ft\n";
const char cl4[]  PROGMEM = "~C~mi\n";
const char cl5[]  PROGMEM = "~C~km\n";
const char cl6[]  PROGMEM = "~C~cm\n";
const char cl7[]  PROGMEM = "~C~mm\n";
const char cl8[]  PROGMEM = "~C~nm\n";
const char cl9[]  PROGMEM = "~C~um\n";
const char cl10[]  PROGMEM = "~C~M";

const char* const CONV_LENGTH_LINES[] PROGMEM = {
  cl0, cl1, cl2, cl3, cl4, cl5, cl6, cl7, cl8, cl9, cl10
};

const size_t CONV_LENGTH_COUNT = sizeof(CONV_LENGTH_LINES) / sizeof(CONV_LENGTH_LINES[0]);

#pragma endregion // End of convLength
#pragma region convArea

const char ca0[]  PROGMEM = "~C~ha\n";
const char ca1[]  PROGMEM = "~C~ac\n";
const char ca2[]  PROGMEM = "~C~m^2\n";
const char ca3[]  PROGMEM = "~C~in^2\n";
const char ca4[]  PROGMEM = "~C~yd^2\n";
const char ca5[]  PROGMEM = "~C~ft^2\n";
const char ca6[]  PROGMEM = "~C~mi^2\n";
const char ca7[]  PROGMEM = "~C~km^2\n";
const char ca8[]  PROGMEM = "~C~cm^2\n";
const char ca9[]  PROGMEM = "~C~mm^2\n";

const char* const CONV_AREA_LINES[] PROGMEM = {
  ca0, ca1, ca2, ca3, ca4, ca5, ca6, ca7, ca8, ca9
};

const size_t CONV_AREA_COUNT = sizeof(CONV_AREA_LINES) / sizeof(CONV_AREA_LINES[0]);

#pragma endregion // End of convLength
#pragma region convVolume

// PROGMEM strings for menu display
const char cv0[]  PROGMEM = "~C~m^3\n";
const char cv1[]  PROGMEM = "~C~cm^3\n";
const char cv2[]  PROGMEM = "~C~mm^3\n";
const char cv3[]  PROGMEM = "~C~km^3\n";
const char cv4[]  PROGMEM = "~C~L\n";
const char cv5[]  PROGMEM = "~C~mL\n";
const char cv6[]  PROGMEM = "~C~in^3\n";
const char cv7[]  PROGMEM = "~C~ft^3\n";
const char cv8[]  PROGMEM = "~C~yd^3\n";
const char cv9[]  PROGMEM = "~C~tsp\n";
const char cv10[]  PROGMEM = "~C~tbsp\n";
const char cv11[]  PROGMEM = "~C~gal\n";
const char cv12[] PROGMEM = "~C~qt\n";
const char cv13[] PROGMEM = "~C~pt\n";
const char cv14[] PROGMEM = "~C~cup\n";
const char cv15[] PROGMEM = "~C~fl oz\n";

const char* const CONV_VOLUME_LINES[] PROGMEM = {
  cv0, cv1, cv2, cv3, cv4, cv5, cv6,
  cv7, cv8, cv9, cv10, cv11, cv12, cv13,
  cv14, cv15
};

const size_t CONV_VOLUME_COUNT = sizeof(CONV_VOLUME_LINES) / sizeof(CONV_VOLUME_LINES[0]);

#pragma endregion // End of convVolume
#pragma region convMass

const char cm0[]  PROGMEM = "~C~kg\n";
const char cm1[]  PROGMEM = "~C~g\n";
const char cm2[]  PROGMEM = "~C~mg\n";
const char cm3[]  PROGMEM = "~C~µg\n";
const char cm4[]  PROGMEM = "~C~t\n";
const char cm5[]  PROGMEM = "~C~lb\n";
const char cm6[]  PROGMEM = "~C~oz\n";
const char cm7[]  PROGMEM = "~C~st\n";
const char cm8[]  PROGMEM = "~C~sh t\n";
const char cm9[]  PROGMEM = "~C~l t\n";

const char* const CONV_MASS_LINES[] PROGMEM = {
  cm0, cm1, cm2, cm3, cm4, cm5, cm6, cm7, cm8, cm9
};

const size_t CONV_MASS_COUNT = sizeof(CONV_MASS_LINES) / sizeof(CONV_MASS_LINES[0]);

#pragma endregion
#pragma region convTemperature

const char ct0[] PROGMEM = "~C~K\n";
const char ct1[] PROGMEM = "~C~C\n";
const char ct2[] PROGMEM = "~C~F\n";
const char ct3[] PROGMEM = "~C~R\n";

const char* const CONV_TEMPERATURE_LINES[] PROGMEM = {
  ct0, ct1, ct2, ct3
};

const size_t CONV_TEMPERATURE_COUNT =
    sizeof(CONV_TEMPERATURE_LINES) / sizeof(CONV_TEMPERATURE_LINES[0]);

#pragma endregion
#pragma region convEnergy

const char ce0[] PROGMEM = "~C~J\n";
const char ce1[] PROGMEM = "~C~kJ\n";
const char ce2[] PROGMEM = "~C~cal\n";
const char ce3[] PROGMEM = "~C~kcal\n";
const char ce4[] PROGMEM = "~C~Wh\n";
const char ce5[] PROGMEM = "~C~kWh\n";
const char ce6[] PROGMEM = "~C~eV\n";
const char ce7[] PROGMEM = "~C~BTU\n";
const char ce8[] PROGMEM = "~C~therm\n";

const char* const CONV_ENERGY_LINES[] PROGMEM = {
  ce0, ce1, ce2, ce3, ce4, ce5, ce6, ce7, ce8
};

const size_t CONV_ENERGY_COUNT =
    sizeof(CONV_ENERGY_LINES) / sizeof(CONV_ENERGY_LINES[0]);

#pragma endregion
#pragma region convSpeed
const char cs0[] PROGMEM = "~C~m/s\n";
const char cs1[] PROGMEM = "~C~km/h\n";
const char cs2[] PROGMEM = "~C~mph\n";
const char cs3[] PROGMEM = "~C~kn\n";
const char cs4[] PROGMEM = "~C~ft/s\n";
const char cs5[] PROGMEM = "~C~cm/s\n";

const char* const CONV_SPEED_LINES[] PROGMEM = { cs0, cs1, cs2, cs3, cs4, cs5 };
const size_t CONV_SPEED_COUNT = sizeof(CONV_SPEED_LINES)/sizeof(CONV_SPEED_LINES[0]);
#pragma endregion
#pragma region convPressure
const char cp0[] PROGMEM = "~C~atm\n";
const char cp1[] PROGMEM = "~C~bar\n";
const char cp2[] PROGMEM = "~C~mbar\n";
const char cp3[] PROGMEM = "~C~Torr\n";
const char cp4[] PROGMEM = "~C~mTorr\n";
const char cp5[] PROGMEM = "~C~Pa\n";
const char cp6[] PROGMEM = "~C~kPa\n";
const char cp7[] PROGMEM = "~C~MPa\n";
const char cp8[] PROGMEM = "~C~psi\n";

const char* const CONV_PRESSURE_LINES[] PROGMEM = {
  cp0, cp1, cp2, cp3, cp4, cp5, cp6, cp7, cp8
};

const size_t CONV_PRESSURE_COUNT = sizeof(CONV_PRESSURE_LINES) / sizeof(CONV_PRESSURE_LINES[0]);
#pragma endregion
#pragma region convData
const char cd0[]  PROGMEM = "~C~bit\n";
const char cd1[]  PROGMEM = "~C~B\n";
const char cd2[]  PROGMEM = "~C~kB\n";
const char cd3[]  PROGMEM = "~C~MB\n";
const char cd4[]  PROGMEM = "~C~GB\n";
const char cd5[]  PROGMEM = "~C~TB\n";
const char cd6[]  PROGMEM = "~C~KiB\n";
const char cd7[]  PROGMEM = "~C~MiB\n";
const char cd8[]  PROGMEM = "~C~GiB\n";
const char cd9[]  PROGMEM = "~C~TiB\n";

const char* const CONV_DATA_LINES[] PROGMEM = {
  cd0, cd1, cd2, cd3, cd4, cd5, cd6, cd7, cd8, cd9
};
const size_t CONV_DATA_COUNT = sizeof(CONV_DATA_LINES)/sizeof(CONV_DATA_LINES[0]);
#pragma endregion
#pragma region convAngle
const char caa0[] PROGMEM = "~C~rad\n";
const char caa1[] PROGMEM = "~C~°\n";
const char caa2[] PROGMEM = "~C~gon\n";
const char caa3[] PROGMEM = "~C~arcmin\n";
const char caa4[] PROGMEM = "~C~arcsec\n";
const char caa5[] PROGMEM = "~C~rev\n";
const char caa6[] PROGMEM = "~C~mrad\n";

const char* const CONV_ANGLE_LINES[] PROGMEM = { caa0, caa1, caa2, caa3, caa4, caa5, caa6 };
const size_t CONV_ANGLE_COUNT = sizeof(CONV_ANGLE_LINES)/sizeof(CONV_ANGLE_LINES[0]);
#pragma endregion
#pragma region convTime

const char cti0[] PROGMEM = "~C~s\n";
const char cti1[] PROGMEM = "~C~ms\n";
const char cti2[] PROGMEM = "~C~µs\n";
const char cti3[] PROGMEM = "~C~ns\n";
const char cti4[] PROGMEM = "~C~min\n";
const char cti5[] PROGMEM = "~C~h\n";
const char cti6[] PROGMEM = "~C~d\n";
const char cti7[] PROGMEM = "~C~wk\n";
const char cti8[] PROGMEM = "~C~yr\n";

const char* const CONV_TIME_LINES[] PROGMEM = {
  cti0, cti1, cti2, cti3, cti4, cti5, cti6, cti7, cti8
};

const size_t CONV_TIME_COUNT = sizeof(CONV_TIME_LINES) / sizeof(CONV_TIME_LINES[0]);

#pragma endregion
#pragma region convPower

const char cpo0[] PROGMEM = "~C~W\n";
const char cpo1[] PROGMEM = "~C~kW\n";
const char cpo2[] PROGMEM = "~C~MW\n";
const char cpo3[] PROGMEM = "~C~hp\n";
const char cpo4[] PROGMEM = "~C~erg/s\n";

const char* const CONV_POWER_LINES[] PROGMEM = {
  cpo0, cpo1, cpo2, cpo3, cpo4
};

const size_t CONV_POWER_COUNT = sizeof(CONV_POWER_LINES) / sizeof(CONV_POWER_LINES[0]);

#pragma endregion
#pragma region convForce

const char cf0[] PROGMEM = "~C~N\n";
const char cf1[] PROGMEM = "~C~dyn\n";
const char cf2[] PROGMEM = "~C~kgf\n";
const char cf3[] PROGMEM = "~C~lbf\n";
const char cf4[] PROGMEM = "~C~ozf\n";

const char* const CONV_FORCE_LINES[] PROGMEM = {
  cf0, cf1, cf2, cf3, cf4
};

const size_t CONV_FORCE_COUNT = sizeof(CONV_FORCE_LINES) / sizeof(CONV_FORCE_LINES[0]);

#pragma endregion
#pragma region convFrequency

const char cfq0[] PROGMEM = "~C~Hz\n";
const char cfq1[] PROGMEM = "~C~kHz\n";
const char cfq2[] PROGMEM = "~C~MHz\n";
const char cfq3[] PROGMEM = "~C~GHz\n";
const char cfq4[] PROGMEM = "~C~rpm\n";

const char* const CONV_FREQUENCY_LINES[] PROGMEM = {
  cfq0, cfq1, cfq2, cfq3, cfq4
};

const size_t CONV_FREQUENCY_COUNT = sizeof(CONV_FREQUENCY_LINES) / sizeof(CONV_FREQUENCY_LINES[0]);

#pragma endregion
#pragma endregion

#endif