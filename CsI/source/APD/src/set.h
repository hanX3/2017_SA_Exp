// #define DEBUGWAVEANALYSIS
// #define DEBUGDRAWOPTION

#define MAXLENGTH 32768
#define POLARITY -1

// Baseline
#define PRETRIGGER    200
#define BASELINESTDCHECK 10.

// RCCR2 Parameters
#define RCCR2PARPRETRIGGER    200
#define RCCR2PARSMOOTHLENGTH  60
#define RCCR2PARRISELENGTH    60
#define RCCR2PARTHRESHOLD     30

// Trapezoid Parameters
#define TRAPPARDECAYLENGTH  6000
#define TRAPPARRISELENGTH   2000
#define TRAPPARFLATTOP      400
#define TRAPPARPRITRIGGER   200
#define TRAPPARPEAKDELAY    0
#define TRAPPARPEAKMEAN     0

//
#define PILEUPMAX 2

// PID
#define Q1START  500
#define Q1STOP   800
#define Q2START  2000
#define Q2STOP   4000

#define FITSTART 0
#define FITSTOP  4000

#define BASELINE            0.
#define BASELINEMIN         -10.
#define BASELINEMAX         10.
#define T0                  400.
#define T0MIN               200.
#define T0MAX               600.
#define TAURC               6250.
#define TAURCMIN            4000.
#define TAURCMAX            8000.
#define TAUFAST             240.
#define TAUFASTMIN          50.
#define TAUFASTMAX          500.
#define TAUSLOW             4000.
#define TAUSLOWMIN          800.
#define TAUSLOWMAX          40000.
#define AMFAST              300.
#define AMFASTMIN           10.
#define AMFASTMAX           3000.
#define AMSLOW              300.
#define AMSLOWMIN           10.
#define AMSLOWMAX           3000.

