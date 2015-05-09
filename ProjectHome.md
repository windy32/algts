## Introduction ##

algts is an application suite for studying issues about link-sharing gateways.

Tests under algts are similar to so called Hardware-in-the-Loop tests, the difference is no simulator, real-time scheduler, or packet analyzer is involved, all trace items are from application layer logic.

Click on QuickSetupGuide if you want the testbed configured and start working as soon as possible.

### The project consists of 4 parts: ###

  * a simulator-like core that resembles ns3 a bit
  * a qt-based gui for further analysis of trace files and more convenience (this part has now be replaced by scripting for flexibility)
  * a tutorial document
  * an api document

### Change log ###

---

**Preview release 0.0.1 - April 18, 2012**

The core, server daemon, emulator daemon and a few test scripts are available, the gui and tutorial document will follow up soon.

**Release 0.1.0 - July 1, 2013**

The first version that can be used for actual gateway analysis. The gui is removed and scripting is suggested for analysis, meanwhile a lot of bugs have been fixed in this version.