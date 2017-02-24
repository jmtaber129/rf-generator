var ds;
 
// Check to see if running from within CCSv4 Scripting Console.
var withinCCS = (ds !== undefined);
 
// Create scripting environment and get debug server if running standalone.
if (!withinCCS)
{
  // Import the DSS packages into our namespace to save on typing
  importPackage(Packages.com.ti.debug.engine.scripting);
  importPackage(Packages.com.ti.ccstudio.scripting.environment);
  importPackage(Packages.java.lang);

  // Create our scripting environment object - which is the main entry point into any script and
  // the factory for creating other Scriptable ervers and Sessions.
  var script = ScriptingEnvironment.instance();

  // Get the Debug Server and start a Debug Session.
  server = script.getServer("DebugServer.1");
}
else // Otherwise leverage existing scripting environment and debug server.
{
  var server = ds;
  var script = env;
}

server.setConfig("./targetConfigs/MSP430G2553.ccxml");
var session = server.openSession(".*");

print("Opened session.");

setActiveBuildConfig("rf-generator","3 si570-register-calculator_test");
buildProject("rf-generator");

print("Project built.");

session.target.connect();

print("Connected to target.\n");

try {


  session.memory.loadProgram("./si570-register-calculator_test/register-calculator_test.out");
  print("Program loaded.");
  
  session.breakpoint.add("si570-register-calculator_test-driver.cc", 38);

  var testCounter = 0;
  var passingTestCounter = 0;
  var FXTAL = 114.285;
  var DCO_MIN = 4850;
  var DCO_MAX = 5670;
  
  var evaluate = function(expression) { 
    return session.expression.evaluate(expression) 
  };

  var verbose = true;

  function testFrequency(frequency) {
    session.expression.evaluate("test_freq = " + frequency);
    session.target.run();

    var integer_rfreq = evaluate("rfreq");
    var rfreq = integer_rfreq / 268435456;
    var n1 = evaluate("n1");
    var hs_div = evaluate("hs_div");
    var reg7 = evaluate("reg7");
    var reg8 = evaluate("reg8");
    var reg9 = evaluate("reg9");
    var reg10 = evaluate("reg10");
    var reg11 = evaluate("reg11");
    var reg12 = evaluate("reg12");
    rfreq = parseFloat(rfreq);
    var dco_freq = FXTAL * rfreq;
    var resultant_freq = FXTAL * rfreq / n1 / hs_div;

    print("Test Frequency: " + frequency);
    if (verbose) {
      print("RFREQ: " + rfreq + ", N1: " + n1 + ", HS_DIV: " + hs_div);
      print("Integer RFREQ: " + integer_rfreq);
      print("DCO Freq: " + dco_freq);
      print("Output freq: " + resultant_freq);
      print("Reg 7: " + reg7);
      print("Reg 8: " + reg8);
      print("Reg 9: " + reg9);
      print("Reg 10: " + reg10);
      print("Reg 11: " + reg11);
      print("Reg 12: " + reg12);
    }

    testCounter++;
    // Check that DCO is within range and output frequency is accurate to 10 ppb.
    if (Math.abs(resultant_freq / frequency - 1) < 0.00000001 && (dco_freq > DCO_MIN && dco_freq < DCO_MAX)) {
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

} finally {
  session.breakpoint.removeAll();
  server.stop();
  session.terminate();
}