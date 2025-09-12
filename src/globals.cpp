#include "globals.h"
#include "sdmmc_cmd.h"

// ===================== DISPLAY =====================
// Main e-ink display object
GxEPD2_BW<GxEPD2_310_GDEQ031T10, GxEPD2_310_GDEQ031T10::HEIGHT> display(GxEPD2_310_GDEQ031T10(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
// Fast full update flag for e-ink
volatile bool GxEPD2_310_GDEQ031T10::useFastFullUpdate = true;
// 256x32 SPI OLED display object
U8G2_SSD1326_ER_256X32_F_4W_HW_SPI u8g2(U8G2_R2, OLED_CS, OLED_DC, OLED_RST);

// ===================== INPUT DEVICES =====================

// Matrix keypad controller
Adafruit_TCA8418 keypad;
// Key layouts
/* migrated to pocketmage_kb.h
char keysArray[4][10] = {
    { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' },
    { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',   8 },  // 8:BKSP
    {   9, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.',  13 },  // 9:TAB, 13:CR
    {   0,  17,  18, ' ', ' ', ' ',  19,  20,  21,   0 }   // 17:SHFT, 18:FN, 19:<-, 20:SEL, 21:->
};
char keysArraySHFT[4][10] = {
    { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' },
    { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',   8 },
    {   9, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',',  13 },
    {   0,  17,  18, ' ', ' ', ' ',  28,  29,  30,   0 }
};
char keysArrayFN[4][10] = {
    { '1', '2', '3', '4', '5', '6', '7',  '8',  '9', '0' },
    { '#', '!', '$', ':', ';', '(', ')', '\'', '\"',   8 },
    {  14, '%', '_', '&', '+', '-', '/',  '?',  ',',  13 },
    {   0,  17,  18, ' ', ' ', ' ',  12,    7,    6,   0 }
};
*/
// Capacitive touch slider
Adafruit_MPR121 cap = Adafruit_MPR121();
volatile long int dynamicScroll = 0;         // Dynamic scroll offset
volatile long int prev_dynamicScroll = 0;    // Previous scroll offset
int lastTouch = -1;                          // Last touch event
unsigned long lastTouchTime = 0;             // Last touch time

// ===================== AUDIO =====================
// Buzzer for sound feedback
Buzzer buzzer(17);

// ===================== RTC =====================
// Real-time clock chip
RTC_PCF8563 rtc;
// Day names
const char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// ===================== USB & STORAGE =====================
// USB mass storage controller
USBMSC msc;
bool mscEnabled = false;          // Is USB MSC active?
sdmmc_card_t* card = nullptr;     // SD card pointer

// ===================== SYSTEM SETTINGS =====================
// Persistent preferences (NVS)
Preferences prefs;
int TIMEOUT;              // Auto sleep timeout (seconds)
bool DEBUG_VERBOSE;       // Extra debug output
bool SYSTEM_CLOCK;        // Show clock on screen
bool SHOW_YEAR;           // Show year in clock
bool SAVE_POWER;          // Enable power saving mode
bool ALLOW_NO_MICROSD;    // Allow running without SD card
bool HOME_ON_BOOT;        // Start home app on boot
int OLED_BRIGHTNESS;      // OLED brightness (0-255)
int OLED_MAX_FPS;         // OLED max FPS

String OTA1_APP;
String OTA2_APP;
String OTA3_APP;
String OTA4_APP;

// ===================== SYSTEM STATE =====================
// E-Ink refresh control
// volatile int einkRefresh = FULL_REFRESH_AFTER; // Partial/full refresh counter
int OLEDFPSMillis = 0;            // Last OLED FPS update time
int KBBounceMillis = 0;           // Last keyboard debounce time
volatile int timeoutMillis = 0;   // Timeout tracking
volatile int prevTimeMillis = 0;  // Previous time for timeout
volatile bool TCA8418_event = false;  // Keypad interrupt event
volatile bool PWR_BTN_event = false;  // Power button event
volatile bool SHFT = false;           // Shift key state
volatile bool FN = false;             // Function key state
volatile bool newState = false;       // App state changed
bool noTimeout = false;               // Disable timeout
volatile bool OLEDPowerSave = false;  // OLED power save mode
volatile bool disableTimeout = false; // Disable timeout globally
volatile int battState = 0;           // Battery state
// volatile int prevBattState = 0;       // Previous battery state
unsigned int flashMillis = 0;         // Flash timing
int prevTime = 0;                     // Previous time (minutes)
uint8_t prevSec = 0;                  // Previous seconds
TaskHandle_t einkHandlerTaskHandle = NULL; // E-Ink handler task
uint8_t partialCounter = 0;           // Counter for partial refreshes
volatile bool forceSlowFullUpdate = false; // Force slow full update

// ===================== KEYBOARD STATE =====================
// char currentKB[4][10];            // Current keyboard layout
KBState CurrentKBState = NORMAL;  // Current keyboard state

// ===================== FILES & TEXT =====================
volatile bool SDCARD_INSERT = false;  // SD card inserted event
bool noSD = false;                    // No SD card present
volatile bool SDActive = false;       // SD card active
String editingFile;                   // Currently edited file
// const GFXfont *currentFont = (GFXfont *)&FreeSerif9pt7b; // Current font
// uint8_t maxCharsPerLine = 0;          // Max chars per line (display)
// uint8_t maxLines = 0;                 // Max lines per screen
// uint8_t fontHeight = 0;               // Font height in pixels
// uint8_t lineSpacing = 6;              // Line spacing in pixels
String workingFile = "";              // Working file name
String filesList[MAX_FILES];          // List of files

// ===================== APP STATES =====================
const String appStateNames[] = { "txt", "filewiz", "usb", "bt", "settings", "tasks", "calendar", "journal", "lexicon", "script" , "loader" }; // App state names
const unsigned char *appIcons[11] = { _homeIcons2, _homeIcons3, _homeIcons4, _homeIcons5, _homeIcons6, taskIconTasks0, _homeIcons7, _homeIcons8, _homeIcons9, _homeIcons11, _homeIcons10}; // App icons
AppState CurrentAppState;             // Current app state

// ===================== TXT APP =====================
volatile bool newLineAdded = true;           // New line added in TXT
std::vector<String> allLines;                // All lines in TXT

// ===================== TASKS APP =====================
std::vector<std::vector<String>> tasks;      // Task list

// ===================== HOME APP =====================
HOMEState CurrentHOMEState = HOME_HOME;      // Current home state



// ===================== CALC APP =====================
#pragma region calcDataStructures
const char* operatorsCalc[] = { "+", "-", "'", "/", "E", "%", ":", "!", "\"" };
const size_t operatorsCalcCount = sizeof(operatorsCalc)/sizeof(operatorsCalc[0]);

const char* functionsCalc[] = {
    // trig
    "sin", "cos", "tan", "asin", "acos", "atan",
    "sinh", "cosh", "tanh", "sec", "csc", "cot", 
    "sech", "csch", "coth", "asec", "acsc", "acot",
    // scientific
    "ln", "log", "sqrt", "cbrt", "abs", "exp",
    "round", "min", "max", "pow", "rand",
    // fun
    "dice", "pick"
};
const size_t functionsCalcCount = sizeof(functionsCalc)/sizeof(functionsCalc[0]);

const char* constantsCalc[] = {
  "inf", "-inf", "pi", "e", "ans"
};
const size_t constantsCalcCount = sizeof(constantsCalc)/sizeof(constantsCalc[0]);

const OpEntry OPS[] = {
  {":",     0, false}, 
  {"+",     1, false}, 
  {"-",     1, false},
  {"*",     2, false}, 
  {"/",     2, false}, 
  {"%",     2, false},
  {"E",     3,  true}, // exponent
  {"\"",    3, false}, // ^
  {"'",     2, false}, // *
  {"!",     4,  true}, 
  {"~neg~", 4,  true}  // unary negation
};
const size_t OPS_N = sizeof(OPS)/sizeof(OPS[0]);

std::map<String, double> variables = {
  {"c",          299792458}, // speed of light [m/s]
  {"g",            9.80665}, // gravitational acceleration [m/s^2]
  {"G",        6.67430e-11}, // gravitational constant [m^3/kg*s^2]
  {"h",      6.2607015e-34}, // Planck's constant [J*s]
  {"rh",   1.054571817e-34}, // reduced Planck's constant [J*s]
  {"q",    1.602176634e-19}, // elementary charge [C]
  {"n",      6.02214076e23}, // Avogadro's number [1/mol]
  {"k",       1.380649e-23}, // Boltzmann constant [J/K]
  {"r",        8.314462618}, // gas constant [mol*K]
  {"eo",  8.8541878128e-12}, // vacuum permittivity [F/m]
  {"uo",  12.5663706144e-7}, // vacuum permeability [H/m]
  {"Z",            376.730}, // impedance of free space [Ohm]
  {"alpha",  0.00729927007}, // fine-structure constant
  {"sigma", 5.670374419e-8}, // Stefan-Boltzmann constant [W/m^2*K^4]
  {"b",     2.897771955e-3}, // Wien's displacement constant [m*K]
  {"me",    9.10938356e-31}, // electron mass [kg]
  {"mp", 1.67262192369e-27}, // proton mass [kg]
  {"nm", 1.67492749804e-27}, // neutron mass [kg]
  {"u",  1.66053906660e-27}, // unified atomic mass unit [kg]
  {"AU",    1.495978707e11}, // astronomical unit [kg]
  {"ly",         9.4607e15}, // light year [m]
  {"pc",         3.0857e16}, // parsec [m]
  {"M",         1.98847e30}  // solar mass [kg]
};

#pragma region units
static const Unit lengthUnits[] = {
    { "meter",         "m",    1.0,        0.0 }, //base
    { "kilometer",     "km",   1000.0,     0.0 },
    { "centimeter",    "cm",   0.01,       0.0 },
    { "millimeter",    "mm",   0.001,      0.0 },
    { "micrometer",    "um",   1e-6,       0.0 },
    { "nanometer",     "nm",   1e-9,       0.0 },
    { "inch",          "in",   0.0254,     0.0 },
    { "foot",          "ft",   0.3048,     0.0 },
    { "yard",          "yd",   0.9144,     0.0 },
    { "mile",          "mi",   1609.34,    0.0 },
    { "nautical mile", "M",    1852.0,     0.0 }
};

static const Unit areaUnits[] = {
    { "Hectare",          "ha",   10000.0,          0.0 },
    { "Acre",             "ac",   4046.8564224,     0.0 },
    { "Square meter",     "m^2",  1.0,              0.0 }, //base
    { "Square inch",      "in^2", 0.00064516,       0.0 },
    { "Square yard",      "yd^2", 0.83612736,       0.0 },
    { "Square foot",      "ft^2", 0.09290304,       0.0 },
    { "Square mile",      "mi^2", 2589988.110336,   0.0 },
    { "Square kilometer", "km^2", 1000000.0,        0.0 },
    { "Square centimeter","cm^2", 0.0001,           0.0 },
    { "Square millimeter","mm^2", 0.000001,         0.0 }
};

static const Unit volumeUnits[] = {
    { "Cubic meter",      "m^3",   1.0,               0.0 }, // base
    { "Cubic centimeter", "cm^3",  1e-6,              0.0 },
    { "Cubic millimeter", "mm^3",  1e-9,              0.0 },
    { "Cubic kilometer",  "km^3",  1e9,               0.0 },
    { "Liter",            "L",     0.001,             0.0 },
    { "Milliliter",       "mL",    1e-6,              0.0 },
    { "Cubic inch",       "in^3",  1.6387e-5,         0.0 },
    { "Cubic foot",       "ft^3",  0.0283168,         0.0 },
    { "Cubic yard",       "yd^3",  0.764555,          0.0 },
    { "Teaspoon",         "tsp",   4.92892159375e-6,  0.0 },
    { "Tablespoon",       "tbsp",  1.478676478125e-5, 0.0 },
    { "US gallon",        "gal",   0.00378541,        0.0 },
    { "US quart",         "qt",    0.000946353,       0.0 },
    { "US pint",          "pt",    0.000473176,       0.0 },
    { "US cup",           "cup",   0.000236588,       0.0 },
    { "US fluid ounce",   "fl oz", 2.9574e-5,         0.0 }
};

static const Unit massUnits[] = {
    { "Kilogram",     "kg",   1.0,           0.0 }, // base
    { "Gram",         "g",    0.001,         0.0 },
    { "Milligram",    "mg",   1e-6,          0.0 },
    { "Microgram",    "µg",   1e-9,          0.0 },
    { "Metric ton",   "t",    1000.0,        0.0 },
    { "Pound",        "lb",   0.45359237,    0.0 },
    { "Ounce",        "oz",   0.0283495,     0.0 },
    { "Stone",        "st",   6.35029,       0.0 },
    { "US ton",       "sh t", 907.18474,     0.0 },
    { "Imperial ton", "l t",  1016.0469088,  0.0 }
};

static const Unit temperatureUnits[] = {
    { "kelvin",     "K",   1.0,         0.0 },     // base
    { "celsius",    "C",  1.0,       273.15 },
    { "fahrenheit", "F",  5.0/9.0,   459.67 },
    { "rankine",    "R",  5.0/9.0,        0 }
};

static const Unit energyUnits[] = {
    { "Joule",         "J",      1.0,              0.0 }, // base
    { "Kilojoule",     "kJ",     1000.0,           0.0 },
    { "Calorie",       "cal",    4.184,            0.0 },
    { "Kilocalorie",   "kcal",   4184.0,           0.0 },
    { "Watt-hour",     "Wh",     3600.0,           0.0 },
    { "Kilowatt-hour", "kWh",    3600000.0,        0.0 },
    { "Electronvolt",  "eV",     1.602176634e-19,  0.0 },
    { "BTU",           "BTU",    1055.06,          0.0 },
    { "Therm",         "therm",  1.055e8,          0.0 }
};

static const Unit speedUnits[] = {
    { "Meter per second",     "m/s",   1.0,              0.0 }, // base
    { "Kilometer per hour",   "km/h",  1000.0/3600.0,    0.0 },
    { "Mile per hour",        "mph",   1609.344/3600.0,  0.0 },
    { "Knot",                 "kn",    1852.0/3600.0,    0.0 },
    { "Foot per second",      "ft/s",  0.3048,           0.0 },
    { "Centimeter per second","cm/s",  0.01,             0.0 }
};

static const Unit pressureUnits[] = {
    { "Atmosphere",  "atm",   101325.0,                  0.0 }, 
    { "Bar",         "bar",   100000.0,                  0.0 }, 
    { "Millibar",    "mbar",  100.0,                     0.0 }, 
    { "Torr",        "Torr",  101325.0/760.0,            0.0 }, 
    { "Millitorr",   "mTorr", (101325.0/760.0)/1000.0,   0.0 }, 
    { "Pascal",      "Pa",    1.0,                       0.0 }, // base
    { "Kilopascal",  "kPa",   1000.0,                    0.0 },       
    { "Megapascal",  "MPa",   1e6,                       0.0 },       
    { "Pound/in^2",  "psi",  6894.757293168,             0.0 }        
};

static const Unit dataUnits[] = {
    { "Bit",        "bit",  1.0/8.0,                  0.0 },
    { "Byte",       "B",    1.0,                      0.0 }, // base
    { "Kilobyte",   "kB",   1000.0,                   0.0 },
    { "Megabyte",   "MB",   1000.0*1000.0,            0.0 },
    { "Gigabyte",   "GB",   1e9,                      0.0 },
    { "Terabyte",   "TB",   1e12,                     0.0 },
    { "Kibibyte",   "KiB",  1024.0,                   0.0 },
    { "Mebibyte",   "MiB",  1024.0*1024.0,            0.0 },
    { "Gibibyte",   "GiB",  1024.0*1024.0*1024.0,     0.0 },
    { "Tebibyte",   "TiB",  1099511627776.0,          0.0 }
};

static const Unit angleUnits[] = {
    { "Radian",     "rad",   1.0,                              0.0 }, // base
    { "Degree",     "deg",   3.14159265358979323846/180.0,     0.0 },
    { "Gradian",    "gon",   3.14159265358979323846/200.0,     0.0 },
    { "Arcminute",  "arcmin",3.14159265358979323846/10800.0,   0.0 },
    { "Arcsecond",  "arcsec",3.14159265358979323846/648000.0,  0.0 },
    { "Revolution", "rev",   2.0*3.14159265358979323846,       0.0 },
    { "Milliradian","mrad",  0.001,                            0.0 }
};

static const Unit timeUnits[] = {
    { "Second",      "s",   1.0,         0.0 }, //base
    { "Millisecond", "ms",  0.001,       0.0 },
    { "Microsecond", "µs",  1e-6,        0.0 },
    { "Nanosecond",  "ns",  1e-9,        0.0 },
    { "Minute",      "min", 60.0,        0.0 },
    { "Hour",        "h",   3600.0,      0.0 },
    { "Day",         "d",   86400.0,     0.0 },
    { "Week",        "wk",  604800.0,    0.0 },
    { "Year",        "yr",  31557600.0,  0.0 }  // 365.25 days
};

static const Unit powerUnits[] = {
    { "Watt",            "W",     1.0,    0.0 }, //base
    { "Kilowatt",        "kW",    1000.0, 0.0 },
    { "Megawatt",        "MW",    1e6,    0.0 },
    { "Horsepower",      "hp",    745.7,  0.0 },
    { "Erg per second",  "erg/s", 1e-7,   0.0 }
};

static const Unit forceUnits[] = {
    { "Newton",        "N",    1.0,       0.0 }, //base
    { "Dyne",          "dyn",  1e-5,      0.0 },
    { "Kilogram-force","kgf",  9.80665,   0.0 },
    { "Pound-force",   "lbf",  4.44822,   0.0 },
    { "Ounce-force",   "ozf",  0.278014,  0.0 }
};

static const Unit frequencyUnits[] = {
    { "Hertz",                 "Hz",   1.0,       0.0 }, //base
    { "Kilohertz",             "kHz",  1000.0,    0.0 },
    { "Megahertz",             "MHz",  1e6,       0.0 },
    { "Gigahertz",             "GHz",  1e9,       0.0 },
    { "Revolutions per minute","rpm",  1.0/60.0,  0.0 }
};
// Helper for array length
template <typename T, size_t N>
constexpr size_t countof(const T (&)[N]) { return N; }
// --- Catalog of views (all constexpr-friendly in C++11) ---
const UnitSet UnitCatalog[] = {
    { "Length",      lengthUnits,     countof(lengthUnits)     },
    { "Area",        areaUnits,       countof(areaUnits)       },
    { "Volume",      volumeUnits,     countof(volumeUnits)     },
    { "Mass",        massUnits,       countof(massUnits)       },
    { "Temperature", temperatureUnits,countof(temperatureUnits)},
    { "Energy",      energyUnits,     countof(energyUnits)     },
    { "Speed",       speedUnits,      countof(speedUnits)      },
    { "Pressure",    pressureUnits,   countof(pressureUnits)   },
    { "Data",        dataUnits,       countof(dataUnits)       },
    { "Angle",       angleUnits,      countof(angleUnits)      },
    { "Time",        timeUnits,       countof(timeUnits)       },
    { "Power",       powerUnits,      countof(powerUnits)      },
    { "Force",       forceUnits,      countof(forceUnits)      },
    { "Frequency",   frequencyUnits,  countof(frequencyUnits)  }
};
const size_t UnitCatalogCount = sizeof(UnitCatalog)/sizeof(UnitCatalog[0]);

// Map unit type choice index -> list source shown in A/B frames
const ProgmemTableSource* const allUnitLists[] = {
  &convLengthSrc,     
  &convAreaSrc,      
  &convVolumeSrc,
  &convMassSrc,
  &convTemperatureSrc,
  &convEnergySrc,
  &convSpeedSrc,
  &convPressureSrc,
  &convDataSrc,
  &convAngleSrc,
  &convTimeSrc,
  &convPowerSrc,
  &convForceSrc,
  &convFrequencySrc   
};

const size_t AllUnitListsCount = sizeof(allUnitLists)/sizeof(allUnitLists[0]);
const UnitSet* CurrentUnitSet = &UnitCatalog[0];          // default: Length
const ProgmemTableSource* CurrentUnitListSrc = &convLengthSrc;  // pairs with Length


FixedArenaSource<512, 16384> calcLines; 
ProgmemTableSource helpSrc(HELP_LINES, HELP_COUNT);
ProgmemTableSource unitTypesSrc(UNIT_TYPES_LINES, UNIT_TYPES_COUNT);
ProgmemTableSource convDirSrc(CONV_DIR_LINES, CONV_DIR_COUNT);
ProgmemTableSource convLengthSrc(CONV_LENGTH_LINES, CONV_LENGTH_COUNT);
ProgmemTableSource convAreaSrc(CONV_AREA_LINES, CONV_AREA_COUNT);
ProgmemTableSource convVolumeSrc(CONV_VOLUME_LINES, CONV_VOLUME_COUNT);
ProgmemTableSource convMassSrc(CONV_MASS_LINES, CONV_MASS_COUNT);
ProgmemTableSource convTemperatureSrc(CONV_TEMPERATURE_LINES, CONV_TEMPERATURE_COUNT);
ProgmemTableSource convEnergySrc(CONV_ENERGY_LINES, CONV_ENERGY_COUNT);
ProgmemTableSource convSpeedSrc(CONV_SPEED_LINES, CONV_SPEED_COUNT);
ProgmemTableSource convPressureSrc(CONV_PRESSURE_LINES, CONV_PRESSURE_COUNT);
ProgmemTableSource convDataSrc(CONV_DATA_LINES, CONV_DATA_COUNT);
ProgmemTableSource convAngleSrc(CONV_ANGLE_LINES, CONV_ANGLE_COUNT);
ProgmemTableSource convTimeSrc(CONV_TIME_LINES, CONV_TIME_COUNT);
ProgmemTableSource convPowerSrc(CONV_POWER_LINES, CONV_POWER_COUNT);
ProgmemTableSource convForceSrc(CONV_FORCE_LINES, CONV_FORCE_COUNT);
ProgmemTableSource convFrequencySrc(CONV_FREQUENCY_LINES, CONV_FREQUENCY_COUNT);
#pragma endregion

#pragma endregion

CALCState CurrentCALCState = CALC0;
Frame *CurrentFrameState = &calcScreen;

std::vector<String> prevTokens;
String cleanExpression = "";
String calculatedResult = "";
String prevLine = "";


int calcSwitchedStates = 0;
int trigType = 1;
int refresh_count = 0;

Unit emptyUnit = {"","",NAN,NAN};

// ===================== FRAME CLASS =====================
std::vector<Frame*> frames = {};
int currentFrameChoice = -1;
// NOTE: if used, reset frameSelection after every updateScrollFromTouch_Frame to add choice functionality
int frameSelection = 0;

#pragma region frameSetup
Frame calcScreen(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP,FRAME_BOTTOM,&calcLines,false,false);
Frame helpScreen(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP,FRAME_BOTTOM,&helpSrc,false,true);
Frame conversionScreen(FRAME_LEFT ,FRAME_RIGHT,FRAME_TOP + 80, FRAME_BOTTOM,&calcLines,false,true);
Frame conversionUnit(FRAME_LEFT ,FRAME_RIGHT,FRAME_TOP, FRAME_BOTTOM + 136,&unitTypesSrc,false,true);
Frame conversionDirection(FRAME_LEFT + 120 ,FRAME_RIGHT + 120,FRAME_TOP + 40, FRAME_BOTTOM + 96,&convDirSrc,false,false);
Frame conversionFrameA(FRAME_LEFT + 24,FRAME_RIGHT + 184,FRAME_TOP + 40, FRAME_BOTTOM + 96,&convLengthSrc,true,true);
Frame conversionFrameB(FRAME_LEFT + 184,FRAME_RIGHT + 24,FRAME_TOP + 40, FRAME_BOTTOM + 96,&convLengthSrc,true,true);
Frame programmingScreen(FRAME_LEFT,FRAME_RIGHT + 80,FRAME_TOP + 16,FRAME_BOTTOM,&calcLines,false,true);
Frame numberSizeFrame(FRAME_LEFT + 220,FRAME_RIGHT,FRAME_TOP+ 16,FRAME_BOTTOM,&calcLines,false,true);
Frame binaryFrame(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP,FRAME_BOTTOM - 16,&calcLines,false,true);
Frame decimalFrame(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP ,FRAME_BOTTOM + 48,&calcLines,false,true);
Frame octalFrame(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP + 40,FRAME_BOTTOM + 16,&calcLines,false,true);
Frame hexadecimalFrame(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP + 72,FRAME_BOTTOM - 16,&calcLines,false,true);
Frame testBitmapScreen(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP,FRAME_BOTTOM + 88,homeIconsAllArray[0],40,40,false,true);
Frame testTextScreen(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP + 88,FRAME_BOTTOM,&helpSrc,false,true);


#pragma endregion
