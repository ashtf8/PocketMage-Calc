#ifndef GLOBALS_H
#define GLOBALS_H


// LIBRARIES
#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <U8g2lib.h>
#include <Adafruit_TCA8418.h>
#include <vector>
#include <Buzzer.h>
#include <USB.h>
#include <USBMSC.h>
#include <SD_MMC.h>
#include <Preferences.h>
#include <Adafruit_MPR121.h>
#include <esp_cpu.h>
#include <RTClib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <assets.h>
#include <config.h>

// calc and frames
#include <pgmspace.h>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>

/* // migrated to pocketmage_eink.h
// FONTS
// 3x7
#include "Fonts/Font3x7FixedNum.h"
#include "Fonts/Font4x5Fixed.h"
#include "Fonts/Font5x7Fixed.h"

// 9x7
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
// 12x7
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
*/

/* // migrated to pocketmage_oled.h
// U8G2 FONTS
//U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
//u8g2_font_4x6_mf
//u8g2_font_5x7_mf
//u8g2_font_spleen5x8_mf
//u8g2_font_boutique_bitmap_9x9_tf
//u8g2_font_courR08_tf.h
*/

#include <Fonts/FreeSerif9pt7b.h>

// ===================== DISPLAY =====================
// E-ink display
extern GxEPD2_BW<GxEPD2_310_GDEQ031T10, GxEPD2_310_GDEQ031T10::HEIGHT> display;
// OLED 
extern U8G2_SSD1326_ER_256X32_F_4W_HW_SPI u8g2;

// ===================== INPUT DEVICES =====================
// Keypad controller
extern Adafruit_TCA8418 keypad; // Matrix keypad
extern char keysArray[4][10];       // Normal key layout
extern char keysArraySHFT[4][10];   // Shift key layout
extern char keysArrayFN[4][10];     // Function key layout

// Touch slider (capacitive)
extern Adafruit_MPR121 cap; // Touch slider
extern volatile long int dynamicScroll;      // Dynamic scroll offset
extern volatile long int prev_dynamicScroll; // Previous scroll offset
extern int lastTouch;                        // Last touch event
extern unsigned long lastTouchTime;          // Last touch time

// ===================== AUDIO =====================
// Buzzer for sound feedback
extern Buzzer buzzer;

// ===================== RTC =====================
// Real-time clock
extern RTC_PCF8563 rtc; // RTC chip
extern const char daysOfTheWeek[7][12]; // Day names

// ===================== USB & STORAGE =====================
// USB mass storage controller
extern USBMSC msc;           // USB MSC object
extern bool mscEnabled;      // Is USB MSC active?
extern sdmmc_card_t* card;   // SD card pointer

// ===================== SYSTEM SETTINGS =====================
// Persistent preferences
extern Preferences prefs;        // NVS preferences
extern int TIMEOUT;              // Auto sleep timeout (seconds)
extern bool DEBUG_VERBOSE;       // Extra debug output
extern bool SYSTEM_CLOCK;        // Show clock on screen
extern bool SHOW_YEAR;           // Show year in clock
extern bool SAVE_POWER;          // Enable power saving mode
extern bool ALLOW_NO_MICROSD;    // Allow running without SD card
extern bool HOME_ON_BOOT;        // Start home app on boot
extern int OLED_BRIGHTNESS;      // OLED brightness (0-255)
extern int OLED_MAX_FPS;         // OLED max FPS

extern String OTA1_APP;
extern String OTA2_APP;
extern String OTA3_APP;
extern String OTA4_APP;

// ===================== SYSTEM STATE =====================
// E-Ink refresh control
// extern volatile int einkRefresh;     // Partial/full refresh counter
extern int OLEDFPSMillis;            // Last OLED FPS update time
extern int KBBounceMillis;           // Last keyboard debounce time
extern volatile int timeoutMillis;   // Timeout tracking
extern volatile int prevTimeMillis;  // Previous time for timeout
extern volatile bool TCA8418_event;  // Keypad interrupt event
extern volatile bool PWR_BTN_event;  // Power button event
extern volatile bool SHFT;           // Shift key state
extern volatile bool FN;             // Function key state
extern volatile bool newState;       // App state changed
extern bool noTimeout;               // Disable timeout
extern volatile bool OLEDPowerSave;  // OLED power save mode
extern volatile bool disableTimeout; // Disable timeout globally
extern volatile int battState;       // Battery state
// extern volatile int prevBattState;   // Previous battery state
extern unsigned int flashMillis;     // Flash timing
extern int prevTime;                 // Previous time (minutes)
extern uint8_t prevSec;              // Previous seconds
extern TaskHandle_t einkHandlerTaskHandle; // E-Ink handler task

// ===================== KEYBOARD STATE =====================
// extern char currentKB[4][10];        // Current keyboard layout
extern volatile bool SDCARD_INSERT;  // SD card inserted event
extern bool noSD;                    // No SD card present
extern volatile bool SDActive;       // SD card active

// ===================== FILES & TEXT =====================
extern String editingFile;           // Currently edited file
//extern const GFXfont *currentFont;   // Current font
//extern uint8_t maxCharsPerLine;      // Max chars per line (display)
//extern uint8_t maxLines;             // Max lines per screen
//extern uint8_t fontHeight;           // Font height in pixels
//extern uint8_t lineSpacing;          // Line spacing in pixels
extern String workingFile;           // Working file name
extern String filesList[MAX_FILES];  // List of files

// ===================== APP STATES =====================
enum KBState { NORMAL, SHIFT, FUNC };    // Keyboard state
extern KBState CurrentKBState;           // Current keyboard state

// extern uint8_t partialCounter;           // E-Ink partial refresh counter
// extern volatile bool forceSlowFullUpdate;// Force slow full update

enum AppState { HOME, TXT, FILEWIZ, USB_APP, BT, SETTINGS, TASKS, CALENDAR, JOURNAL, LEXICON, APPLOADER };
extern const String appStateNames[];     // App state names
extern const unsigned char *appIcons[11]; // App icons
extern AppState CurrentAppState;         // Current app state

// ===================== TXT APP =====================
extern volatile bool newLineAdded;           // New line added in TXT
extern std::vector<String> allLines;         // All lines in TXT

// ===================== TASKS APP =====================
extern std::vector<std::vector<String>> tasks; // Task list

// ===================== HOME APP =====================
enum HOMEState { HOME_HOME, NOWLATER };       // Home app states
extern HOMEState CurrentHOMEState;            // Current home state

// ===================== FUNCTION PROTOTYPES =====================
// <sysFunc.cpp>
/* migrated to pocketmage_sys
void checkTimeout();
void  IRAM_ATTR PWR_BTN_irq();
void TCA8418_irq(); // migrated to pocketmage_keypad.h
char updateKeypress(); // migrated to pocketmage_keypad.h
void printDebug();
void saveFile();
void writeMetadata(const String& path);
void loadFile(bool showOLED = true);
void delFile(String fileName);
void deleteMetadata(String path);
void renFile(String oldFile, String newFile);
void renMetadata(String oldPath, String newPath);
void copyFile(String oldFile, String newFile);
void updateBattState();
void setCpuSpeed(int newFreq);
void playJingle(String jingle); // migrated to pocketmage_bz.h
void deepSleep(bool alternateScreenSaver = false);
void loadState(bool changeState = true);
void updateScrollFromTouch(); // migrated to pocketmage_touch.h
void setTimeFromString(String timeStr);
*/
//int stringToInt(String str);
//String removeChar(String str, char character);
//void appendToFile(String path, String inText);
//String vectorToString();
//void stringToVector(String inputText);

/* //migrated to pocketmage_sd.h
// microSD
void listDir(fs::FS &fs, const char *dirname);
void readFile(fs::FS &fs, const char *path);
String readFileToString(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
*/

/* //migrated to pocketmage_oled.h
// <OLEDFunc.cpp> 
void oledWord(String word, bool allowLarge = false, bool showInfo = true);
void oledLine(String line, bool doProgressBar = true, String bottomMsg = "");
void oledScroll();
void infoBar();
*/

/* // migrated to pocketmage_eink.h
// <einkFunc.cpp>
void refresh();
void statusBar(String input, bool fullWindow = false);
void einkTextPartial(String text, bool noRefresh = false);
int  countLines(String input, size_t maxLineLength = 29);
void einkTextDynamic(bool doFull_, bool noRefresh = false);
void setTXTFont(const GFXfont *font);
void setFastFullRefresh(bool setting);
void drawStatusBar(String input);
void multiPassRefresh(int passes);
*/

// <PocketMage>
void einkHandler(void *parameter); // moved from EinkFunc.cpp
void applicationEinkHandler();
void processKB();


// ===================== CALC APP =====================
// max refreshes before a full refresh is forced (change with caution)
#define REFRESH_MAX_CALC 5
#define SCROLL_MAX 8
#define SCROLL_MED 4
#define SCROLL_SML 2
#define FRAME_TOP 32                                  // top for large calc frame
#define FRAME_LEFT 10                                 // left for large calc frame
#define FRAME_RIGHT 10                                // right for large calc frame
#define FRAME_BOTTOM 32                               // bottom for large calc frame
enum CALCState { CALC0, CALC1, CALC2, CALC3, CALC4, CALCFONT };
struct Unit {
    const char* name;  
    const char* symbol;  
    // factor * (value + offset) = convert from in to basis unit
    // (inBase / to.factor) - to.offset  = convert from basis unit to output unit
    double factor;         
    double offset;         
};
struct UnitSet {
  const char* category;
  const Unit* data;
  size_t      size;
};
extern Unit emptyUnit;
extern CALCState CurrentCALCState;
extern int refresh_count;
extern String cleanExpression;
extern String calculatedResult;
extern int calcSwitchedStates;
extern String prevLine;
extern std::map<String, double> variables;
extern const char* operatorsCalc[];
extern const size_t operatorsCalcCount;
struct OpEntry { const char* token; uint8_t prec; bool rightAssoc; };
extern const OpEntry OPS[];
extern const size_t OPS_N;
extern const char* functionsCalc[];
extern const size_t functionsCalcCount;
extern const char* constantsCalc[];
extern const size_t constantsCalcCount;
extern std::vector<String> prevTokens;
extern int trigType;

// ===================== FRAME CLASS =====================
# define MAX_FRAMES 100
# define X_OFFSET 4
#pragma region textSource
// bit flags for alignment or future options
enum LineFlags : uint8_t { LF_NONE=0, LF_RIGHT= 1<<0, LF_CENTER= 1<<1 };
struct LineView {
  const char* ptr;   // points to NUL-terminated string in RAM or PROGMEM
  uint16_t    len;   // byte length (no need to include '\0')
  uint8_t     flags; // LineFlags
};
// read-only interface for any line list (PROGMEM table, arena, etc.)
struct TextSource {
  virtual ~TextSource() {}
  virtual size_t   size() const = 0;
  virtual LineView line(size_t i) const = 0; // never allocates
};
template<size_t MAX_LINES, size_t BUF_BYTES>
struct FixedArenaSource : TextSource {
  char     buf[BUF_BYTES];
  uint16_t off[MAX_LINES];
  uint16_t len_[MAX_LINES];
  uint8_t  flags_[MAX_LINES];
  size_t   nLines = 0;
  size_t   used   = 0;

  size_t size() const override { return nLines; }

  LineView line(size_t i) const override {
    return { buf + off[i], len_[i], flags_[i] };
  }

  void clear() { nLines = 0; used = 0; }

  // Returns false if out of capacity; caller can choose to drop the oldest, etc.
  bool pushLine(const char* s, uint16_t L, uint8_t flags = LF_NONE) {
    if (nLines >= MAX_LINES || used + L + 1 > BUF_BYTES) return false;
    memcpy(buf + used, s, L);
    buf[used + L] = '\0';
    off[nLines]   = (uint16_t)used;
    len_[nLines]  = L;
    flags_[nLines]= flags;
    used         += L + 1;
    nLines++;
    return true;
  }
};
struct ProgmemTableSource : TextSource {
  // table is a PROGMEM array of PROGMEM pointers to '\0'-terminated strings
  const char* const* table; // PROGMEM
  size_t count;

  ProgmemTableSource(const char* const* t, size_t n) : table(t), count(n) {}

  size_t size() const override { return count; }

  LineView line(size_t i) const override {
    const char* p = (const char*)pgm_read_ptr(&table[i]);
    // length from PROGMEM
    uint16_t L = (uint16_t)strlen_P(p);
    return { p, L, LF_NONE };
  }
};

extern const char* const HELP_LINES[] PROGMEM;
extern const size_t HELP_COUNT;
extern const char* const UNIT_TYPES_LINES[] PROGMEM;
extern const size_t UNIT_TYPES_COUNT;
extern const char* const CONV_DIR_LINES[] PROGMEM;
extern const size_t CONV_DIR_COUNT;
extern const char* const CONV_LENGTH_LINES[] PROGMEM;
extern const size_t CONV_LENGTH_COUNT;
extern const char* const CONV_AREA_LINES[] PROGMEM;
extern const size_t CONV_AREA_COUNT;
extern const char* const CONV_VOLUME_LINES[] PROGMEM;
extern const size_t CONV_VOLUME_COUNT;
extern const char* const CONV_MASS_LINES[] PROGMEM;
extern const size_t CONV_MASS_COUNT;
extern const char* const CONV_TEMPERATURE_LINES[] PROGMEM;
extern const size_t CONV_TEMPERATURE_COUNT;
extern const char* const CONV_ENERGY_LINES[] PROGMEM;
extern const size_t CONV_ENERGY_COUNT;
extern const char* const CONV_SPEED_LINES[] PROGMEM;
extern const size_t CONV_SPEED_COUNT;
extern const char* const CONV_PRESSURE_LINES[] PROGMEM;
extern const size_t CONV_PRESSURE_COUNT;
extern const char* const CONV_DATA_LINES[] PROGMEM;
extern const size_t CONV_DATA_COUNT;
extern const char* const CONV_ANGLE_LINES[] PROGMEM;
extern const size_t CONV_ANGLE_COUNT;
extern const char* const CONV_TIME_LINES[] PROGMEM;
extern const size_t CONV_TIME_COUNT;
extern const char* const CONV_POWER_LINES[] PROGMEM;
extern const size_t CONV_POWER_COUNT;
extern const char* const CONV_FORCE_LINES[] PROGMEM;
extern const size_t CONV_FORCE_COUNT;
extern const char* const CONV_FREQUENCY_LINES[] PROGMEM;
extern const size_t CONV_FREQUENCY_COUNT;



extern const UnitSet UnitCatalog[];
extern const size_t  UnitCatalogCount;

extern const ProgmemTableSource* const allUnitLists[];
extern const size_t              AllUnitListsCount;

extern const UnitSet* CurrentUnitSet;
extern const ProgmemTableSource* CurrentUnitListSrc;

extern FixedArenaSource<512, 16384> calcLines;
extern ProgmemTableSource helpSrc;
extern ProgmemTableSource unitTypesSrc;
extern ProgmemTableSource convDirSrc;
extern ProgmemTableSource convLengthSrc;
extern ProgmemTableSource convAreaSrc;
extern ProgmemTableSource convVolumeSrc;
extern ProgmemTableSource convMassSrc;
extern ProgmemTableSource convTemperatureSrc;
extern ProgmemTableSource convEnergySrc;
extern ProgmemTableSource convSpeedSrc;
extern ProgmemTableSource convPressureSrc;
extern ProgmemTableSource convDataSrc;
extern ProgmemTableSource convAngleSrc;
extern ProgmemTableSource convTimeSrc;
extern ProgmemTableSource convPowerSrc;
extern ProgmemTableSource convForceSrc;
extern ProgmemTableSource convFrequencySrc;
extern const UnitSet* CurrentUnitSet; 

#pragma endregion
#pragma region frameSetup
class Frame {
public:
  // what kind of content this frame holds
  enum class Kind : uint8_t { none, text, bitmap };

  // geometry
  int left, right, top, bottom;
  int origLeft, origRight, origTop, origBottom;
  int extendLeft, extendRight, extendTop, extendBottom;
  int bitmapW = 0;
  int bitmapH = 0;
  // flags/state switch to uint8_t flag
  bool cursor   = false;
  bool box      = false;
  bool invert   = false;
  bool overlap  = false;


  int   choice     = -1;
  long  scroll     = 0;
  long  prevScroll = -1;
  int   maxLines   = 0;
  long  lastTotal  = -1;

  // content (only one valid at a time)
  Kind kind = Kind::none;
  const TextSource* source = nullptr;  // for text frames
  const uint8_t* bitmap    = nullptr;  // for bitmap frames
  const GFXfont *font = (GFXfont *)&FreeSerif9pt7b;

  
  const Unit *unit = nullptr;
  // base constructor for common fields
  Frame(int left, int right, int top, int bottom, 
        bool cursor=false, bool box=false)
  : left(left), right(right), top(top), bottom(bottom),
    extendBottom(bottom), origBottom(bottom), cursor(cursor), box(box) {}

  // constructor for text frames
  Frame(int left, int right, int top, int bottom,
        const TextSource* linesPtr,
        bool cur=false, bool bx=false)
  : Frame(left, right, top, bottom, cur, bx) {
    kind   = Kind::text;
    source = linesPtr;
  }

  // constructor for bitmap frames
  Frame(int left, int right, int top, int bottom,
        const uint8_t* bitmapPtr, int width, int height,
        bool cursor=false, bool box=false)
  : Frame(left, right, top, bottom, cursor, box) {
    kind      = Kind::bitmap;
    bitmap    = bitmapPtr;
    bitmapW      = width;
    bitmapH      = height;
  }

  bool hasText()   const { return kind == Kind::text   && source; }
  bool hasBitmap() const { return kind == Kind::bitmap && bitmap; }
};
extern Frame calcScreen;
extern Frame conversionScreen;
extern Frame helpScreen;
extern Frame conversionUnit;
extern Frame conversionDirection;
extern Frame conversionFrameA;
extern Frame conversionFrameB;
extern Frame conversionTypes;
extern Frame testBitmapScreen;
extern Frame testBitmapScreen1;
extern Frame testBitmapScreen2;
extern Frame testTextScreen;
extern Frame *CurrentFrameState;
extern int currentFrameChoice;
extern int frameSelection;
extern std::vector<Frame*> frames;
#pragma endregion

// ===================== FUNCTION PROTOTYPES =====================
// <sysFunc.cpp>
// SYSTEM
// migrate to pocketmage_sys (planned)
void checkTimeout();
void PWR_BTN_irq();
//void TCA8418_irq(); // migrated to pocketmage_keypad.h
//char updateKeypress(); // migrated to pocketmage_keypad.h
void printDebug();
String vectorToString();
void stringToVector(String inputText);
void saveFile();
void writeMetadata(const String& path);
void loadFile(bool showOLED = true);
void delFile(String fileName);
void deleteMetadata(String path);
void renFile(String oldFile, String newFile);
void renMetadata(String oldPath, String newPath);
void copyFile(String oldFile, String newFile);
void updateBattState();
String removeChar(String str, char character);
void appendToFile(String path, String inText);
void setCpuSpeed(int newFreq);
void deepSleep(bool alternateScreenSaver = false);
void loadState(bool changeState = true);
int stringToInt(String str);
void updateScrollFromTouch();
void setTimeFromString(String timeStr);

/* //migrated to pocketmage_sd.h
// microSD
void listDir(fs::FS &fs, const char *dirname);
void readFile(fs::FS &fs, const char *path);
String readFileToString(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
*/

/* //migrated to pocketmage_oled.h
// <OLEDFunc.cpp> 
void oledWord(String word, bool allowLarge = false, bool showInfo = true);
void oledLine(String line, bool doProgressBar = true, String bottomMsg = "");
void oledScroll();
void infoBar();
*/

/* // migrated to pocketmage_eink.h
// <einkFunc.cpp>
void refresh();
void statusBar(String input, bool fullWindow = false);
void einkTextPartial(String text, bool noRefresh = false);
int  countLines(String input, size_t maxLineLength = 29);
void einkTextDynamic(bool doFull_, bool noRefresh = false);
void setTXTFont(const GFXfont *font);
void setFastFullRefresh(bool setting);
void drawStatusBar(String input);
void multiPassRefresh(int passes);
*/

// <FILEWIZ.cpp>
void FILEWIZ_INIT();
void processKB_FILEWIZ();
void einkHandler_FILEWIZ();

// <TXT.cpp>
void TXT_INIT();
void processKB_TXT_NEW();
void einkHandler_TXT_NEW();

// <CALC.cpp>

void einkHandler_CALC();
void processKB_CALC();
void CALC_INIT();
  // algorithms NOTE: exposed for other apps if needed
std::deque<String> convertToRPN(String expression);
String evaluateRPN(std::deque<String> rpnQueue,const Unit *convA,const Unit *convB);
std::vector<String> tokenize(const String& expression);


// <HOME.cpp>
void HOME_INIT();
void einkHandler_HOME();
void processKB_HOME();

// <TASKS.cpp>
void TASKS_INIT();
void sortTasksByDueDate(std::vector<std::vector<String>> &tasks);
void updateTaskArray();
void einkHandler_TASKS();
void processKB_TASKS();


// <CALENDAR.cpp>
void CALENDAR_INIT();
void processKB_CALENDAR();
void einkHandler_CALENDAR();

// <LEXICON.cpp>
void LEXICON_INIT();
void processKB_LEXICON();
void einkHandler_LEXICON();

// <SETTINGS.cpp>
void SETTINGS_INIT();
void processKB_settings();
void einkHandler_settings();

// <JOURNAL.cpp>
void JOURNAL_INIT();
void processKB_JOURNAL();
void einkHandler_JOURNAL();

// <PocketMage>
void einkHandler(void *parameter); // moved from EinkFunc.cpp
void applicationEinkHandler();
void processKB();

// <FRAMES.cpp>
  // main functions
void einkFramesDynamic(std::vector<Frame*> &frames, bool doFull_);
  // text boxes
std::vector<String> formatText(Frame &frame,int maxTextWidth);
void drawLineInFrame(String &srcLine, int lineIndex, Frame &frame, int usableY, bool clearLine, bool isPartial);
void drawFrameBox(int usableX, int usableY, int usableWidth, int usableHeight,bool invert);
int computeCursorX(Frame &frame, bool rightAlign, bool centerAlign, int16_t x1, uint16_t lineWidth);
  // String formatting
static size_t sliceThatFits(const char* s, size_t n, int maxTextWidth);
std::vector<String> sourceToVector(const TextSource* src);
String frameChoiceString(const Frame& f);  
  //scroll
void updateScroll(Frame *currentFrameState,int prevScroll,int currentScroll, bool reset = false);
void updateScrollFromTouch_Frame();
void oledScrollFrame(); 
//void updateScroll(Frame *currentFrameState,int prevScroll,int currentScroll, bool reset);
void getVisibleRange(Frame *f, long totalLines, long &startLine, long &endLine);


#endif // GLOBALS_H