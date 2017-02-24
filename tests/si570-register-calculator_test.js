var ds;
 
// Check to see if running from within CCSv4 Scripting Console
var withinCCS = (ds !== undefined);
 
// Create scripting environment and get debug server if running standalone
if (!withinCCS)
{
  // Import the DSS packages into our namespace to save on typing
  importPackage(Packages.com.ti.debug.engine.scripting);
  importPackage(Packages.com.ti.ccstudio.scripting.environment);
  importPackage(Packages.java.lang);

  // Create our scripting environment object - which is the main entry point into any script and
  // the factory for creating other Scriptable ervers and Sessions
  var script = ScriptingEnvironment.instance();

  // Get the Debug Server and start a Debug Session
  server = script.getServer("DebugServer.1");
}
else // otherwise leverage existing scripting environment and debug server
{
  var server = ds;
  var script = env;
}

script.setCurrentDirectory("F:/Symlink-d folders/James/workspace_v6_1_3/rf-generator/");

server.setConfig("./targetConfigs/MSP430G2553.ccxml");
var session = server.openSession(".*");

print("Opened session.");

setActiveBuildConfig("rf-generator","3 si570-register-calculator_test");
buildProject("rf-generator");

print("Set build config.");

session.target.connect();

print("Connected to target.\n");

session.memory.loadProgram("./si570-register-calculator_test/register-calculator_test.out");

session.breakpoint.add("si570-register-calculator_test-driver.cc", 25);

var testCounter = 0;
var passingTestCounter = 0;
var FXTAL = 114.285;
var DCO_MIN = 4850;
var DCO_MAX = 5670;

var verbose = false;

function testFrequency(frequency) {
  session.expression.evaluate("test_freq = " + frequency);
  session.target.run();
  
  var rfreq = session.expression.evaluateToString("rfreq");
  var n1 = session.expression.evaluate("n1");
  var hs_div = session.expression.evaluate("hs_div");
  rfreq = parseFloat(rfreq);
  var dco_freq = FXTAL * rfreq;
  var resultant_freq = FXTAL * rfreq / n1 / hs_div;
  
  print("Test Frequency: " + frequency);
  if (verbose) {
    print("RFREQ: " + rfreq + ", N1: " + n1 + ", HS_DIV: " + hs_div);
    print("DCO Freq: " + dco_freq);
    print("Output freq: " + resultant_freq);
  }
  
  testCounter++;
  if (Math.abs(resultant_freq - frequency) < 0.00001 && (dco_freq > DCO_MIN && dco_freq < DCO_MAX)) {
    print("PASS.\n");
    passingTestCounter++;
  } else {
    print("FAIL.\n");
  }
}

testFrequency(140.0);
testFrequency(110.22515);
testFrequency(10.0);
testFrequency(160.0);

print("" + passingTestCounter + " out of " + testCounter + " tests pass.");
if (passingTestCounter === testCounter) {
  print("ALL TESTS PASS.");
} else {
  print("SOME TESTS FAIL.");
}

session.expression.evaluate("isTesting = 0");

server.stop();
session.terminate();