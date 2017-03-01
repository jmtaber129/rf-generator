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

session.target.connect();

print("Connected to target.\n");

try {
  session.memory.loadProgram("./i2c-slave/rf-generator.out");
  print("Program loaded.");
  
  session.breakpoint.add("i2c-slave-main.cc", 68);
  
  var evaluate = function(expression) { 
    return session.expression.evaluate(expression) 
  };

  var verbose = false;
  
  session.target.run();
  
  var reg7, reg8, reg9, reg10, reg11, reg12;
  
  for (var i = 0; i < 90; i++) {
    if (i % 9 == 0) {
      print("Case " + (i/9 + 1) + ":");
    }
    var address = evaluate("registers[" + i + "].address");
    var value = evaluate("registers[" + i + "].value");
    if (verbose) {
      print("Address: " + address + ", \tValue: " + value);
    }
    switch (address) {
      case 7: 
        reg7 = value;
        break;
      case 8:
        reg8 = value;
        break;
      case 9:
        reg9 = value;
        break;
      case 10:
        reg10 = value;
        break;
      case 11:
        reg11 = value;
        break;
      case 12:
        reg12 = value;
        break;
    }
    if (i % 9 == 8) {
      hs_div = (reg7 >>> 5) + 4;
      n1 = ((reg7 & 0x1F) << 2) + (reg8 >>> 6) + 1;
      var left_shift_32 = 4294967296;
      var left_shift_24 = left_shift_32 / 256;
      var left_shift_16 = left_shift_24 / 256;
      var left_shift_8 = left_shift_16 / 256;
      rfreq = (reg8 & 0x3F) * left_shift_32 + reg9 * left_shift_24 + 
        reg10 * left_shift_16 + reg11 * left_shift_8 + reg12;
      rfreq = rfreq / (1 << 28);
      
      if (verbose) {
        print("HS_DIV: " + hs_div + ", \tN1: " + n1);
        print("RFREQ: " + rfreq);
      }
      
      var freq = 114.285 * rfreq / hs_div / n1;
      print("Resultant frequency: " + freq);
    }
  }

} finally {
  session.breakpoint.removeAll();
  server.stop();
  session.terminate();
}