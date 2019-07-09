#   This file is part of aither.
#   Copyright (C) 2015-18  Michael Nucci (mnucci@pm.me)
#
#   Aither is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Aither is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#
#   This script runs regression tests to test builds on linux and macOS for
#   travis ci, and windows for appveyor

import os
import optparse
import shutil
import sys
import datetime
import subprocess
import time

class regressionTest:
    def __init__(self):
        self.caseName = "none"
        self.iterations = 100
        self.procs = 1
        self.residuals = [1.0, 1.0, 1.0, 1.0, 1.0]
        self.ignoreIndices = []
        self.location = os.getcwd()
        self.runDirectory = "."
        self.exePath = "OneFLOW"
        self.mpirunPath = "mpirun"
        self.percentTolerance = 0.01
        self.isRestart = False
        self.restartFile = "none"
        self.passedStatus = "none"
        self.isProfile = False

    def SetRegressionCase(self, name):
        self.caseName = name

    def SetNumberOfIterations(self, num):
        self.iterations = num

    def SetNumberOfProcessors(self, num):
        self.procs = num

    def Processors(self):
        return self.procs

    def PassedStatus(self):
        return self.passedStatus

    def SetResiduals(self, resid):
        self.residuals = resid

    def SetRunDirectory(self, path):
        self.runDirectory = path

    def SetExePath(self, path):
        self.exePath = path

    def SetMpirunPath(self, path):
        self.mpirunPath = path

    def SetIgnoreIndices(self, ind):
        self.ignoreIndices.append(ind)

    def SetPercentTolerance(self, per):
        self.percentTolerance = per

    def GoToRunDirectory(self):
        os.chdir(self.runDirectory)

    def SetRestart(self, resFlag):
        self.isRestart = resFlag

    def SetProfile(self, profFlag):
        self.isProfile = profFlag

    def SetRestartFile(self, resFile):
        self.restartFile = resFile

    def ReturnToHomeDirectory(self):
        os.chdir(self.location)

    def GetTestCaseResiduals(self):
        fname = self.caseName + ".resid"
        print( "fname = " ,fname)
        rfile = open(fname, "r")
        lastLine = rfile.readlines()[-1]
        rfile.close()
        tokens = lastLine.split()
        print(tokens)
        print(len(tokens))
        print("len(self.residuals)=",len(self.residuals))
        len(self.residuals)
        resids = [float(res) for res in tokens[0:len(self.residuals)]]
        print(resids)
        return resids

    def CompareResiduals(self, returnCode):
        testResids = self.GetTestCaseResiduals()
        resids = []
        truthResids = []
        for ii in range(0, len(testResids)):
            if ii not in self.ignoreIndices:
                resids.append(testResids[ii])
                truthResids.append(self.residuals[ii])
        if (returnCode == 0):
            passing = [abs(resid - truthResids[ii]) <= self.percentTolerance * truthResids[ii]
                       for ii, resid in enumerate(resids)]
        else:
            passing = [False for ii in resids]
        return passing, resids, truthResids

    def GetResiduals(self):
        return self.residuals
        
    # change input file to have number of iterations specified for test
    def ModifyInputFile(self):
        fname = self.caseName + ".inp"
        fnameBackup = fname + ".old"
        print("fname = ", fname)
        print("fnameBackup = ", fnameBackup)
        shutil.move(fname, fnameBackup)
        print("fname = ", fname)
        print("fnameBackup = ", fnameBackup)
        with open(fname, "w") as fout:
            with open(fnameBackup, "r") as fin:
                for line in fin:
                    if "iterations:" in line:
                        fout.write("iterations: " + str(self.iterations) + "\n")
                    elif "outputFrequency:" in line:
                        fout.write("outputFrequency: " + str(self.iterations) + "\n")
                    elif "restartFrequency:" in line and self.isProfile:
                        fout.write("restartFrequency: " + str(self.iterations) + "\n")
                    else:
                        fout.write(line)

    # modify the input file and run the test
    def RunCase(self):
        self.GoToRunDirectory()
        print("---------- Starting Test:", self.caseName, "----------")
        print("Current directory:", os.getcwd())
        print("Modifying input file...")
        self.ModifyInputFile()
        #cmd = self.exePath + " " + self.caseName + ".inp > " + self.caseName + ".out"
        cmd = self.exePath
        print(cmd)
        start = datetime.datetime.now()
        interval = start
        #process = subprocess.Popen(cmd, shell=True)
        process = subprocess.Popen(cmd)
        while process.poll() is None:
            current = datetime.datetime.now()
            if (current - interval).total_seconds() > 60.:
                print("----- Run Time: %s -----" % (current - start))
                interval = current
            time.sleep(0.5)
        returnCode = process.poll()
        print("process.poll()=",process.poll())
        print("111------------------------haha----------")
        print("returnCode=",returnCode)

        if (returnCode == 0):
            print("Simulation completed with no errors")
            # test residuals for pass/fail
            passed, resids, truth = self.CompareResiduals(returnCode)
            if all(passed):
                print("All tests for", self.caseName, "PASSED!")
                self.passedStatus = "PASSED"
            else:
                print("Tests for", self.caseName, "FAILED!")
                print("Residuals should be:", truth)
                print("Residuals are:", resids)
                self.passedStatus = "MISMATCH"
        else:
            print("ERROR: Simulation terminated with errors")
            self.passedStatus = "ERRORS"
        duration = datetime.datetime.now() - start

        print("Test Duration:", duration)
        print("---------- End Test:", self.caseName, "----------")
        print("")
        print("")
        self.ReturnToHomeDirectory()
        return passed


def main():
    # Set up options
    parser = optparse.OptionParser()
    parser.add_option("-e", "--exePath", action="store", dest="exePath",
                      default="OneFLOW",
                      help="Path to OneFLOW executable. Default = OneFLOW")
    parser.add_option("-o", "--operatingSystem", action="store",
                      dest="operatingSystem", default="linux",
                      help="Operating system that tests will run on [linux/macOS/windows]. Default = linux")
    parser.add_option("-m", "--mpirunPath", action="store",
                      dest="mpirunPath", default="mpirun",
                      help="Path to mpirun. Default = mpirun")
    parser.add_option("-b", "--build", action="store",
                      dest="build", default="release",
                      help="build type used in compilation. Default = release")

    options, remainder = parser.parse_args()

    # travis macOS images have 1 proc, ubuntu have 2
    # appveyor windows images have 2 procs
    maxProcs = 2
    if (options.operatingSystem == "macOS"):
        maxProcs = 1

    isProfile = options.build == "debug"
    numIterations = 100
    numIterationsShort = 20
    numIterationsRestart = 50
    if isProfile:
      numIterations = 1
      numIterationsShort = 1
      numIterationsRestart = 1

    totalPass = True

    # ------------------------------------------------------------------
    # Regression tests
    # ------------------------------------------------------------------

    # ------------------------------------------------------------------
    # subsonic cylinder
    # laminar, inviscid, lu-sgs
    subCyl = regressionTest()
    subCyl.SetRegressionCase("subsonicCylinder")
    subCyl.SetExePath(options.exePath)
    subCyl.SetRunDirectory("subsonicCylinder")
    subCyl.SetProfile(isProfile)
    subCyl.SetNumberOfProcessors(1)
    subCyl.SetNumberOfIterations(numIterations)
    subCyl.SetResiduals(
        [1.8751e-01, 2.6727e-01, 3.1217e-01, 7.9662e-01, 1.8639e-01])
    subCyl.SetIgnoreIndices(3)
    subCyl.SetMpirunPath(options.mpirunPath)

    # run regression case
    passed = subCyl.RunCase()
    totalPass = totalPass and all(passed)

    # ------------------------------------------------------------------
    # regression test overall pass/fail
    # ------------------------------------------------------------------
    errorCode = 0
    if totalPass:
        print("All tests passed!")
    else:
        print("ERROR: Some tests failed")
        errorCode = 1
    print("--------------------------------------------------")
    print("subsonicCylinder:", subCyl.PassedStatus())

    sys.exit(errorCode)

if __name__ == "__main__":
    main()
